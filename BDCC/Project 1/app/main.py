# Imports
import warnings
warnings.filterwarnings("ignore", category=FutureWarning)

import flask
import logging
import os
import tfmodel
from google.cloud import bigquery
from google.cloud import storage

# Set up logging
logging.basicConfig(level=logging.INFO,
                     format='%(asctime)s - %(levelname)s - %(message)s',
                     datefmt='%Y-%m-%d %H:%M:%S')

PROJECT = os.environ.get('GOOGLE_CLOUD_PROJECT') 
logging.info('Google Cloud project is {}'.format(PROJECT))

# Initialisation
logging.info('Initialising app')
app = flask.Flask(__name__)

logging.info('Initialising BigQuery client')
BQ_CLIENT = bigquery.Client()

BUCKET_NAME = PROJECT + '.appspot.com'
logging.info('Initialising access to storage bucket {}'.format(BUCKET_NAME))
APP_BUCKET = storage.Client().bucket(BUCKET_NAME)

logging.info('Initialising TensorFlow classifier')
TF_CLASSIFIER = tfmodel.Model(
    app.root_path + "/static/tflite/model.tflite",
    app.root_path + "/static/tflite/dict.txt"
)

logging.info('Initialising Vision Client')
from google.cloud import vision
from google.oauth2 import service_account
credentials = service_account.Credentials. \
    from_service_account_file('./static/key/bdcc-project1-346914-a1fd72420bb8.json')
client = vision.ImageAnnotatorClient(credentials=credentials)

import json
from google.cloud.vision import AnnotateImageResponse

logging.info('Initialisation complete')

# End-point implementation
@app.route('/')
def index():
    return flask.render_template('index.html')

@app.route('/classes')
def classes():
    results = BQ_CLIENT.query(
    '''
        Select Description, COUNT(*) AS NumImages
        FROM `bdcc-project1-346914.openimages.image_labels`
        JOIN `bdcc-project1-346914.openimages.classes` USING(Label)
        GROUP BY Description
        ORDER BY Description
    ''').result()
    logging.info('classes: results={}'.format(results.total_rows))
    data = dict(results=results)
    return flask.render_template('classes.html', data=data)

@app.route('/relations')
def relations():
    results = BQ_CLIENT.query(
    '''
        Select Relation, COUNT(*) AS NumImages
        FROM `bdcc-project1-346914.openimages.relations`
        GROUP BY Relation
        ORDER BY Relation
    ''').result()
    data = dict(results=results)
    return flask.render_template('relations.html', data=data)

@app.route('/image_info')
def image_info():
    image_id = flask.request.args.get('image_id')
    
    classes = BQ_CLIENT.query(
    '''
        SELECT Description
        FROM `bdcc-project1-346914.openimages.image_labels`
        JOIN `bdcc-project1-346914.openimages.classes` USING(Label)
        WHERE ImageId = '{0}'
        ORDER BY Description  
    '''.format(image_id)
    ).result()

    relations = BQ_CLIENT.query(
    '''
        SELECT f.Description, Relation, s.Description
        FROM `bdcc-project1-346914.openimages.relations`
        JOIN `bdcc-project1-346914.openimages.classes` f ON(f.Label=Label1)
        JOIN `bdcc-project1-346914.openimages.classes` s ON(s.Label=Label2) 
        WHERE ImageId = '{0}'
        ORDER BY f.Description, Relation, s.Description
    '''.format(image_id)
    ).result()
    
    data = dict(image_id=image_id,
                classes=classes,
                relations=relations)

    return flask.render_template('image_info.html', data=data)

@app.route('/image_search')
def image_search():
    description = flask.request.args.get('description')
    image_limit = flask.request.args.get('image_limit', default=10, type=int)
    results = BQ_CLIENT.query(
    '''
        SELECT ImageId
        FROM `bdcc-project1-346914.openimages.image_labels`
        JOIN `bdcc-project1-346914.openimages.classes` USING(Label)
        WHERE Description = '{0}' 
        ORDER BY ImageId
        LIMIT {1}  
    '''.format(description, image_limit)
    ).result()
    logging.info('image_search: description={} limit={}, results={}'\
           .format(description, image_limit, results.total_rows))
    data = dict(description=description, 
                image_limit=image_limit,
                results=results)
    return flask.render_template('image_search.html', data=data)

@app.route('/relation_search')
def relation_search():
    class1 = flask.request.args.get('class1', default='%')
    relation = flask.request.args.get('relation', default='%')
    class2 = flask.request.args.get('class2', default='%')
    image_limit = flask.request.args.get('image_limit', default=10, type=int)
    
    results = BQ_CLIENT.query(
    '''
        SELECT r.ImageId, f.Description, r.Relation, s.Description
        FROM `bdcc-project1-346914.openimages.relations` r
        JOIN `bdcc-project1-346914.openimages.classes` f ON (f.Label = Label1)
        JOIN `bdcc-project1-346914.openimages.classes` s ON (s.Label = Label2)
        WHERE f.Description LIKE '{0}' AND r.Relation LIKE '{1}' AND s.Description LIKE '{2}'
        ORDER BY r.ImageId
        LIMIT {3}
    '''.format(class1, relation, class2, image_limit)
    ).result()

    data = dict(results=results,
                class1=class1,
                relation=relation,
                class2=class2,
                image_limit=image_limit)
    return flask.render_template('relation_search.html', data=data)

@app.route('/image_search_multiple')
def image_search_multiple():
    descriptions = flask.request.args.get('descriptions').split(',')
    image_limit = flask.request.args.get('image_limit', default=10, type=int)
    
    results = BQ_CLIENT.query(
    '''
        SELECT ImageID, ARRAY_AGG(Description) AS Descriptions
        FROM `bdcc-project1-346914.openimages.image_labels`
        JOIN `bdcc-project1-346914.openimages.classes` USING(Label)
        WHERE Description IN UNNEST({0})
        GROUP BY ImageID
        ORDER BY array_length(Descriptions) DESC, ImageID ASC
        LIMIT {1}
    '''.format(descriptions, image_limit)
    ).result()

    data = dict(descriptions=descriptions, 
                image_limit=image_limit,
                results=results)

    return flask.render_template('image_search_multiple.html', data=data)

@app.route('/image_classify_classes')
def image_classify_classes():
    with open(app.root_path + "/static/tflite/dict.txt", 'r') as f:
        data = dict(results=sorted(list(f)))
        return flask.render_template('image_classify_classes.html', data=data)
 
@app.route('/image_classify', methods=['POST'])
def image_classify():
    files = flask.request.files.getlist('files')
    min_confidence = flask.request.form.get('min_confidence', default=0.25, type=float)
    results = []
    if len(files) > 1 or files[0].filename != '':
        for file in files:
            classifications = TF_CLASSIFIER.classify(file, min_confidence)
            blob = storage.Blob(file.filename, APP_BUCKET)
            blob.upload_from_file(file, blob, content_type=file.mimetype)
            blob.make_public()
            logging.info('image_classify: filename={} blob={} classifications={}'\
                .format(file.filename,blob.name,classifications))
            results.append(dict(bucket=APP_BUCKET,
                                filename=file.filename,
                                classifications=classifications))
    
    data = dict(bucket_name=APP_BUCKET.name, 
                min_confidence=min_confidence, 
                results=results)
    return flask.render_template('image_classify.html', data=data)


@app.route('/label_detection', methods=['POST'])
def label_detection():
    files = flask.request.files.getlist('files')
    results = []
    if len(files) > 1 or files[0].filename != '':
        for file in files:
            blob = storage.Blob(file.filename, APP_BUCKET)
            blob.upload_from_file(file, blob, content_type=file.mimetype)
            blob.make_public()

            response = client.annotate_image({
                'image': 
                    {'source': 
                        {'image_uri': 'https://storage.googleapis.com/{0}/{1}'.format(APP_BUCKET.name, file.filename)}},
                'features': 
                    [{'type_': vision.Feature.Type.LABEL_DETECTION}]
            })

            response = AnnotateImageResponse.to_json(response)

            response = json.loads(response)

            annotations = []
            for annotation in response['labelAnnotations']:
                annotations.append(dict(label=annotation['description'], score='%.2f'%annotation['score']))

            results.append(dict(bucket=APP_BUCKET,
                                filename=file.filename,
                                annotations=annotations))
    
    data = dict(bucket_name=APP_BUCKET.name, results=results)
    return flask.render_template('label_detection.html', data=data)

if __name__ == '__main__':
    # When invoked as a program.
    logging.info('Starting app')
    app.run(host='0.0.0.0', port=8080, debug=True)
