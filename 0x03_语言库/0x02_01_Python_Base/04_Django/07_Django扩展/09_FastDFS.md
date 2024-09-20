# FastDFS

## 1. 安装fastdfs依赖包

~~~ shell
unzip libfastcommon-master.zip
cd libfastcommon-master的
./make.sh
./make.sh install
~~~



## 2. 安装fastdfs

~~~ shell
unzip fastdfs-master.zip
cd fastdfs-master
./make.sh
./make.sh install
~~~



## 3. 配置跟踪服务器tracker

~~~ shell
cp /etc/fdfs/tracker.conf.sample /etc/fdfs/tracker.conf
mkdir –p /home/python/fastdfs/tracker
vim /etc/fdfs/tracker.conf
base_path=/home/python/fastdfs/tracker
~~~



## 4. 配置存储服务器storage

~~~ shell
cp /etc/fdfs/storage.conf.sample /etc/fdfs/storage.conf
mkdir –p /home/python/fastdfs/storage

vim /etc/fdfs/storage.conf
base_path=/home/python/fastdfs/storage
store_path0=/home/python/fastdfs/storage
tracker_server=自己ubuntu虚拟机的ip地址:22122
~~~



## 5. 启动 tracker 和 storage

~~~ shell
service fdfs_trackerd start
service fdfs_storaged start
~~~



## 6. 测试是否安装成功

~~~ shell
cp /etc/fdfs/client.conf.sample /etc/fdfs/client.conf

vim /etc/fdfs/client.conf
base_path=/home/python/fastdfs/tracker
tracker_server=自己ubuntu虚拟机的ip地址:22122

# 上传文件测试：
fdfs_upload_file /etc/fdfs/client.conf aa.jpg
# 如果返回类似group1/M00/00/00/rBIK6VcaP0aARXXvAAHrUgHEviQ394.jpg的文件id则说明文件上传成功
~~~



## 7. 安装nginx及fastdfs-nginx-module

~~~ shell
yum -y install pcre-devel
yum -y install openssl openssl-devel
tar xf nginx-1.8.1.tar.gz
unzip fastdfs-nginx-module-master.zip
cd nginx-1.8.1
./configure --prefix=/usr/local/nginx/ --add-module=/root/fastdfs-nginx-module-master/src/
make && make install
cp fastdfs-nginx-module-master/src/mod_fastdfs.conf  /etc/fdfs/mod_fastdfs.conf

vim /etc/fdfs/mod_fastdfs.conf
connect_timeout=10
tracker_server=自己ubuntu虚拟机的ip地址:22122
url_have_group_name=true
store_path0=/home/python/fastdfs/storage

cp fastdfs-master/conf/http.conf  /etc/fdfs/
cp fastdfs-master/conf/mime.types /etc/fdfs/

vim /usr/local/nginx/conf/nginx.conf
server {
       listen       8888;
       server_name  localhost;
       location ~/group[0-9]/ {
           ngx_fastdfs_module;
       }
       error_page   500 502 503 504  /50x.html;
       location = /50x.html {
       root   html;
       }
   }
   
启动nginx
/usr/local/nginx/sbin/nginx
~~~

![1581385866550-05dbd590-34f9-42e8-b0ba-1af84d47444f](D:\Note\python\Django\图片\1581385866550-05dbd590-34f9-42e8-b0ba-1af84d47444f.png)



## 8. 使用python连接

~~~ shell
pip install fdfs_client-py-master.zip
~~~

