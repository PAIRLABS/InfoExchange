# coding=UTF-8
from arango import ArangoClient
import csv

LQ = "{"
RQ = "}"

#########setup name_list from arangodb###############

arangodb_client = ArangoClient(protocol='http', host='e1.pairlabs.ai', port=8529)
db1 = arangodb_client.db('bob', username='root', password='pair')
persons = db1.aql.execute('for per in person return per.name')


name_list = []
for result in persons:
    name_list.append(result.encode('UTF-8'))

######################################################

fake_list = [];
for name in name_list[0:10]:
    fake_list.append(str(name))

#####################################################
db = arangodb_client.db('test', username='root', password='pair')

with open('output.csv', 'wb') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['Professor', 'Num', 'List : '])
    
    for name in name_list:
        data = [name]
        arr = set()
        cmd = 'for per in person filter per.name=="{0}" for pap in 1..1 outbound per author_of for per2 in 1..1 inbound pap author_of filter per2.name!="{0}" return per2.name'.format(name)
        friends = db.aql.execute(cmd)
        for f in friends:
            arr.add(f.encode('UTF-8'))
        arr = list(arr)
        data.append(str(len(arr)))
        data = data+arr
        writer.writerow(data)

csvfile.close()

#    print len(arr)



#var Cnt = new Set(db._query(aql`for per in person filter per.name=="包傑奇" for pap in 1..1 outbound per author_of for per2 in 1..1 inbound pap author_of return per2.name`).toArray())
#print("包傑奇",":",Cnt.size.toString())
