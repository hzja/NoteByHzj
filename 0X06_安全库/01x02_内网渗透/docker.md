_Docker启动命令_: systemctl start docker

docker images 查看下载过的镜像<br />docker image ls：列出所有的镜像  

docker-compose up -d <br />docker exec -it d2c06d8f5809 /bin/bash <br /> <br />迅速判断docker环境<br />root@d2c06d8f5809:/# ls -alh /.dockerenv<br />root@d2c06d8f5809:~# cat /proc/1/cgroup<br /> <br />docker pull了镜像之后启动容器：<br />docker run -d -p 8080:8080 -v /var/run/docker.sock:/var/run/docker.sock -e VUL_IP=192.168.201.129 98ae1c9f436d

直接启动服务<br />docker run -d images的id<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693756724480-2ad664c5-9255-4f2d-9325-a4ccae3bc118.png#averageHue=%23262a35&clientId=uf23c376e-7f4b-4&from=paste&height=64&id=uc31db663&originHeight=80&originWidth=555&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=18987&status=done&style=none&taskId=u48d77242-464b-466a-8b95-dca5b577359&title=&width=444)

dokcer容器无法使用的命令<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611400649483-19b9e976-4cb6-4661-be38-f54be315ceb1.png#averageHue=%23090704&height=215&id=YBAoC&originHeight=430&originWidth=986&originalType=binary&ratio=1&rotation=0&showTitle=false&size=206325&status=done&style=none&title=&width=493)<br />Bash命令权限可能不够<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611400761558-c1fa13c6-54e7-4edf-bc46-fa5ef81cf5b0.png#averageHue=%23100d09&height=51&id=as5qQ&originHeight=102&originWidth=884&originalType=binary&ratio=1&rotation=0&showTitle=false&size=61583&status=done&style=none&title=&width=442)

docker镜像保存<br />[https://www.php.cn/faq/489196.html](https://www.php.cn/faq/489196.html)

docker镜像导出\导入<br />[https://blog.csdn.net/weixin_42857718/article/details/131192738](https://blog.csdn.net/weixin_42857718/article/details/131192738)

docker镜像上传<br />[https://www.python100.com/html/85074.html](https://www.python100.com/html/85074.html)

docker的命令详解<br />[https://blog.csdn.net/m0_51913750/article/details/131563704](https://blog.csdn.net/m0_51913750/article/details/131563704)

<a name="dKgM4"></a>
## 利用Dockerfile进行命令执行

- 首先得知道已有的docker镜像

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693755970609-086d635e-10f2-4dca-a487-9eb4e321ef5b.png#averageHue=%23272b36&clientId=uf23c376e-7f4b-4&from=paste&height=102&id=u1a510d57&originHeight=128&originWidth=654&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=39030&status=done&style=none&taskId=u66a88bc6-1730-4161-b616-259246e5e7a&title=&width=523.2)

- 构建Dokcerfile
```
FROM cidersecurity/goat-gitea
RUN echo "aufeng" >>aufeng.txt
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693756472606-a182c5c1-0677-406c-8144-75cf7078302c.png#averageHue=%23252933&clientId=uf23c376e-7f4b-4&from=paste&height=86&id=uc5f089ee&originHeight=108&originWidth=415&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=13291&status=done&style=none&taskId=u0fd2d621-b061-4252-bfde-ae0abdef751&title=&width=332)

- 镜像构建

docker build -t aufeng .<br />-t后面是新构建的镜像名称，点号是指该目录下的文件，build构建时会自动寻找Dockerfile进行构建<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693756608022-f377b41d-f2ec-4db1-b760-e8270532f706.png#averageHue=%23252934&clientId=uf23c376e-7f4b-4&from=paste&height=300&id=ua3ebdfca&originHeight=375&originWidth=720&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=93751&status=done&style=none&taskId=ufffc29b4-a16e-470c-878a-d333594ef9f&title=&width=576)

- 进入container查看

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693756670917-d42d7577-b71f-4f85-b987-5208951b8120.png#averageHue=%23262b36&clientId=uf23c376e-7f4b-4&from=paste&height=59&id=u94d360e4&originHeight=74&originWidth=579&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=18402&status=done&style=none&taskId=u6c022442-0769-47d9-b668-761bf865025&title=&width=463.2)<br />可以发现Dockerfile构建时run执行的命令已经成功执行<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693756769878-fb919752-bcfb-4c1d-9d93-b396a0bc9215.png#averageHue=%23242834&clientId=uf23c376e-7f4b-4&from=paste&height=156&id=u2f7d3ce4&originHeight=195&originWidth=750&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=50899&status=done&style=none&taskId=u5580e84b-d579-4500-b6be-49dd631ed43&title=&width=600)
