Nginx
<a name="Cs1SN"></a>
### 一、Nginx的虚拟主机介绍 
虚拟主机指在一台物理主机服务器上划分出多个磁盘空间，每个磁盘空间都是一个虚拟主机，每台虚拟主机都可以对外提供Web服务，并且互不干扰。<br />就类似本地虚拟机，可以在Window系统上面安装一个虚拟机，在虚拟机里面安装很多个系统。那这样的话就可以模拟很多个服务器。<br />Nginx虚拟主机配置, 可以在一台机器上面配置很多个虚拟主机配置多个域名。那这样的话就可以靠虚拟主机转发到不同的应用,就不用每个系统都配置一个Nginx。<br />![2021-05-20-19-55-05-606496.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621512129372-d6178073-d733-465d-91e2-cadb051737b5.png#clientId=ub9f95e69-f6d1-4&from=ui&id=uf06798fd&originHeight=603&originWidth=1038&originalType=binary&size=1881462&status=done&style=shadow&taskId=u56f087d1-96fd-4e6e-a537-c252d558ebc)
<a name="Ytqal"></a>
### 二、搭建前端静态服务器 
前面讲解了很多基础知识以及这个环境的搭建，现在就来开发第一个案例实操。<br />一个系统它有前端页面跟后端服务组成。这个就不像最开始学那个编码的时候，前端代码跟后端代码放在一起。这一种的话性能很低，完全支撑不了高并发访问。<br />现在公司基本都是前后端分离的架构。前端专门负责开发前端的页面，后端专门开发对应的后端接口。<br />后端部署的话一般都是集群部署，然后会增加对应的缓存跟消息队列，提高接口的一个性能。<br />那前端这一块一般怎么去部署呢？<br />比如vue开发或者React开发的项目，一般会把前端代码单独放到一个文件服务器上面或者购买CDN服务，用户访问前端代码，前端代码会发起异步请求后端的接口，然后渲染数据最终通过浏览展示出来。<br />这个跟下面的图也是一样的，可以用Nginx做一个文件服务器，用户访问Nginx去找对应的HTML代码返回给浏览器，再请求后端接口拿数据在浏览器上渲染出来。<br />![2021-05-20-19-55-05-760537.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621512167052-6885f4d3-7f72-4e8b-9a26-5dfb8b3dcd47.png#clientId=ub9f95e69-f6d1-4&from=ui&id=u4c4e2d1b&originHeight=249&originWidth=815&originalType=binary&size=610116&status=done&style=none&taskId=uc88e43a7-01dd-44a1-999e-b174cdb87f4)<br />那Nginx怎么配置静态代码服务器呢？跟着步骤即可，编辑/usr/local/nginx/conf/nginx.conf文件。<br />找到server虚拟主机节点，配置下静态html文件路径，root可以是相对路径，也可以是绝对路径，这边使用nginx 里面的html文件夹，index 配置表示会根据html文件路径去找默认的首页文件，先找index.html<br />如果找不到就找index.htm<br />![2021-05-20-19-55-05-905720.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621512175041-d4432b6e-1f2a-44e2-8a04-3a6c2a881d62.png#clientId=ub9f95e69-f6d1-4&from=ui&id=u3f41c3cc&originHeight=465&originWidth=851&originalType=binary&size=107872&status=done&style=shadow&taskId=u44e1efdd-6d37-42a3-9474-ce3c81bb686)
```nginx
server {
  listen       80;
  server_name  aabbccdd.com; # 本地虚拟主机的配置域名

  location / {
    root   /usr/local/nginx/html; #相对路径或者绝对路径都可以
    index  xdclass.html;
  }
}
```
为了方便验证，需要准备对应的前端代码，直接修改/usr/local/nginx/html/index.html里面的代码，修改完保存, 然后重启Nginx即可验证<br />![2021-05-20-19-55-06-088182.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621512186455-f7654df8-61ca-4902-9577-6bd920c774da.png#clientId=ub9f95e69-f6d1-4&from=ui&id=ue193ecac&originHeight=462&originWidth=847&originalType=binary&size=146981&status=done&style=shadow&taskId=ud7d50506-d84c-4d2c-aeb0-18dcfa65a34)<br />访问配置好的本地域名解析，aabbccdd.com 就可以访问到静态代码文件，包括如果使用vue或者react 开发的前端代码，也可以按照这种方式进行一个配置。<br />![2021-05-20-19-55-06-258653.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621512195645-c40e5182-bec0-4668-bba1-53982da57636.png#clientId=ub9f95e69-f6d1-4&from=ui&id=u198bb12a&originHeight=611&originWidth=1080&originalType=binary&size=86366&status=done&style=shadow&taskId=ua2e0e5c5-4322-4d8d-a27d-32c0dd6d4ca)
<a name="yDPmm"></a>
### 三、Nginx搭建文件服务器 
一般开发一个系统，肯定会涉及到一个文件的上传好比用户的头像, 或者PDF文件，那这些东西应该存储到哪里呢？<br />以前学项目的时候，基本就是把图片传到项目本身里面, 如果文件数量少的话还可以接受。<br />那如果图片的数量有几万几十万到几百万个，那这个缺点很明显，用Tomcat返回对应的文件，那这个肯定很占用的内存资源、带宽资源以及并发量，基本很少公司会这样做，公司基本就是使用文件服务器或者云厂商提供的CDN。<br />那这一个文件服务器是怎么操作的流程呢？前端用户上传文件到的后端服务器，后端服务器存储临时文件，然后再把文件传到文件服务器，拼装好访问的文件访问URL，再把这个URL存储到对应的数据库，最后把URL返回给咱们这个前端用户，这一个就是完整的文件上传的流程。<br />![2021-05-20-19-55-06-380323.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621512379151-bab0c413-0a0b-40d0-92ee-3b9540f03938.png#clientId=ub9f95e69-f6d1-4&from=ui&id=udb22f2d9&originHeight=592&originWidth=922&originalType=binary&size=1640786&status=done&style=shadow&taskId=ude94b86d-b308-4e58-8c84-74d2147eb21)<br />这样的好处很明显，后续用户访问对应的文件的时候不用经过后端接口，直接访问单独的文件服务器。<br />后端的接口更适合于处理业务逻辑，合适的功能做合适的事情。<br />那使用Nginx怎么去搭建这个图片服务器呢？操作步骤很简单，只要找到在nginx.conf配置文件到虚拟主机，然后根据location做个路径的映射就可以。
```nginx
server {
  listen   80;
  server_name  aabbccdd.com;
  location /app/img {
    alias /usr/local/software/img/;
  }
}
```
上面这个配置的意思就是用户访问 aabbccdd.com/app/img/ ,命中配置的location路径，Nginx就会根据对应的文件名称，去这个/usr/local/software/img/文件夹下面找一下对应的文件返回给用户。<br />这个alias相当于取了一个别名，把用户访问的资源路径映射到这个配置的文件夹下面。<br />为了方便测试，直接在这个文件夹上面上传几个图片和文本，上传上去之后，重启Nginx。<br />![2021-05-20-19-55-06-579985.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621512398931-51ff36a0-8d48-4335-836c-b7ca7f637788.png#clientId=ub9f95e69-f6d1-4&from=ui&id=u28f958f8&originHeight=496&originWidth=874&originalType=binary&size=253326&status=done&style=none&taskId=ua7f0da66-f4a4-4752-a2ce-5b74cd95262)<br />就可以通过浏览器访问对应的资源路径，Nginx就会根据路径找对应的文件返回。
