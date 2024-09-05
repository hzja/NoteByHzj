instantbox 能够仅通过浏览器的情况下，在不到 30s 的时间内，就可以使用 web 来操作一个开箱即用的 Linux 系统，当然这里的 "系统" 是通过 docker 实现的，所以也不能直接认为就是如同 KVM 般隔离更严格的虚拟化系统。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701655145459-2cf2e9b6-2466-40b3-a5f7-26607445bb8b.png#averageHue=%23f5f5f4&clientId=u45c32397-2c72-4&from=paste&id=u3bf6f282&originHeight=311&originWidth=796&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u02e032e5-8fde-4b92-a270-1402fe0f012&title=)
<a name="xkKiC"></a>
## 1、应用场景

- 演讲时，临时需要一个干净的 Linux 环境，可以尝试使用 instantbox 为观众做演示
- 当学校教学/LUG 活动需要大家一起进行 Linux 实验时，可以让暂时无法安装 Linux 的同学体验到 Linux 的魅力
- 有了一个灵感，想要在干净的环境下尝试
- 在外边没有携带设备时，instantbox 可以在任何一台设备上对服务器进行管理（跳板机）
- 看到 GitHub 上某个非常感兴趣的项目想要尝试，却因为该项目运行在 Linux 而望而却步？instantbox 可以立刻获得一个干净的环境，可以开放一个端口用于测试需要使用端口的程序（例如开放 80 端口进行测试 nginx）
- instantbox 由于使用 docker 作为支持，所以我们使用了 cgroups 来对性能进行管理，如果想测试的某个应用在某个性能下是否能够运行，使用 instantbox 是一个非常好的选择
<a name="mtfID"></a>
## 2、快速开始
可以在这里选择一个喜欢的系统，然后系统将自动创建该系统的容器，并自动打开新的网页进入 webshell 交互<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701655145482-3eddb63b-4a00-4e47-8fe5-b418f7f8571e.png#averageHue=%23eeeeee&clientId=u45c32397-2c72-4&from=paste&id=ua3f38392&originHeight=693&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf3f10e2c-7847-4c53-945a-0fd601092be&title=)
<a name="E4xRU"></a>
## 3、如何部署
已装有 Docker 的 Linux 系统
```bash
mkdir instantbox && cd $_
bash <(curl -sSL https://raw.githubusercontent.com/instantbox/instantbox/master/init.sh)
```
默认请访问 http://<ip地址>:8888 来进行测试
<a name="nKIIB"></a>
## 4、传送门
开源地址：[https://github.com/instantbox/instantbox](https://github.com/instantbox/instantbox)
