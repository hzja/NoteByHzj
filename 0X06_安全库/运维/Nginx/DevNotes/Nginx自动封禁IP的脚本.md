Nginx
<a name="XOYsC"></a>
## Nginx封禁指定IP的实现

1. 在Nginx的conf目录下创建一个blockip.conf文件
2. 里面放需要封禁的IP，格式如下
```nginx
deny 1.2.3.4;
```

3. 在Nginx的HTTP的配置中添加如下内容
```nginx
include blockips.conf;
```
![2023-01-01-13-25-55.336178900.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1672550847231-b6026bc8-2364-4d2b-9e2d-957869aa2954.png#averageHue=%23070404&clientId=ub429ecab-f2e3-4&from=ui&id=u0d3447a6&originHeight=445&originWidth=862&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26129&status=done&style=none&taskId=u6c32bef0-da45-4a44-9c8d-54d6294b6d4&title=)

4. 重启 Nginx
```bash
/usr/local/nginx/sbin/nginx -s reload
```

5. 然后就会看到IP被封禁了，喜提403

![2023-01-01-13-25-55.514946500.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1672550847244-65c4d338-267c-431f-b4f4-577e9558565a.png#averageHue=%23fefefe&clientId=ub429ecab-f2e3-4&from=ui&id=vQswu&originHeight=698&originWidth=1209&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13596&status=done&style=none&taskId=u428cc77d-746b-47cb-a1eb-216326ced9d&title=)
<a name="LVmjy"></a>
## 如何实现使用Nginx自动封禁ip的功能

1. AWK统计access.log，记录每分钟访问超过60次的ip，然后配合Nginx进行封禁
2. 编写shell脚本
3. crontab定时跑脚本

好了上面操作步骤列出来了，那先来实现第一个吧<br />![2023-01-01-13-25-55.593502200.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1672550948367-08f35490-bb4d-4916-bed4-7024b060f0cd.png#averageHue=%231c0e0c&clientId=ub429ecab-f2e3-4&from=ui&id=ARFE6&originHeight=306&originWidth=1041&originalType=binary&ratio=1&rotation=0&showTitle=false&size=33872&status=done&style=none&taskId=u0c868c15-9874-438e-a9e2-62b6fa5c926&title=)<br />**操作一：** AWK统计access.log，记录每分钟访问超过60次的ip
```bash
awk '{print $1}' access.log | sort | uniq -cd | awk '{if($1>60)print $0}'
```

1. `awk '{print $1}' access.log`取出access.log的第一列即为ip。
2. `sort | uniq -cd`去重和排序
3. `awk '{if($1>60)print $0}'` 判断重复的数量是否超过60个，超过60个就展示出来

**操作二：** 编写shell脚本，实现整体功能（写了注释代码）
```bash
#不能把别人IP一直封着吧，这里就清除掉了
echo "" > /usr/local/nginx/conf/blockip.conf

#前面最开始编写的统计数据功能
ip_list=$(awk '{print $1}' access.log | sort | uniq -cd | awk '{if($1>60)print $0}')

#判断这个变量是否为空
if test -z "$ip_list"
then
        #为空写入 11.log中，并重新启动ngnix
        echo "为空"  >> /usr/local/nginx/logs/11.log

        /usr/local/nginx/sbin/nginx -s reload

else
        #如果不为空 前面加上 deny格式和ip写入blockip.conf中
        echo "deny" $ip_list > /usr/local/nginx/conf/blockip.conf
    
        #因为前面携带了行数，所有我们需要去除掉前面的行数，写入后在读取一次
        ip_list2=$(awk '{print $3}' /usr/local/nginx/conf/blockip.conf)
        
        #最后再把读取出来的值，在次写入到blockip.conf中
        echo "deny" $ip_list2";"> /usr/local/nginx/conf/blockip.conf

        #重启ngnix
        /usr/local/nginx/sbin/nginx -s reload
        #清空之前的日志，从最新的开始截取
        echo "" > /usr/local/nginx/logs/access.log

fi
```
**操作三：** 使用crontab定时，来实现访问每分钟超过60的
```bash
crontab -e 
* * * * * cd /usr/local/nginx/logs/ && sh ip_test.sh  每一分钟运行一次
systemctl restart crond.service 重启一下配置既可
```
![2023-01-01-13-25-55.800783300.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1672550948364-8a899822-2ae1-48eb-91c6-a1e22cb582f7.png#averageHue=%23140c0b&clientId=ub429ecab-f2e3-4&from=ui&id=ua9d37975&originHeight=182&originWidth=1320&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10563&status=done&style=none&taskId=u19739a1b-75dd-44d8-a793-2cc15f38e38&title=)
