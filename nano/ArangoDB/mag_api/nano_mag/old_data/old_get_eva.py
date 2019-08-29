# coding=UTF-8
from arango import ArangoClient
import httplib, urllib, base64
from argparse import ArgumentParser
import json

import traceback
import time

arangodb_client = ArangoClient(protocol='http', host='e1.pairlabs.ai', port=8529)
db = arangodb_client.db('test', username='root', password='pair')
persons = db.aql.execute('for per in person return per.name')

name_list = [];
for result in persons:
    name_list.append(result.encode('UTF-8','ignore')) #strip() ???

headers = {
    'Ocp-Apim-Subscription-Key': '0cf2c61493ec498daa84808b6522d1f9',
}

try: 
    conn = httplib.HTTPSConnection('api.labs.cognitive.microsoft.com')
except Exception as e:
    print("connection error")

#fake_list = [];
#for name in name_list[0:3]:
#    fake_list.append(str(name))

err_cnt=0

for name in name_list:
    try:
        params = urllib.urlencode({
            # Request parameters
            'expr': ("Composite(AA.AuN=='"+ name + "')"),    #"Composite(AA.AuN=='徐國鎧')",
            'model': 'latest',
            'count': '10',
            'offset': '0',
            'orderby': '',
            'attributes': 'Id,Ti,Y,D,L,CC,AA.AuN,AA.AuId,F.FN,F.FId',
        })
    
        conn.request("GET", "/academic/v1.0/evaluate?%s" % params, "{body}", headers)
        response = conn.getresponse()
        data = response.read()
        json_data = json.loads(data);  #to list type
        for e in json_data['entities']:
            print e['Id']
        #expr = json_data['entities'][0]['Id']   #['entities'][0]['Ti']
        
        #MAG_id = json_data['entities'][0]['Id']
      #  language = json_data['entities'][0]['L']
      #  year = json_data['entities'][0]['Y']
      #  date = json_data['entities'][0]['D']
      #  citation_cnt = json_data['entities'][0]['CC']
      #  author = json_data['entities'][0]['AA'][0]['AuN']
      #  author_id = json_data['entities'][0]['AA'][0]['AuId']
      #  field = json_data['entities'][0]['F'][0]['FN']
      #  fid = json_data['entities'][0]['F'][0]['FId']

        #print MAG_id  #MAG_id  + language + year +data + citation_cnt + author+author_id+field + fid

        #print name + ' : ' + data+'\n'
        #print len(json_data['entities'])
        #print type(json_data)
        #print json_data['entities'][0]
    except Exception as e:
        print "err name :  "+name   #
        print traceback.format_exc()
        err_cnt+=1
        pass
    time.sleep(1)
print 'err_cnt : ' + str(err_cnt) + " / " + str(len(name_list))
conn.close()


"""
try:
    dict[key] += 1
except KeyError:
    dict[key] = 1

Id -> MAG_id
Ti -> title
L -> language
Y -> year
D -> date
CC -> citation_cnt
F.FN -> field
F.FId -> MAG_file_id

AA.AuN AA.AuId -> person



"""