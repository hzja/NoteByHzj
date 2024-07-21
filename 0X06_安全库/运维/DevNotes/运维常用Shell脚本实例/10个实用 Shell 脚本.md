LinuxShell
<a name="o9b6U"></a>
### 监控 MySQL 主从同步状态是否异常脚本
```bash
#!/bin/bash
HOST=localhost
USER=root
PASSWD=123.com
IO_SQL_STATUS=$(mysql -h$HOST -u$USER -p$PASSWD -e 'show slave status\G' 2>/dev/null |awk '/Slave_.*_Running:/{print $1$2}')
for i in $IO_SQL_STATUS; do
    THREAD_STATUS_NAME=${i%:*}
    THREAD_STATUS=${i#*:}
    if [ "$THREAD_STATUS" != "Yes" ]; then
        echo "Error: MySQL Master-Slave $THREAD_STATUS_NAME status is $THREAD_STATUS!" |mail -s "Master-Slave Staus" [url=mailto:xxx@163.com]xxx@163.com[/url]
    fi
done
```
<a name="TJBad"></a>
### 目录文件变化监控和实时文件同步
```bash
#!/bin/bash

MON_DIR=/opt
inotifywait -mqr --format %f -e create $MON_DIR |\
while read files; do
   rsync -avz /opt /tmp/opt
   #echo "$(date +'%F %T') create $files" | mail -s "dir monitor" [url=mailto:xxx@163.com]xxx@163.com[/url]
done
```
<a name="TqB9Z"></a>
### 批量创建 100 用户并设置密码脚本
```bash
#!/bin/bash
DATE=$@
USER_FILE=user.txt
for USER in $USER_LIST; do
    if ! id $USER &>/dev/null; then
        PASS=$(echo $RANDOM |md5sum |cut -c 1-8)
        useradd $USER
        echo $PASS |passwd --stdin $USER &>/dev/null
        echo "$USER   $PASS" >> $USER_FILE
        echo "$USER User create successful."
    else
        echo "$USER User already exists!"
    fi
done
```
<a name="YTvQm"></a>
### 批量检测网站是否异常脚本
```bash
#!/bin/bash
URL_LIST="www.baidu.com [url=http://www.ctnrs.com]www.ctnrs.com[/url]"
for URL in $URL_LIST; do
    FAIL_COUNT=0
    for ((i=1;i<=3;i++)); do
        HTTP_CODE=$(curl -o /dev/null --connect-timeout 3 -s -w "%{http_code}" $URL)
        if [ $HTTP_CODE -eq 200 ]; then
            echo "$URL OK"
            break
        else
            echo "$URL retry $FAIL_COUNT"
            let FAIL_COUNT++
        fi
    done
    if [ $FAIL_COUNT -eq 3 ]; then
        echo "Warning: $URL Access failure!"
    fi
done
```
<a name="gPmAQ"></a>
### 批量主机远程执行命令脚本
```bash
#!/bin/bash
COMMAND=$*
HOST_INFO=host.info
for IP in $(awk '/^[^#]/{print $1}' $HOST_INFO); do
    USER=$(awk -v ip=$IP 'ip==$1{print $2}' $HOST_INFO)
    PORT=$(awk -v ip=$IP 'ip==$1{print $3}' $HOST_INFO)
    PASS=$(awk -v ip=$IP 'ip==$1{print $4}' $HOST_INFO)
    expect -c "
       spawn ssh -p $PORT $USER@$IP
       expect {
          \"(yes/no)\" {send \"yes\r\"; exp_continue}
          \"password:\" {send \"$PASS\r\"; exp_continue}
          \"$USER@*\" {send \"$COMMAND\r exit\r\"; exp_continue}
       }
    "
    echo "-------------------"
done
```
<a name="R3Dca"></a>
### 一键部署 LNMP 网站平台脚本
```bash
#!/bin/bash
NGINX_V=1.15.6
PHP_V=5.6.36
TMP_DIR=/tmp

INSTALL_DIR=/usr/local

PWD_C=$PWD

echo
echo -e "\tMenu\n"
echo -e "1. Install Nginx"
echo -e "2. Install PHP"
echo -e "3. Install MySQL"
echo -e "4. Deploy LNMP"
echo -e "9. Quit"

function command_status_check() {
        if [ $? -ne 0 ]; then
                echo $1
                exit
        fi
}

function install_nginx() {
    cd $TMP_DIR
    yum install -y gcc gcc-c++ make openssl-devel pcre-devel wget
    wget [url=http://nginx.org/download/nginx-]http://nginx.org/download/nginx-[/url]${NGINX_V}.tar.gz
    tar zxf nginx-${NGINX_V}.tar.gz
    cd nginx-${NGINX_V}
    ./configure --prefix=$INSTALL_DIR/nginx \
    --with-http_ssl_module \
    --with-http_stub_status_module \
    --with-stream
    command_status_check "Nginx - 平台环境检查失败！"
    make -j 4
    command_status_check "Nginx - 编译失败！"
    make install
    command_status_check "Nginx - 安装失败！"
    mkdir -p $INSTALL_DIR/nginx/conf/vhost
    alias cp=cp ; cp -rf $PWD_C/nginx.conf $INSTALL_DIR/nginx/conf
    rm -rf $INSTALL_DIR/nginx/html/*
    echo "ok" > $INSTALL_DIR/nginx/html/status.html
    echo '<?php echo "ok"?>' > $INSTALL_DIR/nginx/html/status.php
    $INSTALL_DIR/nginx/sbin/nginx
    command_status_check "Nginx - 启动失败！"
}

function install_php() {
        cd $TMP_DIR
    yum install -y gcc gcc-c++ make gd-devel libxml2-devel \
        libcurl-devel libjpeg-devel libpng-devel openssl-devel \
        libmcrypt-devel libxslt-devel libtidy-devel
    wget [url=http://docs.php.net/distributions/php-]http://docs.php.net/distributions/php-[/url]${PHP_V}.tar.gz
    tar zxf php-${PHP_V}.tar.gz
    cd php-${PHP_V}
    ./configure --prefix=$INSTALL_DIR/php \
    --with-config-file-path=$INSTALL_DIR/php/etc \
    --enable-fpm --enable-opcache \
    --with-mysql --with-mysqli --with-pdo-mysql \
    --with-openssl --with-zlib --with-curl --with-gd \
    --with-jpeg-dir --with-png-dir --with-freetype-dir \
    --enable-mbstring --enable-hash
    command_status_check "PHP - 平台环境检查失败！"
    make -j 4
    command_status_check "PHP - 编译失败！"
    make install
    command_status_check "PHP - 安装失败！"
    cp php.ini-production $INSTALL_DIR/php/etc/php.ini
    cp sapi/fpm/php-fpm.conf $INSTALL_DIR/php/etc/php-fpm.conf
    cp sapi/fpm/init.d.php-fpm /etc/init.d/php-fpm
    chmod +x /etc/init.d/php-fpm
    /etc/init.d/php-fpm start
    command_status_check "PHP - 启动失败！"
}

read -p "请输入编号：" number
case $number in
    1)
        install_nginx;;
    2)
        install_php;;
    3)
        install_mysql;;
    4)
        install_nginx
        install_php
        ;;
    9)
        exit;;
esac
```
<a name="lo05o"></a>
### 一键查看服务器资源利用率
```bash
#!/bin/bash
function cpu() {
    NUM=1
    while [ $NUM -le 3 ]; do
        util=`vmstat |awk '{if(NR==3)print 100-$15"%"}'`
        user=`vmstat |awk '{if(NR==3)print $13"%"}'`
        sys=`vmstat |awk '{if(NR==3)print $14"%"}'`
        iowait=`vmstat |awk '{if(NR==3)print $16"%"}'`
        echo "CPU - 使用率: $util , 等待磁盘IO响应使用率: $iowait"
        let NUM++
        sleep 1
    done
}

function memory() {
    total=`free -m |awk '{if(NR==2)printf "%.1f",$2/1024}'`
    used=`free -m |awk '{if(NR==2) printf "%.1f",($2-$NF)/1024}'`
    available=`free -m |awk '{if(NR==2) printf "%.1f",$NF/1024}'`
    echo "内存 - 总大小: ${total}G , 使用: ${used}G , 剩余: ${available}G"
}

function disk() {
    fs=$(df -h |awk '/^\/dev/{print $1}')
    for p in $fs; do
        mounted=$(df -h |awk '$1=="'$p'"{print $NF}')
        size=$(df -h |awk '$1=="'$p'"{print $2}')
        used=$(df -h |awk '$1=="'$p'"{print $3}')
        used_percent=$(df -h |awk '$1=="'$p'"{print $5}')
        echo "硬盘 - 挂载点: $mounted , 总大小: $size , 使用: $used , 使用率: $used_percent"
    done
}

function tcp_status() {
    summary=$(ss -antp |awk '{status[$1]++}END{for(i in status) printf i":"status[i]" "}')
    echo "TCP连接状态 - $summary"
}

cpu
memory
disk
tcp_status
```
<a name="KGLGf"></a>
### 找出占用 CPU 内存过高的进程脚本
```bash
ps -eo user,pid,pcpu,pmem,args --sort=-pcpu  |head -n 10

ps -eo user,pid,pcpu,pmem,args --sort=-pmem  |head -n 10
```
<a name="iPw6s"></a>
### 自动发布 Java 项目（Tomcat）
```bash
#!/bin/bash
DATE=$(date +%F_%T)

TOMCAT_NAME=$1
TOMCAT_DIR=/usr/local/$TOMCAT_NAME
ROOT=$TOMCAT_DIR/webapps/ROOT

BACKUP_DIR=/data/backup
WORK_DIR=/tmp
PROJECT_NAME=tomcat-java-demo

# 拉取代码
cd $WORK_DIR
if [ ! -d $PROJECT_NAME ]; then
   git clone [url=https://github.com/lizhenliang/tomcat-java-demo]https://github.com/lizhenliang/tomcat-java-demo[/url]
   cd $PROJECT_NAME
else
   cd $PROJECT_NAME
   git pull
fi

# 构建
mvn clean package -Dmaven.test.skip=true
if [ $? -ne 0 ]; then
   echo "maven build failure!"
   exit 1
fi

# 部署
TOMCAT_PID=$(ps -ef |grep "$TOMCAT_NAME" |egrep -v "grep|$$" |awk 'NR==1{print $2}')
[ -n "$TOMCAT_PID" ] && kill -9 $TOMCAT_PID
[ -d $ROOT ] && mv $ROOT $BACKUP_DIR/${TOMCAT_NAME}_ROOT$DATE
unzip $WORK_DIR/$PROJECT_NAME/target/*.war -d $ROOT
$TOMCAT_DIR/bin/startup.sh
```
<a name="T9hF3"></a>
### 自动发布 PHP 项目脚本
```bash
#!/bin/bash
DATE=$(date +%F_%T)

WWWROOT=/usr/local/nginx/html/$1


BACKUP_DIR=/data/backup
WORK_DIR=/tmp
PROJECT_NAME=php-demo


# 拉取代码
cd $WORK_DIR
if [ ! -d $PROJECT_NAME ]; then
   git clone [url=https://github.com/lizhenliang/php-demo]https://github.com/lizhenliang/php-demo[/url]
   cd $PROJECT_NAME
else
   cd $PROJECT_NAME
   git pull
fi


# 部署
if [ ! -d $WWWROOT ]; then
   mkdir -p $WWWROOT
   rsync -avz --exclude=.git $WORK_DIR/$PROJECT_NAME/* $WWWROOT
else
   rsync -avz --exclude=.git $WORK_DIR/$PROJECT_NAME/* $WWWROOT
fi
```
