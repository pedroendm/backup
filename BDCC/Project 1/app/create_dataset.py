"""
	Creates the Big Query data set from the csv files.
"""

import google.cloud.bigquery as bq
import pandas as pd
import time
import sys

PROJECT_ID='bdcc-project1-346914'

dataset_name = "openimages"

print('Creating client for project ' + PROJECT_ID)
client = bq.Client(project=PROJECT_ID)

print('Creating dataset '+ dataset_name)
dataset = client.create_dataset(dataset_name, exists_ok=True)

# -------- classes -----------
classes = pd.read_csv("./data/classes.csv")

table_name = PROJECT_ID + '.' + dataset_name + '.classes'
print('Creating table ' + table_name)

# Delete the table in case you're running this for the second time
client.delete_table(table_name, not_found_ok=True)

# Create the table
table = bq.Table(table_name)
table.schema = (
        bq.SchemaField('Label',       'STRING'),
        bq.SchemaField('Description', 'STRING')
)
client.create_table(table)

# Load the data
print('Loading data into ' + table_name)
load_job = client.load_table_from_dataframe(classes, table)

while load_job.running():
  print('waiting for the load job to complete')
  time.sleep(1)

if load_job.errors == None:
  print('Load complete!')
else:
  print(load_job.errors)

# -------- image-labels -----------
image_labels = pd.read_csv("./data/image-labels.csv")

table_name = PROJECT_ID + '.' + dataset_name + '.image_labels'
print('Creating table ' + table_name)

# Delete the table in case you're running this for the second time
client.delete_table(table_name, not_found_ok=True)

# Create the table
table = bq.Table(table_name)
table.schema = (
        bq.SchemaField('ImageID', 'STRING'),
        bq.SchemaField('Label',   'STRING')
)
client.create_table(table)

# Load the data
print('Loading data into ' + table_name)
load_job = client.load_table_from_dataframe(image_labels, table)

while load_job.running():
  print('waiting for the load job to complete')
  time.sleep(1)

if load_job.errors == None:
  print('Load complete!')
else:
  print(load_job.errors)

# -------- relations -----------
relations = pd.read_csv("./data/relations.csv")

table_name = PROJECT_ID + '.' + dataset_name + '.relations'
print('Creating table ' + table_name)

# Delete the table in case you're running this for the second time
client.delete_table(table_name, not_found_ok=True)

# Create the table
table = bq.Table(table_name)
table.schema = (
        bq.SchemaField('ImageID',  'STRING'),
        bq.SchemaField('Label1',   'STRING'),
        bq.SchemaField('Relation', 'STRING'),
        bq.SchemaField('Label2',   'STRING')
)
client.create_table(table)

# Load the data
print('Loading data into ' + table_name)
load_job = client.load_table_from_dataframe(relations, table)

while load_job.running():
  print('waiting for the load job to complete')
  time.sleep(1)

if load_job.errors == None:
  print('Load complete!')
else:
  print(load_job.errors)
  
