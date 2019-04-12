## ArangoDB's Concept

> source ：[ArangoDB](https://docs.arangodb.com/3.4/Manual/index.html)

> 1.) ArangoDB introduction
>
> 2.) ArangoDB's Data Models
>
> 3.) next tutorial：`2_ArangoDB in Docker.md`



#### 1.) ArangoDB introduction

---

​	ArangoDB is a native multi-model, open-source **database** with flexible **data models for documents, graphs, and key-values.** 

​	Build high performance applications using a convenient SQL-like query language(**AQL**) or **JavaScript** extensions.



#### 2.) ArangoDB's Data Models

---

##### a. document : 

​	**A document** Can be seen as the basic element in ArangoDB, which can represent a entity (vertex in graph) or relationship (edge in graph).

​	A document contains zero or more **attributes**, each of them having a value, which can either be 

​		atomic type : **number**, **string**, **boolean** or **null**

​		compound type : **array** or embedded **document/object**.

​	Arrays and sub-object can contain all of these types, which means that **nested data structure** can be represented in a single document：

```
# example : a document represent a person who names'Ned Stark', age 41, trait...,etc.
[
  {
    "_key": "91814",
    "_id": "person/91814",
    "_rev": "_Yeq4OaK---",
    "name": "Ned",
    "surname": "Stark",
    "alive": true,
    "age": 41,
    "traits": {
      "hair": "brown",
      "sigil": "grey direwolf",
      "son": [
        "Robb",
        "Arya",
        "John",
        "..."
      ]}}]
```

​	※documents stored in a binary format call [VelocyPack](https://github.com/arangodb/velocypack) ,which is closely follow the **JSON format.**

​	

​	Otherwise, note that there are **three special attributes** will automatically generated when a new document is create, they are `_key` `_id` and `_rev`：

​	`_key`：a unique string represent this document, can be assigned by unused string or be randomized when nothing assign.

​	`_id`：<collections_name> + '/' + `_key`

​	`_rev`：a unique string represent **revision number of  document**, can be used for conditional modifications. Here is an example of how to avoid lost updates in case multiple clients are accessing the documents in parallel：

```javascript
//example in arangosh, by javascript

doc = db._document("person/91814")        //assign Ned Stark's document to variable doc
db._update("person/91814",{ age : 42 })  // OK
db._update(doc,{age : 43 }) //error,as revision number not match, update is rejected. 
```



​	Furthermore, when a document represent a **relationship** ( edge in graph), it will has two more special attribute, `_from` and `_to`, which used to store the `_id` of two documents in this relationship.



##### b.) collections :

​	Documents are grouped into **collections**, A collection contains zero or more documents. For usual, we store similar type of document in the same collection, for example : document "Ned stark" and "Jacky Cheng" store in collection "person", document "NASA" store in collection "organization".   

​	However, ArangoDB is schema-less, which means that there is no need to define what attribute should have, every single document can have a completely different structure and still stored together in a single collection, but it will indeed increase your query time according with the amount of data and the complexity of query code.

​	Moreover, as we mentioned in document , there are two type of collections： **document collection** and **edge collection**：

​	**document collection**：also named as vertex collections, use to store normal document.

​	**edge collection**：also store document, but they include two special attributes：`_from` and `_to` (as mentioned)

​	In other words, two documents (vertices) stored in document collections are linked by a document (edge) stored in an edge collection, and form ArangoDB's **graph data model**.



##### c.) database:

​	Collections exist inside of database. There can be one or more database. Different databases' data are isolated from each others. The default database `_system` is special, which can not be removed and has authority to manage other database .



### 3.) next tutorial： `2_ArangoDB in Docker.md`

---

next  .md

