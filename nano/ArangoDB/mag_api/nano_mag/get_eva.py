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
for name in name_list[0:35]:
    fake_list.append(str(name))

empty_cnt=0

with open('paper_cmd', 'w') as arango_cmd:
    arango_cmd.write("db._useDatabase('test');\n")
    arango_cmd.write("db._create('paper');\n")
    arango_cmd.write("db._createEdgeCollection('author_of')\n\n")
    arango_cmd.write("var person_id,paper_id\n\n")
    
    # problem_list1 = ['陳慶瀚','曾煜棋','蘇木春'] no problem
    problem_dict2 = { '陳樹群':'2148036750','曹昱':'2713448962','楊明德':'2164335109', '王文俊':'2801557857' ,'李蔡彥':'2138428116','郭建宏':'2574358011' }
    problem_dict3 = { '杭學鳴':'hsuehming hang','張添烜':'tiansheuan chang' }
    problem_list4 = ['王新民','蔡宗翰']

    arango_cmd.write("var problem_map2 = Object.create(null);\n")
    for k,v in problem_dict2.items():
    	statement = "problem_map2['{0}']='{1}' \n".format(k,v);
    	arango_cmd.write(statement)

    arango_cmd.write("var problem_map3 = Object.create(null);\n")
    for k,v in problem_dict3.items():
    	statement = "problem_map3['{0}']='{1}' \n".format(k,v);
    	arango_cmd.write(statement)


    for name in name_list:
        friend_num = 0

        if problem_dict2.has_key(name):
        	exprs = exprs = "Composite(AA.AuId="+ problem_dict2[name] + ")"
        elif problem_dict3.has_key(name):
        	exprs = "Composite(AA.AuN=='"+ problem_dict3[name] + "')"
        elif name in problem_list4:
        	continue
        else :
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
                statement_1+=']})._id;\n'
                arango_cmd.write(statement_1)
                for PERSON in ENTITY['AA']:
                    friend_num +=1  
                    author       = PERSON['AuN']    # if(new person) new person with MAG_id & upgrade person with MAG_id & build relation
                    author_id    = PERSON['AuId']
                    statement_2 ='    var mids = []\n' 
                    statement_2+='    if ("{0}"==problem_map3["{1}"] || "{2}"==problem_map2["{1}"]) {3} \n'.format(author,name,author_id,LQ)  # or author_id == problem2[name]
                    statement_2+='        //----- English Name (problem_dict2)-----\n'
                    statement_2+='        person_id = db._query(`for per in person filter per.name=="{0}" return per._id`).toArray()[0] \n'.format(name)
                    statement_2+='        mids = mids.concat(db._document(person_id)["MAG_id"]) \n'
                    statement_2+='        mids.push("{0}")\n'.format(author_id)
                    statement_2+='        mids = Array.from(new Set(mids.filter( function(e){0} return e {1} )))\n'.format(LQ,RQ)
                    statement_2+='        db._update(person_id, {1}English_name:"{0}",MAG_id: mids{2});   {2}\n'.format(author,LQ,RQ)
                    
                    statement_2+='    else if(person_id = db._query(`for per in person filter per.name=="{0}" return per._id`).toArray()[0]) {1}\n'.format(author,LQ)
                    statement_2+='        mids = mids.concat(db._document(person_id)["MAG_id"]) \n'
                    statement_2+='        mids.push("{0}")\n'.format(author_id)
                    statement_2+='        mids = Array.from(new Set(mids.filter( function(e){0} return e {1} )))\n'.format(LQ,RQ)
                    statement_2+='        db._update(person_id, {0}MAG_id: mids{1});  {1}\n'.format(LQ,RQ)
                    statement_2+='    else\n'
                    statement_2+='        person_id = db.person.save({2} name:"{0}",MAG_id:"{1}" {3})._id\n'.format(author,author_id,LQ,RQ)
                    arango_cmd.write(statement_2)
                    statement_3 ='    db.author_of.save(person_id,paper_id,{})\n'  #save and print after every person_id(statement_2)
                    arango_cmd.write(statement_3)
                arango_cmd.write("}\n")
                friend_num -=1
            #print  name + ' : ' + str(friend_num)
            if (friend_num>=50): print name + " : " + str(friend_num)
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



#db._query(aql`for per in person filter per.name=="蘇木春" for pap in 1..1 outbound per author_of for per2 in 1..1 inbound pap author_of return per2.name`).toArray().length