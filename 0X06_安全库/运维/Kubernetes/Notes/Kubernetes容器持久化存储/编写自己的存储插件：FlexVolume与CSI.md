Kubernetes<br />在 Kubernetes 中，存储插件的开发有两种方式：FlexVolume 和 CSI。
<a name="KMF3p"></a>
## Flexvolume 的原理和使用方法
举个例子，现在要编写的是一个使用 NFS 实现的 FlexVolume 插件。对于一个 FlexVolume 类型的 PV 来说，它的 YAML 文件如下所示：
```yaml
apiVersion: v1
kind: PersistentVolume
metadata:
  name: pv-flex-nfs
spec:
  capacity:
    storage: 10Gi
  accessModes:
    - ReadWriteMany
  flexVolume:
    driver: "k8s/nfs"
    fsType: "nfs"
    options:
      server: "10.10.0.25" # 改成你自己的NFS服务器地址
      share: "export"
```
可以看到，这个 PV 定义的 Volume 类型是 flexVolume。并且指定了这个 Volume 的 driver 叫作 k8s/nfs。这个名字很重要。<br />而 Volume 的 options 字段，则是一个自定义字段。也就是说，它的类型，其实是 `map[string]string`。所以，可以在这一部分自由地加上想要定义的参数。<br />在这个例子里，options 字段指定了 NFS 服务器的地址（server: “10.10.0.25”），以及 NFS 共享目录的名字（share: “export”）。当然，这里定义的所有参数，后面都会被 FlexVolume 拿到。
:::tips
备注：可以使用[这个 Docker 镜像](https://github.com/ehough/docker-nfs-server)轻松地部署一个试验用的 NFS 服务器。
:::
像这样的一个 PV 被创建后，一旦和某个 PVC 绑定起来，这个 FlexVolume 类型的 Volume 就会进入到Volume 处理流程。<br />这个流程的名字叫作“两阶段处理”，即“Attach 阶段”和“Mount 阶段”。它们的主要作用，是在 Pod 所绑定的宿主机上，完成这个 Volume 目录的持久化过程，比如为虚拟机挂载磁盘（Attach），或者挂载一个 NFS 的共享目录（Mount）。<br />而在具体的控制循环中，这两个操作实际上调用的，正是 Kubernetes 的 pkg/volume 目录下的存储插件（Volume Plugin）。在这个例子里，就是 pkg/volume/flexvolume 这个目录里的代码。<br />当然了，这个目录其实只是 FlexVolume 插件的入口。以“Mount 阶段”为例，在 FlexVolume 目录里，它的处理过程非常简单，如下所示：
```go
// SetUpAt creates new directory.
func (f *flexVolumeMounter) SetUpAt(dir string, fsGroup *int64) error {
  ...
  call := f.plugin.NewDriverCall(mountCmd)

  // Interface parameters
  call.Append(dir)

  extraOptions := make(map[string]string)

  // pod metadata
  extraOptions[optionKeyPodName] = f.podName
  extraOptions[optionKeyPodNamespace] = f.podNamespace

  ...

  call.AppendSpec(f.spec, f.plugin.host, extraOptions)

  _, err = call.Run()

  ...

  return nil
}
```
上面这个名叫 `SetUpAt()` 的方法，正是 FlexVolume 插件对“Mount 阶段”的实现位置。而 `SetUpAt()` 实际上只做了一件事，那就是封装出了一行命令（即：NewDriverCall），由 kubelet 在“Mount 阶段”去执行。<br />在这个例子中，kubelet 要通过插件在宿主机上执行的命令，如下所示：
```
/usr/libexec/kubernetes/kubelet-plugins/volume/exec/k8s~nfs/nfs mount <mount dir> <json param>
```
其中，/usr/libexec/kubernetes/kubelet-plugins/volume/exec/k8s~nfs/nfs 就是插件的可执行文件的路径。这个名叫 nfs 的文件，正是要编写的插件的实现。它可以是一个二进制文件，也可以是一个脚本。总之，只要能在宿主机上被执行起来即可。<br />而且这个路径里的 k8s~nfs 部分，正是这个插件在 Kubernetes 里的名字。它是从 driver="k8s/nfs"字段解析出来的。<br />这个 driver 字段的格式是：vendor/driver。比如，一家存储插件的提供商（vendor）的名字叫作 k8s，提供的存储驱动（driver）是 nfs，那么 Kubernetes 就会使用 k8s~nfs 来作为插件名。<br />所以说，当编写完了 FlexVolume 的实现之后，一定要把它的可执行文件放在每个节点的插件目录下。<br />而紧跟在可执行文件后面的“mount”参数，定义的就是当前的操作。在 FlexVolume 里，这些操作参数的名字是固定的，比如 init、mount、unmount、attach，以及 dettach 等等，分别对应不同的 Volume 处理操作。<br />而跟在 mount 参数后面的两个字段：<mount dir>和<json params>，则是 FlexVolume 必须提供给这条命令的两个执行参数。<br />其中第一个执行参数，正是 kubelet 调用 `SetUpAt()` 方法传递来的 dir 的值。它代表的是当前正在处理的 Volume 在宿主机上的目录。在例子里，这个路径如下所示：
```
/var/lib/kubelet/pods/<Pod ID>/volumes/k8s~nfs/test
```
其中，test 正是前面定义的 PV 的名字；而 k8s~nfs，则是插件的名字。可以看到，插件的名字正是从声明的 driver="k8s/nfs"字段里解析出来的。<br />而第二个执行参数，则是一个 JSON Map 格式的参数列表。在前面 PV 里定义的 options 字段的值，都会被追加在这个参数里。此外，在 `SetUpAt()` 方法里可以看到，这个参数列表里还包括了 Pod 的名字、Namespace 等元数据（Metadata）。<br />在明白了存储插件的调用方式和参数列表之后，这个插件的可执行文件的实现部分就非常容易理解了。<br />在这个例子中，直接编写了一个简单的 shell 脚本来作为插件的实现，它对“Mount 阶段”的处理过程，如下所示：
```bash
domount() {
 MNTPATH=$1
 
 NFS_SERVER=$(echo $2 | jq -r '.server')
 SHARE=$(echo $2 | jq -r '.share')
 
 ...
 
 mkdir -p ${MNTPATH} &> /dev/null
 
 mount -t nfs ${NFS_SERVER}:/${SHARE} ${MNTPATH} &> /dev/null
 if [ $? -ne 0 ]; then
  err "{ \"status\": \"Failure\", \"message\": \"Failed to mount ${NFS_SERVER}:${SHARE} at ${MNTPATH}\"}"
  exit 1
 fi
 log '{"status": "Success"}'
 exit 0
}
```
可以看到，当 kubelet 在宿主机上执行“`nfs mount <mount dir> <json params>`”的时候，这个名叫 nfs 的脚本，就可以直接从`<mount dir>`参数里拿到 Volume 在宿主机上的目录，即：`MNTPATH=$1`。而在 PV 的 options 字段里定义的 NFS 的服务器地址（options.server）和共享目录名字（options.share），则可以从第二个`<json params>`参数里解析出来。这里，使用了 jq 命令，来进行解析工作。<br />有了这三个参数之后，这个脚本最关键的一步，当然就是执行：`mount -t nfs ${NFS_SERVER}:/${SHARE} ${MNTPATH}` 。这样，一个 NFS 的数据卷就被挂载到了 MNTPATH，也就是 Volume 所在的宿主机目录上，一个持久化的 Volume 目录就处理完了。<br />需要注意的是，当这个 `mount -t nfs` 操作完成后，必须把一个 JOSN 格式的字符串，比如：{“status”: “Success”}，返回给调用者，也就是 kubelet。这是 kubelet 判断这次调用是否成功的唯一依据。<br />综上所述，在“Mount 阶段”，kubelet 的 VolumeManagerReconcile 控制循环里的一次“调谐”操作的执行流程，如下所示：
```bash
kubelet --> pkg/volume/flexvolume.SetUpAt() --> /usr/libexec/kubernetes/kubelet-plugins/volume/exec/k8s~nfs/nfs mount <mount dir> <json param>
```
备注：这个 NFS 的 FlexVolume 的完整实现，在[这个 GitHub 库](https://github.com/kubernetes/examples/blob/master/staging/volumes/flexvolume/nfs)里。而如果想用 Go 语言编写 FlexVolume 的话，这里也有一个[很好的例子](https://github.com/kubernetes/frakti/tree/master/pkg/flexvolume)供参考。<br />像 NFS 这样的文件系统存储，并不需要在宿主机上挂载磁盘或者块设备。所以也就不需要实现 attach 和 dettach 操作了。<br />不过，像这样的 FlexVolume 实现方式，虽然简单，但局限性却很大。<br />比如，跟 Kubernetes 内置的 NFS 插件类似，这个 NFS FlexVolume 插件，也不能支持 Dynamic Provisioning（即：为每个 PVC 自动创建 PV 和对应的 Volume）。除非再为它编写一个专门的 External Provisioner。<br />再比如，插件在执行 `mount` 操作的时候，可能会生成一些挂载信息。这些信息，在后面执行 `unmount` 操作的时候会被用到。可是，在上述 FlexVolume 的实现里，没办法把这些信息保存在一个变量里，等到 `unmount` 的时候直接使用。<br />这个原因也很容易理解：FlexVolume 每一次对插件可执行文件的调用，都是一次完全独立的操作。所以，只能把这些信息写在一个宿主机上的临时文件里，等到 `unmount` 的时候再去读取。<br />这也是为什么，需要有 Container Storage Interface（CSI）这样更完善、更编程友好的插件方式。
<a name="KmpLm"></a>
## CSI
<a name="WzNnZ"></a>
### CSI 插件体系的设计原理
其实，通过前面对 FlexVolume 的讲述，应该可以明白，默认情况下，Kubernetes 里通过存储插件管理容器持久化存储的原理，可以用如下所示的示意图来描述：<br />![6a553321623f6b58f5494b25091592ef[1].png](https://cdn.nlark.com/yuque/0/2022/png/396745/1642495080350-6ba2e08a-cdaf-40aa-b626-e787d5e13ef9.png#clientId=u44e123b0-d9c0-4&from=ui&id=u7c243431&originHeight=946&originWidth=1212&originalType=binary&ratio=1&rotation=0&showTitle=false&size=81335&status=done&style=shadow&taskId=u17dab66e-b30b-49fa-b0bd-02322a8d58b&title=)<br />可以看到，在上述体系下，无论是 FlexVolume，还是 Kubernetes 内置的其他存储插件，它们实际上担任的角色，仅仅是 Volume 管理中的“Attach 阶段”和“Mount 阶段”的具体执行者。而像 Dynamic Provisioning 这样的功能，就不是存储插件的责任，而是 Kubernetes 本身存储管理功能的一部分。<br />相比之下，CSI 插件体系的设计思想，就是把这个 Provision 阶段，以及 Kubernetes 里的一部分存储管理功能，从主干代码里剥离出来，做成了几个单独的组件。这些组件会通过 Watch API 监听 Kubernetes 里与存储相关的事件变化，比如 PVC 的创建，来执行具体的存储管理动作。<br />而这些管理动作，比如“Attach 阶段”和“Mount 阶段”的具体操作，实际上就是通过调用 CSI 插件来完成的。<br />这种设计思路，可以用如下所示的一幅示意图来表示：<br />![d4bdc7035f1286e7a423da851eee89ad[1].png](https://cdn.nlark.com/yuque/0/2022/png/396745/1642495240663-0f800a3a-7739-4ab9-a403-a972485fee17.png#clientId=u44e123b0-d9c0-4&from=ui&id=uab42b649&originHeight=941&originWidth=1880&originalType=binary&ratio=1&rotation=0&showTitle=false&size=160006&status=done&style=shadow&taskId=uc2ce82d0-5f36-40e5-a5d1-4f0eb529e2c&title=)<br />可以看到，这套存储插件体系多了三个独立的外部组件（External Components），即：Driver Registrar、External Provisioner 和 External Attacher，对应的正是从 Kubernetes 项目里面剥离出来的那部分存储管理功能。<br />需要注意的是，External Components 虽然是外部组件，但依然由 Kubernetes 社区来开发和维护。<br />而图中最右侧的部分，就是需要编写代码来实现的 CSI 插件。一个 CSI 插件只有一个二进制文件，但它会以 gRPC 的方式对外提供三个服务（gRPC Service），分别叫作：CSI Identity、CSI Controller 和 CSI Node。
<a name="Cj0FH"></a>
### 三个 External Components
<a name="y9hIm"></a>
#### Driver Registrar 组件，负责将插件注册到 kubelet 里面
其中，Driver Registrar 组件，负责将插件注册到 kubelet 里面（这可以类比为，将可执行文件放在插件目录下）。而在具体实现上，Driver Registrar 需要请求 CSI 插件的 Identity 服务来获取插件信息。
<a name="JOXPa"></a>
#### External Provisioner 组件，负责的正是 Provision 阶段
而 External Provisioner 组件，负责的正是 Provision 阶段。在具体实现上，External Provisioner 监听（Watch）了 APIServer 里的 PVC 对象。当一个 PVC 被创建时，它就会调用 CSI Controller 的 CreateVolume 方法，创建对应 PV。<br />此外，如果使用的存储是公有云提供的磁盘（或者块设备）的话，这一步就需要调用公有云（或者块设备服务）的 API 来创建这个 PV 所描述的磁盘（或者块设备）了。<br />不过，由于 CSI 插件是独立于 Kubernetes 之外的，所以在 CSI 的 API 里不会直接使用 Kubernetes 定义的 PV 类型，而是会自己定义一个单独的 Volume 类型。<br />为了方便叙述，把 Kubernetes 里的持久化卷类型叫作 PV，把 CSI 里的持久化卷类型叫作 CSI Volume，务必区分清楚。
<a name="zdmyt"></a>
#### External Attacher 组件，负责的正是“Attach 阶段”
最后一个 External Attacher 组件，负责的正是“Attach 阶段”。在具体实现上，它监听了 APIServer 里 VolumeAttachment 对象的变化。VolumeAttachment 对象是 Kubernetes 确认一个 Volume 可以进入“Attach 阶段”的重要标志。<br />一旦出现了 VolumeAttachment 对象，External Attacher 就会调用 CSI Controller 服务的 ControllerPublish 方法，完成它所对应的 Volume 的 Attach 阶段。<br />而 Volume 的“Mount 阶段”，并不属于 External Components 的职责。当 kubelet 的 VolumeManagerReconciler 控制循环检查到它需要执行 Mount 操作的时候，会通过 pkg/volume/csi 包，直接调用 CSI Node 服务完成 Volume 的“Mount 阶段”。<br />在实际使用 CSI 插件的时候，会将这三个 External Components 作为 sidecar 容器和 CSI 插件放置在同一个 Pod 中。由于 External Components 对 CSI 插件的调用非常频繁，所以这种 sidecar 的部署方式非常高效。
<a name="XSRX0"></a>
### CSI 插件的里三个服务：CSI Identity、CSI Controller 和 CSI Node
<a name="Oj8eA"></a>
#### CSI 插件的 CSI Identity 服务，负责对外暴露这个插件本身的信息
其中，CSI 插件的 CSI Identity 服务，负责对外暴露这个插件本身的信息，如下所示：
```go
service Identity {
  // return the version and name of the plugin
  rpc GetPluginInfo(GetPluginInfoRequest)
    returns (GetPluginInfoResponse) {}
  // reports whether the plugin has the ability of serving the Controller interface
  rpc GetPluginCapabilities(GetPluginCapabilitiesRequest)
    returns (GetPluginCapabilitiesResponse) {}
  // called by the CO just to check whether the plugin is running or not
  rpc Probe (ProbeRequest)
    returns (ProbeResponse) {}
}
```
而 CSI Controller 服务，定义的则是对 CSI Volume（对应 Kubernetes 里的 PV）的管理接口，比如：创建和删除 CSI Volume、对 CSI Volume 进行 Attach/Dettach（在 CSI 里，这个操作被叫作 Publish/Unpublish），以及对 CSI Volume 进行 Snapshot 等，它们的接口定义如下所示：
```go
service Controller {
  // provisions a volume
  rpc CreateVolume (CreateVolumeRequest)
    returns (CreateVolumeResponse) {}
    
  // deletes a previously provisioned volume
  rpc DeleteVolume (DeleteVolumeRequest)
    returns (DeleteVolumeResponse) {}
    
  // make a volume available on some required node
  rpc ControllerPublishVolume (ControllerPublishVolumeRequest)
    returns (ControllerPublishVolumeResponse) {}
    
  // make a volume un-available on some required node
  rpc ControllerUnpublishVolume (ControllerUnpublishVolumeRequest)
    returns (ControllerUnpublishVolumeResponse) {}
    
  ...
  
  // make a snapshot
  rpc CreateSnapshot (CreateSnapshotRequest)
    returns (CreateSnapshotResponse) {}
    
  // Delete a given snapshot
  rpc DeleteSnapshot (DeleteSnapshotRequest)
    returns (DeleteSnapshotResponse) {}
    
  ...
}
```
不难发现，CSI Controller 服务里定义的这些操作有个共同特点，那就是它们都无需在宿主机上进行，而是属于 Kubernetes 里 Volume Controller 的逻辑，也就是属于 Master 节点的一部分。<br />需要注意的是，CSI Controller 服务的实际调用者，并不是 Kubernetes（即：通过 pkg/volume/csi 发起 CSI 请求），而是 External Provisioner 和 External Attacher。这两个 External Components，分别通过监听 PVC 和 VolumeAttachement 对象，来跟 Kubernetes 进行协作。<br />而 CSI Volume 需要在宿主机上执行的操作，都定义在了 CSI Node 服务里面，如下所示：
```go
service Node {
  // temporarily mount the volume to a staging path
  rpc NodeStageVolume (NodeStageVolumeRequest)
    returns (NodeStageVolumeResponse) {}
    
  // unmount the volume from staging path
  rpc NodeUnstageVolume (NodeUnstageVolumeRequest)
    returns (NodeUnstageVolumeResponse) {}
    
  // mount the volume from staging to target path
  rpc NodePublishVolume (NodePublishVolumeRequest)
    returns (NodePublishVolumeResponse) {}
    
  // unmount the volume from staging path
  rpc NodeUnpublishVolume (NodeUnpublishVolumeRequest)
    returns (NodeUnpublishVolumeResponse) {}
    
  // stats for the volume
  rpc NodeGetVolumeStats (NodeGetVolumeStatsRequest)
    returns (NodeGetVolumeStatsResponse) {}
    
  ...
  
  // Similar to NodeGetId
  rpc NodeGetInfo (NodeGetInfoRequest)
    returns (NodeGetInfoResponse) {}
}
```
需要注意的是，“Mount 阶段”在 CSI Node 里的接口，是由 NodeStageVolume 和 NodePublishVolume 两个接口共同实现的。
<a name="NZpdH"></a>
## 总结
相比于 FlexVolume，CSI 的设计思想，把插件的职责从“两阶段处理”，扩展成了 Provision、Attach 和 Mount 三个阶段。其中，Provision 等价于“创建磁盘”，Attach 等价于“挂载磁盘到虚拟机”，Mount 等价于“将该磁盘格式化后，挂载在 Volume 的宿主机目录上”。<br />在有了 CSI 插件之后，唯一区别在于：

- 当 AttachDetachController 需要进行“Attach”操作时（“Attach 阶段”），它实际上会执行到 pkg/volume/csi 目录中，创建一个 VolumeAttachment 对象，从而触发 External Attacher 调用 CSI Controller 服务的 ControllerPublishVolume 方法。
- 当 VolumeManagerReconciler 需要进行“Mount”操作时（“Mount 阶段”），它实际上也会执行到 pkg/volume/csi 目录中，直接向 CSI Node 服务发起调用 NodePublishVolume 方法的请求。

以上，就是 CSI 插件最基本的工作原理了。
<a name="FLVwJ"></a>
### CSI插件编写指南
为了能够覆盖到 CSI 插件的所有功能，这一次选择了 DigitalOcean 的块存储（Block Storage）服务，来作为实践对象。<br />DigitalOcean 是业界知名的“最简”公有云服务，即：它只提供虚拟机、存储、网络等为数不多的几个基础功能，其他功能一概不管。而这，恰恰就使得 DigitalOcean 成了在公有云上实践 Kubernetes 的最佳选择。<br />编写的 CSI 插件的功能，就是让运行在 DigitalOcean 上的 Kubernetes 集群能够使用它的块存储服务，作为容器的持久化存储。<br />有了 CSI 插件之后，持久化存储的用法就非常简单了，只需要创建一个如下所示的 StorageClass 对象即可：
```yaml
kind: StorageClass
apiVersion: storage.k8s.io/v1
metadata:
  name: do-block-storage
  namespace: kube-system
  annotations:
    storageclass.kubernetes.io/is-default-class: "true"
provisioner: com.digitalocean.csi.dobs
```
有了这个 StorageClass，External Provisoner 就会为集群中新出现的 PVC 自动创建出 PV，然后调用 CSI 插件创建出这个 PV 对应的 Volume，这正是 CSI 体系中 Dynamic Provisioning 的实现方式。
:::tips
备注：`storageclass.kubernetes.io/is-default-class: "true"`的意思，是使用这个 StorageClass 作为默认的持久化存储提供者。
:::
不难看到，这个 StorageClass 里唯一引人注意的，是 `provisioner=com.digitalocean.csi.dobs` 这个字段。显然，这个字段告诉了 Kubernetes，请使用名叫 `com.digitalocean.csi.dobs` 的 CSI 插件来处理这个 StorageClass 相关的所有操作。<br />那么，Kubernetes 又是如何知道一个 CSI 插件的名字的呢？<br />这就需要从 CSI 插件的第一个服务 CSI Identity 说起了。<br />其实，一个 CSI 插件的代码结构非常简单，如下所示：
```bash
tree $GOPATH/src/github.com/digitalocean/csi-digitalocean/driver  
$GOPATH/src/github.com/digitalocean/csi-digitalocean/driver 
├── controller.go
├── driver.go
├── identity.go
├── mounter.go
└── node.go
```
其中，CSI Identity 服务的实现，就定义在了 driver 目录下的 identity.go 文件里。<br />当然，为了能够让 Kubernetes 访问到 CSI Identity 服务，需要先在 driver.go 文件里，定义一个标准的 gRPC Server，如下所示：
```go
// Run starts the CSI plugin by communication over the given endpoint
func (d *Driver) Run() error {
 ...
 
 listener, err := net.Listen(u.Scheme, addr)
 ...
 
 d.srv = grpc.NewServer(grpc.UnaryInterceptor(errHandler))
 csi.RegisterIdentityServer(d.srv, d)
 csi.RegisterControllerServer(d.srv, d)
 csi.RegisterNodeServer(d.srv, d)
 
 d.ready = true // we're now ready to go!
 ...
 return d.srv.Serve(listener)
}
```
可以看到，只要把编写好的 gRPC Server 注册给 CSI，它就可以响应来自 External Components 的 CSI 请求了。<br />CSI Identity 服务中，最重要的接口是 GetPluginInfo，它返回的就是这个插件的名字和版本号，如下所示：
:::tips
备注：CSI 各个服务的接口可以在这里找到[它的 protoc 文件](https://github.com/container-storage-interface/spec/blob/master/csi.proto)。
:::
```go
func (d *Driver) GetPluginInfo(ctx context.Context, req *csi.GetPluginInfoRequest) (*csi.GetPluginInfoResponse, error) {
    resp := &csi.GetPluginInfoResponse{
        Name:          driverName,
        VendorVersion: version,
    }
    ...
}
```
其中，driverName 的值，正是"com.digitalocean.csi.dobs"。所以说，Kubernetes 正是通过 GetPluginInfo 的返回值，来找到在 StorageClass 里声明要使用的 CSI 插件的。
:::tips
备注：CSI 要求插件的名字遵守[“反向 DNS”格式](https://en.wikipedia.org/wiki/Reverse_domain_name_notation)。
:::
另外一个 GetPluginCapabilities 接口也很重要。这个接口返回的是这个 CSI 插件的“能力”。<br />比如，当编写的 CSI 插件不准备实现“Provision 阶段”和“Attach 阶段”（比如，一个最简单的 NFS 存储插件就不需要这两个阶段）时，就可以通过这个接口返回：本插件不提供 CSI Controller 服务，即：没有 `csi.PluginCapability_Service_CONTROLLER_SERVICE` 这个“能力”。这样，Kubernetes 就知道这个信息了。<br />最后，CSI Identity 服务还提供了一个 Probe 接口。Kubernetes 会调用它来检查这个 CSI 插件是否正常工作。<br />一般情况下，建议在编写插件时给它设置一个 Ready 标志，当插件的 gRPC Server 停止的时候，把这个 Ready 标志设置为 false。或者，可以在这里访问一下插件的端口，类似于健康检查的做法。<br />然后，要开始编写 CSI 插件的第二个服务，即 CSI Controller 服务了。它的代码实现，在 controller.go 文件里。这个服务主要实现的就是 Volume 管理流程中的“Provision 阶段”和“Attach 阶段”。<br />“Provision 阶段”对应的接口，是 CreateVolume 和 DeleteVolume，它们的调用者是 External Provisoner。以 CreateVolume 为例，它的主要逻辑如下所示：
```go
func (d *Driver) CreateVolume(ctx context.Context, req *csi.CreateVolumeRequest) (*csi.CreateVolumeResponse, error) {
    ...

    volumeReq := &godo.VolumeCreateRequest{
        Region:        d.region,
        Name:          volumeName,
        Description:   createdByDO,
        SizeGigaBytes: size / GB,
    }

    ...

    vol, _, err := d.doClient.Storage.CreateVolume(ctx, volumeReq)

    ...

    resp := &csi.CreateVolumeResponse{
        Volume: &csi.Volume{
            Id:            vol.ID,
            CapacityBytes: size,
            AccessibleTopology: []*csi.Topology{
                {
                    Segments: map[string]string{
                        "region": d.region,
                    },
                },
            },
        },
    }

    return resp, nil
}
```
可以看到，对于 DigitalOcean 这样的公有云来说，CreateVolume 需要做的操作，就是调用 DigitalOcean 块存储服务的 API，创建出一个存储卷（`d.doClient.Storage.CreateVolume`）。如果使用的是其他类型的块存储（比如 Cinder、Ceph RBD 等），对应的操作也是类似地调用创建存储卷的 API。<br />而“Attach 阶段”对应的接口是 ControllerPublishVolume 和 ControllerUnpublishVolume，它们的调用者是 External Attacher。以 ControllerPublishVolume 为例，它的逻辑如下所示：
```go
func (d *Driver) ControllerPublishVolume(ctx context.Context, req *csi.ControllerPublishVolumeRequest) (*csi.ControllerPublishVolumeResponse, error) {
    ...

    dropletID, err := strconv.Atoi(req.NodeId)

    // check if volume exist before trying to attach it
    _, resp, err := d.doClient.Storage.GetVolume(ctx, req.VolumeId)

    ...

    // check if droplet exist before trying to attach the volume to the droplet
    _, resp, err = d.doClient.Droplets.Get(ctx, dropletID)

    ...

    action, resp, err := d.doClient.StorageActions.Attach(ctx, req.VolumeId, dropletID)

    ...

    if action != nil {
        ll.Info("waiting until volume is attached")
        if err := d.waitAction(ctx, req.VolumeId, action.ID); err != nil {
            return nil, err
        }
    }

    ll.Info("volume is attached")
    return &csi.ControllerPublishVolumeResponse{}, nil
}
```
可以看到，对于 DigitalOcean 来说，ControllerPublishVolume 在“Attach 阶段”需要做的工作，是调用 DigitalOcean 的 API，将前面创建的存储卷，挂载到指定的虚拟机上（`d.doClient.StorageActions.Attach`）。<br />其中，存储卷由请求中的 VolumeId 来指定。而虚拟机，也就是将要运行 Pod 的宿主机，则由请求中的 NodeId 来指定。这些参数，都是 External Attacher 在发起请求时需要设置的。<br />External Attacher 的工作原理，是监听（Watch）了一种名叫 VolumeAttachment 的 API 对象。这种 API 对象的主要字段如下所示：
```go
// VolumeAttachmentSpec is the specification of a VolumeAttachment request.
type VolumeAttachmentSpec struct {
    // Attacher indicates the name of the volume driver that MUST handle this
    // request. This is the name returned by GetPluginName().
    Attacher string

    // Source represents the volume that should be attached.
    Source VolumeAttachmentSource

    // The node that the volume should be attached to.
    NodeName string
}
```
而这个对象的生命周期，正是由 AttachDetachController 负责管理的。<br />这个控制循环的职责，是不断检查 Pod 所对应的 PV，在它所绑定的宿主机上的挂载情况，从而决定是否需要对这个 PV 进行 Attach（或者 Dettach）操作。<br />而这个 Attach 操作，在 CSI 体系里，就是创建出上面这样一个 VolumeAttachment 对象。可以看到，Attach 操作所需的 PV 的名字（Source）、宿主机的名字（NodeName）、存储插件的名字（Attacher），都是这个 VolumeAttachment 对象的一部分。<br />而当 External Attacher 监听到这样的一个对象出现之后，就可以立即使用 VolumeAttachment 里的这些字段，封装成一个 gRPC 请求调用 CSI Controller 的 ControllerPublishVolume 方法。
<a name="a7T39"></a>
#### 最后，就可以编写 CSI Node 服务了。
CSI Node 服务对应的，是 Volume 管理流程里的“Mount 阶段”。它的代码实现，在 node.go 文件里。<br />kubelet 的 VolumeManagerReconciler 控制循环会直接调用 CSI Node 服务来完成 Volume 的“Mount 阶段”。<br />不过，在具体的实现中，这个“Mount 阶段”的处理其实被细分成了 NodeStageVolume 和 NodePublishVolume 这两个接口。<br />这里的原因其实也很容易理解：对于磁盘以及块设备来说，它们被 Attach 到宿主机上之后，就成为了宿主机上的一个待用存储设备。而到了“Mount 阶段”，首先需要格式化这个设备，然后才能把它挂载到 Volume 对应的宿主机目录上。<br />在 kubelet 的 VolumeManagerReconciler 控制循环中，这两步操作分别叫作 MountDevice 和 SetUp。<br />其中，MountDevice 操作，就是直接调用了 CSI Node 服务里的 NodeStageVolume 接口。顾名思义，这个接口的作用，就是格式化 Volume 在宿主机上对应的存储设备，然后挂载到一个临时目录（Staging 目录）上。<br />对于 DigitalOcean 来说，它对 NodeStageVolume 接口的实现如下所示：
```go
func (d *Driver) NodeStageVolume(ctx context.Context, req *csi.NodeStageVolumeRequest) (*csi.NodeStageVolumeResponse, error) {
    ...

    vol, resp, err := d.doClient.Storage.GetVolume(ctx, req.VolumeId)

    ...

    source := getDiskSource(vol.Name)
    target := req.StagingTargetPath

    ...

    if !formatted {
        ll.Info("formatting the volume for staging")
        if err := d.mounter.Format(source, fsType); err != nil {
            return nil, status.Error(codes.Internal, err.Error())
        }
    } else {
        ll.Info("source device is already formatted")
    }

    ...

    if !mounted {
        if err := d.mounter.Mount(source, target, fsType, options...); err != nil {
            return nil, status.Error(codes.Internal, err.Error())
        }
    } else {
        ll.Info("source device is already mounted to the target path")
    }

    ...
    return &csi.NodeStageVolumeResponse{}, nil
}
```
可以看到，在 NodeStageVolume 的实现里，首先通过 DigitalOcean 的 API 获取到了这个 Volume 对应的设备路径（getDiskSource）；然后，把这个设备格式化成指定的格式（ d.mounter.Format）；最后，把格式化后的设备挂载到了一个临时的 Staging 目录（StagingTargetPath）下。<br />而 SetUp 操作则会调用 CSI Node 服务的 NodePublishVolume 接口。有了上述对设备的预处理工作后，它的实现就非常简单了，如下所示：
```go
func (d *Driver) NodePublishVolume(ctx context.Context, req *csi.NodePublishVolumeRequest) (*csi.NodePublishVolumeResponse, error) {
    ...
    source := req.StagingTargetPath
    target := req.TargetPath

    mnt := req.VolumeCapability.GetMount()
    options := mnt.MountFlag
    ...

    if !mounted {
        ll.Info("mounting the volume")
        if err := d.mounter.Mount(source, target, fsType, options...); err != nil {
            return nil, status.Error(codes.Internal, err.Error())
        }
    } else {
        ll.Info("volume is already mounted")
    }

    return &csi.NodePublishVolumeResponse{}, nil
}
```
可以看到，在这一步实现中，只需要做一步操作，即：将 Staging 目录，绑定挂载到 Volume 对应的宿主机目录上。<br />由于 Staging 目录，正是 Volume 对应的设备被格式化后挂载在宿主机上的位置，所以当它和 Volume 的宿主机目录绑定挂载之后，这个 Volume 宿主机目录的“持久化”处理也就完成了。<br />对于文件系统类型的存储服务来说，比如 NFS 和 GlusterFS 等，它们并没有一个对应的磁盘“设备”存在于宿主机上，所以 kubelet 在 VolumeManagerReconciler 控制循环中，会跳过 MountDevice 操作而直接执行 SetUp 操作。所以对于它们来说，也就不需要实现 NodeStageVolume 接口了。<br />在编写完了 CSI 插件之后，就可以把这个插件和 External Components 一起部署起来。<br />首先，需要创建一个 DigitalOcean client 授权需要使用的 Secret 对象，如下所示：
```yaml
apiVersion: v1
kind: Secret
metadata:
  name: digitalocean
  namespace: kube-system
stringData:
  access-token: "a05dd2f26b9b9ac2asdas__REPLACE_ME____123cb5d1ec17513e06da"
```
接下来，通过一句指令就可以将 CSI 插件部署起来：
```bash
$ kubectl apply -f https://raw.githubusercontent.com/digitalocean/csi-digitalocean/master/deploy/kubernetes/releases/csi-digitalocean-v0.2.0.yaml
```
这个 CSI 插件的 YAML 文件的主要内容如下所示（其中，非重要的内容已经被略去）：
```yaml
kind: DaemonSet
apiVersion: apps/v1beta2
metadata:
  name: csi-do-node
  namespace: kube-system
spec:
  selector:
    matchLabels:
      app: csi-do-node
  template:
    metadata:
      labels:
        app: csi-do-node
        role: csi-do
    spec:
      serviceAccount: csi-do-node-sa
      hostNetwork: true
      containers:
        - name: driver-registrar
          image: quay.io/k8scsi/driver-registrar:v0.3.0
          ...
        - name: csi-do-plugin
          image: digitalocean/do-csi-plugin:v0.2.0
          args :
            - "--endpoint=$(CSI_ENDPOINT)"
            - "--token=$(DIGITALOCEAN_ACCESS_TOKEN)"
            - "--url=$(DIGITALOCEAN_API_URL)"
          env:
            - name: CSI_ENDPOINT
              value: unix:///csi/csi.sock
            - name: DIGITALOCEAN_API_URL
              value: https://api.digitalocean.com/
            - name: DIGITALOCEAN_ACCESS_TOKEN
              valueFrom:
                secretKeyRef:
                  name: digitalocean
                  key: access-token
          imagePullPolicy: "Always"
          securityContext:
            privileged: true
            capabilities:
              add: ["SYS_ADMIN"]
            allowPrivilegeEscalation: true
          volumeMounts:
            - name: plugin-dir
              mountPath: /csi
            - name: pods-mount-dir
              mountPath: /var/lib/kubelet
              mountPropagation: "Bidirectional"
            - name: device-dir
              mountPath: /dev
      volumes:
        - name: plugin-dir
          hostPath:
            path: /var/lib/kubelet/plugins/com.digitalocean.csi.dobs
            type: DirectoryOrCreate
        - name: pods-mount-dir
          hostPath:
            path: /var/lib/kubelet
            type: Directory
        - name: device-dir
          hostPath:
            path: /dev
---
kind: StatefulSet
apiVersion: apps/v1beta1
metadata:
  name: csi-do-controller
  namespace: kube-system
spec:
  serviceName: "csi-do"
  replicas: 1
  template:
    metadata:
      labels:
        app: csi-do-controller
        role: csi-do
    spec:
      serviceAccount: csi-do-controller-sa
      containers:
        - name: csi-provisioner
          image: quay.io/k8scsi/csi-provisioner:v0.3.0
          ...
        - name: csi-attacher
          image: quay.io/k8scsi/csi-attacher:v0.3.0
          ...
        - name: csi-do-plugin
          image: digitalocean/do-csi-plugin:v0.2.0
          args :
            - "--endpoint=$(CSI_ENDPOINT)"
            - "--token=$(DIGITALOCEAN_ACCESS_TOKEN)"
            - "--url=$(DIGITALOCEAN_API_URL)"
          env:
            - name: CSI_ENDPOINT
              value: unix:///var/lib/csi/sockets/pluginproxy/csi.sock
            - name: DIGITALOCEAN_API_URL
              value: https://api.digitalocean.com/
            - name: DIGITALOCEAN_ACCESS_TOKEN
              valueFrom:
                secretKeyRef:
                  name: digitalocean
                  key: access-token
          imagePullPolicy: "Always"
          volumeMounts:
            - name: socket-dir
              mountPath: /var/lib/csi/sockets/pluginproxy/
      volumes:
        - name: socket-dir
          emptyDir: {}
```
可以看到，编写的 CSI 插件只有一个二进制文件，它的镜像是 digitalocean/do-csi-plugin:v0.2.0。
<a name="oLRjy"></a>
#### 部署 CSI 插件的常用原则是：
<a name="S2eA0"></a>
##### 第一，通过 DaemonSet 在每个节点上都启动一个 CSI 插件，来为 kubelet 提供 CSI Node 服务。
这是因为，CSI Node 服务需要被 kubelet 直接调用，所以它要和 kubelet“一对一”地部署起来。<br />此外，在上述 DaemonSet 的定义里面，除了 CSI 插件，还以 sidecar 的方式运行着 driver-registrar 这个外部组件。它的作用，是向 kubelet 注册这个 CSI 插件。这个注册过程使用的插件信息，则通过访问同一个 Pod 里的 CSI 插件容器的 Identity 服务获取到。<br />需要注意的是，由于 CSI 插件运行在一个容器里，那么 CSI Node 服务在“Mount 阶段”执行的挂载操作，实际上是发生在这个容器的 Mount Namespace 里的。可是，真正希望执行挂载操作的对象，都是宿主机 /var/lib/kubelet 目录下的文件和目录。<br />所以，在定义 DaemonSet Pod 的时候，需要把宿主机的 /var/lib/kubelet 以 Volume 的方式挂载进 CSI 插件容器的同名目录下，然后设置这个 Volume 的 mountPropagation=Bidirectional，即开启双向挂载传播，从而将容器在这个目录下进行的挂载操作“传播”给宿主机，反之亦然。
<a name="BPn9m"></a>
##### 第二，通过 StatefulSet 在任意一个节点上再启动一个 CSI 插件，为 External Components 提供 CSI Controller 服务。
所以，作为 CSI Controller 服务的调用者，External Provisioner 和 External Attacher 这两个外部组件，就需要以 sidecar 的方式和这次部署的 CSI 插件定义在同一个 Pod 里。<br />为什么会用 StatefulSet 而不是 Deployment 来运行这个 CSI 插件呢。<br />这是因为，由于 StatefulSet 需要确保应用拓扑状态的稳定性，所以它对 Pod 的更新，是严格保证顺序的，即：只有在前一个 Pod 停止并删除之后，它才会创建并启动下一个 Pod。<br />而像上面这样将 StatefulSet 的 replicas 设置为 1 的话，StatefulSet 就会确保 Pod 被删除重建的时候，永远有且只有一个 CSI 插件的 Pod 运行在集群中。这对 CSI 插件的正确性来说，至关重要。<br />而在一开始，就已经定义了这个 CSI 插件对应的 StorageClass（即：do-block-storage），所以接下来只需要定义一个声明使用这个 StorageClass 的 PVC 即可，如下所示：
```yaml
apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: csi-pvc
spec:
  accessModes:
  - ReadWriteOnce
  resources:
    requests:
      storage: 5Gi
  storageClassName: do-block-storage
```
当把上述 PVC 提交给 Kubernetes 之后，就可以在 Pod 里声明使用这个 csi-pvc 来作为持久化存储了。这一部分使用 PV 和 PVC 的内容。
<a name="SGNHv"></a>
## 总结
这里以一个 DigitalOcean 的 CSI 插件为例，分享了编写 CSI 插件的具体流程。<br />举个例子，对于一个部署了 CSI 存储插件的 Kubernetes 集群来说：<br />当用户创建了一个 PVC 之后，前面部署的 StatefulSet 里的 External Provisioner 容器，就会监听到这个 PVC 的诞生，然后调用同一个 Pod 里的 CSI 插件的 CSI Controller 服务的 CreateVolume 方法，创建出对应的 PV。<br />这时候，运行在 Kubernetes Master 节点上的 Volume Controller，就会通过 PersistentVolumeController 控制循环，发现这对新创建出来的 PV 和 PVC，并且看到它们声明的是同一个 StorageClass。所以，它会把这一对 PV 和 PVC 绑定起来，使 PVC 进入 Bound 状态。<br />然后，用户创建了一个声明使用上述 PVC 的 Pod，并且这个 Pod 被调度器调度到了宿主机 A 上。这时候，Volume Controller 的 AttachDetachController 控制循环就会发现，上述 PVC 对应的 Volume，需要被 Attach 到宿主机 A 上。所以，AttachDetachController 会创建一个 VolumeAttachment 对象，这个对象携带了宿主机 A 和待处理的 Volume 的名字。<br />这样，StatefulSet 里的 External Attacher 容器，就会监听到这个 VolumeAttachment 对象的诞生。于是，它就会使用这个对象里的宿主机和 Volume 名字，调用同一个 Pod 里的 CSI 插件的 CSI Controller 服务的 ControllerPublishVolume 方法，完成“Attach 阶段”。<br />上述过程完成后，运行在宿主机 A 上的 kubelet，就会通过 VolumeManagerReconciler 控制循环，发现当前宿主机上有一个 Volume 对应的存储设备（比如磁盘）已经被 Attach 到了某个设备目录下。于是 kubelet 就会调用同一台宿主机上的 CSI 插件的 CSI Node 服务的 NodeStageVolume 和 NodePublishVolume 方法，完成这个 Volume 的“Mount 阶段”。<br />至此，一个完整的持久化 Volume 的创建和挂载流程就结束了。
