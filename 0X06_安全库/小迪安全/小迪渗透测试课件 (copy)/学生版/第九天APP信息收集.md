# 第九天APP信息收集

## 一、bp抓取手机数据包

手机IP地址：192.168.1.3

kali：192.168.1.9



+ burp配置![image58](D:\Note\网络安全\图片\image58.png)



+ 手机配置

![image59](D:\Note\网络安全\图片\image59.png)



+ 抓包测试

![image61](D:\Note\网络安全\图片\image61.png)



+ 配置证书

​		在浏览器中输入192.168.1.9:8888下载证书并重名为ca.cer 然后导入

![image62](D:\Note\网络安全\图片\image62.png)



+ 导入证书

  设置-密码与安全-系统安全-加密与凭证-从sd卡安装-然后搜索ca.cer

![image63](D:\Note\网络安全\图片\image63.png)



+ 抓包测试

![image64](D:\Note\网络安全\图片\image64.png)



## 二、对抓取的数据包进行分析

​		通过对抓取到的数据包进行分析、数据包上面可能有域名有ip地址。对上面的数据包进行信息收集、借助shodan、钟馗之眼、fofa等工具进行端口扫描

![image65](D:\Note\网络安全\图片\image65.png)

![image66](D:\Note\网络安全\图片\image66.png)

![image67](D:\Note\网络安全\图片\image67.png)