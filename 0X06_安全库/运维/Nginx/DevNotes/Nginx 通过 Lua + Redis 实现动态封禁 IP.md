Nginx Lua Redis<br />为了封禁某些爬虫或者恶意用户对服务器的请求，需要建立一个动态的 IP 黑名单。对于黑名单之内的 IP ，拒绝提供服务。<br />实现 IP 黑名单的功能有很多途径：<br />1、在操作系统层面，配置 iptables，拒绝指定 IP 的网络请求；<br />2、在 Web Server 层面，通过 Nginx 自身的 deny 选项 或者 lua 插件 配置 IP 黑名单；<br />3、在应用层面，在请求服务之前检查一遍客户端 IP 是否在黑名单。<br />为了方便管理和共享，选择通过 Nginx+Lua+Redis 的架构实现 IP 黑名单的功能，架构图如下：

![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1632914844994-42c292d1-3c3e-4f6a-a5ba-c0018f5e183f.jpeg)
<a name="OhCZG"></a>
#### 1. 安装 Nginx+Lua模块，推荐使用 OpenResty，这是一个集成了各种 Lua 模块的 Nginx 服务器：
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632914549880-bb1c19fb-a126-4c89-907c-5f0612f951e9.webp#clientId=u85f3fa15-249d-4&from=paste&id=u5d2aefd3&originHeight=198&originWidth=732&originalType=url&ratio=1&status=done&style=none&taskId=u9c363ce9-25c9-46b4-bca3-d219978dcb8)
<a name="TtpD6"></a>
#### 2. 安装并启动 Redis 服务器
<a name="zLEUp"></a>
#### 3. 配置 Nginx 示例：
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632914549759-fc209b09-4f72-4ad8-9ab0-c1e04a8d0605.webp#clientId=u85f3fa15-249d-4&from=paste&id=u4252ab8d&originHeight=261&originWidth=640&originalType=url&ratio=1&status=done&style=none&taskId=u87b7075e-aa97-4bd6-93bb-519f31ae9fa)<br />Nginx 配置，其中
```nginx
lua_shared_dict ip_blacklist 1m
```
由 Nginx 进程分配一块 1M 大小的共享内存空间，用来缓存 IP 黑名单。
```nginx
access_by_lua_file lua/ip_blacklist.lua;
```
指定 lua 脚本位置。
<a name="JbqWB"></a>
#### 4. 配置 lua 脚本，定期从 Redis 获取最新的 IP 黑名单。
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632914549777-151ac5ed-099f-480c-9782-53b830e05752.webp#clientId=u85f3fa15-249d-4&from=paste&id=u47c1a8d5&originHeight=1136&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u23984f15-18ce-4a5b-b771-2b4c95c8102)
<a name="ZYZyA"></a>
#### 5. 在 Redis 服务器上新建 Set 类型的数据 ip_blacklist，并加入最新的 IP 黑名单。
完成以上步骤后，重新加载 nginx，配置便开始生效了。这时访问服务器，如果 IP 地址在黑名单内的话，将出现拒绝访问，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632914549776-d231fa46-62a2-4059-9e2e-fb323a166558.webp#clientId=u85f3fa15-249d-4&from=paste&id=ucd19b378&originHeight=228&originWidth=732&originalType=url&ratio=1&status=done&style=shadow&taskId=ua626f3e8-3b09-438a-811f-d3186a20793)<br />以上，便是 Nginx+Lua+Redis 实现的 IP 黑名单功能，具有如下优点：

1. 配置简单、轻量，几乎对服务器性能不产生影响；
2. 多台服务器可以通过Redis实例共享黑名单；
3. 动态配置，可以手工或者通过某种自动化的方式设置 Redis 中的黑名单。
