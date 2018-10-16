# OpenPAI Manual

### Note: Because the Development of OpenPAI is still unstable, so we decide to focus on specific version (v0.7.1) in order to maintain the consistency of environment setup 

### Generate Configuration by Dev-box 
1. Refer the setup step from Microsoft
[OpenPAI Official Setup Tutorial](https://github.com/Microsoft/pai/blob/v0.7.1/pai-management/doc/cluster-bootup.md#singlebox)
2. The configuration of quick-start.yaml, note that the machine contain the dev-box container can't be assigned as neither master or worker machine. The dev-box machine should only servers as a maintain purpose.
3. To generate the configuration from quick-start.yaml, use the   
`./paictl.py cluster generate-configuration -i quick-start/quick-start.yaml -o ~/pai-config -f`  
, refer from [Microsoft/pai: 2ec8f9a](https://github.com/Microsoft/pai/commit/2ec8f9a6b30355a1775aeb1fef7639331a88100d)
### Customize the Config
1. In the pai-config/cluster-configuratio.yaml, which lists all the machine specs and roles in the cluster. you can overwrite the machine spec if the detection of specs is wrong or GENERIC (Define the machine SKU), the correction of machine information will decrease the chance the situation of No Detected Node Number or Wrong GPU Number Detected on PAI Dashboard.   
![No Detected Node Number on Dashboard] (https://trello-attachments.s3.amazonaws.com/5b927169f92efd6e23d36606/5b927942c82934201c00d412/20fd83d09847a4def0353282e16037aa/Screen_Shot_2018-09-10_at_11.51.57_AM.png)
2. In the pai-config/services-configuration.yaml, there're many trivial naming you can set, however, you must overwrite the docker-tag to "v0.7.1", it's important otherwise the later deployment will fail due the inconsistent between docker containers.
### Deployment
1. Can refer to the previous official setup tutorial   
[OpenPAI Official Setup Tutorial](https://github.com/Microsoft/pai/blob/v0.7.1/pai-management/doc/cluster-bootup.md#singlebox)
2. After deploy the cluster successfully, The OpenPAI service will run up some test jobs periodically. You can terminate this function trough out paictl.py in Dev-Box  
`python paictl.py service delete -p $CONFIG_PATH -n end-to-end-test`
### HDFS Data Upload Mechanism
* Because user can only update their data trough HDFS Web or API interface, which is not convenience. We can set up a ftp service with HDFS file system mounted on the Master Node, and user can upload their dataset easily. The following are the setup steps.
 1. Add the CDH5 Repository  
```
$ wget http://archive.cloudera.com/cdh5/one-click-install/trusty/amd64/cdh5-repository_1.0_all.deb
$ sudo dpkg -i cdh5-repository_1.0_all.deb
```
2. Install the Hadoop-Dfs-Fuse Tool  
```
$ sudo apt-get update
$ sudo apt-get install hadoop-hdfs-fuse
```
3. Mount to Local System  
```
$ mkdir -p your-mount-directory
$ sudo hadoop-fuse-dfs dfs://hdfs-name-node-address:9000 your-mount-directory
```
### Disclosure of OpenPAI Platform
1. About the GPU maximum setting, Maximum GPU number depends on SKU of your machine  
2. The User Login Flow of Platform  
a. Web Portal is basically an agent, which send the username and password to REST Server, and get Token back as cookie. [Login Component](https://github.com/Microsoft/pai/blob/master/webportal/src/app/user/user-login/user-login.component.js)  
b. All login credential will run through REST Server.
Username and password will save in Etcd2,
With ExpressJWT, after username and password authorized by Etcd2, ExpressJWT will give out a Token.
REST Server will determine which is admin based on the Token.
[Token Ref.](https://github.com/Microsoft/pai/blob/master/rest-server/src/models/token.js) and [DBUtil Ref.](https://github.com/Microsoft/pai/blob/9b445f7d04777185c05b2de198d7901574e7c4fc/rest-server/src/util/dbUtil.js)
3. About The Role of FrameworkLauncher, which is served as a middleware between Resource Manager and RestAPI Server, and Framework itself also have a RestAPI to receive jobs details.








