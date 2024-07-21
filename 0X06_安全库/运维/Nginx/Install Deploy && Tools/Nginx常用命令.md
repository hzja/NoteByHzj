Nginx
<a name="G0WoN"></a>
### Nginx常用命令
```bash
nginx -s stop       快速关闭Nginx，可能不保存相关信息，并迅速终止web服务。
nginx -s quit       平稳关闭Nginx，保存相关信息，有安排的结束web服务。
nginx -s reload     因改变了Nginx相关配置，需要重新加载配置而重载。
nginx -s reopen     重新打开日志文件。
nginx -c filename   为 Nginx 指定一个配置文件，来代替缺省的。
nginx -t            不运行，而仅仅测试配置文件。nginx 将检查配置文件的语法的正确性，并尝试打开配置文件中所引用到的文件。
nginx -v            显示 nginx 的版本。
nginx -V            显示 nginx 的版本，编译器版本和配置参数。
```
<a name="lzW2Z"></a>
#### 启动nginx(windows下start nginx);
```bash
nginx -c /usr/local/nginx/conf/nginx.conf
```
<a name="o5mcw"></a>
#### 停止ngix
```bash
nginx -s quit
```
<a name="QSecm"></a>
#### 重新载入nginx(当配置信息发生修改时)
```bash
nginx -s reload
```
<a name="9jQ2D"></a>
### 打开日志文件
```bash
nginx -s reopen
```
<a name="I4O4k"></a>
#### 查看版本
```bash
nginx -v
```
<a name="D26O0"></a>
#### 查看nginx的配置文件的目录
```bash
nginx -t
```
<a name="hIKuX"></a>
#### 查看帮助信息
```bash
nginx -h
```
