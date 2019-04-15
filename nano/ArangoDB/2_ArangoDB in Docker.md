## ArangoDB in Docker

> step 0 : finish previous tutorial : `1_ArangoDB Concept.md`
>
> step 1 : docker run arangodb
>
> step 2 : View your ArangoDb's web interface on browser 
>
> step 3 : next tutorial：`3_ArangoDB query method.md`

#### step 1 : docker run ArangoDB

---

> images source : [docker hub](https://hub.docker.com/_/arangodb)

##### 1.1) docker run 

```
[host] $ docker run -v [local-path]:[docker-path] -e ARANGO_ROOT_PASSWORD=[your_pwd] -p 8529:8529 --name [container_name] -d arangodb
```

> ※ `-v` to persist your data by  volume ( input / output file or .js )
>
> ※ `-e` to choose authentication way, except  `ARANGO_ROOT_PASSWORD`, `ARANGO_RANDOM_ROOT_PASSWORD` and `ARANGO_NO_AUTH`, view [docker hub](https://hub.docker.com/_/arangodb) for detail.
>
> ※ Arangodb listen to port `8529` , the `-p 8529:8520` expose this port to host



##### 1.2) docker exec (execute container's shell)

```
[host] $ docke exec -it [container_name] /bin/sh
[container] /# [cmd]
```



#### step 2 : View your ArangoDb's web interface on browser 

---

After docker run your container, you can access ArangoDB web interface in your browser at：

```xml
http://[host]:8529
```

and then log in 

```
#default username ： root
#password		  ： [your_pwd]   #set when docker run
```

if nothing strange happened, Congratulations!  You and your database are ready for fighting! 



#### step 3 : next tutorial：`3_ArangoDB query method.md`

---

view next tutorial.