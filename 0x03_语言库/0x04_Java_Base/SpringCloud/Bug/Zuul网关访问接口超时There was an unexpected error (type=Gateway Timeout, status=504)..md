<a name="vTAMU"></a>
# 1、浏览器页面报错如下
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575947631583-a2aad7b7-d156-4a8e-bc0b-9209bf6bf53a.png#align=left&display=inline&height=327&originHeight=327&originWidth=764&size=40838&status=done&style=none&width=764)
<a name="IibNx"></a>
# 2、问题分析
使用Zuul搭建网关，通过网关访问一个设置了延时的接口，结果并没有进入延时回调函数，而是直接显示超时
<a name="l3aPp"></a>
# 3、解决办法
在网关服务的配置文件添加响应延时配置

```yaml
ribbon:
  ReadTimeout: 120000  #请求处理的超时时间
  ConnectTimeout: 30000  #请求连接的超时时间
```

