PI=0
COUNTRY=1
CITY=2
CO_TYPE=3
ORG=4
CO_NAME=5
DEPARTMENT=6
TITLE=7
EMAIL=8

LQ = "{"
RQ = "}"

import csv

if __name__ == '__main__':
    with open('pairlabs_cooperation.csv', newline='') as csvfile:
        lines = csv.reader(csvfile, delimiter=',')
        next(lines, None)  # skip the headers
        organizations = dict()
        cooperation_object = dict()
        cooperation = []
        for line in lines:
            organizations[line[ORG]]=line[COUNTRY]
            cooperation_object[line[CO_NAME]]=line[PI:EMAIL+1]
            cooperation.append((line[CO_TYPE],line[PI],line[CO_NAME]))
    csvfile.close()
    with open('co_arangosh_cmd', 'w') as arango_cmd:
        arango_cmd.write("db._useDatabase('test');\n")
        arango_cmd.write("db._create('team');\n")
        arango_cmd.write("db._createEdgeCollection('team_in_org');\n")
        arango_cmd.write("db._createEdgeCollection('cooperation_person');\n")
        arango_cmd.write("db._createEdgeCollection('cooperation_team');\n")
        for key,value in organizations.items():
            statement = 'db.org.save({2} name : "{0}" , conuntry : "{1}" {3});\n'.format(key,value,LQ,RQ)
            arango_cmd.write(statement)
        for key,value in cooperation_object.items():
            statement =  'var org_id = db._query(aql`for school in org filter school.name=="{0}" return school._id`).toArray()[0];\n'.format(value[ORG])
            if value[CO_TYPE]=="person":
                statement += 'var person_id = db.person.save({3} name : "{0}", email : "{1}", expertise : "{2}"{4})._id;\n'.format(key,value[EMAIL],'DL,CNN,RNN',LQ,RQ)
                statement += 'db.work.save(person_id, org_id,{2} department:"{0}", title:"{1}"{3});\n'.format(value[DEPARTMENT],value[TITLE],LQ,RQ)
            elif value[CO_TYPE]=="team":
                statement += 'var team_id = db.team.save( {2} name : "{0}" , email : "{1}" {3})._id;\n'.format(key,value[EMAIL],LQ,RQ)
                statement += 'db.team_in_org.save(team_id, org_id,{2} department:"{0}", title:"{1}"{3});\n'.format(value[DEPARTMENT],value[TITLE],LQ,RQ)
            arango_cmd.write(statement)
        for t in cooperation:
            statement = 'var pi_id = db._query(aql`for pi in person filter pi.name=="{0}" return pi._id`).toArray()[0];\n'.format(t[1])
            if t[0]=="person":
                statement+= 'var cooperator_id = db._query(aql`for coo in person filter coo.name=="{0}" return coo._id`).toArray()[0];\n'.format(t[2])
                statement+= 'db.cooperation_person.save(pi_id,cooperator_id, { });\n'
            elif t[0]=="team":
                statement+= 'var team_id = db._query(aql`for te in team filter te.name=="{0}" return te._id`).toArray()[0];\n'.format(t[2])
                statement+= 'db.cooperation_team.save(pi_id,team_id,{ });\n'
            arango_cmd.write(statement)
    arango_cmd.close()
