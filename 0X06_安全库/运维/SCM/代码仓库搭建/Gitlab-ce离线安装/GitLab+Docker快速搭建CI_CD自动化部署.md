Gitlab Docker
<a name="dIgPX"></a>
## 什么是持续集成(Continuous integration)?
<a name="IeKuD"></a>
### CI
在持续集成环境中，开发人员将会频繁得提交代码到主干。这些新提交在最终合并到主线之前，都需要通过编译和自动化测试进行验证。这样做是基于之前持续集成过程中很重视自动化测试验证结果，以保障所有得提交在合并主干之后得质量问题，对可能出现得一些问题进行预计。
<a name="dNeuz"></a>
### 持续交付(Continuous Delivery)
持续交付就是讲得应用发布出去的过程。这个过程可以确保尽量可能快的实现交付。这就意味着除了自动化测试，还需要有自动化的发布流，以及通过一个按键就可以随时随地实现应用的部署上线<br />通过持续交付，可以决定每天，每周，每两周发布一次，这完全可以根据自己的业务进行设置。<br />但是，如果真的希望体验持续交付的优势，就需要先进行小批量发布，尽快部署到生产线，以便在出现问题时方便进行故障排除。
<a name="sPtpH"></a>
### 持续部署(Continuous Deployment)
如果想更加深入一步的话，就是持续部署了。通过这个方式，任何修改通过了所有已有的工作流就会直接和客户见面。没有人为干预(没有一键部署按钮)，只有当一个修改在工作流中构建失败才能阻止它部署到产品线。<br />持续部署是一个很优秀的方式，可以加速与客户的反馈循环，但是会给团队带来压力，因为不再有"发布日"了。开发人员可以专注于构建软件，他们看到他们修改在他们完成工作后几分钟就上线了。基本上，当开发人员在主分支合并一个提交时，这个分支将被构建，测试，如果一切顺利，则部署到生产环境中。
<a name="HPKYt"></a>
### 持续集成需求

- 持续集成是通过平台串联各个开发环节，实现和沉淀工作自动化的方法。
- 线上代码和代码仓库不同步，影响迭代和团队协作。
- 静态资源发布依赖人工，浪费开发人力。
- 缺少自动化测试，产品质量得不到保障
- 文案简单修改上线，需要技术介入。

![2021-05-31-12-18-44-522012.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435261286-5de56a71-8b95-4365-9053-dd31549904de.png#averageHue=%23a0a2a4&clientId=u0ce648ad-e542-4&from=ui&id=u93df003e&originHeight=801&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2600312&status=done&style=shadow&taskId=uf590b4d6-ff6f-4015-9073-cd861fdb3c0&title=)
<a name="rQ39P"></a>
## Gitlab
Gitlab是一个开源的版本管理系统，实现一个自托管的Git项目仓库，可通过Web界面进行访问公开的或者私人项目。它拥有与Github类似的功能，能够浏览源码，管理缺陷和注释，可以管理团队对仓库的访问，它非常易于浏览提交的版本并提供一个文件历史库。团队成员可以利用内置的简单的聊天程序进行交流。它还提供一个代码片段收集功能可以实现代码复用。<br />GitLab对于系统性能有要求，所以需要将克隆出来的虚拟机的内存提高到至少2G以上。
<a name="TSiAK"></a>
### Gitlab安装
<a name="xGBvk"></a>
#### 方法一：
```bash
sudo docker run --detach \
  --hostname localhost \
  --publish 443:443 --publish 8084:8084 --publish 222:22 \
  --name gitlab \
  --restart always \
  --volume /home/docker/gitlab/config:/etc/gitlab \
  --volume /home/docker/gitlab/logs:/var/log/gitlab \
  --volume /home/docker/gitlab/data:/var/opt/gitlab \
  gitlab/gitlab-ce:latest 
```
localhost:主机名，即虚拟机的ip，8084可以自己定义端口号，restart重启方式，volume目录挂载，gitlab/gitlab-ce:latest镜像名。
<a name="N9xR3"></a>
#### 方法二：
```bash
docker pull twang2218/gitlab-ce-zh 
```
等待其拉取，然后在 /home下新建docker目录，再在其下新建gitlab目录，进入gitlab目录，在当前目录下新建docker-compose.yml配置文件，编写内容如下。
```yaml
version: '3'
services:
   web:
     image: 'twang2218/gitlab-ce-zh'   #gitlab镜像
     restart: always
     privileged: true  #权限
     hostname: ''       #主机名,即虚拟机的ip
     environment:
        TZ: 'Asia/Shanghai'
        GITLAB_OMNIBUS_CONFIG: |
            external_url '' #主机名,即虚拟机的ip
            gitlab_rails['gitlab_shell_ssh_port'] = 2222
            unicorn['port'] = 8888
            nginx['listen_port'] = 8084
     ports:
        - '8084:8084'
        - '8443:443'
        - '2222:22'
     volumes:
        - './config:/etc/gitlab'
        - './logs:/var/log/gitlab'
        - './data:/var/opt/gitlab' 
```
执行`docker-compose up`，然后进入等待时间，等它下好了去通过自己设置的虚拟机的ip和端口号访问。<br />![2021-05-31-12-18-44-666626.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435283866-5e0eeffc-9ef5-455d-ade6-a61b49d433f6.png#averageHue=%23fcfbfb&clientId=u0ce648ad-e542-4&from=ui&id=ubbc30073&originHeight=595&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1931589&status=done&style=shadow&taskId=ua52da3fc-e21c-4a69-8fad-af2830a328c&title=)<br />如果安装过程中有报错权限问题，那么加上`privileged: true`<br />查看方式：
```bash
root@iZm5ebvlfc3n55vzckl9ifZ:# docker ps
CONTAINER ID        IMAGE                         COMMAND                  CREATED             STATUS                 PORTS                                                                         NAMES
ddc7d0e214ef        twang2218/gitlab-ce-zh        "/assets/wrapper"        30 hours ago        Up 6 hours (healthy)   80/tcp, 0.0.0.0:8084->8084/tcp, 0.0.0.0:2222->22/tcp, 0.0.0.0:8443->443/tcp   gitlab_web_1 
```
通过虚拟主机的ip+端口访问，此时需要设置管理员密码，账号为root，密码最少为8位。<br />登录成功后，如下：<br />![2021-05-31-12-18-44-808248.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435299353-c9320890-b132-4718-9710-f2259f6eda68.png#averageHue=%23fbfbfa&clientId=u0ce648ad-e542-4&from=ui&id=u84b231be&originHeight=524&originWidth=1000&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1575140&status=done&style=shadow&taskId=u882c56ae-d29d-4a8a-aba4-7fc12e52262&title=)
<a name="MK47Z"></a>
### 项目创建
点击 + 号 --> 新建项目<br />![2021-05-31-12-18-44-930918.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435314574-4a7f327f-0913-4713-a04b-24ed061643c9.png#averageHue=%23fcfbfb&clientId=u0ce648ad-e542-4&from=ui&id=uc214ba8e&originHeight=174&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=false&size=334868&status=done&style=shadow&taskId=u723ece40-a8b2-47ea-bec1-48fbd8c4077&title=)<br />输入项目名称及描述信息，设置可见等级：私有，内部，公开。<br />![2021-05-31-12-18-45-059620.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435330888-b481f240-a054-4e72-98c9-0e86571c8779.png#averageHue=%23fbfbfa&clientId=u0ce648ad-e542-4&from=ui&id=ua99cca03&originHeight=467&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1516069&status=done&style=shadow&taskId=ub93e8401-b5d3-4c8f-ab56-023f876bc6f&title=)
<a name="bIWcT"></a>
#### 初始化项目
可以选择通过增加一个README的方式来初始化项目，如下：<br />![2021-05-31-12-18-45-245093.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435353592-55e0d9ec-d803-4757-9fb9-daf8b69bd216.png#averageHue=%23f7efe6&clientId=u0ce648ad-e542-4&from=ui&id=uef324183&originHeight=526&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1707606&status=done&style=shadow&taskId=ub6251b35-0018-4b7f-989b-f75b7e53b86&title=)<br />![2021-05-31-12-18-45-419612.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435402206-8c120b87-9299-4d9b-90d6-d9e4bf386684.png#averageHue=%23fcfcfc&clientId=u0ce648ad-e542-4&from=ui&id=u02e1adb4&originHeight=700&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2272441&status=done&style=shadow&taskId=ub98289cd-19de-496f-a94b-535d532c1c5&title=)<br />创建项目的时候有一个问题，在自己最开始定义了端口号，创建项目的时候会没有端口号，这时候clone项目的时候会访问不了，这时候在最开始安装定义目录里面config目录下找到gitlab.rb，编辑它，搜索external_url，没有就添加external_url:主机ip+端口号，有就修改就行了。这时候就可以去克隆项目了。当然也可以通过下面方法去把项目推送到gitlab上面：<br />![2021-05-31-12-18-45-625372.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435434898-581c56d0-ca30-4efa-9421-c36dfc9e3f5a.png#averageHue=%23e5e5e5&clientId=u0ce648ad-e542-4&from=ui&id=u4d724f36&originHeight=654&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2123114&status=done&style=shadow&taskId=uaeba1e5f-c48b-49be-9eb2-ab4af2f94ce&title=)![2021-05-31-12-18-45-771978.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435434720-a43caad7-f5ae-4e2b-8913-6b1258803c6c.png#averageHue=%23f9f9f9&clientId=u0ce648ad-e542-4&from=ui&id=uc61c57de&originHeight=538&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1746551&status=done&style=shadow&taskId=ude6bcc39-9f77-4acb-b4e9-6690eba1e9a&title=)
<a name="ydJlZ"></a>
## Gitlab-Runner
<a name="pP97f"></a>
### 安装
```bash
sudo docker run -d --name gitlab-runner --restart always \
  -v /home/gitlab-runner/config:/etc/gitlab-runner \
  -v /var/run/docker.sock:/var/run/docker.sock \
  gitlab/gitlab-runner:latest 
```
映射/var/run/docker.sock这个文件是为了让容器可以通过/var/run/docker.sock与Docker守护进程通信，管理其他Docker容器 -v /home/gitlab-runner/config:/etc/gitlab-runner是将runner的配置文件映射到宿主机/home/gitlab-runner/config方便调整和查看配置<br />安装完成需要去注册Gitlab-Runner。<br />运行`docker ps`查看：
```bash
root@iZm5ebvlfc3n55vzckl9ifZ:/home/docker/gitlab# docker ps
CONTAINER ID        IMAGE                         COMMAND                  CREATED             STATUS                 PORTS                                                                         NAMES
ed6c7a038263        gitlab/gitlab-runner:latest   "/usr/bin/dumb-init …"   24 hours ago        Up 24 hours                                                                                          gitlab-runner
ddc7d0e214ef        twang2218/gitlab-ce-zh        "/assets/wrapper"        30 hours ago        Up 6 hours (healthy)   80/tcp, 0.0.0.0:8084->8084/tcp, 0.0.0.0:2222->22/tcp, 0.0.0.0:8443->443/tcp   gitlab_web_1 
```
<a name="HVZBE"></a>
### 注册
```bash
docker run --rm -v /srv/gitlab-runner/config:/etc/gitlab-runner gitlab/gitlab-runner register \
  --non-interactive \
  --executor "docker" \
  --docker-image alpine:latest \
  --url "" \
  --registration-token "" \
  --description "first-register-runner" \
  --tag-list "vue3-app" \
  --run-untagged="true" \
  --locked="false" \
  --access-level="not_protected" 
```
注册需要输出url，token，描述，tag，执行器等，url和token怎么来的呢?在设置->CI/CD->Runner里面，这里面注册了一个专用的和共享的Runner，正常情况用专用Runner就可以了。共享版Runner是登录root账户在头部小扳手图片里面的Runner得到url和token，然后去注册。这里面的tag值会在编写.gitlab-ci.yml时用到。<br />![2021-05-31-12-18-45-969448.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435462061-79683957-cdba-4502-83e1-7f50534969e0.png#averageHue=%23f2f2f1&clientId=u0ce648ad-e542-4&from=ui&id=u14861c2b&originHeight=523&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1697854&status=done&style=shadow&taskId=u6eeb4700-c197-4a42-956e-157aa70feca&title=)
<a name="bHRHB"></a>
### 运行流水线
在项目根目录里面创建一个.gitlab-ci.yml，编写代码如下:
```yaml
image: node:alpine

stages: # 分段
  - install
  - eslint
  - build
  - deploy

cache: # 缓存
  paths:
    - node_modules

job_install:
  tags:
    - vue3-app
  stage: install
  script:
    - npm install

job_build:
  tags:
    - vue3-app
  stage: build
  script:
    - npm run build 
```
参数说明：

- stages：pipeline的阶段列表，定义整个pipeline阶段
- stage：定义某个job的所在阶段
- image：指定一个基础Docker进行作为基础运行环境，比如：node,python,java
- tags：用于指定Runner，tags的取值范围是在该项目可惜可见的runner tags中，也就是前面设置的那个tag
- only/except：知道当前任务条件
- when：实现在发生故障时仍能运行的作业
- cache：讲当前工作环境目录中的一些文件，文件夹存储起来，用于在各个任务初始化的时候恢复
- environment：指定部署相关任务的环境，并非真实环境，是对要部署到某环境的任务的归类。方便在gitlab上聚合以便进行回滚和重新部署操作
- artifacts：保留文档。在每次 job 之前runner会清除未被 git 跟踪的文件。为了让编译或其他操作后的产物可以留存到后续使用，添加该参数并设置保留的目录，保留时间等。被保留的文件将被上传到gitlab以备后续使用。
- dependencies：任务依赖。指定job的前置job。添加该参数后，可以获取到前置job的artifacts。注意如果前置 job 执行失败，导致没能生成artifacts，则 job 也会直接失败。

编写好上面代码后推送到gitlab后就会自己执行里面的语句：<br />![2021-05-31-12-18-46-108078.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435480729-3bd7d27a-523c-42b8-ae55-63d0412ca2d5.png#averageHue=%23434343&clientId=u0ce648ad-e542-4&from=ui&id=uf833f45f&originHeight=430&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1395964&status=done&style=shadow&taskId=u1d27bfb8-89a0-437d-9b32-1466669981a&title=)
<a name="jYESK"></a>
### 部署
在项目中创建一个Dockerfile，代码如下：
```dockerfile
FROM node:latest as builder
WORKDIR /app
COPY  package.json
RUN npm install --registry=http://registry.npm.taobao.org
COPY ..
RUN npm run build

FROM nginx:latest
COPY --from=builder /app/dist /usr/share/nginx/html 
```
`.gitlab-ci.yml`修改如下：
```yaml
image: node:alpine

stages: # 分段
  - install
  - eslint
  - build
  - deploy

cache: # 缓存
  paths:
    - node_modules

job_install:
  tags:
    - vue3-app
  stage: install
  script:
    - npm install

job_build:
  tags:
    - vue3-app
  stage: build
  script:
    - npm run build

job_deploy:
    image: docker
    stage: deploy
    script:
      - docker build -t appimages
      - if [ $(docker ps -aq --filter name=app-container) ]; then docker rm -f app-container;fi
      - docker run -d -p 8082:80 --name app-container appimages 

```
if语句判断：使用docker命令去搜索docker容器里面是否有一个name为app-container的容器，如果有就销毁掉，销毁掉是为了使用新的容器重新运行。<br />这里`image:docker`不写的话会报错：<br />![2021-05-31-12-18-46-246708.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435512230-627b1a32-8ea0-46ec-9c3d-8501a0f4c092.png#averageHue=%23343434&clientId=u0ce648ad-e542-4&from=ui&id=u0211ffb3&originHeight=678&originWidth=826&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1683561&status=done&style=none&taskId=u1865707d-6137-4fc3-afd2-a5cef69ba86&title=)<br />代码推送后，流水线工作，到第三步就会出下报错：<br />![2021-05-31-12-18-46-374367.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435530506-6d09076c-dd9d-48e9-8ac2-a7610f4eb60c.png#averageHue=%23efefef&clientId=u0ce648ad-e542-4&from=ui&id=uf4f0e234&originHeight=195&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=633098&status=done&style=none&taskId=u766d1b1b-2fd0-49fa-8aae-034da267355&title=)![2021-05-31-12-18-46-535934.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435531165-93902667-b150-4662-ad52-b2885e5d7b61.png#averageHue=%23333333&clientId=u0ce648ad-e542-4&from=ui&id=uf3ecbf81&originHeight=686&originWidth=820&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1691057&status=done&style=none&taskId=u92da260e-a2be-499d-95f3-63bb49ca39a&title=)<br />解决办法，在runner配置文件中配置docker命令：
```dockerfile
"/usr/bin/docker:/usr/bin/docker", "/var/run/docker.sock:/var/run/docker.sock" 
```
在gitlab-runner->config-vim config.toml，找到注册runner所对应的token，在volumes数组里面加入上面命令：
```dockerfile
concurrent = 1
check_interval = 0

[session_server]
  session_timeout = 1800
  
[[runners]]
  name = "first-register-runner"
  url = ""
  token = ""
  executor = "docker"
  [runners.custom_build_dir]
  [runners.cache]
    [runners.cache.s3]
    [runners.cache.gcs]
    [runners.cache.azure]
  [runners.docker]
    tls_verify = false
    image = "alpine:latest"
    privileged = false
    disable_entrypoint_overwrite = false
    oom_kill_disable = false
    disable_cache = false
    volumes = ["/cache","/usr/bin/docker:/usr/bin/docker", "/var/run/docker.sock:/var/run/docker.sock"]
    shm_size = 0 
```
再去重新运行失败的Jobs，这时候发现成功了：<br />![2021-05-31-12-18-46-693522.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622435549418-a19fbc7f-7604-415d-90ae-41b236a1cec0.png#averageHue=%231b1a19&clientId=u0ce648ad-e542-4&from=ui&id=u4c167151&originHeight=792&originWidth=1000&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2380721&status=done&style=none&taskId=u4497ba36-8cfb-4446-bee1-584114143ae&title=)<br />然后通过前面注册的端口号去访问，可以正常访问项目。
