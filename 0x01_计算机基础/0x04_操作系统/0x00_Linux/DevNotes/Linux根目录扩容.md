Linux
<a name="n9Vts"></a>
## Linux根目录扩容的步骤
<a name="V7Z4A"></a>
### 查询磁盘使用情况
```bash
df -hl
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643945756758-2c23649a-0297-44ae-8094-82396cf4fb29.webp#clientId=ud6ab5a02-d804-4&from=paste&id=u00376f55&originHeight=369&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u62232658-f122-461d-b8ba-6d048987eeb&title=)
<a name="KQcBo"></a>
### 查询磁盘
```bash
fdisk -l
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643945756699-8f22395b-1f72-4dd8-b901-8b0b20104259.webp#clientId=ud6ab5a02-d804-4&from=paste&id=u4cc12f3f&originHeight=553&originWidth=701&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3b6e4419-9046-4797-a4b9-642cf8b447b&title=)
<a name="RCEZd"></a>
### 使用添加的磁盘
```bash
fdisk /dev/sdb
```
输入这条命令后会出现下图，跟着操作命令命令即可<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643945756444-8aa7d4e1-9294-4ecb-996a-a0bccd77ea7a.webp#clientId=ud6ab5a02-d804-4&from=paste&id=u2e8a578b&originHeight=597&originWidth=761&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue985d6fd-e645-4dfc-8e81-88eda618a85&title=)
<a name="SVxyP"></a>
### 创建物理卷
```bash
pvcreate /dev/sdb1
```
<a name="XZM9t"></a>
### 查询创建的物理卷
```bash
pvdisplay
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643945757058-24d671b5-4fe0-415e-93f0-032a888a689f.webp#clientId=ud6ab5a02-d804-4&from=paste&id=u9d048552&originHeight=1030&originWidth=1018&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud2222a43-aaa0-473a-b136-ceca87adbf0&title=)
<a name="IUsiM"></a>
### 通过vgextend将新添加卷加入到卷组centos中
```bash
vgextend centos /dev/sdb1
```
<a name="aRsZJ"></a>
### 查看卷组空闲块Free PE
```bash
vgdisplay
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643945756423-185b40bc-aae5-4ccc-b951-5e8a4fbbf14f.webp#clientId=ud6ab5a02-d804-4&from=paste&id=u6f3357d8&originHeight=828&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0a74ef76-6f14-4aad-a9b0-ea8b9f406aa&title=)
<a name="TSvkV"></a>
### 向要扩容的目录添加扩容容量
通过 lvextend 命令扩展根目录挂载点 /dev/mapper/centos-root
```bash
lvextend -L +60G /dev/mapper/centos-root
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643945757257-318662bb-209f-4dfa-9d04-dd34bc045f49.webp#clientId=ud6ab5a02-d804-4&from=paste&id=u27111e76&originHeight=60&originWidth=751&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3bb328de-512c-4c09-aea9-66077ec157f&title=)
```bash
xfs_growfs /dev/mapper/centos-root
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643945757559-2d7bd81b-04b2-446b-a333-e22e4f3c38a5.webp#clientId=ud6ab5a02-d804-4&from=paste&id=u9c685120&originHeight=362&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub8684658-f918-4ea6-a92a-2ce28b7a672&title=)
<a name="D0m58"></a>
### `df -hl` 查看扩容情况
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643945757833-47fcd384-d97f-4622-a391-279767f79b14.webp#clientId=ud6ab5a02-d804-4&from=paste&id=uc2b30e4e&originHeight=432&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc9754a28-76b8-4ab5-8236-87a4a8891e3&title=)<br />看到上图红框地方，说明扩容成功。
