from arango import ArangoClient
import httplib, urllib, base64
from argparse import ArgumentParser
import json


arangodb_client = ArangoClient(protocol='http', host='e1.pairlabs.ai', port=8529)
db = arangodb_client.db('test', username='root', password='pair')
persons = db.aql.execute('for per in person return per.name')

name_list = [];
for result in persons:
    name_list.append(result.encode('UTF-8','ignore'))

headers = {
    'Ocp-Apim-Subscription-Key': 'e91cf8abb8ed4e039fb5dd2d5105f8f2',
}

try: 
    conn = httplib.HTTPSConnection('api.labs.cognitive.microsoft.com')
except Exception as e:
    print("connection error")

text_file = open("empty_person.txt", "w");
respons_cnt = 0
empty_cnt = 0

for name in name_list:
    try:
        params = urllib.urlencode({
            # Request parameters
            'query': 'papers by '+ name,
            'complete': '1',
            'count': 1,
            'offset': '0',
            'timeout': '',
            'model': 'latest',
        })
    
        conn.request("GET", "/academic/v1.0/interpret?%s" % params, "{body}", headers)
        response = conn.getresponse()
        data = response.read()
        json_data = json.loads(data);  #to list type
        #print len(json_data['interpretations'])
        #print json_data['interpretations'][1]['logprob']
        expr = json_data['interpretations'][0]['rules'][0]['output']['value']   #.encode('UTF-8','ignore')
        print expr
        respons_cnt+=1
    except Exception as e:
        #print("[Errno {0}] {1}".format(e.errno, e.strerror))
        #print("WTF??? "+name)
        text_file.write("%s\n" % name)
        empty_cnt+=1
        pass
text_file.close()
conn.close()

print 'get' + str(respons_cnt)
print 'empty' + str(empty_cnt)