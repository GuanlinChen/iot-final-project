import boto
import boto.dynamodb2
import boto.dynamodb2
from boto.dynamodb2.fields import HashKey, RangeKey, KeysOnlyIndex, GlobalAllIndex
from boto.dynamodb2.table import Table
from boto.dynamodb2.types import NUMBER
import sys
import socket
serverIP = sys.argv[1]
port = sys.argv[2]
ACCOUNT_ID = '268305115229'
IDENTITY_POOL_ID = 'us-east-1:949cc2b6-8a51-4ec3-bbc2-f1d76f4cd489'


ROLE_ARN = 'arn:aws:iam::268305115229:role/Cognito_EdisonAppUnauth_Role'


# Use cognito to get an identity.
cognito = boto.connect_cognito_identity()
cognito_id = cognito.get_id(ACCOUNT_ID, IDENTITY_POOL_ID)
oidc = cognito.get_open_id_token(cognito_id['IdentityId'])

# Further setup your STS using the code below
sts = boto.connect_sts()
assumedRoleObject = sts.assume_role_with_web_identity(ROLE_ARN, "XX", oidc['Token'])
DYNAMODB_TABLE_NAME = 'dynamotable'

# Prepare DynamoDB client
client_dynamo = boto.dynamodb2.connect_to_region(
    'us-east-1',
    aws_access_key_id=assumedRoleObject.credentials.access_key,
    aws_secret_access_key=assumedRoleObject.credentials.secret_key,
    security_token=assumedRoleObject.credentials.session_token)

from boto.dynamodb2.table import Table
table_dynamo = Table(DYNAMODB_TABLE_NAME, connection=client_dynamo)

users = Table.create('env_data', schema=[
     HashKey('time'),
     RangeKey('humidity'),
     #RangeKey('light'),
     #RangeKey('pressure'),
     #RangeKey('temperature')
 ], throughput={
     'read': 5,
     'write': 15,
 }, global_indexes=[
     GlobalAllIndex('EverythingIndex', parts=[
         HashKey('time'),
     ],
     throughput={
         'read': 1,
         'write': 1,
     })
 ],                                                                                 
                                                                                    
connection = client_dynamo                                                          
                                                                                
)                                                                               
                                                                                 
#use table                                                                      
                                                                                    
table = Table('users', connection = client_dynamo)                 

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((serverIP, int(port)))
for x in range(2):
    data = s.recv(1024)
    #print data
    l = data.split('#')
    for i in range(len(l) - 1):
        print l[i]
        print type(l[i])
    #print 'seperate'
s.close()
table.put_item(data = {
    'time': '82050',
    'humidity': '38.0',
    'light': '1540.0',
    'temperature': '25260'
    })
"""
def put():
        
    username = raw_input('Please input the username you would like to add:')
    uid = raw_input('Please input the cuid:')
    users.put_item(data = {                                            
    'name': username,                                                             
    'id': uid,                                                               
    'account_type': 'standard_user'                                             
                    })                                                                               
#add a new item  
'pressure': '101518.0',                                              
#delete item 
def delete():
    name = raw_input('Please input the username you would like to delete:')
    uid = raw_input('Please input the cuid you would like to delete:')
    users = Table('users', connection = client_dynamo)                 
    users.delete_item(name = name, id = uid) 

#view item                                                         
                                                                   
                        
def scan():

#view all the information                         
    result_set = users.scan()                         
    for user in result_set:                           
            print "name:%s uid: %d" % (user['name'],int(user['id']))
"""


    



                                                 



    
