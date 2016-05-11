import boto.dynamodb
import os
import time


AWS_ACCESS_KEY_ID = "AKIAINLLDVTP6TDFWMOQ"
AWS_SECRET_ACCESS_KEY = "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"
REGION_NAME = "us-east-1"
TABLE_NAME = "xdk_table"
FILENAME = "yxz5_5.csv"

conn = boto.dynamodb.connect_to_region(aws_access_key_id = AWS_ACCESS_KEY_ID,
									aws_secret_access_key = AWS_SECRET_ACCESS_KEY,
									region_name = REGION_NAME)

fileRead = open(FILENAME)
lines = fileRead.readlines()
fileRead.close()

last = lines[-1].split(",")
timestamp = int(last[0])
humidity = float(last[1])
light = float(last[2])
temperature = float(last[4])
localtime = time.strftime('%D %H:%M:%S', time.localtime())

#print conn.list_tables()
table = conn.get_table(TABLE_NAME)
item_data = {
	'device_time': timestamp,
	'local_time': localtime,
	'humidity': humidity,
	'light': light,
	'temp': temperature,
}
item = table.new_item(hash_key=timestamp , attrs=item_data)
item.put()
print 'time:',timestamp, '; humidity:', humidity, '; light:',light, ', temp:', temperature







