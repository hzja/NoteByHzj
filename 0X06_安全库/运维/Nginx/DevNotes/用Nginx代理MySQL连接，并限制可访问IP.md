<a name="QC5oh"></a>
## 1、前言
生产环境基本上都部署在云服务器上，例如应用服务器、MySQL服务器等。如果MySQL服务器直接暴露在公网，就会存在很大的风险，为了保证数据安全，MySQL服务器的端口是不对外开放的。<br />线上业务遇到bug了，开发的小伙伴需要远程连接MySQL来查看数据，那应该怎么办呢？<br />可以通过Nginx代理（“跳板机”）来进行连接。
<a name="STQgQ"></a>
## 2、Nginx代理连接
要实现对连接的代理转发，需要一台服务器并安装Nginx，且与MySQL服务器处于一个内网之中，内网之间可以访问。<br />其次，需要用到`ngx_stream_core_module`模块，该模块不是默认构建的，需要在configure时添加`--with-stream`来进行构建。<br />既然要用到`ngx_stream_core_module`模块，首当其冲，是看看其提供的指令，才知道怎么来进行配置。
<a name="AqYPh"></a>
### 1）stream
该指令定义了stream服务器。与http块平级，定义在main块中。<br />作用域：main<br />语法：`stream {...}`<br />示例：
```
stream {
     server {
         ......
     }
 }
```
<a name="DZOhh"></a>
### 2）server
该指令定义一个虚拟主机，与http块中的server类似。可以在stream块中定义多个server块。<br />作用域：stream<br />语法：`server {...}`
```nginx
stream {
  server {
    ......
  }
  server {
    ......
  }
}
```
<a name="fhxEd"></a>
### 3）listen
该指令定义虚拟主机server要监听的socket的地址和端口。<br />作用域：server<br />语法：`listen address:port;`<br />示例：
```nginx
listen 127.0.0.1:3306;
listen *:3306;
# 效果与listen *:3306一样
listen 3306;
listen localhost:3306;
```
<a name="h5Fkz"></a>
### 4）配置示例
MySQL服务器，端口3306（单机环境）
```nginx
stream  {
  server {
    listen 3306;
    proxy_pass 192.168.110.101:3306;
  }
}
```
MySQL服务器，端口3306（集群环境）
```nginx
stream  {
  upstream mysql_socket {
    server 192.168.110.101:3306;
  }
  server {
    listen 3306;
    proxy_pass mysql_socket;
  }
}
```
此时，就可以通过例如Navicat等客户端进行连接。
<a name="fbwmD"></a>
## 3、限制访问IP
实现了对连接的代理，所有人都可以通过访问Nginx来连接MySQL服务器，解决了外网无法连接的问题。<br />为了更进一步的缩小访问范围，保证数据安全，可以限制只有公司网络的IP地址可以通过Nginx进行连接。<br />Nginx提供了ngx_stream_access_module模块，其指令非常简单，仅包含allow和deny指令。
<a name="CdfOm"></a>
### 1）allow
该指令设置指定的IP允许访问。可以和deny指令配合使用<br />作用域：stream, server<br />语法：`allow address | CIDR | unix: | all;`<br />示例：
```nginx
# 允许192.168.110.1访问
allow 192.168.110.1;

# 允许192.168.110.1到192.168.255.254
allow 192.168.110.0/16;

# 允许192.168.110.1到192.168.110.254
allow 192.168.110.0/24;

# 允许所有的IP访问
allow all;
```
<a name="NZmvD"></a>
### 2）deny
该指令设置指定的IP禁止访问。可以和allow指令配合使用。<br />作用域：stream, server<br />语法：`deny address | CIDR | unix: | all;`
```nginx
# 禁止192.168.110.1访问
deny 192.168.110.1;

# 禁止192.168.110.1到192.168.255.254
deny 192.168.110.0/16;

# 禁止192.168.110.1到192.168.110.254
deny 192.168.110.0/24;

# 禁止所有的IP访问
deny all;
```
<a name="VjcOU"></a>
### 3）配置示例
禁止所有的IP访问，192.168.110.100除外。
```nginx
allow 192.168.110.100;
deny all;
```
**Tips：如果指定了allow，需要配合deny使用，否则就是允许所有的IP地址访问。**
<a name="e68v3"></a>
## 4、综合案例
只允许192.168.110.100通过Nginx连接MySQL服务器。
```nginx
stream  {
  allow 192.168.110.100;
  deny all;
  server {
    listen 3306;
    proxy_pass 192.168.110.101:3306;
  }
}
```
