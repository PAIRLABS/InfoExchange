## ArangoDB query method

step 0 : finish previous tutorials : `1_ArangoDB Concept.md` and `2_ArangoDB in Docker.md`

method 1 : arangosh (JavaScript)

method 2 : AQL

method 3 : python (accsee ArangoDB server and query by javascript or AQL)



#### method 1 : arangosh (JavaScript)

---

> source ：<https://docs.arangodb.com/3.4/Manual/Programs/Arangosh/>
>
> **Make sure** you have already finish pervious tutorial, especially lesson  `ArangoDB's Data Models` in `1_ArangoDB Concept.md`.



##### 1.1) introduction

​	The arangoDB shell (arangosh) is a command-line client tool than can be used for administration of ArangoDB servers.

​	It offer a V8 **JavaScript** shell environment, in which you can use JS interfaces and modules and modules like the `db` object to manage collections ,query data, access graph model or other feature.  However, it can also be used as interactive shell (REPL) to execute a JavaScript string or file. 



##### 1.2) invoke arangosh

​	execute you arangodb's container (we've already done in `2_ArangoDB in Docker.md`), and input `arangosh` to invoke your ArangoDB shell.

```
	[host] $ docke exec -it [container_name] /bin/sh
	
	[container] /# arangosh
	Please specify a password:[your_pwd]
	
	127.0.0.1:8529@_system>  [arangoshell]
```



##### 1.3) basic operation cmd to models in arangosh

##### ​	a. Database

​		create	：`db._createDatabase(<name>)`	

​		list 		：`db._databases()` 

​		use		：`db._useDatabase(<name>)`

​		drop	：`db._dropDatabase(<name>)`

```
//example
	127.0.0.1:8529@_system> db._createDatabase('test')
	true
	127.0.0.1:8529@_system> db._useDatabase('test')
	true
	127.0.0.1:8529@test>
```



##### ​	b. Collection   (collection_name = col_name)

​		create document collection ： `db._create(<name>)` or `db._createDocumentCollection(<name>)` 

​		create edge collection		  ：  `db._createEdgeCollection(<name>)`

​		list		：`db._collections()`

​		list one 	：`db._collection(<name>)` or `db.<col_name>`

​		drop	：`db._drop(<name>)` or `db.<col_name>.drop()`

```
//example
	127.0.0.1:8529@test> db._create('person')
	[ArangoCollection 156044, "person" (type document, status loaded)]
		
	127.0.0.1:8529@test> db._create('paper')
	[ArangoCollection 156059, "paper" (type document, status loaded)]
	
	127.0.0.1:8529@test> db._createEdgeCollection('author_of')
	[ArangoCollection 156069, "author_of" (type edge, status loaded)]
	
	127.0.0.1:8529@test> db._collections()
    [
      [ArangoCollection 156020, "_appbundles" (type document, status loaded)],
      [ArangoCollection 156014, "_apps" (type document, status loaded)],
      [ArangoCollection 155996, "_aqlfunctions" (type document, status loaded)],
      [ArangoCollection 155999, "_frontend" (type document, status loaded)],
      [ArangoCollection 155982, "_graphs" (type document, status loaded)],
      [ArangoCollection 156005, "_jobs" (type document, status loaded)],
      [ArangoCollection 155985, "_modules" (type document, status loaded)],
      [ArangoCollection 156002, "_queues" (type document, status loaded)],
      [ArangoCollection 155988, "_routing" (type document, status loaded)],
      [ArangoCollection 156069, "author_of" (type edge, status loaded)],
      [ArangoCollection 156059, "paper" (type document, status loaded)],
      [ArangoCollection 156044, "person" (type document, status loaded)]
    ]
	
```



##### 	c. Document (in a collection) 

​		**create document**：`db.<col_name>.save(<attr>)`

​		**update document**：`db._update(<id>,<attr>)` or `db.<col_name>.update(<key>,<attr>)`

​		**check exist**		 ：`db._exists(<id>)`

​		**list all document** ：`db.<col_name>.toArray()`

​		**count document** ：`db.<col_name>.count()`

​		**list a document** 	 ：`db._document(<id>)`  or     `db.<col_name>.document(<key>)`

​		**drop document**   ：`db._remove(<id>)`      or      `db.<col_name>.remove(<key>)`

​		**drop all documents**：`db.<col_name>.truncate()`

​		**Searching for document with specific attributes**：

​							`db.<col_name>.byExample( <attr> ).toArray()`

```
//example
	127.0.0.1:8529@test> db.person.save({ _key:'assign_key',name:'Ned',age:41 })
    {
      "_id" : "person/assign_key",
      "_key" : "assign_key",
      "_rev" : "_YfAvhn6---"
    }
    
    127.0.0.1:8529@test> db.person.update('person/assign_key',{age:42, weapen:'sword'})
    {
      "_id" : "person/assign_key",
      "_key" : "assign_key",
      "_rev" : "_YfA5hSS--_",
      "_oldRev" : "_YfAvhn6---"
    }

    
    127.0.0.1:8529@test> db.person.save({ name:'Arya',weapon:'niddle'  })
    {
      "_id" : "person/156218",
      "_key" : "156218",
      "_rev" : "_YfAxFJ----"
    }

    127.0.0.1:8529@test>  db.paper.save( {name:'Stark family note'} )
    {
      "_id" : "paper/156315",
      "_key" : "156315",
      "_rev" : "_YfA0Cf----"
    }

//creat edge
	127.0.0.1:8529@test> 
	db.author_of.save({_from:'person/assign_key',_to:'paper/156315', attr:'sth' } )
    {
      "_id" : "author_of/156377",
      "_key" : "156377",
      "_rev" : "_YfA1zJK---"
    }

	127.0.0.1:8529@test> db.person.toArray()
    [
      {
        "_key" : "156218",
        "_id" : "person/156218",
        "_rev" : "_YfAxFJ----",
        "name" : "Arya",
        "weapon" : "niddle"
      },
      {
        "_key" : "assign_key",
        "_id" : "person/assign_key",
        "_rev" : "_YfA5hSS--_",
        "name" : "Ned",
        "age" : 42,
        "weapen" : "sword"
      }
    ]

```



##### ​	d. AQL in arangosh

```
db._query(<AQL>).toArray()
```

```
//example

127.0.0.1:8529@test> db._query('for per in person filter per.age==42 return per.name').toArray()
[
  "Ned"
]

127.0.0.1:8529@test> db._query('for per in person filter per.age==42 return per').toArray()
[
  {
    "_key" : "assign_key",
    "_id" : "person/assign_key",
    "_rev" : "_YfA5hSS--_",
    "name" : "Ned",
    "age" : 42,
    "weapen" : "sword"
  }
]

127.0.0.1:8529@test> db._query('for per in person filter per.name=="Ned" for pap in 1..1 outbound per author_of return pap ').toArray()
[
  {
    "_key" : "156315",
    "_id" : "paper/156315",
    "_rev" : "_YfA0Cf----",
    "name" : "Stark family note"
  }
]


```



##### 	e. Other's operation：

```
[arangosh] > db.<press tab>
```





#### method 2 : AQL

---

##### 2.1) introduction

​	ArangoDB query language (AQL) is similar to the Structured Query Language (SQL). AQL support modifying collection data, but does not support data-definition operation such as creating and dropping databases and collections, which can be done by arangosh (Javascript) or web interface directly.   It is pure data manipulation language (DML), not a data definition language (DDL) or data control language(DCL).

##### 2.2) open your ArangoDb's web interface on browser 

​	Access it in your browser at：

```xml
	http://[host]:8529
```

​	and then log in 

```
	#default username ： root
	#password		  ： [your_pwd]   #set when docker run
```

##### 2.3) AQL tutorial：

> ​	source ： <https://docs.arangodb.com/3.4/AQL/Tutorial/>



#### method 3 : python (accsee ArangoDB server and query by AQL or JavaScript )

---

AQL directly ：

```python
#python 3

from arango import ArangoClient
LQ = "{"
RQ = "}"


arangodb_client = ArangoClient(protocol='http', host='e3.pairlabs.ai', port=8529)
db = arangodb_client.db('test', username='root', password='pair')
persons = db.aql.execute('for per in person return per.name')

for per in persons：
	print(per)

cmd = 'for per in person filter per.name=="Arya" update per._key with {0} age : 13 {1} in person'.format(LQ,RQ)
db.aql.execute(cmd)
    
print('finish')
```



JavaScript：

```python
#python 3

import os

with open('import_cmd', 'w') as arango_cmd:
    cmd = 'db.query(`for per in person filter per.name=="Arya" update per._key with {0} age : 14 {1} in person`)'.format(LQ,RQ)
    arango_cmd.write(cmd)
    
    arango_cmd.close()

f = os.popen(" arangosh --server.endpoint tcp://[host IP]:8529  --server.password [your_pwd] --javascript.execute import_cmd; ")

print('finish')

#f = os.popen(" arangosh --server.endpoint tcp://192.168.180.140:8529  --server.password 'pair' --javascript.execute second_import_cmd; ")
 
```

