# coding=UTF-8
from arango import ArangoClient
import httplib, urllib, base64
from argparse import ArgumentParser
import json

import traceback
import time

LQ = "{"
RQ = "}"

#########setup name_list from arangodb###############

arangodb_client = ArangoClient(protocol='http', host='e1.pairlabs.ai', port=8529)
db = arangodb_client.db('test', username='root', password='pair')
persons = db.aql.execute('for per in person return per.name')

name_list = [];
for result in persons:
    name_list.append(result.encode('UTF-8','ignore'))

#####################################################

headers = {
    'Ocp-Apim-Subscription-Key': '0cf2c61493ec498daa84808b6522d1f9',
}

conn = httplib.HTTPSConnection('api.labs.cognitive.microsoft.com')

fake_list = [];
for name in name_list[0:3]:
    fake_list.append(str(name))

empty_cnt=0

for name in fake_list:
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
    
    print name + " : \n"
    try:
        for ENTITY in json_data['entities']:  #save paper
            MAG_id       = ENTITY['Id']         #save attr
            title        = ENTITY['Ti']         #save attr
            language     = ENTITY['L']          #save attr
            year         = ENTITY['Y']          
            date         = ENTITY['D']          #save attr
            citation_cnt = ENTITY['CC']         #save attr
            for PERSON in ENTITY['AA']:  
                author       = PERSON['AuN']    #save attr & save relation
                author_id    = PERSON['AuId']   #save attr & update mag_id
            for FIELD in ENTITY['F']:
                field        = FIELD['FN']      #save attr
                fid          = FIELD['FId']     #save attr
            print title
    except KeyError:
        print name
        print traceback.format_exc()
        empty_cnt+=1
        pass
    time.sleep(1)  #MAG allow Evaluate 1 time per second
print 'err_cnt : ' + str(empty_cnt) + " / " + str(len(name_list))
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

db.person.save({  'name' : "徐國鎧", expertise:[{ name:'Tim',nid:'123' },{name:'John',nid:'456'}],email: "kkshy'u@ee.ncu.edu.tw"  })._id

"""