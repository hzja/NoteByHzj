某次应急中，客户吓坏了，说是内网流量分析设备中有很多webshell连接告警，作为一名卑微但又不失理想的安服仔，毅然直奔前线....

<a name="RMClX"></a>
## 过程
去到现场后，直接打开客户的流量分析设备，的确看到一堆冒红的webshell连接告警，看到来源ip有公网的，直接先排查公网对内部系统的攻击，但是排查后，发现目标攻击的系统并没有被入侵，但为什么内部系统会有大量的内网对内网webshell连接尝试何失败日志？如下图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1650378698058-8c9c52f5-8d39-46c1-a900-ae0d94ad8215.png#clientId=u15010547-63e1-4&from=paste&height=142&id=ufcb33b1c&originHeight=177&originWidth=887&originalType=binary&ratio=1&rotation=0&showTitle=false&size=65389&status=done&style=none&taskId=u88af57fe-8421-4ca8-99c9-e61136ddb9e&title=&width=709.6)

于是申请到15.1的主机上进行排查分析，登陆到192.168.15.1服务器上进行排查，发现无异常外联<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1650379879884-6efe32bb-8323-4392-82fd-d9192d971636.png#clientId=u15010547-63e1-4&from=paste&height=174&id=u5d45a5a9&originHeight=218&originWidth=927&originalType=binary&ratio=1&rotation=0&showTitle=false&size=232676&status=done&style=none&taskId=ub3a49184-52de-4c01-a2ca-2dacf36a693&title=&width=741.6)

也没有异常的进程，查看最近登陆的日期，发现并无任何异常。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1650378882685-53088496-5272-4d0d-992c-f15bf4c71c47.png#clientId=u15010547-63e1-4&from=paste&height=545&id=uc8e8e252&originHeight=681&originWidth=1169&originalType=binary&ratio=1&rotation=0&showTitle=false&size=561536&status=done&style=none&taskId=ucdb9c6ef-b629-4be6-8959-7b90d2a587c&title=&width=935.2)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1650378930303-46c1f73d-5424-4c3c-b7aa-b2e72ce16a01.png#clientId=u15010547-63e1-4&from=paste&height=247&id=u5f4816aa&originHeight=309&originWidth=844&originalType=binary&ratio=1&rotation=0&showTitle=false&size=208836&status=done&style=none&taskId=u25344762-93cf-446e-9e82-10baec55370&title=&width=675.2)

最后排查到该主机具备公网域名，并且有做nginx配置，查看nginx配置存在include *.conf ，所以对/usr/local/nginx/conf.d/目录下进行排查<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1650379145639-6ff1e8a2-1d81-4e6e-b2f7-e0864f022aa4.png#clientId=u15010547-63e1-4&from=paste&height=583&id=ua20c7188&originHeight=729&originWidth=813&originalType=binary&ratio=1&rotation=0&showTitle=false&size=299369&status=done&style=none&taskId=u5a94217b-0595-4f78-9e6d-0da0857f1f3&title=&width=650.4)

所以对/usr/local/nginx/conf.d/目录下进行排查，最后终于发现了问题，app.conf文件配置了内网15.8 ，15.9 ，15.10的分发，<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1650379277979-6281d534-c38a-4b04-a79d-a9293666907f.png#clientId=u15010547-63e1-4&from=paste&height=444&id=ud2942212&originHeight=555&originWidth=1029&originalType=binary&ratio=1&rotation=0&showTitle=false&size=247826&status=done&style=none&taskId=ud5ff473e-28fd-4c05-873f-3b899a8992b&title=&width=823.2)

查看nginx的access.log日志，发现外网攻击者扫描过这个nginx，以及许多扫描的webshell名字路径。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1650379424396-f2f8a150-1342-48a2-8d89-7fedfbab03db.png#clientId=u15010547-63e1-4&from=paste&height=134&id=u8bcce173&originHeight=168&originWidth=909&originalType=binary&ratio=1&rotation=0&showTitle=false&size=187960&status=done&style=none&taskId=u6fb30960-b205-4549-9ad0-f0072629c42&title=&width=727.2)

所以基本可以确定，因为测试区的nginx能被互联网访问，所以攻击者扫描外网时，nginx把扫描的路径分发到内网的几台服务器中，并且流量设备只是简单的对文件名进行了常见webshell名字匹配，匹配成功则出现告警，没有判断响应包的状态码是否正常，所以产生了很多内网对内网ip的webshell连接告警，实际上，服务器并没有沦陷，只是攻击者在外网的扫描行为触发。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1650379541456-84fe291e-719c-4efc-943b-3e02124eb48d.png#clientId=u15010547-63e1-4&from=paste&height=334&id=u1ee77740&originHeight=418&originWidth=1038&originalType=binary&ratio=1&rotation=0&showTitle=false&size=105067&status=done&style=none&taskId=u06a3ba4d-8008-4688-8c8d-9c78707179d&title=&width=830.4)

<a name="sPg6d"></a>
## 后话
回头看，原来就这么一回事，但是实际上，在客户现场折腾了一天，踩了不少坑，反推了不少思路，最后才排查出来的问题，有进步就是好事了，安服的宿命就是如此。


