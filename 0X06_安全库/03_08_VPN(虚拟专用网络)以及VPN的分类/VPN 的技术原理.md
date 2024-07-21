# VPN

## SSL VPN技术

## ![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919693757-a2fe9eda-e71e-4aad-bd07-29bd9a327829.png#clientId=u19e58dd9-b562-4&from=paste&id=u58c14feb&originHeight=336&originWidth=493&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u431c2209-3f99-4ee7-950d-faf2e68cfac&title=)

SSl协议支队通信双方传输的应用数据进行加密，而不是对从一个主机到另一个主机的所有数据进行加密。

### IPSec缺陷
​        由于IPSec是基于网络层的协议，很难穿越NAT和防火墙，特别是在接入一些防护措施较为严格的个人网络和公共计算机时，往往会导致访问受阻。移动用户使用IPSec VPN需要安装专用的客户端软件，为日益增长的用户群发放、安装、配置、维护客户端软件已经使管理员不堪重负。因此，IPSec VPN在Point- to-Site远程移动通信方面并不适用。

### SSL VPN功能技术
#### 虚拟网关
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919694209-d2dcd3b5-c517-486d-ac1c-0258cea98b43.png#clientId=u19e58dd9-b562-4&from=paste&id=uce974e24&originHeight=661&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u271cf0d0-3068-43ef-944b-1bbbf3bd595&title=)

​	每个虚拟网关都是独立可管理的，可以配置各自的资源、用户、认证方式、访问控制规则以及管理员等。当企业有多个部门时，可以为每个部门或者用户群体分配不同的虚拟网关，从而形成完全隔离的访问体系。


#### WEB代理
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919694457-b0458d08-f420-4d98-879d-2783d4570905.png#clientId=u19e58dd9-b562-4&from=paste&id=ud1bac901&originHeight=303&originWidth=614&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6d6814a7-eb82-4b4b-97e6-718ed0ca7d1&title=)<br />

​	它将远端浏览器的页面请求(采用https协议)转发给web服务器，然后将服务器的响应回传给终端用户，提供细致到URL的权限控制，即可控制到用户对某一张具体页面的访问。



web代理实现对内网Web资源的安全访问：

​	Web代理有两种实现方式：Web-Link和Web改写（默认)。

- Web-link采用ActiveX控件方式，对页面进行转发。
- Web改写方式采用脚本改写方式，将请求所得页面上的链接进行改写，其他网页内容不作修改。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919694561-1054d689-df84-4429-b4ed-424c364fff28.png#clientId=u19e58dd9-b562-4&from=paste&id=u56cf52e5&originHeight=352&originWidth=626&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0aa4da6e-7f85-4188-af37-062170b18e1&title=)<br />从业务交互流程可以看出，Web代理功能的基本实现原理是将远程用户访问Web Server的过程被分成了两个阶段。首先是远程用户与NGFW虛拟网关之间建立HTTPS会话，然后NGFW虚拟网关再与Web Server建立HTTP会话。虚拟网关在远程用户访问企业内网Web Server中起到了改写、转发Web请求的作用。


#### 文件共享
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919694678-6cfaf13c-bedf-414b-b3a4-95e1e269603b.png#clientId=u19e58dd9-b562-4&from=paste&id=u258b7f2a&originHeight=356&originWidth=625&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9f602095-047d-4f20-a40c-90e3ed9dd93&title=)<br />文件共享实现过程

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919694779-33ecb234-564d-4f0f-a60d-9fbe878b26d2.png#clientId=u19e58dd9-b562-4&from=paste&id=u44c2e8c8&originHeight=162&originWidth=504&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u94554084-c549-4a63-8445-375316262e6&title=)

- 客户端向内网文件服务器发起HTTPS格式的请求，发送到USG防火墙。
- USG防火墙将HTTPS格式的请求报文转换为SMB格式的报文。
- USG防火墙发送SMB格式的请求报文给文件服务器。
- 文件服务器接受请求报文，将请求结果发送给USG防火墙，用的是SMB报文。
- USG防火墙将SMB应答报文转换为HTTPS格式。
- 将请求结果(HTTPS格式的报文)发送到客户端。




#### 端口转发

提供丰富的内网TCP应用服务。

广泛支持静态端口的TCP应用：

- 单端口单服务器（如：Telnet，SSH，MS RDP， VNC等）。
- 单端口多服务器（如：Lotus Notes）。
- 多端口多服务器（如：Outlook）。

支持动态端口的TCP应用：

- 动态端口（如：FTP，Oracle）。

提供端口级的访问控制。




##### 端口转发实现原理

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919694945-46c99be5-9385-49ff-b5e8-fdb21ac583f5.png#clientId=u19e58dd9-b562-4&from=paste&id=u07cf1c3d&originHeight=735&originWidth=620&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf8aced32-44bf-4900-a8f6-452de080996&title=)




##### 端口转发特点

- 实现对内网TCP应用的广泛支持。
- 远程桌面、outlook、Notes、FTP等。
- 所有数据流都经过加密认证。
- 对用户进行统一的授权、认证。
- 提供对TCP应用的访问控制。
- 只需标准浏览器，不用安装客户端。




#### 网络扩展

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919695123-09445281-a40a-4bea-9fe5-302419ca5d6f.png#clientId=u19e58dd9-b562-4&from=paste&id=u6f226f1c&originHeight=412&originWidth=613&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8deb73c3-2fce-4a2d-8669-6cbf318c951&title=)

- 分离模式：用户可以访问远端企业内网(通过虚拟网卡)和本地局域网(通过实际网卡)，不能访问Internet。
- 全路由模式：用户只允许访问远端企业内网(通过虚拟网卡)，不能访问Internet和本地局域网。
- 手动模式：用户可以访问远端企业内网特定网段的资源(通过虚拟网卡)，对其它Internet和本地局域网的访问不受影响(通过实际网卡)。网段冲突时优先访问远端企业内网。




##### 网络扩展实现过程

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919695183-26720e3c-a076-4608-8d61-352108cd02c9.png#clientId=u19e58dd9-b562-4&from=paste&id=u3e2e5735&originHeight=707&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uae670417-83ed-4854-a4e0-30f94d6dde2&title=)报文封装过程



可靠传输模式

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919695218-ab1ddc4b-1bd7-4b37-851b-1257e4f67d67.png#clientId=u19e58dd9-b562-4&from=paste&id=u6575fcee&originHeight=678&originWidth=909&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u82c939c3-75d2-4bb1-9f09-b3d55dc9c3d&title=)<br />

快速传输模式

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919695213-a320ce10-e90c-4891-aa62-c5df77105b52.png#clientId=u19e58dd9-b562-4&from=paste&id=ucbcb097a&originHeight=705&originWidth=875&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufc11688f-da9f-442a-a4cf-fffeba48e2a&title=)


#### 终端安全
##### 主机检查
终端安全是在请求接入内网的主机上部署一个软件，通过该软件检查终端主机的安全状况。主要包括:主机检查、缓存清理。主机检查：检查用户用来访问内网资源的主机是否符合安全要求。

主机检查策略包括如下检查项：

- 杀毒软件检查
- 防火墙检查
- 注册表检查
- 文件检查
- 端口检查
- 进程检查
- 操作系统检查

##### 缓存清理
USG可以在用户访问虚拟网关结束时，采用必要的手段清除终端.上的访问痕迹(例如生成的临时文件、Cookie等)，以防止泄密，杜绝安全隐患。

清理范围：

- Internet临时文件
- 浏览器自动保存的密码
- Cookie记录
- 浏览器的访问历史记录
- 回收站和最近打开的文档列表
- 指定文件或文件夹

#### 完善的日志功能

- 日志查询
- 日志导出
- 虚拟网关管理员日志
- 用户日志
- 系统日志

#### 认证授权
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919695605-efc72f4b-ae14-4fb1-b9f3-2c34de734733.png#clientId=u19e58dd9-b562-4&from=paste&id=u9b1a94d7&originHeight=598&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u80b107d3-2239-436b-a8a4-f2b0b43dfa0&title=)




#### 证书匿名认证

NGFW只通过验证用户的客户端证书来验证用户的身份。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919695698-e3d3a6f0-328a-4fa4-b2ac-b2e9ff536280.png#clientId=u19e58dd9-b562-4&from=paste&id=ud4f9a44e&originHeight=537&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2700d3da-4c08-4c63-8087-8c76efc07b4&title=)

1. 用户在SSLVPN网关登录界面选择证书后，客户端会将客户端证书发送给网关。
2. 网关会将客户端证书以及自己引用的CA证书的名称发送给证书模块。
3. 证书模块会根据网关引用的CA证书检查客户端证书是否可信，并将结果返回给网关：
- 如果网关引用的CA证书与客户端证书是同一个CA机构颁发的，且客户端证书在有效期内，则证书模块认为客户端证书可信，用户认证通过，继续执行4。
- 如果证书模块认为客户端证书不可信，用户认证不通过，则执行5。
4. 网关根据用户过滤字段从客户端证书中提取用户名。
- 网关会从自己的角色授权列表中查找用户所属角色从而确认此用户的业务权限。
5. 网关将认证结果返回给客户端。

认证结果为通过的用户能够登录SSLVPN网关界面，以相应的业务权限来使用SSL VPN业务。<br />认证结果为不通过的用户会在客户端上看到“您的证书验证非法，请提供合法的证书”。

#### 证书挑战认证
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919695963-2f864f65-e3ad-4779-8397-78e1ef89383e.png#clientId=u19e58dd9-b562-4&from=paste&id=u51c9aef4&originHeight=601&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0fb3e362-6156-4742-b856-8dcdca57a18&title=)

证书挑战认证是指将验证客户端证书与本地认证或服务器认证结合起来。<br />证书+本地用户名密码证书+服务器认证


### SSL VPN应用场景
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919696078-eac8641a-eabf-4d2d-b02b-9a5c3b4df4be.png#clientId=u19e58dd9-b562-4&from=paste&id=u240d965f&originHeight=437&originWidth=624&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8022114d-8285-4f86-8ab3-3fbf41d1ce0&title=)

#### SSL VPN单臂组网模式应用场景分析
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919696184-3429bd0b-4c8c-42d6-a572-f7a84b321e85.png#clientId=u19e58dd9-b562-4&from=paste&id=u33e41d7e&originHeight=433&originWidth=625&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5542cb82-83af-4462-979f-bc307ead577&title=)<br />在网络规划时，SVN的接口IP为内网IP地址，此地址需要能与所有被访问需求的服务器路由可达。防火墙上需配置nat server,将SVN的地址映射到防火墙的某一公网IP. 上。也可以只映射部分端口，如443。如果外网用户有管理SVN的需求，还需要映射SSH、Telnet等端口。


#### SSLVPN双臂组网模式应用场景分析
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919696307-58fd8e29-30f3-427b-9ac0-d7e89473eaac.png#clientId=u19e58dd9-b562-4&from=paste&id=ua29eb14e&originHeight=442&originWidth=638&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8d5e2df7-c009-4458-88e8-46615dc1af7&title=)

- 在此类组网环境中，SVN使用两个不同的网口连接外网与内网，这种组网方式下，具有清晰的内网、外网概念;无需做额外的配置，外网口对应虚拟网关IP，内网口配置内网管理IP。
- 虛拟网关IP不一定需要经过NAT转换，只要外网用户能够访问此虚拟网关IP地址即可。内外网接口没有特定的物理接口，任何一个物理接口都可以作为内网或外网接口。
- 图中路由器和交换机之间处于连接状态。这是因为客户网络中可能有部分应用不需要经过SSL加密，而是直接通过防火墙访问外网。这时就需要在交换机和路由器上配置策略路由，需要建立SSLVPN的流量就转发到SVN上，而普通的应用就直接通过防火墙访问外网。

### SSL VPN配置步骤
#### 1.配置接口
#### 2.配置安全策略

- 放行Untrust到Local安全区域的SSL VPN流量
- 放行Local到Trust安全区域的业务流量
#### 3.配置VPNDB
#### 4.虚拟网关配置
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919696564-cc911059-7635-476a-b730-d9256d8527dc.png#clientId=u19e58dd9-b562-4&from=paste&id=ubd29233e&originHeight=353&originWidth=735&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9b534610-0546-493b-b644-0f2d8c8ae42&title=)
#### 5.业务选择
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652919696700-e93c7fd5-febe-44b1-9184-1ef8d0dc9553.png#clientId=u19e58dd9-b562-4&from=paste&id=uf6484d04&originHeight=409&originWidth=630&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u591c1eb7-6813-4133-9300-6deb8bd25e8&title=) <br />ensp将防火墙该功能阉割
