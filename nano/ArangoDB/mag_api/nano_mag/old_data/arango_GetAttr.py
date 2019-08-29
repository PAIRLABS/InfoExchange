#pip install python-arango

from arango import ArangoClient

# Initialize the client for ArangoDB.
arangodb_client = ArangoClient(protocol='http', host='e1.pairlabs.ai', port=8529)

# Connect to "_system" database as root user.
db = arangodb_client.db('test', username='root', password='pair')

persons = db.aql.execute('for per in person return per.name')

name_list = [];
for result in persons:
	#print (result.encode('UTF-8','ignore'))
	name_list.append(result.encode('UTF-8','ignore'))
	#name_list.append(result.encode('UTF-8','ignore').strip())

for name in name_list:
	print name

#print name_list