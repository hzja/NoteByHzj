Docker<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646493871-c65b0edf-5f24-4ac8-8336-88c4aff43459.webp#clientId=udda907a7-3546-4&from=paste&id=u27020ae7&originHeight=185&originWidth=547&originalType=url&ratio=1&status=done&style=shadow&taskId=uee2f5ed2-ec03-4f5c-94b7-0026f636bb0)
<a name="QMxOT"></a>
## Image Definition
镜像（Image）就是一堆只读层（read-only layer）的统一视角，也许这个定义有些难以理解，下面的这张图能够帮助理解镜像的定义。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646494157-47de7bb0-f256-4835-ae7e-7f6d7c1e362d.webp#clientId=udda907a7-3546-4&from=paste&id=u6ed736d5&originHeight=187&originWidth=624&originalType=url&ratio=1&status=done&style=shadow&taskId=uee87fa8c-314a-4da8-aa9a-d73df41f214)<br />从左边看到多个只读层，它们重叠在一起。除了最下面一层，其它层都会有一个指针指向下一层。这些层是Docker内部的实现细节，并且能够 在主机（译者注：运行Docker的机器）的文件系统上访问到。统一文件系统（union file system）技术能够将不同的层整合成一个文件系统，为这些层提供了一个统一的视角，这样就隐藏了多层的存在，在用户的角度看来，只存在一个文件系统。可以在图片的右边看到这个视角的形式。<br />可以在主机文件系统上找到有关这些层的文件。需要注意的是，在一个运行中的容器内部，这些层是不可见的。它们存在于/var/lib/docker/aufs目录下。
<a name="Zf3Bn"></a>
## `sudo tree -L 1 /var/lib/docker/`
```bash
/var/lib/docker/
├── aufs
├── containers
├── graph
├── init
├── linkgraph.db
├── repositories-aufs
├── tmp
├── trust
└── volumes
7 directories, 2 files
```
<a name="OSIA1"></a>
## Container Definition
容器（container）的定义和镜像（image）几乎一模一样，也是一堆层的统一视角，唯一区别在于容器的最上面那一层是可读可写的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646494142-f982ea74-086d-42c9-bb7e-fb6bad38ee67.webp#clientId=udda907a7-3546-4&from=paste&id=u207e1154&originHeight=174&originWidth=651&originalType=url&ratio=1&status=done&style=shadow&taskId=u70f9c538-db9f-423d-a46b-2d8a5455b67)<br />容器的定义并没有提及容器是否在运行。<br />要点：容器 = 镜像 + 可读层。并且容器的定义并没有提及是否要运行容器。<br />接下来，将会讨论运行态容器。
<a name="qZHgx"></a>
## Running Container Definition
一个运行态容器（running container）被定义为一个可读写的统一文件系统加上隔离的进程空间和包含其中的进程。下面这张图片展示了一个运行中的容器。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646494028-7755131d-9d93-48e3-ac5c-ad4491fb12a0.webp#clientId=udda907a7-3546-4&from=paste&id=u33656dd6&originHeight=198&originWidth=622&originalType=url&ratio=1&status=done&style=shadow&taskId=u24f0fff7-82a8-4681-b045-eca7d880492)<br />正是文件系统隔离技术使得Docker成为了一个前途无量的技术。一个容器中的进程可能会对文件进行修改、删除、创建，这些改变都将作用于可读写层（read-write layer）。下面这张图展示了这个行为。<br />![2021-09-03-13-21-56-637177.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630646868972-f07b7d3f-d976-4d81-b72f-2a222543cb33.png#clientId=udda907a7-3546-4&from=ui&id=u4956e20e&originHeight=198&originWidth=551&originalType=binary&ratio=1&size=22878&status=done&style=shadow&taskId=u9d034a01-11d1-42f9-96c5-bac9650163d)<br />可以通过运行以下命令来验证上面所说的：
```bash
docker run ubuntu touch happiness.txt
```
即便是这个ubuntu容器不再运行，依旧能够在主机的文件系统上找到这个新文件。
<a name="ibNCQ"></a>
## `find / -name happiness.txt`
```bash
/var/lib/docker/aufs/diff/860a7b...889/happiness.txt
```
<a name="awmbu"></a>
## Image Layer Definition
为了将零星的数据整合起来，提出了镜像层（image layer）这个概念。下面的这张图描述了一个镜像层，通过图片能够发现一个层并不仅仅包含文件系统的改变，它还能包含了其他重要信息。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646518912-d83a0dc5-7266-44b0-9f76-97beb8ebdd3f.webp#clientId=udda907a7-3546-4&from=paste&id=ue106f5d6&originHeight=79&originWidth=704&originalType=url&ratio=1&status=done&style=shadow&taskId=u7606e1fb-6a2c-4207-8cd7-c8909bdc2d3)<br />元数据（metadata）就是关于这个层的额外信息，它不仅能够让Docker获取运行和构建时的信息，还包括父层的层次信息。需要注意，只读层和读写层都包含元数据。<br />![2021-09-03-13-21-56-874190.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630646887059-cf5b25ef-ec1f-4d74-a420-36e067578f8c.png#clientId=udda907a7-3546-4&from=ui&id=u5b0dc2c2&originHeight=181&originWidth=287&originalType=binary&ratio=1&size=11956&status=done&style=shadow&taskId=ue85e6eb9-d5eb-4e5b-9bfb-5c717bf8805)<br />除此之外，每一层都包括了一个指向父层的指针。如果一个层没有这个指针，说明它处于最底层。<br />![2021-09-03-13-21-56-967176.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630646887093-d67951a0-a8cb-480e-9a5a-5c83ae086288.png#clientId=udda907a7-3546-4&from=ui&id=qrsJ7&originHeight=261&originWidth=359&originalType=binary&ratio=1&size=23362&status=done&style=shadow&taskId=ucc0840cf-00fb-4c8b-9bad-b6e1f7fb354)<br />Metadata Location: 在自己的主机上镜像层（image layer）的元数据被保存在名为”json”的文件中，比如说：
```bash
/var/lib/docker/graph/e809f156dc985.../json
```
e809f156dc985...就是这层的id<br />一个容器的元数据好像是被分成了很多文件，但或多或少能够在/var/lib/docker/containers/目录下找到，就是一个可读层的id。这个目录下的文件大多是运行时的数据，比如说网络，日志等等。
<a name="eJuuw"></a>
## 全局理解（Tying It All Together）
现在，结合上面提到的实现细节来理解Docker的命令。
<a name="Ht4KM"></a>
### `docker create`
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646519608-6a4cb68c-e735-45a9-9e1c-fb284b624285.webp#clientId=udda907a7-3546-4&from=paste&id=u99dfaf02&originHeight=154&originWidth=630&originalType=url&ratio=1&status=done&style=none&taskId=ud81c7842-9074-4c07-872e-5f485d49bd4)<br />`docker create` 命令为指定的镜像（image）添加了一个可读写层，构成了一个新的容器。注意，这个容器并没有运行。<br />![2021-09-03-13-21-57-159182.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630646930352-9f66c68e-75f9-4982-a1d7-149ef9959988.png#clientId=udda907a7-3546-4&from=ui&id=uef2aa144&originHeight=121&originWidth=603&originalType=binary&ratio=1&size=15254&status=done&style=shadow&taskId=u11e74d62-ad60-4ef6-a9f5-e1792c4dd11)
<a name="f14zM"></a>
### `docker start`
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646520168-90db7346-f0af-44cc-8de7-ddcc6e5afcc0.webp#clientId=udda907a7-3546-4&from=paste&id=ueb2d5776&originHeight=267&originWidth=632&originalType=url&ratio=1&status=done&style=none&taskId=uc1d72c36-1540-4965-99a9-67a9a40a699)<br />`Docker start`命令为容器文件系统创建了一个进程隔离空间。注意，每一个容器只能够有一个进程隔离空间。
<a name="uXo7o"></a>
### `docker run`
![2021-09-03-13-21-57-367177.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630646948010-4a6af0e9-b916-4242-8d47-d7953f2b0847.jpeg#clientId=udda907a7-3546-4&from=ui&id=ue450a928&originHeight=266&originWidth=628&originalType=binary&ratio=1&size=18611&status=done&style=none&taskId=u880c4bf2-f842-48ee-87b9-1a0e341bf9f)<br />看到这个命令，通常会有一个疑问：`docker start` 和 `docker run`命令有什么区别。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646520894-1b70159e-e2e0-4ae8-b84b-bec5c3c6c24d.webp#clientId=udda907a7-3546-4&from=paste&id=u7d0ba1dd&originHeight=282&originWidth=929&originalType=url&ratio=1&status=done&style=shadow&taskId=u7a1b983e-8af2-4191-aa9b-b4aa98b9335)<br />从图片可以看出，`docker run` 命令先是利用镜像创建了一个容器，然后运行这个容器。这个命令非常的方便，并且隐藏了两个命令的细节，但从另一方面来看，这容易让用户产生误解。<br />题外话：继续之前有关于Git的话题，`docker run`命令类似于`git pull`命令。`git pull`命令就是`git fetch` 和 `git merge`两个命令的组合，同样的，`docker run`就是`docker create`和`docker start`两个命令的组合。
<a name="ZlHkC"></a>
### `docker ps`
![2021-09-03-13-21-57-716986.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630646968906-7a663f9f-4ac8-4688-a04b-d8a0119b7558.jpeg#clientId=udda907a7-3546-4&from=ui&id=uf9704d93&originHeight=266&originWidth=630&originalType=binary&ratio=1&size=12125&status=done&style=none&taskId=u7549e97a-bf59-480b-8446-59a57b09ec0)<br />`docker ps` 命令会列出所有运行中的容器。这隐藏了非运行态容器的存在，如果想要找出这些容器，需要使用下面这个命令。
<a name="wWesz"></a>
### `docker ps –a`
![2021-09-03-13-21-57-805762.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630646968914-1445599b-2b44-4fed-ac30-6775fc4d1714.jpeg#clientId=udda907a7-3546-4&from=ui&id=oAvJT&originHeight=370&originWidth=632&originalType=binary&ratio=1&size=16802&status=done&style=none&taskId=u8e292dd8-a3b4-47a6-934d-b313c902548)<br />`docker ps –a`命令会列出所有的容器，不管是运行的，还是停止的。
<a name="WXsGr"></a>
### `docker images`
![2021-09-03-13-21-57-926474.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630646982044-3980338a-c93e-4f41-b443-cb71d7cc7dfe.jpeg#clientId=udda907a7-3546-4&from=ui&id=u9d41e619&originHeight=193&originWidth=629&originalType=binary&ratio=1&size=12122&status=done&style=none&taskId=uade5692b-1623-4984-8308-7e72281f5e7)<br />`docker images`命令会列出了所有顶层（top-level）镜像。实际上，在这里没有办法区分一个镜像和一个只读层，所以提出了top-level 镜像。只有创建容器时使用的镜像或者是直接pull下来的镜像能被称为顶层（top-level）镜像，并且每一个顶层镜像下面都隐藏了多个镜像层。
<a name="QgtW5"></a>
### `docker images –a`
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646522000-37bec856-cbd2-4b3c-8378-a9d12c700e5f.webp#clientId=udda907a7-3546-4&from=paste&id=uae67a542&originHeight=269&originWidth=628&originalType=url&ratio=1&status=done&style=none&taskId=u4ba44434-eadb-413b-872a-45be57a540f)<br />`docker images –a`命令列出了所有的镜像，也可以说是列出了所有的可读层。如果想要查看某一个`image-id`下的所有层，可以使用`docker history`来查看。
<a name="HesMJ"></a>
### `docker stop`
![2021-09-03-13-21-58-105509.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630647007496-004cfce7-186c-482a-8634-93372ab7cc18.jpeg#clientId=udda907a7-3546-4&from=ui&id=uc68c3e93&originHeight=335&originWidth=629&originalType=binary&ratio=1&size=20758&status=done&style=none&taskId=u0503c0f2-4ebe-42de-b701-12afaf57879)<br />`docker stop`命令会向运行中的容器发送一个SIGTERM的信号，然后停止所有的进程。
<a name="ZjFDz"></a>
### `docker kill`
![2021-09-03-13-21-58-188517.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630647007489-893b2497-9c33-484c-8742-c192e75807b1.jpeg#clientId=udda907a7-3546-4&from=ui&id=De5Ms&originHeight=333&originWidth=629&originalType=binary&ratio=1&size=20680&status=done&style=none&taskId=u0d150c7d-f1cb-4e9f-a5f8-dcc77fe825e)<br />`docker kill` 命令向所有运行在容器中的进程发送了一个不友好的SIGKILL信号。
<a name="vMJhj"></a>
### `docker pause`
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646523135-89454703-bdfb-48eb-8c1b-8bbc2e01e657.webp#clientId=udda907a7-3546-4&from=paste&id=u6ba0eb95&originHeight=333&originWidth=659&originalType=url&ratio=1&status=done&style=none&taskId=u65136cdd-599c-48ac-9b58-6850bc3c2ba)<br />`docker stop`和`docker kill`命令会发送UNIX的信号给运行中的进程，`docker pause`命令则不一样，它利用了cgroups的特性将运行中的进程空间暂停。具体的内部原理可以在这里找到：[https://www.kernel.org/doc/Doc](https://www.kernel.org/doc/Doc) ... m.txt，但是这种方式的不足之处在于发送一个SIGTSTP信号对于进程来说不够简单易懂，以至于不能够让所有进程暂停。
<a name="e6oTM"></a>
### `docker rm`
![2021-09-03-13-21-58-390049.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630647024191-15a29004-7110-40a2-8ff9-dfd7f3c6adb7.jpeg#clientId=udda907a7-3546-4&from=ui&id=u6b08134d&originHeight=164&originWidth=628&originalType=binary&ratio=1&size=14442&status=done&style=none&taskId=u528fe31e-74d6-4624-a129-ab54fbe6f41)<br />`docker rm`命令会移除构成容器的可读写层。注意，这个命令只能对非运行态容器执行。
<a name="MP6JM"></a>
### `docker rmi`
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646523537-166b4243-2210-4a84-85ed-d964e2ec63cb.webp#clientId=udda907a7-3546-4&from=paste&id=ub59adc00&originHeight=166&originWidth=629&originalType=url&ratio=1&status=done&style=none&taskId=ua4cc2744-7600-4039-bf7a-c8011773f31)<br />`docker rmi` 命令会移除构成镜像的一个只读层。只能够使用`docker rmi`来移除最顶层（top level layer）（也可以说是镜像），也可以使用`-f`参数来强制删除中间的只读层。
<a name="IcZWe"></a>
### `docker commit`
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646523938-293302f1-b4e8-4030-8de7-b0b825db920c.webp#clientId=udda907a7-3546-4&from=paste&id=ufb42827d&originHeight=140&originWidth=629&originalType=url&ratio=1&status=done&style=shadow&taskId=ub99a2901-7e11-4452-b607-4e99367f424)<br />`docker commit`命令将容器的可读写层转换为一个只读层，这样就把一个容器转换成了不可变的镜像。<br />![2021-09-03-13-21-58-717049.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630647038112-b0012337-800e-446a-998d-b59e2990f4ed.png#clientId=udda907a7-3546-4&from=ui&id=u14683d79&originHeight=142&originWidth=581&originalType=binary&ratio=1&size=15953&status=done&style=shadow&taskId=u30a4d199-5ef1-456e-9f68-4a0a50a6a99)
<a name="sjuiD"></a>
### `docker build`
![2021-09-03-13-21-58-821063.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630647038141-074bba53-5148-479c-bbc7-d9f005dea603.jpeg#clientId=udda907a7-3546-4&from=ui&id=pIYTK&originHeight=265&originWidth=629&originalType=binary&ratio=1&size=21011&status=done&style=none&taskId=ua6b0073e-5988-4e0e-8551-00b9c4ead3e)<br />`docker build`命令非常有趣，它会反复的执行多个命令。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646524579-216aa72b-3b12-4e6d-b6bf-aa9dd3c14ca7.webp#clientId=udda907a7-3546-4&from=paste&id=u647956a4&originHeight=301&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ufb34c8a5-e0a8-4984-944c-8dea58bfc5e)<br />从上图可以看到，`build`命令根据Dockerfile文件中的FROM指令获取到镜像，然后重复地1）`run`（`create`和`start`）、2）修改、3）`commit`。在循环中的每一步都会生成一个新的层，因此许多新的层会被创建。
<a name="VbKyQ"></a>
### `docker exec`
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646524858-18814411-5c1d-440b-82ec-09a3e679afe7.webp#clientId=udda907a7-3546-4&from=paste&id=u0276de3e&originHeight=140&originWidth=627&originalType=url&ratio=1&status=done&style=none&taskId=u5a610104-2e27-4ea2-bca0-e9c300327a9)<br />`docker exec` 命令会在运行中的容器执行一个新进程。
<a name="pcKjG"></a>
### `docker inspect or`
![2021-09-03-13-21-59-173529.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630647055653-b76c60ae-c14c-4040-9d17-48c63222359b.jpeg#clientId=udda907a7-3546-4&from=ui&id=ue7127695&originHeight=246&originWidth=629&originalType=binary&ratio=1&size=17945&status=done&style=none&taskId=ue47a328e-16c1-4646-b6aa-a3ca2d41b8c)<br />`docker inspect`命令会提取出容器或者镜像最顶层的元数据。
<a name="MZVmd"></a>
### `docker save`
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630646525456-d2e0ffae-35ca-4538-9834-3b4e12698c35.webp#clientId=udda907a7-3546-4&from=paste&id=u6fc6c3f5&originHeight=303&originWidth=627&originalType=url&ratio=1&status=done&style=none&taskId=ub82944b5-9d5f-48e3-a2f5-481cb280441)<br />`docker save`命令会创建一个镜像的压缩文件，这个文件能够在另外一个主机的Docker上使用。和`export`命令不同，这个命令为每一个层都保存了它们的元数据。这个命令只能对镜像生效。
<a name="dLAvW"></a>
### `docker export`
![2021-09-03-13-21-59-402535.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630647069716-24d92c57-728b-4fcb-b0ab-26e1fac6dd3d.jpeg#clientId=udda907a7-3546-4&from=ui&id=u1ed3a7b9&originHeight=302&originWidth=628&originalType=binary&ratio=1&size=23330&status=done&style=none&taskId=uaf25b4a7-1b19-4325-9867-e1e40b327b1)<br />`docker export`命令创建一个tar文件，并且移除了元数据和不必要的层，将多个层整合成了一个层，只保存了当前统一视角看到的内容（译者注：`expoxt`后 的容器再`import`到Docker中，通过`docker images –tree`命令只能看到一个镜像；而`save`后的镜像则不同，它能够看到这个镜像的历史镜像）。
<a name="OloEy"></a>
### `docker history`
![2021-09-03-13-21-59-537526.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630647069716-283d347b-7fb1-4c25-9e91-0fed3a2a3dbf.jpeg#clientId=udda907a7-3546-4&from=ui&id=t4r9d&originHeight=194&originWidth=630&originalType=binary&ratio=1&size=13782&status=done&style=none&taskId=u4d64e32a-899c-44e5-af7f-84b5aad10b9)<br />`docker history`命令递归地输出指定镜像的历史镜像。
