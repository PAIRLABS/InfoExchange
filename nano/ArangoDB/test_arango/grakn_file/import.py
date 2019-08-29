PROJ=0
NAME=1
ORG=2
DEPT=3
TITLE=4
EMAIL=5
PHONE=6
MOBILE=7
POSITION=8


import csv

if __name__ == '__main__':
    with open('pairlabs-pi.csv', newline='') as csvfile:
        lines = csv.reader(csvfile, delimiter=',')
        next(lines, None) 
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
    with open('import_data.gql', 'w') as gqlfile:
        for org in organizations:
            statement = "insert $o isa organization has name '{}';\n".format(org)
            gqlfile.write(statement)
        for proj in projects:
            statement = "insert $p isa project has title '{}', has keyword '{}';\n".format(proj,'DL,CNN,RNN')
            gqlfile.write(statement)
        for key,value in professors.items():
            statement = "insert $p isa person has name '{}', has email '{}', has expertise '{}';\n".format(key,value[3],'DL,CNN,RNN')
            gqlfile.write(statement)
            statement = "match $p isa person has name '{}'; $o isa organization has name '{}'; insert $nr (Institute: $o, Researcher: $p) isa work has department '{}', has title '{}';\n".format(key,value[0],value[1],value[2])
            gqlfile.write(statement)
        for t in participation:
            statement = "match $p isa person has name '{}'; $o isa project has title '{}'; insert $nr (Proj: $o, Participant: $p) isa participation has position '{}';\n".format(t[1], t[0], t[2])
            gqlfile.write(statement)
    gqlfile.close()
