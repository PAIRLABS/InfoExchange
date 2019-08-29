PROJ=0
NAME=1
ORG=2
DEPT=3
TITLE=4
EMAIL=5
PHONE=6
MOBILE=7
POSITION=8
lq="{"
rq="}"

import csv

if __name__ == '__main__':
    with open('pairlabs-pi.csv', newline='') as csvfile:
        lines = csv.reader(csvfile, delimiter=',')
        next(lines, None)  # skip the headers
        projects = set()
        organizations = set()
        professors = dict()
        participation = []
        for line in lines:
            proj = '/'.join(line[PROJ].split('/')[1:])
            projects.add(proj)
            organizations.add(line[ORG])
            professors[line[NAME]]=line[ORG:POSITION+1]
            participation.append((proj,line[NAME],line[POSITION]))
    csvfile.close()
    with open('arangosh_cmd', 'w') as arango_cmd:
        arango_cmd.write("db._dropDatabase('test'); \n")
        arango_cmd.write("db._createDatabase('test');\n")
        arango_cmd.write("db._useDatabase('test');\n")
        arango_cmd.write("db._create('person');\n")
        arango_cmd.write("db._create('proj');\n")
        arango_cmd.write("db._create('org');\n")
        arango_cmd.write("db._createEdgeCollection('participation');\n")
        arango_cmd.write("db._createEdgeCollection('work');\n")
        for org in organizations:
            statement = "db.org.save({1} name : '{0}'{2});\n".format(org,lq,rq)
            arango_cmd.write(statement)
        for proj in projects:
            statement = "db.proj.save({2} title : '{0}', keyword : '{1}'{3});\n".format(proj,'DL,CNN,RNN',lq,rq)
            arango_cmd.write(statement)
        for key,value in professors.items():
            statement = "var person_id = db.person.save({3} name : '{0}', email : '{1}', expertise : '{2}'{4})._id;\n".format(key,value[3],'DL,CNN,RNN',lq,rq)
            arango_cmd.write(statement)
            statement =  "var org_id = db._query(`for school in org filter school.name=='{0}' return school._id`).toArray()[0];\n".format(value[0])
            statement += "db.work.save(person_id, org_id,{2} department:'{0}', title:'{1}'{3});\n".format(value[1],value[2],lq,rq)
            arango_cmd.write(statement)
        for t in participation:
            statement = "var person_id = db._query(`for per in person filter per.name=='{0}' return per._id`).toArray()[0];\n".format(t[1])
            statement+= "var proj_id = db._query(`for pro in proj filter pro.title=='{0}' return pro._id`).toArray()[0]; \n".format(t[0])
            statement+= "db.participation.save(person_id, proj_id, {1}position : '{0}' {2}  );\n".format(t[2],lq,rq) 
            arango_cmd.write(statement)
    arango_cmd.close()


""" statement += "var person_id = db.query(`for per in person filter per.name=='{0}' return per_id`).toArray()[0];\n".format(key)  """

""" var org_id = db._query(`for school in org filter school.name=='{0}' return school._id`).toArray()[0] \n """
"""statement = " var proj_id = db._query(`for pro in proj filter pro.title='{0}' return pro._id`).toArray()[0]; \n
                var person_id = db.query(`for per in person filter per.name=='{1}' return per_id`).toArray()[0]; \n 
                db.participation.save(person_id,proj_id,{position:'{2}'}); ".format(value[0],key,value[8])"""