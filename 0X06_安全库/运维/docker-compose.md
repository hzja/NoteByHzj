<a name="A08tQ"></a>
## 一、安装compose
官网地址：[https://docs.docker.com/compose/](https://docs.docker.com/compose/)<br />官网上面安装的compose是从GitHub上面去下载一个文件![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1651021375923-bbb9679a-8424-4150-875d-ff13f3722d75.png#clientId=u4ebc3155-0e64-4&from=paste&height=668&id=u42b88cbb&originHeight=919&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=224296&status=done&style=none&taskId=u84a60f27-4add-4ff1-9e14-daa8b14730b&title=&width=1396.3636363636363)<br />国内安装地址
```shell
sudo curl -L https://get.daocloud.io/docker/compose/releases/download/1.25.1/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose

sudo chmod +x /usr/local/bin/docker-compose
root@ubuntu:~# docker-compose -v
docker-compose version 1.25.0, build unknown
root@ubuntu:~#
docker-compose version 1.25.0, build unknown
docker-py version: 4.1.0
CPython version: 3.9.5
OpenSSL version: OpenSSL 1.1.1j  16 Feb 2021
root@ubuntu:~#

```
<a name="GF0q5"></a>
## 二、cosmpose体验
[https://docs.docker.com/compose/gettingstarted/](https://docs.docker.com/compose/gettingstarted/)<br />docker官方案例演示
```python
 mkdir composetest
 cd composetest
```
app.py
```python
import time

import redis
from flask import Flask

app = Flask(__name__)
cache = redis.Redis(host='redis', port=6379)

def get_hit_count():
    retries = 5
    while True:
        try:
            return cache.incr('hits')
        except redis.exceptions.ConnectionError as exc:
            if retries == 0:
                raise exc
            retries -= 1
            time.sleep(0.5)

@app.route('/')
def hello():
    count = get_hit_count()
    return 'Hello World! I have been seen {} times.\n'.format(count)
```
requirements.txt
```python
flask
redis
```
Dockerfile
```python
# syntax=docker/dockerfile:1
FROM python:3.7-alpine
WORKDIR /code
ENV FLASK_APP=app.py
ENV FLASK_RUN_HOST=0.0.0.0
RUN apk add --no-cache gcc musl-dev linux-headers
COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt
EXPOSE 5000
COPY . .
CMD ["flask", "run"]
```
docker-compose.yml
```yaml
version: "3.9"
services:
  web:
    build: .
    ports:
      - "8000:5000"
  redis:
    image: "redis:alpine"
```
docker-compose up<br />我在启动的时候出现了一个问题就是说我的版本过老，在网上找的教程是将3.9改成了2.0就可以运行了但是没有明白是什么原理,第二件事我真的是个傻逼明明是`8000`端口我非要访问5000我是真的服了自己<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1651037534911-fe1831d3-4353-4aa7-aa52-89d1d1f6a0e4.png#clientId=u4ebc3155-0e64-4&from=paste&height=266&id=ub34afc87&originHeight=366&originWidth=840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12631&status=done&style=none&taskId=u75358f64-4698-430b-9785-0daeb37c762&title=&width=610.9090909090909)
<a name="QgirP"></a>
## 三、yaml文件规则
compose yaml文件规则<br />官网示例：[https://docs.docker.com/compose/compose-file/compose-file-v3/](https://docs.docker.com/compose/compose-file/compose-file-v3/)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1651038409526-d7158ad5-7ccf-49d1-b945-5ab9e0ab7a44.png#clientId=u4ebc3155-0e64-4&from=paste&height=529&id=uc91f4517&originHeight=727&originWidth=1170&originalType=binary&ratio=1&rotation=0&showTitle=false&size=38648&status=done&style=none&taskId=u51952ae4-e79f-4579-8c25-41cfdc27ffe&title=&width=850.9090909090909)
```python
version: '' #版本
service:
  服务1： web
    #服务配置
    images
    build
    network
  服务2： redis
    #服务配置
    images
    build
    network
  服务3：
    。。。。

其他配置
  volumns
  configer
```
在官网中可以详细的看到service下面可以使用的命令
> [https://docs.docker.com/compose/compose-file/compose-file-v3/#service-configuration-reference](https://docs.docker.com/compose/compose-file/compose-file-v3/#service-configuration-reference)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1651038921384-48ba78f6-a00d-4e2f-9005-6df419eacdfe.png#clientId=u4ebc3155-0e64-4&from=paste&height=483&id=uec68e061&originHeight=664&originWidth=292&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25591&status=done&style=none&taskId=u3ddc3673-99c4-4885-9924-85d65c7c86a&title=&width=212.36363636363637)<br />重要参数解析

- container_name 指定容器运行的名称
```python
container_name: my-web-container
```

- depends-on 解决容器的启动顺序
```python
version: "3.9"
services:
  web:
    build: .
    depends_on:
      - db
      - redis
  redis:
    image: redis
  db:
    image: postgres
```
depends_on为了解决顺序首先启动`postgres`然后在启动`redis`最后在`build`

- deploy 指定容器运行的副本
```yaml
version: "3.9"
services:
  redis:
    image: redis:alpine
    deploy:
      replicas: 6
      placement:
        max_replicas_per_node: 1
      update_config:
        parallelism: 2
        delay: 10s
      restart_policy:
        condition: on-failure
```

- expose  暴露端口
```python
expose:
  - "3000"
  - "8000"
```
<a name="zDBPu"></a>
## 四、wp开源项目
[https://docs.docker.com/samples/wordpress/](https://docs.docker.com/samples/wordpress/)
```yaml
 cd my_wordpress/
 vim docker-compose.yml
 version: "3.9"
    
services:
  db:
    image: mysql:5.7
    volumes:
      - db_data:/var/lib/mysql
    restart: always
    environment:
      MYSQL_ROOT_PASSWORD: somewordpress
      MYSQL_DATABASE: wordpress
      MYSQL_USER: wordpress
      MYSQL_PASSWORD: wordpress
    
  wordpress:
    depends_on:
      - db
    image: wordpress:latest
    volumes:
      - wordpress_data:/var/www/html
    ports:
      - "8000:80"
    restart: always
    environment:
      WORDPRESS_DB_HOST: db
      WORDPRESS_DB_USER: wordpress
      WORDPRESS_DB_PASSWORD: wordpress
      WORDPRESS_DB_NAME: wordpress
volumes:
  db_data: {}
  wordpress_data: {}


docker-compose up -d
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1651041629044-272170c3-e78b-4180-ae3a-21aced873525.png#clientId=u4ebc3155-0e64-4&from=paste&height=135&id=u895af3ad&originHeight=186&originWidth=1548&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29045&status=done&style=none&taskId=u02bc5e62-c285-4c85-9e62-cf11b21ad46&title=&width=1125.8181818181818)
<a name="zoKNF"></a>
## 五、spring 实战

