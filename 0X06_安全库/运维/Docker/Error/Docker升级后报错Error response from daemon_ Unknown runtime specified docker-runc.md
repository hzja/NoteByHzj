Docker
<a name="xzAps"></a>
## Docker升级后报错如下
```bash
Error response from daemon: Unknown runtime specified docker-runc
Error: failed to start containers: mysql
```
<a name="yrBxS"></a>
## 解决方法：
更改/var/lib/docker/containers目录中的文件参数，把docker-runc替换为runc<br />命令：
```bash
grep -rl 'docker-runc' /var/lib/docker/containers/ | xargs sed -i 's/docker-runc/runc/g'
```
:::tips
注：`grep -rl`：递归搜索目录和子目录，只列出含有匹配的文本行的文件名，而不显示具体的匹配内容<br />`xargs`：衔接执行之前得到的值<br />总体意思是把/var/lib/docker/containers中含有‘docker-runc’的文件搜索出来，并把‘docker-runc’字符为runc
:::

重新启动docker
```bash
systemctl restart docker
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631846365627-25925042-2d36-46e6-a015-f9a887c123be.png#clientId=u5d28bd39-33ca-4&from=paste&height=470&id=u233ae840&originHeight=1411&originWidth=3323&originalType=binary&ratio=1&size=1782693&status=done&style=none&taskId=u7856de0e-e64d-4c67-9855-1daaa69fd82&width=1107.6666666666667)
