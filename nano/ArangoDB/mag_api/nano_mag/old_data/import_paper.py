
#

if __name__ == '__main__':
	with open('import3', 'w') as arango_cmd:
		arango_cmd.write("db._useDatabase('test');\n")
        arango_cmd.write("db._create('paper');\n")
        arango_cmd.write("db._createEdgeCollection('author_of');\n")

        #statement += 'var person_id = db.person.save({3} name : "{0}", email : "{1}", expertise : "{2}"{4})._id;\n'.format(key,value[EMAIL],'DL,CNN,RNN',LQ,RQ)
        #db.person.save({  'name' : "徐國鎧", expertise:[{ name:'Tim',nid:'123' },{name:'John',nid:'456'},],email: "kkshy'u@ee.ncu.edu.tw"  })._id
        #db._update(person_id, {email: "correct" });
        #statement += 'var team_id = db.team.save( {2} name : "{0}" , email : "{1}" {3})._id;\n'.format(key,value[EMAIL],LQ,RQ)
        #statement += 'db.team_in_org.save(team_id, org_id,{2} department:"{0}", title:"{1}"{3});\n'.format(value[DEPARTMENT],value[TITLE],LQ,RQ)