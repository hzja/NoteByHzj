Nginx <br />ngx_http_secure_link_module模块用于检查请求链接的真伪，保护资源免受未经授权的访问，限制链接的生命周期。通过将请求中传递的校验和值与请求计算的值进行比较来验证请求链接的真实性。如果链接的生命周期有限并且时间已过，则该链接将被视为过时。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600617076726-a66311ea-2a4e-419f-a781-9aa3dd9b6aa6.png#align=left&display=inline&height=353&originHeight=1058&originWidth=1977&size=166373&status=done&style=shadow&width=659)<br />验证原理：

- 用户在客户端点击下载按钮，服务器收到请求后生成一个下载地址返回给客户端。<br />
- 客户端使用这个生成的下载地址去请求资源，此时nginx去做校验，校验链接地址真伪和链接地址是否过期。如果链接地址是真的并且链接地址没有过期，就给客户端返回下载资源。<br />

nginx验证和服务器生成密钥规则要一致，否则验证是不会通过的<br />该模块提供两种备选操作模式：

- 第一种模式secure_link_secret指令启用，用于检查请求链接的真实性以及保护资源免受未经授权的访问<br />
- 第二种模式secure_link和secure_link_md5指令启用，也用于限制链接生命周期。<br />

更详细参考官网<br />[http://nginx.org/en/docs/http/ngx_http_secure_link_module.html](http://nginx.org/en/docs/http/ngx_http_secure_link_module.html)
<a name="VpcSM"></a>
## 1、配置模块语法
<a name="99560f15"></a>
### ngx_http_secure_link_module主要有以下三个语法
<a name="xqKkN"></a>
#### secure_link
```
Syntax: secure_link expression;
Default: —
Context: http, server, location
```
<a name="pSNyZ"></a>
#### secure_link_md5
```
Syntax: secure_link_md5 expression;
```
```
Default: —
Context: http, server, location
```
<a name="fgM0R"></a>
#### secure_link_secret
```
Syntax: secure_link_secret word;
Default: —
Context: location
```
<a name="5Xlsl"></a>
## 2、安装ngx_http_secure_link_module模块
首先检查nginx是否已安装模块<br />
```bash
nginx -V
```
结果如下，没有安装ngx_http_secure_link_module模块<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600616146054-7e870cdd-452b-4370-97a7-ca71c7056b38.png#align=left&display=inline&height=163&originHeight=490&originWidth=3323&size=544388&status=done&style=none&width=1107.6666666666667)<br />输出nginx所有已安装模块，检查是否有ngx_http_secure_link_module，因为这个模块没有默认编译，在编译Nginx时，必须使用明确的配置参数
```
--with-http_secure_link_module
```
<a name="C9GM0"></a>
### 安装
执行以下命令
```bash
#先安装secure link 其实这个组件nginx本身就有，只不过默认不安装
./configure  --prefix=/usr/local/nginx --user=nginx \
--group=nginx --with-http_secure_link_module \
--with-http_stub_status_module
```
执行mak编译nginx
```bash
make
```
make之后的操作需要注意，如果nginx第一次安装，直接执行make install即可
```bash
make install
```
如果单纯添加模块，不需要install，而是执行以下操作，将打过补丁的nginx二进制文件覆盖/usr/local/nginx/sbin/目录中的文件即可
```bash
#备份之前的nginx
mv /usr/local/nginx/sbin/nginx /usr/local/nginx/sbin/nginx.bak
#拷贝新的nginx到sbin目录
cp /nginx源码目录/objs/nginx  /usr/local/nginx/sbin/
```
<a name="PH7Eo"></a>
## 3、ngx_http_secure_link_module配置
在server节点增加以下location配置
```nginx
location / {
    root html; 
    #这里配置了2个参数一个是md5，一个是expires
    secure_link $arg_md5,$arg_expires;
    #md5的哈希格式为: expires+url+addr
    #expires为时间戳单位s
    #url为请求地址
    #remote_addr为远程IP地址
    #mysecure为密钥
    secure_link_md5 "mysecure$secure_link_expires$uri$remote_addr";
    if ($secure_link = "") {
    #资源不存在或哈希比对失败
        return 402;
    }
    if ($secure_link = "0") {
        #时间戳过期
        return 404;
    }
    if ($request_filename ~* ^.*?\.(mp4)$){
        #直接下载防止打开文件  格式: (mp4|txt|jpg)
        add_header Content-Disposition 'attachment;';
    }
}
#设置400,404的跳转
#在server节点
error_page 400 = 400.html; #跳转400页面
error_page 404 = 404.html; #跳转404页面
```
在html目录添加400.html和404.htmll两个界面
<a name="VVjCH"></a>
## 4、JAVA计算防盗链地址
需要使用到commons-codec-1.10.jar包，请自行下载
```java
package com.alibaba.csp.sentinel.datasource.consul;
import org.apache.commons.codec.binary.Base64;
import org.apache.commons.codec.digest.DigestUtils;
/**
 * 防盗链资源下载地址
 *
 */
public class GenSourceDownloadPath {
    final static String HTTP = "http://";
    final static String ST = "?md5=";
    final static String E = "&expires=";
    final static String F = "/";
    final static String WARN = "path参数最好带上 \"/\" ,例: \"/abc.mp4\" ";
    final static String SERVER_IP = "127.0.0.1";
    final static Long expires = 20L; //过期时间 秒
    final static String secret = "mysecure"; //密钥
    public static String execute(String path) {
        if (path.indexOf(F) == -1) {
            path =  F + path;
            System.out.println(WARN);
        }
        // +n代表n秒后地址失效
        String time = String.valueOf(System.currentTimeMillis() / 1000 + expires); 
        // $secure_link_expires$uri$remote_addr
        String md5Str = secret + time + path + SERVER_IP;
        byte[] md5byte = DigestUtils.md5(md5Str);
        String base64 = Base64.encodeBase64URLSafeString(md5byte);
        return HTTP + SERVER_IP + path+ ST + base64 + E + time;
    }
    public static void main(String[] args) {
        System.out.println(execute("my.conf"));
    }
}
```
现在html目录存放一个图片文件my.conf，该文件是需要访问演示的文件<br />执行GenSourceDownloadPath生成一个访问的URL：
```http
http://127.0.0.1/my.conf?md5=YO4diAmlVa8NfHlMBCMFaw&expires=1598071052
```
执行访问可以正常打开文件，如果没有带参数访问则返回402，超过20s访问，不在有权限访问
:::tips
备注：按官方示例mysecure放在最后，需加一个空格与参数分隔<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600617165003-07039d70-8d08-4a5b-8aac-e32a3f394a58.png#align=left&display=inline&height=245&originHeight=736&originWidth=1754&size=85429&status=done&style=none&width=584.6666666666666)
:::
在使用java计算时也需要多加一个空格
```java
package com.alibaba.csp.sentinel.datasource.consul;
import org.apache.commons.codec.binary.Base64;
import org.apache.commons.codec.digest.DigestUtils;
/**
 * 防盗链资源下载地址
 *
 */
public class GenSourceDownloadPath {
    final static String HTTP = "http://";
    final static String ST = "?md5=";
    final static String E = "&expires=";
    final static String F = "/";
    final static String WARN = "path参数最好带上 \"/\" ,例: \"/abc.mp4\" ";
    final static String SERVER_IP = "127.0.0.1";
    final static Long expires = 20L; //过期时间 秒
    final static String secret = "mysecure"; //密钥
    public static String execute(String path) {
        if (path.indexOf(F) == -1) {
            path =  F + path;
            System.out.println(WARN);
        }
        // +n代表n秒后地址失效
        String time = String.valueOf(System.currentTimeMillis() / 1000 + expires); 
        // $secure_link_expires$uri$remote_addr
        String md5Str = time + path + SERVER_IP + " " + secret;
        byte[] md5byte = DigestUtils.md5(md5Str);
        String base64 = Base64.encodeBase64URLSafeString(md5byte);
        return HTTP + SERVER_IP + path+ ST + base64 + E + time;
    }
    public static void main(String[] args) {
        System.out.println(execute("my.conf"));
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600616696325-a05dfe22-aad2-444d-99ab-6a801d75d6b7.png#align=left&display=inline&height=283&originHeight=848&originWidth=1643&size=186879&status=done&style=shadow&width=547.6666666666666)
