"""
    Generates the automl.csv file from the classes in the dict.txt file,
    which consists of 100 examples for each class (80 examples for training, 10 for verification and 10 for testing).
"""

import os
import logging
from google.cloud import bigquery
from google.cloud import storage

# Set up logging
logging.basicConfig(level=logging.INFO,
                     format='%(asctime)s - %(levelname)s - %(message)s',
                     datefmt='%Y-%m-%d %H:%M:%S')

# Initialisation
PROJECT = os.environ.get('GOOGLE_CLOUD_PROJECT') 
logging.info('Google Cloud project is {}'.format(PROJECT))

logging.info('Initialising BigQuery client')
BQ_CLIENT = bigquery.Client()

BUCKET_NAME = PROJECT + '.appspot.com'
logging.info('Initialising access to storage bucket {}'.format(BUCKET_NAME))
APP_BUCKET = storage.Client().bucket(BUCKET_NAME)

# Get the target classes
classes = []
with open("./static/tflite/dict.txt", "r") as f:
    for line in f:
        classes.append(line[:-1])

with open("./static/tflite/automl.csv", "w") as f:
    for c in classes:
        logging.info(f'Gathering images for the {c} class...')
        # Get 100 random ImageIDs for the current class (for the classes choosen, there are always, at least, 100 images)
        results = BQ_CLIENT.query(
        '''
            Select ImageID
            FROM `bdcc-project1-346914.openimages.image_labels`
            JOIN `bdcc-project1-346914.openimages.classes` USING (Label)
            WHERE Description = '{0}'
            ORDER BY RAND()
            LIMIT 100
        '''.format(c)
        ).result()
        
        results = [result[0] for result in results]
        
        # Write to the automl.csv file
        for i in range(0, 80):
            f.write(f'TRAIN,gs://bdcc_open_images_dataset/images/{results[i]}.jpg,{c}\n')
        for i in range(80, 90):
            f.write(f'VALIDATION,gs://bdcc_open_images_dataset/images/{results[i]}.jpg,{c}\n')
        for i in range(90, 100):
            f.write(f'TEST,gs://bdcc_open_images_dataset/images/{results[i]}.jpg,{c}\n')
        logging.info('Done')