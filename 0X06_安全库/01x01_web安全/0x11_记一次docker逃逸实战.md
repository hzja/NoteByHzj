# 记录一次某次红蓝对抗实战的docker逃逸

~~~ tex
Docker逃逸是指攻击者在成功入侵一个Docker容器后，进一步获取宿主机上的权限或访问宿主机上的资源的行为。Docker作为一种流行的容器化技术，虽然在设计上提供了隔离性，但在某些情况下，仍然可能存在安全漏洞或配置不当，导致容器与宿主机之间的隔离被突破。以下是一些可能导致Docker逃逸的原因和方法[^13^][^14^][^16^][^17^][^19^][^22^]：

1. **内核漏洞**：Docker容器共享宿主机的内核，如果宿主机的内核存在安全漏洞，如Dirty COW (CVE-2016-5195)，攻击者可能利用这些漏洞实现逃逸[^16^][^22^]。

2. **Docker软件设计漏洞**：Docker软件本身可能存在设计上的安全漏洞，例如runc的漏洞CVE-2019-5736，攻击者通过特定的操作可以获取宿主机的root权限，实现逃逸[^14^][^16^]。

3. **特权模式和配置不当**：Docker的特权模式（`--privileged`）会给予容器内root用户与宿主机root用户相同的权限。如果不当使用特权模式，或者容器配置不当，如挂载了宿主机的目录，攻击者可能利用这些配置不当实现逃逸[^13^][^19^][^22^]。

4. **Docker Remote API未授权访问**：如果Docker Remote API未进行适当的授权和保护，攻击者可能通过未授权访问API来实现逃逸[^16^]。

5. **功能机制（capabilities）和sock通信方式**：Docker使用功能机制提供细粒度的权限控制，如果这些功能被不当配置，或者sock通信方式被滥用，也可能成为逃逸的途径[^17^]。

为了防止Docker逃逸，可以采取以下措施：

- 定期更新和修补宿主机和Docker软件，以修复已知的安全漏洞。
- 避免使用特权模式，或者仅在绝对必要时使用，并严格控制权限。
- 正确配置容器的网络和存储，避免不必要的宿主机资源暴露。
- 保护Docker Remote API，确保只有授权用户才能访问。
- 监控和审计容器的活动，以便及时发现和响应可疑行为。

Docker逃逸是一个严重的安全问题，需要通过综合的安全措施和良好的配置实践来防范。
~~~



某个站点存在shiro，直接打，默认key。<br />但是这里有个奇怪的点，就是只有用静态页面回显才能成功利用feihong工具打，不然其他回显可能利用失败甚至连默认key都测不了。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1619696762839-667c14da-8808-49dc-90db-2f36af03c799.png#id=vF074&originHeight=584&originWidth=733&originalType=binary&status=done&style=none)<br />确认root权限无误<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1619696763392-9512ad15-4291-4db9-8951-d42ff702fa60.png#id=SnFoC&originHeight=164&originWidth=509&originalType=binary&status=done&style=none)<br />直接写入webshell，利用蝎子直接上webshell，一波操作猛如虎<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1619696763917-7e418ad2-8b8b-4b3e-b9fa-043e7867059f.png#id=y3sRG&originHeight=116&originWidth=571&originalType=binary&status=done&style=none)<br />突然ifconfig发现没有这条命令？！怀疑是docker容器<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1619696764418-ff0b289f-e722-4399-b2ff-870cd7b94380.png#id=VuVac&originHeight=119&originWidth=573&originalType=binary&status=done&style=none)<br />判断是否存在docker容器，发现还真的是。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1619696765006-605e900f-3ac5-4b52-995f-16ca1a51392f.png#id=S55ei&originHeight=475&originWidth=652&originalType=binary&status=done&style=none)<br />确认是docker容器无误了。于是百度寻求逃逸的思路。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1619696765484-b2586d30-57a8-4340-bd2e-26e7c2d8a2a6.png#id=gIvA6&originHeight=70&originWidth=645&originalType=binary&status=done&style=none)<br />于是乎找到一个利用特权模式下逃逸的情况，因为管理员利用了这条命令docker run —privileged来启动docker，所以docker容器被赋予了特权模式。这个时候攻击者即使在docker环境下也可通过mount命令将外部宿主机磁盘设备挂载进容器内部，获取对整个宿主机的文件读写权限。<br />操作如下 fdisk -l   箭头所指，带星号的目录就是可以用来挂载的。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1619696766106-9e82160c-ab2f-4a12-bbb7-042899b4cbd1.png#id=kwsCs&originHeight=641&originWidth=640&originalType=binary&status=done&style=none)<br />新建一个目录。这个时候ls还看不到目录下的内容<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1619696766474-15b1821e-bb9a-423e-8a17-f603c3d13d82.png#id=rboUU&originHeight=149&originWidth=608&originalType=binary&status=done&style=none)<br />将带星号的目录挂载至新建的目录。通过mount命令。然后再ls新建的目录，这个时候就能看到宿主机内部目录了！逃逸成功。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1619696766965-4de34579-6190-4fd3-aac6-8e9610e2e957.png#id=EWDcz&originHeight=430&originWidth=682&originalType=binary&status=done&style=none)

这个时候，蝎子的目录也会有多出来的挂载目录<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1619696767297-072de0e2-59f7-46cb-9cec-1422d3b1d3f6.png#id=eNWgM&originHeight=764&originWidth=1030&originalType=binary&status=done&style=none)
