<a name="coc3p"></a>
### 报错内容：
```bash
Permission denied, please try again
```
<a name="VOtzT"></a>
### 解决办法：
修改ssh的配置文件，允许root账号以密码方式登录。
```bash
vim /etc/ssh/sshd_config
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625068121208-01d27f49-c4f3-4976-befb-fd943bbf24ae.png#clientId=ufc1263c2-e2d3-4&from=paste&height=583&id=u493c3d9c&originHeight=1750&originWidth=3323&originalType=binary&ratio=3&size=2072523&status=done&style=none&taskId=u1a234c5b-33bc-434e-8474-9bc2c96c227&width=1107.6666666666667)<br />默认禁止Root用户使用密码登录，将这一行注释放开，值配置为"yes"即可<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625068140856-d75daf8d-cc15-465d-832f-af090da27e57.png#clientId=ufc1263c2-e2d3-4&from=paste&height=583&id=u6003f8d5&originHeight=1750&originWidth=3323&originalType=binary&ratio=3&size=2022908&status=done&style=none&taskId=u71f05033-755b-4850-8857-883e9053ff6&width=1107.6666666666667)<br />然后重启ssh服务
```bash
service ssh restart
```
