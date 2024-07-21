Linux KVM
<a name="7JW3q"></a>
### 虚拟机管理
<a name="K4BRO"></a>
#### 1、创建虚拟机
```
virsh define damo.xml          //创建的虚拟机不是活动的
virsh create damo.xml          //创建的虚拟机是活动的，但是虚拟机关机之后会消失
damo.xml是虚拟机的配置文件，一般位置/etc/libvirt/qemu/下，也可自己编辑配置文件
```
<a name="08JSN"></a>
#### 2、查看KVM虚拟机的状态
```bash
virsh list
```
<a name="0a3fk"></a>
#### 3、查看所有虚拟机的状态
```bash
virsh list --all
```
<a name="Vwa4a"></a>
#### 4、启动虚拟机
```bash
virsh start  虚拟机name
```
<a name="idXE9"></a>
#### 5、关闭虚拟机
```bash
virsh shutdown  虚拟机name     //正常关闭虚拟机
virsh destroy  虚拟机name      //强制关闭虚拟机
```
<a name="GBbtY"></a>
#### 6、重启虚拟机
```bash
virsh reboot
```
<a name="D94ZZ"></a>
#### 7、挂起/暂停虚拟机
```bash
virsh  suspend  虚拟机name
```
<a name="KBP2i"></a>
#### 8、恢复挂起/暂停的虚拟机
```bash
virsh resume  虚拟机name
```
<a name="gcFKn"></a>
#### 9、删除虚拟机
```bash
virsh undefine 虚拟机name  //同时删除virsh列表里面的name与当前配置文件，删除的虚拟机必须是不活动的
```
<a name="UycBd"></a>
#### 10、删除虚拟机并删除磁盘文件
```bash
virsh undefine --storage 目标文件，用逗号分开的目标或者源路径列表
```
<a name="2e75q"></a>
#### 11、删除虚拟机并删除所有磁盘文件
```bash
virsh undefine --remove-all-storage
```
<a name="loloF"></a>
#### 12、KVM虚拟机随物理机开机启动
```bash
virsh autostart 虚拟机名
```
<a name="zX1Zl"></a>
#### 13、禁止开机启动
```bash
virsh autostart --disable
```
<a name="EZo1B"></a>
#### 14、显示虚拟机的基本信息
```bash
virsh dominfo 虚拟机name
```
<a name="QxvUb"></a>
#### 15、显示虚拟机的当前配置文件
```bash
virsh dumpxml 虚拟机name
```
<a name="79uVW"></a>
### 快照管理
<a name="dZCb3"></a>
#### 1、查看快照列表
```bash
virsh snapshot-list
```
<a name="iB0ov"></a>
#### 2、编辑快照
```bash
virsh snapshot-edit --snapshotname
```
<a name="PyITX"></a>
#### 3、恢复快照
```bash
virsh snapshot-revert --snapshotname
```
<a name="df7b572c"></a>
#### 4、删除快照
```bash
virsh snapshot-delete --snapshotname
```
<a name="w1K6s"></a>
#### 5、创建快照
```bash
virsh snapshot-create-as --name --description "this is a test snapshot"
```
<a name="3E6S2"></a>
#### 6、克隆虚拟机
```bash
virt-clone -o node1 -n node2 -f /home/kvm/images/node3.img      //node1是需要克隆的虚拟机，node2是克隆之后的虚拟机name，/home/kvm/images/是克隆的镜像的存储位置，node3.img 是镜像的名称与格式
```
<a name="Fs89V"></a>
#### 7、命令帮助信息
```bash
virt-clone --help
```
<a name="2OO7V"></a>
#### 8、查看帮助信息
```bash
virsh snapshot-create-as --help
```
<a name="wvRHJ"></a>
### 硬盘管理
<a name="OksZr"></a>
#### 1、查看镜像
```bash
qemu-img info /kvm/centos1_1.qcow2(路径)
```
<a name="4KjKc"></a>
#### 2、创建镜像
```bash
qemu-img create -f qcow2(格式) /kvm/centos1_1.qcow2(路径) 20G(容量)
```
<a name="eXeHB"></a>
#### 3、修改镜像容量扩容
```bash
qemu-img resize /kvm/centos1_1.qcow2(路径) +100G(增加的容量)
```
<a name="OpDbL"></a>
#### 4、删除镜像
```bash
rm -rf 谨慎使用
```
<a name="OculF"></a>
#### 5、查看实例
```bash
virsh dumpxml KVM实例名
```
<a name="D5Ofc"></a>
#### 6、保存
```bash
virsh save KVM实例名
virsh edit KVM实例名
```
<a name="RcAig"></a>
### vcpu管理
<a name="ijkE6"></a>
#### 1、查看vcpu信息
```bash
virsh vcpuinfo
```
<a name="867df6a5"></a>
#### 2、查询vcpu亲和性
```bash
virsh vcpupin
```
<a name="HqB5q"></a>
#### 3、显示本机vcpu最大值
```bash
virsh maxvcpus
```
<a name="byVSL"></a>
#### 4、使用vcpu数
```bash
virsh setvcpus 4 --config
```
<a name="9fIes"></a>
#### 5、减少vcpu
```
virsh setvcpus 1 --config
virsh setvcpus 1 --maximum --config
```
<a name="6p6g7"></a>
### 内存管理
<a name="nBBUv"></a>
#### 1、设置内存分配
```bash
virsh setmem [--size] 5G --current
```
<a name="JdHWs"></a>
#### 2、减少内存
```bash
virsh setmem [--size] 512M --current
virsh setmaxmem [--size] 512M --current
```
<a name="6qs5m"></a>
#### 3、设置最大内存限制值
```bash
virsh setmaxmem [--size] 5G --current
```
