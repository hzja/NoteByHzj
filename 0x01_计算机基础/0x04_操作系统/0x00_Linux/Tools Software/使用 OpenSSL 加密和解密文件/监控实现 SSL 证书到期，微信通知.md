<a name="lIa89"></a>
## 1、准备工作
创建一个企业微信账号，并创建一个组，在组里面配置企业微信提供的机器人。将机器人提供的 WebHook 地址保存。
<a name="T6oBQ"></a>
## 2、编写脚本
```bash
[root@Nginx ~]# cat ssl-monitor.sh
#!/bin/bash

# 定义网站域名和端口号信息
WebName="www.baidu.com"
Port="443"

# 通过 Openssl 工具获取到当前证书的到期时间
Cert_END_Time=$(echo | openssl s_client -servername ${
   
     WebName} -connect ${
   
     WebName}:${
   
     Port} 2> /dev/null | openssl x509 -noout -dates | grep 'After' | awk -F '=' '{print $2}' | awk '{print $1,$2,$4}')

# 将证书的到期时间转化成时间戳
Cert_NED_TimeStamp=$(date +%s -d "$Cert_END_Time")

# 定义当前时间的时间戳
Create_TimeStamp=$(date +%s)

# 通过计算获取到证书的剩余天数
Rest_Time=$(expr $(expr $Cert_NED_TimeStamp - $Create_TimeStamp) / 86400)

# 配置告警提示信息
echo "$WebName  网站的 SSL 证书还有 $Rest_Time 天后到期" > ssl-monitor.txt

# 判断出证书时间小于 30 天的
if [ $Rest_Time -lt 30 ];then

# 定义企业微信机器人的 API 接口
WebHook='https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=2743320b-0a2c-404b-87bc-25fedf1ff67a'

# 通过 Curl 命令来发送 Post 请求
curl "${WebHook}" -H 'Content-Type: application/json' -d '
{
    "msgtype": "text",
    "text": { 
        "content": "'"$(cat ssl-monitor.txt)"'"
    }
}' &> /dev/null
fi
```
因为上面有配置条件判断，所以在验证时，可以先将条件判断删除。
<a name="WSmjH"></a>
## 3、手动验证
```bash
[root@Nginx ~]# bash ssl-monitor.sh 
```
<a name="xy498"></a>
## 4、配置到 CronJob 中
```bash
[root@Nginx ~]# crontab -e
* 23 * * * /bin/bash /root/ssl-monitor.sh
```
