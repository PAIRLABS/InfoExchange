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
for name in name_list[0:15]:
    fake_list.append(str(name))

empty_cnt=0

with open('paper_cmd', 'w') as arango_cmd:
    arango_cmd.write("db._useDatabase('test');\n")
    arango_cmd.write("db._create('paper');\n")
    arango_cmd.write("db._createEdgeCollection('author_of')\n")
    arango_cmd.write("var person_id,paper_id\n")
    for name in fake_list:
        friend_num = 0

        exprs = "Composite(AA.AuN=='"+ name + "')"
  	
        params = urllib.urlencode({
            # Request parameters
            'expr': exprs,    #"Composite(AA.AuN=='徐國鎧')",
            'model': 'latest',
            'count': '100',
            'offset': '0',
            'orderby': '',
            'attributes': 'Id,Ti,Y,D,L,CC,AA.AuN,AA.AuId,F.FN,F.FId',
        })

        conn.request("GET", "/academic/v1.0/evaluate?%s" % params, "{body}", headers)
        response = conn.getresponse()
        data = response.read()
        json_data = json.loads(data);  #to list type
    
        statement = "\n//----- {0} -----\n".format(name)
        arango_cmd.write(statement)
        try:
            author_onlyid = '1'
            for ENTITY in json_data['entities']:  #save paper
                MAG_id       = ENTITY['Id']         #save attr
                title        = ENTITY['Ti']         #save attr
                try :
                    language     = ENTITY['L']          #save attr
                except:
                    pass
                year         = ENTITY['Y']          
                date         = ENTITY['D']          #save attr
                citation_cnt = ENTITY['CC']         #save attr
                statement_1 = 'if (!(paper_id=db._query(`for pap in paper filter pap.MAG_id=="{0}" return pap._id`).toArray()[0])) {1}\n'.format(MAG_id,LQ)
                statement_1 += '    paper_id = db.paper.save( {6} MAG_id:"{0}", title:"{1}", language:"{2}", year:"{3}", date:"{4}", citation_cnt:"{5}", fields:['.format(MAG_id,title,language,year,date,citation_cnt,LQ)
                try : 
                    for FIELD in ENTITY['F']:
                        field        = FIELD['FN']      #save attr
                        fid          = FIELD['FId']     #save attr
                        statement_1+='{2}name:"{0}", mag_id:"{1}"{3}, '.format(field,fid,LQ,RQ)
                except:
                    pass
                statement_1+='],authors:['
                for PERSON in ENTITY['AA']:  
                    author       = PERSON['AuN']    #save attr
                    author_id    = PERSON['AuId']
                    statement_1+='{2}name:"{0}", mag_id:"{1}"{3}, '.format(author,author_id,LQ,RQ)
                statement_1+=']})._id;   }\n';
                arango_cmd.write(statement_1)
                for PERSON in ENTITY['AA']:
                    friend_num +=1  
                    author       = PERSON['AuN']    # if(new person) new person with MAG_id & upgrade person with MAG_id & build relation
                    author_id    = PERSON['AuId']
                    statement_2 ='if(person_id = db._query(`for per in person filter per.name=="{0}" return per._id`).toArray()[0])\n'.format(author)
                    statement_2+='    db._update(person_id, {1}MAG_id: "{0}" {2});\n'.format(author_id,LQ,RQ)
                    statement_2+='else\n'
                    statement_2+='    person_id = db.person.save({2} name:"{0}",MAG_id:"{1}" {3})._id\n'.format(author,author_id,LQ,RQ)
                    arango_cmd.write(statement_2)
                    statement_3 = 'db.author_of.save(person_id,paper_id,{})\n'  #save and print after every person_id(statement_2)
                    arango_cmd.write(statement_3)
                friend_num -=1
            print  name + ' : ' + str(friend_num)
            if (friend_num<=0) : empty_cnt+=1
        except KeyError:
            print 'error : \n'
            print traceback.format_exc()
            empty_cnt+=1
            pass
        time.sleep(1)  #MAG allow Evaluate 1 time per second

    print 'err_cnt : ' + str(empty_cnt) + " / " + str(len(name_list))


    arango_cmd.close()

conn.close()