# coding=UTF-8
from arango import ArangoClient
import httplib, urllib, base64
from argparse import ArgumentParser
import json

import traceback
import time
import sys
reload(sys)                         # 2
sys.setdefaultencoding('utf-8')

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
for name in name_list[0:2]:
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
    
    print "//" + name + " : \n"
    print "db._createEdgeCollection('author_of')"
    try:
        for ENTITY in json_data['entities']:  #save paper
            MAG_id       = ENTITY['Id']         #save attr
            title        = ENTITY['Ti']         #save attr
            language     = ENTITY['L']          #save attr
            year         = ENTITY['Y']          
            date         = ENTITY['D']          #save attr
            citation_cnt = ENTITY['CC']         #save attr
            statement_1 = 'var paper_id = db.paper.save( {6} MAG_id:"{0}", title:"{1}", language:"{2}", year:"{3}", date:"{4}", citation_cnt:"{5}", fields:['.format(MAG_id,title,language,year,date,citation_cnt,LQ)
            statement_2 = 'var person_id;\n'
            print statement_2
            statement_3 = 'db.author_of.save(person_id,paper_id,{})\n'  #save and print after every person_id(statement_2)
            for FIELD in ENTITY['F']:
                field        = FIELD['FN']      #save attr
                fid          = FIELD['FId']     #save attr
                statement_1+='{2}name:"{0}", mag_id:"{1}"{3}, '.format(field,fid,LQ,RQ)
            statement_1+='],authors:['
            for PERSON in ENTITY['AA']:  
                author       = PERSON['AuN']    #save attr
                author_id    = PERSON['AuId']
                statement_1+='{2}name:"{0}", mag_id:"{1}"{3}, '.format(author,author_id,LQ,RQ)
            statement_1+=']})._id;\n'
            print statement_1
            for PERSON in ENTITY['AA']:  
                author       = PERSON['AuN']    # if(new person) new person with MAG_id & upgrade person with MAG_id & build relation
                author_id    = PERSON['AuId']
                statement_2 ='if(person_id = db._query(`for per in person filter per.name=="{0}" return per._id`).toArray()[0])\n'.format(author)
                statement_2+='    db._update(person_id, {1}MAG_id: "{0}" {2});\n'.format(author_id,LQ,RQ)
                statement_2+='else\n'
                statement_2+='    person_id = db.person.save({2} name:"{0}",MAG_id:"{1}" {3})._id\n'.format(author,author_id,LQ,RQ)
                print statement_2
                print statement_3

    except KeyError:
        print name
        print traceback.format_exc()
        empty_cnt+=1
        pass
    time.sleep(1)  #MAG allow Evaluate 1 time per second
    
print 'err_cnt : ' + str(empty_cnt) + " / " + str(len(name_list))
conn.close()


"""

Id -> MAG_id
Ti -> title
L -> language
Y -> year
D -> date
CC -> citation_cnt
F.FN -> field
F.FId -> MAG_file_id

AA.AuN AA.AuId -> person

if(!db.person.exists('911975')) db._collections()

db.person.save({  'name' : "徐國鎧", expertise:[{ name:'Tim',nid:'123' },{name:'John',nid:'456'}],email: "kkshy'u@ee.ncu.edu.tw"  })._id

"""