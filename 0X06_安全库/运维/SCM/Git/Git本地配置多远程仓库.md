Git
<a name="qBrlA"></a>
# 1、生成各远程库的公钥私钥
```bash
$ ssh-keygen -t rsa -f ~/.ssh/id_rsa_gitlab -C "fcscanf@outlook.com"
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596520990159-f00f2108-ba15-4dde-938e-14789caa71c4.png#align=left&display=inline&height=424&originHeight=1272&originWidth=2278&size=232790&status=done&style=shadow&width=759.3333333333334)
<a name="BWbFn"></a>
# 2、将公钥添加到远程仓库
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596521055346-c9d9e548-7a7e-44ff-ad5f-98ecaa6c319d.png#align=left&display=inline&height=469&originHeight=1408&originWidth=2614&size=506019&status=done&style=none&width=871.3333333333334)
<a name="L846h"></a>
# 3、在用户目录下的.ssh文件夹下创建config文件
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596521168031-768c4d9b-d782-473c-8c22-5aefe0e4db8c.png#align=left&display=inline&height=554&originHeight=1663&originWidth=2189&size=305833&status=done&style=none&width=729.6666666666666)
<a name="pXpVe"></a>
# 4、在config文件配置远程仓库的信息
```
#注释说明
#Host 别名
#HostName 域名或ip
#User 登陆服务器用的账号
#Port 端口号(默认22，可以不填，如果服务器修改了ssh登录端口号，此处需要修改)
#IdentityFile 密钥文件的位置

#user for github
Host github.com
HostName github.com
User git
IdentityFile ~/.ssh/id_rsa
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596521213873-d2330eb1-f41f-4427-ad24-fea043eabaf0.png#align=left&display=inline&height=597&originHeight=1792&originWidth=2230&size=349403&status=done&style=none&width=743.3333333333334)
