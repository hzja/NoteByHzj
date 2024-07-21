Kubernetes<br />ceph是一个开源的分布式存储，它支持对象、块、文件三种存储类型，常用做kubernetes底层共享存储使用<br />在kubernetes中，为了解耦存储使用的问题，抽象了PV（PersistentVolume）和PVC（PersistentVolumeClaim），从而让使用者不用关心具体的基础设施，当需要存储资源的时候，只要像CPU和内存一样，声明要多少即可

- **PV：PV描述的是持久化存储卷，主要定义的是一个持久化存储在宿主机上的目录，比如一个NFS的挂载目录**
- **PVC：PVC描述的是Pod所希望使用的持久化存储的属性，比如，Volume存储的大小、可读写权限等等**

Kubernetes管理员设置好网络存储的类型，提供对应的PV描述符配置到Kubernetes，使用者需要存储的时候只需要创建PVC，然后在Pod中使用Volume关联PVC，即可让Pod使用到存储资源，它们之间的关系如下图所示<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681279271767-ffd40cfc-b2b1-47bc-bab4-b27ecf34eb3d.png#averageHue=%23fdfdfd&clientId=u6da02022-e006-4&from=paste&id=u7e4f4de1&originHeight=538&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc5dbabe9-b895-40d2-b775-cb766a3b810&title=)<br />使用PV和PVC方法虽然能实现屏蔽底层存储，但是PV创建比较复杂，Kubernetes解决这个问题的方法是提供动态配置PV的方法，可以自动创PV。管理员可以部署PV配置器（provisioner），然后定义对应的StorageClass，这样开发者在创建PVC的时候就可以选择需要创建存储的类型，PVC会把StorageClass传递给PV provisioner，由provisioner自动创建PV。如CCE就提供csi-disk、csi-nas、csi-obs等StorageClass，在声明PVC时加上StorageClassName，就可以自动创建PV，并自动创建底层的存储资源<br />简单交代一下kubernetes中存储的几个基本知识点，来说一下使用误区<br />前面说到PV，PV是集群中的一块网络存储，它有三种访问模式，分别是：

- `**ReadWriteOnce**`**（RWO）：是最基本的方式，可读可写，但只支持被单个节点挂载。**
- `**ReadOnlyMany**`**（ROX）：可以以只读的方式被多个节点挂载。**
- `**ReadWriteMany**`**（RWX）：这种存储可以以读写的方式被多个节点共享。**

说的误区就是这里，在网上很多的文档中，都说ceph的块存储不支持ReadWriteMany（RWX），也就一直在这样的错误认知中，还很胸有成竹的认定ceph的块不支持RWX，无法多节点共享，直到大佬把官方说明甩过来<br />![ceph官方截图](https://cdn.nlark.com/yuque/0/2023/png/396745/1681279271736-8781a33e-ffc1-4ef2-8226-4a2f0f055ae9.png#averageHue=%23eaeaea&clientId=u6da02022-e006-4&from=paste&id=u7d358e4b&originHeight=231&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u7ff4326b-cd10-4c62-b639-888a7fab16a&title=ceph%E5%AE%98%E6%96%B9%E6%88%AA%E5%9B%BE "ceph官方截图")<br />实际上是否支持RWX或者RWO，有一个关键参数，是volumeMode，通常使用，大多时候是filesystem，也就是以文件的形式挂载到pod内进行使用，而底层如果是块存储，是支持block方式挂载，只要上层pod可以使用block就可以<br />通过以下实验看下：<br />底层ceph块存储和`StorageClass`提前已经装好了，通过创建pv、pvc，使用不同的accessmodes和volumeMode实验<br />![storageclass](https://cdn.nlark.com/yuque/0/2023/png/396745/1681279271750-5e8d8f01-d645-40ba-9f70-7238a0598b6b.png#averageHue=%232c2c2c&clientId=u6da02022-e006-4&from=paste&id=uc5ea7dd2&originHeight=906&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u6b194860-4822-40d2-bd38-30389fce01d&title=storageclass "storageclass")<br />![ceph 块存储集群](https://cdn.nlark.com/yuque/0/2023/png/396745/1681279271806-40b81e5a-bf4b-41a9-ba4a-01f131ef6617.png#averageHue=%23262626&clientId=u6da02022-e006-4&from=paste&id=udb20ada0&originHeight=345&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uf726d487-9aa7-4ea7-9f1c-2c51e196b0d&title=ceph%20%E5%9D%97%E5%AD%98%E5%82%A8%E9%9B%86%E7%BE%A4 "ceph 块存储集群")<br />先创建一个基于块的`PersistentVolumeClaim`，它利用上面创建的基于ceph-csi的`StorageClass`，可以使用以下 YAML 从csi-rbd-sc StorageClass请求原始块存储
```yaml
apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: raw-block-pvc
spec:
  accessModes:
    - ReadWriteOnce
  volumeMode: Block
  resources:
    requests:
      storage: 1Gi
  storageClassName: csi-rbd-sc
```
`accessModes`是RWO，`volumeMode`是block，看一下是否能成功创建<br />![RWO-block创建成功](https://cdn.nlark.com/yuque/0/2023/png/396745/1681279271731-6b80a4e8-6956-4eca-bc40-32cdbbc110f7.png#averageHue=%23292929&clientId=u6da02022-e006-4&from=paste&id=u67bda294&originHeight=410&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u34c81092-771c-4df7-bd0f-90e255185c0&title=RWO-block%E5%88%9B%E5%BB%BA%E6%88%90%E5%8A%9F "RWO-block创建成功")<br />可以看到，状态是Bound，正常创建成功<br />接着用上面创建的基于ceph-csi的`StorageClass`创建基于文件系统的`PersistentVolumeClaim`
```yaml
apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: rbd-pvc
spec:
  accessModes:
    - ReadWriteOnce
  volumeMode: Filesystem
  resources:
    requests:
      storage: 1Gi
  storageClassName: csi-rbd-sc
```
`accessMode`还是RWO，`volumeMode`是filesystem，看是否创建成功<br />![RWO-filesystem创建成功](https://cdn.nlark.com/yuque/0/2023/png/396745/1681279272370-7918bb90-767b-4cab-b6bf-c25c97b9530b.png#averageHue=%23292929&clientId=u6da02022-e006-4&from=paste&id=u65190695&originHeight=386&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u907c06bb-0011-425b-82c0-6c1420cb7be&title=RWO-filesystem%E5%88%9B%E5%BB%BA%E6%88%90%E5%8A%9F "RWO-filesystem创建成功")<br />可以看到状态是bound，正常创建成功<br />接着试下RWX<br />先试试用block的`volumeMode`
```yaml
apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: raw-block-pvc-rwx
spec:
  accessModes:
    - ReadWriteMany
  volumeMode: Block
  resources:
    requests:
      storage: 1Gi
  storageClassName: csi-rbd-sc
```
看下是否能正常bound<br />![RWX-block创建成功](https://cdn.nlark.com/yuque/0/2023/png/396745/1681279272627-81415131-68d5-454c-9625-7afb39adfca7.png#averageHue=%23272727&clientId=u6da02022-e006-4&from=paste&id=udb7b3db7&originHeight=357&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u04dfe60b-782c-4f38-ba2d-22c686b6c1c&title=RWX-block%E5%88%9B%E5%BB%BA%E6%88%90%E5%8A%9F "RWX-block创建成功")<br />可以看到，基于块存储，RWX模式也是可以正常bound的<br />接着再试下filesystem的`volumeMode`
```yaml
apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: rbd-pvc-rwx
spec:
  accessModes:
    - ReadWriteMany
  volumeMode: Filesystem
  resources:
    requests:
      storage: 1Gi
  storageClassName: csi-rbd-sc
```
apply看下是否正常bound<br />![RWX-filesystem无法创建成功](https://cdn.nlark.com/yuque/0/2023/png/396745/1681279272692-cd7317f2-81b1-4a52-a442-9df854cc1947.png#averageHue=%23262626&clientId=u6da02022-e006-4&from=paste&id=u98e02daa&originHeight=353&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u45719b5f-e223-44ff-b2d5-1e14395874f&title=RWX-filesystem%E6%97%A0%E6%B3%95%E5%88%9B%E5%BB%BA%E6%88%90%E5%8A%9F "RWX-filesystem无法创建成功")<br />可以看到`accessModes`是RWX的情况下，`volumeMode`为filesystem是不支持的，无法创建成功，而如果ceph是文件存储的类型时，`accessModes`是RWX，`volumeMode`是filesystems才可以创建成功。<br />总结：块存储或文件存储是否支持RWX或RWO，不单取决于它是块还是文件，还取决于上层pvc的`accessModes`，以及`volumeMode`。
