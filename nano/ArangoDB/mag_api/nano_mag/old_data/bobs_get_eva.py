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
for name in name_list[13:20]:
    fake_list.append(str(name))

empty_cnt=0

with open('paper_cmd', 'w') as arango_cmd:
    arango_cmd.write("db._dropDatabase('bob'); \n")
    arango_cmd.write("db._createDatabase('bob');\n")
    arango_cmd.write("db._useDatabase('bob');\n")
    arango_cmd.write("db._create('person');\n")
    arango_cmd.write("db._create('paper');\n")
    arango_cmd.write("db._createEdgeCollection('author_of')\n")
    arango_cmd.write("db.person.save({name : '陳永昇',expertise : 'cs', email:'test.email'});\n")

    for name in name_list:
    	print name + ' : '
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
    
        statement = "//----- {0} -----\n".format(name)
    	arango_cmd.write(statement)
        try:
            for ENTITY in json_data['entities']:  #save paper
                MAG_id       = ENTITY['Id']         #save attr
                title        = ENTITY['Ti']         #save attr
                try :
                    language     = ENTITY['L']          #save attr
                except:
                    print('empty language')
                    pass
                year         = ENTITY['Y']          
                date         = ENTITY['D']          #save attr
                citation_cnt = ENTITY['CC']         #save attr
                statement_1 = 'var paper_id = db.paper.save( {6} MAG_id:"{0}", title:"{1}", language:"{2}", year:"{3}", date:"{4}", citation_cnt:"{5}", fields:['.format(MAG_id,title,language,year,date,citation_cnt,LQ)
                statement_2 = 'var person_id;\n'
                arango_cmd.write(statement_2)
                statement_3 = 'db.author_of.save(person_id,paper_id,{})\n'  #save and print after every person_id(statement_2)
                try : 
                    for FIELD in ENTITY['F']:
                        field        = FIELD['FN']      #save attr
                        fid          = FIELD['FId']     #save attr
                        statement_1+='{2}name:"{0}", mag_id:"{1}"{3}, '.format(field,fid,LQ,RQ)
                except:
                    print('empty field')
                    pass
                statement_1+='],authors:['
                for PERSON in ENTITY['AA']:  
                    author       = PERSON['AuN']    #save attr
                    author_id    = PERSON['AuId']
                    statement_1+='{2}name:"{0}", mag_id:"{1}"{3}, '.format(author,author_id,LQ,RQ)
                statement_1+=']})._id;\n'
                arango_cmd.write(statement_1)
                for PERSON in ENTITY['AA']:  
                    author       = PERSON['AuN']    # if(new person) new person with MAG_id & upgrade person with MAG_id & build relation
                    author_id    = PERSON['AuId']
                    statement_2 ='if(person_id = db._query(`for per in person filter per.name=="{0}" return per._id`).toArray()[0])\n'.format(author)
                    statement_2+='    db._update(person_id, {1}MAG_id: "{0}" {2});\n'.format(author_id,LQ,RQ)
                    statement_2+='else\n'
                    statement_2+='    person_id = db.person.save({2} name:"{0}",MAG_id:"{1}" {3})._id\n'.format(author,author_id,LQ,RQ)
                    arango_cmd.write(statement_2)
                    arango_cmd.write(statement_3)
            print 'OK\n'
        except KeyError:
            print 'error : \n'
            print traceback.format_exc()
            empty_cnt+=1
            pass
        time.sleep(1)  #MAG allow Evaluate 1 time per second

    print 'err_cnt : ' + str(empty_cnt) + " / " + str(len(name_list))


    statement = 'var graph_module = require("@arangodb/general-graph");\n'
    statement+= 'var edgeDefinitions = graph_module._edgeDefinitions();\n' 
    statement+= 'var r1 = graph_module._relation("author_of", "person", "paper");\n'
    statement+='graph_module._extendEdgeDefinitions(edgeDefinitions, r1);\n'
    statement+='graph_module._create("graph1",edgeDefinitions);\n'
    arango_cmd.write(statement)

    arango_cmd.close()

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