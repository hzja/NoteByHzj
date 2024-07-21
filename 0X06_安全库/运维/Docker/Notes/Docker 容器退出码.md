Docker
<a name="zifGh"></a>
## 为什么容器没有运行？
回答这个问题需要知道 Docker 容器为什么退出，退出码会提示容器停止运行的情况。本文列出了最常见的退出码，来回答两个重要问题：

- **这些退出码是什么意思？**
- **导致该退出码的动作是什么？**

exit code：代表一个进程的返回码，通过系统调用 exit_group 来触发。在 POSIX 中，0 代表正常的返回码，而 1-255 代表异常返回码，不过一般错误码都是 1。这里有一张附表 Appendix E. Exit Codes With Special Meanings
<a name="f4V2a"></a>
## 如何查看退出码
<a name="cefz7"></a>
### 方法一：查看 pod 中的容器退出码
```bash
$ kubectl describe pod xxx 
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1659364801085-13e12e64-5a27-4a65-8765-684953511f1b.jpeg#clientId=u1d69e7a5-8c58-4&from=paste&id=u87dd2b3d&originHeight=289&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uda457c94-303e-40b4-9958-6a65e98d26d&title=)
<a name="Wv7WC"></a>
### 方法二：用 Docker 查看
```bash
$ docker ps --filter "status=exited"
$ docker inspect <container-id> --format='{{.State.ExitCode}}'
```
<a name="BJeAm"></a>
### 方法三：手动输出
```bash
$ docker container run alpine sh -c "exit 1"

$ docker container ls -a

CONTAINER ID   IMAGE    COMMAND            CREATED              STATUS                       
61c688005b3a   alpine   "sh -c 'exit 1'"   About a minute ago   Exited (1) 3 seconds ago
```
<a name="kkpKB"></a>
## 常见退出码
<a name="cUPCN"></a>
### Exit Code 0

- **退出代码0表示特定容器没有附加前台进程。**
- **该退出代码是所有其他后续退出代码的例外。**
- **这不一定意味着发生了不好的事情。如果开发人员想要在容器完成其工作后自动停止其容器，则使用此退出代码。**

如果执行 `docker run hello-world`，会得到“Hello from docker!”，但查看容器的时候`docker ps -a | grep hello-world`，会发现状态码为 0<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1659364801008-80c9239d-54d7-4c49-9006-203405bd083f.jpeg#clientId=u1d69e7a5-8c58-4&from=paste&id=uc29675f6&originHeight=67&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u23c3888a-f58d-4a65-8299-3bb0cf1d733&title=)
<a name="w3FI5"></a>
### Exit Code 1

- **程序错误，或者 Dockerfile 中引用不存在的文件，如 entrypoint 中引用了错误的包**
- **程序错误可以很简单，例如 “除以0”，也可以很复杂，比如空引用或者其他程序 crash**
<a name="nV2fC"></a>
### Exit Code 137

- **表明容器收到了 SIGKILL 信号，进程被杀掉，对应 **`**kill -9**`
- **引发 SIGKILL 的是 Docker Kill。这可以由用户或由 Docker 守护程序来发起，手动执行：**`**docker kill**`
- **137 比较常见，如果 pod 中的 limit 资源设置较小，会运行内存不足导致 OOMKilled，此时 state 中的 "OOMKilled" 值为 true，可以在系统的 dmesg 中看到 oom 日志**
<a name="EXQdC"></a>
### Exit Code 139

- **表明容器收到了 SIGSEGV 信号，无效的内存引用，对应 **`**kill -11**`
- **一般是代码有问题，或者 docker 的基础镜像有问题**
<a name="j58QD"></a>
### Exit Code 143

- **表明容器收到了 SIGTERM 信号，终端关闭，对应 **`**kill -15**`
- **一般对应 **`**docker stop**`**  命令**
- **有时 **`**docker stop**`** 也会导致 Exit Code 137。发生在与代码无法处理 SIGTERM 的情况下，docker 进程等待十秒钟然后发出 SIGKILL 强制退出。**
<a name="FITat"></a>
### 不常用的一些 Exit Code

- **Exit Code 126：权限问题或命令不可执行**
- **Exit Code 127：Shell 脚本中可能出现错字且字符无法识别的情况**
- **Exit Code 1 或 255：因为很多程序员写异常退出时习惯用 **`**exit(1)**`** 或 **`**exit(-1)**`**，-1 会根据转换规则转成 255。这个一般是自定义 code，要看具体逻辑。**
<a name="RtvPu"></a>
### 退出状态码的区间

- **必须在 0-255 之间，0 表示正常退出**
- **外界将程序中断退出，状态码在 129-255**
- **程序自身异常退出，状态码一般在 1-128**
- **假如写代码指定的退出状态码时不在 0-255 之间，例如：**`**exit(-1)**`**，这时会自动做一个转换，最终呈现的状态码还是会在 0-255 之间。把状态码记为 code，当指定的退出时状态码为负数，那么转换公式如下：256 – (|code| % 256)**
<a name="wQXvt"></a>
## 参考

- [http://tldp.org/LDP/abs/html/exitcodes.html](http://tldp.org/LDP/abs/html/exitcodes.html)
- [https://imroc.io/posts/kubernetes/analysis-exitcode/](https://imroc.io/posts/kubernetes/analysis-exitcode/)
- [https://medium.com/better-programming/understanding-docker-container-exit-codes-5ee79a1d58f6](https://medium.com/better-programming/understanding-docker-container-exit-codes-5ee79a1d58f6)
