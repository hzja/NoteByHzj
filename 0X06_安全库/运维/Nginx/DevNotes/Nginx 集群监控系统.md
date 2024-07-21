Nginx<br />搭建了Nginx集群后，需要继续深入研究的就是日常Nginx监控。<br />Nginx如何监控？答案就是通过Nginx-status，实时获取到Nginx监控数据后，如何和现有监控系统集成？一个很好的解决方案：Nginx+Telegraf+Influxdb+Grafana<br />即通过Telegraf监控插件定时收集Nginx的监控状态，存储到时序数据库Influxdb中，然后通过Grafana展现即可。
<a name="jQJ4p"></a>
## 一、Nginx启用nginx-status功能
源码编译安装的nginx，那么需要在编译的时候加上对应的模块
```nginx
./configure --with-http_stub_status_module
```
使用 `./configure --help` 能看到更多的模块支持。然后编译安装即可。<br />如果是直接 `apt-get install` 安装的 nginx，那么使用命令来查看是否支持 stub_status 这个模块。<br />如下命令：`nginx –V` 看看是否有 `--with-http_stub_status_module` 这个模块。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647522957527-620e566c-1ee9-4ffa-bcc9-1ba0029a9813.png#clientId=ue59c94f1-5ee8-4&from=paste&id=u5838b1fa&originHeight=166&originWidth=559&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u626cef74-421d-4110-a313-d219b7e2027&title=)<br />修改Nginx配置文件：在Server章节中增加：
```nginx
location /nginx-status {
  allow 127.0.0.1; //允许的IP
  deny all;
  stub_status on;
  access_log off;
}
```
Reload 重启Nginx，查看Nginx-Status<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647522956447-94e27c98-39ec-4ada-aa7f-ab72bb92a623.png#clientId=ue59c94f1-5ee8-4&from=paste&id=uf2150285&originHeight=85&originWidth=526&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue7d2f241-bdba-44b4-a2b9-d4ae5b3d249&title=)<br />输出信息的说明：

- `active connections` – 活跃的连接数量
- `server accepts handled requests` — 总共处理了11989个连接，成功创建11989次握手，总共处理了11991个请求
- reading — 读取客户端的连接数.
- writing — 响应数据到客户端的数量
- waiting — 开启 keep-alive 的情况下，这个值等于 active – (reading+writing)，意思就是 Nginx 已经处理完正在等候下一次请求指令的驻留连接.
<a name="IddYo"></a>
## 二、Telegraf安装配置Nginx监控
关于Telegraf的安装，请参考官方介绍：[https://www.influxdata.com/time-series-platform/telegraf/](https://www.influxdata.com/time-series-platform/telegraf/)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647522956442-820569bb-d1e8-4e86-9d2d-bf262210bd81.png#clientId=ue59c94f1-5ee8-4&from=paste&id=uffb3826f&originHeight=240&originWidth=985&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u478589c7-e440-4131-b500-933d4ec37cb&title=)
```bash
wget https://dl.influxdata.com/telegraf/releases/telegraf-1.4.3-1.x86_64.rpm
sudo yum localinstall telegraf-1.4.3-1.x86_64.rpm
```
然后，在配置文件teldgraf.conf中配置Influxdb连接<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647522956540-0e8eb612-f71d-4771-8e0f-2bc0452a6d23.png#clientId=ue59c94f1-5ee8-4&from=paste&id=u1c55e74c&originHeight=292&originWidth=619&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud044c349-d4f1-44e1-86b2-35fa9f5f9e7&title=)<br />增加对Nginx的监控<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647522956393-f14361fa-4623-4749-bf1c-32be95870b2d.png#clientId=ue59c94f1-5ee8-4&from=paste&id=u7105b57b&originHeight=671&originWidth=689&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue39fe86a-607e-4701-9fb1-21907884ccd&title=)<br />配置完成，重启telegraf服务即可。
<a name="USPk6"></a>
## 三、Grafana集成Nginx监控
Grafana中支持Influxdb数据源，配置上上个步骤的Influxdb数据源之后，定制Nginx监控图表：

- 数据源：Influxdb
- FROM：nginx
- SELECT：field（accepts）

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647522956761-c1a03a4f-c3ec-418c-90b5-3b75dce7ff32.png#clientId=ue59c94f1-5ee8-4&from=paste&id=u31213463&originHeight=248&originWidth=1058&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud4a9c603-c510-47e8-859f-f609c4418ec&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647522956797-5902ef5e-730a-446c-9692-c8b0c6a9f3ea.png#clientId=ue59c94f1-5ee8-4&from=paste&id=u1ce4364f&originHeight=473&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0686a5ee-fdad-42d8-9469-6aab692edab&title=)展现效果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647522956850-301bc368-f94b-406f-9fe8-6759406bf3ad.png#clientId=ue59c94f1-5ee8-4&from=paste&id=u27b238bb&originHeight=579&originWidth=946&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u087cd8c0-4a67-4a04-b5e3-98cefa57d13&title=)<br />以上通过Nginx+Telegraf+Influxb+Grafana，实现了Nginx的监控，非常方便。
