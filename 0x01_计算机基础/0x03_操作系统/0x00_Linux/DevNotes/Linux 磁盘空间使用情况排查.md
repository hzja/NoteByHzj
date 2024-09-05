Linux 磁盘<br />在服务器运维过程中，时常会收到服务器磁盘空间告警<br />登录服务器，通过`df -Hl`查看<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970677496-7cdd27d2-7998-4cc6-8016-4b2e024961aa.webp#clientId=uffab3d57-85d4-4&from=paste&id=u1a8d3b56&originHeight=1&originWidth=1&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6d860be1-363a-499d-8490-b27ddfc3b9b&title=)![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970677327-e13fb2f1-7f7d-4c40-bb8f-322935f1e354.webp#clientId=uffab3d57-85d4-4&from=paste&id=u645fac1a&originHeight=218&originWidth=645&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u34c0a28e-7209-4098-a0f6-29879c917ac&title=)<br />和告警信息一致，接着就是要找到导致磁盘空间满的目录或文件
<a name="Nho2u"></a>
## 如何找到占用空间大的目录或文件？
一种比较笨的方法是，在根目录下，通过`du -hs`命令，列出各目录所占空间大小<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970677289-8a445ef4-33d6-4d5b-8d92-6982eea3a5b0.webp#clientId=uffab3d57-85d4-4&from=paste&id=u22c97174&originHeight=674&originWidth=1012&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u641f6d22-223f-4bb7-ab63-7342be77150&title=)<br />之后再用同样的方法继续到对应目录下去找<br />再相对高效一点的方法是通过`du`的`-d`参数，或`--max-depth`，设置查询的目录深度，目录深度增加，所查询的目录，展示出来会很多，这个时候可以通过`grep`进行过滤
```bash
du -h -d 2|grep [GT] |sort -nr
du -h --max-depth=2|grep [GT] |sort -nr
```
通过这样的方式，可以搜出以G或者T为单位的占用磁盘空间的大目录，并排序<br />或者可以通过`find`来查询
```bash
find / -type f -size +1G -exec du -h {} \;
```
从效率上来说，find要比du要更快速、灵活<br />通过这两种方法，可以快速找到占用磁盘空间的罪魁祸首<br />就这么简单？很多时候会发现，通过find或du查半天，发现所有加起来的占用空间，和df看到的磁盘空间占用，相差很大，就比如上面的两张图<br />通过df查看，磁盘使用37G，但是在根目录下通过du -hs 查看，总共加起来差不多10G，没有隐藏目录，那空间被谁吃了？<br />很明显，有空间被已删除文件占用，文件删除了，但是资源没释放<br />有一个很好用的命令：lsof，可以通过以下命令去查看
```bash
lsof +L1
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970677976-8c9bdec1-e089-438b-8f90-c8496531e431.webp#clientId=uffab3d57-85d4-4&from=paste&id=u432e5397&originHeight=360&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufa90ec73-e107-4ec4-a01c-9ead4346a0e&title=)<br />从结果可以看出，有一个28G左右的大日志文件，删除了，但是空间没释放，这是很常见的一种情况<br />对应的解决方法就是，重启tomcat应用，释放空间
<a name="mx4WG"></a>
## 磁盘空间莫名被吃？
还有一种经常遇到的问题，就是，通过`df`查看到的磁盘<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970677941-61b584af-0226-4a0a-938f-6b3aca3db28d.webp#clientId=uffab3d57-85d4-4&from=paste&id=u35cde3c2&originHeight=183&originWidth=924&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9cf5a1b2-1525-42b1-8d41-e445a493041&title=)<br />会发现，Used和Avail加起来不够Size，莫名被吃掉一部分<br />其实这是Linux文件系统的一种安全策略，它默认会为root用户保留5%的磁盘空间，留作紧急情况使用。这样能保证有些关键应用（比如数据库）在硬盘满的时候有点余地，不致于马上就 crash<br />可以通过tune2fs修改预留空间的比例
```bash
tune2fs -m 1 /dev/vda1
```
通过下图可以看到前后对比![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970678026-df3a6945-3892-4b2d-a19a-00bb9caad173.webp#clientId=uffab3d57-85d4-4&from=paste&id=uc8b4e1c2&originHeight=431&originWidth=1011&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc86cb809-9996-42f8-b20f-5f44683e8a1&title=)<br />这样被吃掉的空间，就吐出来了
