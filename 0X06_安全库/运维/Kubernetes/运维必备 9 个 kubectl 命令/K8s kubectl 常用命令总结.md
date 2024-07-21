Kubernetes kubectl
<a name="M2Gfl"></a>
## 1、kubectl用法详解
<a name="jHZGW"></a>
### 1、kubectl语法
```bash
kubectl [command] [Type] [NAME] [flags]
```

- command: 子命令，用于操作kubernetes集群资源对象的命令，例如：create, delete, describe, get, apply等等
- TYPE: 资源对象的类型，区分大小写，能以单数，复数或者简写形式表示。例如以下3中TYPE是等价的。

kubectl get pod pod1kubectl get pods pod1kubectl get po pod1

- NAME：资源对象的名称，区分大小写。如果不指定名称，系统则将返回属于TYPE的全部对象的列表，例如：kubectl get pods 将返回所有pod的列表
- flags: kubectl 子命令的可选参数，例如使用 -s 指定api server的url地址而不用默认值。

kubectl可操作的资源对象类型以及缩写：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635005855878-b6cc888d-8ff7-4aeb-9e51-e02fb434eeec.webp#clientId=u098d7bce-c336-4&from=paste&id=u70d4afd9&originHeight=1797&originWidth=480&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc9191c63-906e-42ac-90fb-4c31aa80f3e&title=)<br />在一个命令行中也可以同时对多个资源对象进行操作，以多个TYPE和NAME的组合表示，示例如下：<br />获取多个pod的信息：<br />获取多种对象的信息：
```bash
kubectl get pods pod1 pod2
```
同时应用多个YAML文件，以多个-f file参数表示：
```bash
kubectl get pod/pod1  rc/rc1
kubectl get pod -f pod1.yaml -f pod2.yamlkubectl create -f pod1.yaml -f rc1.yaml -f service1.yaml
```
<a name="Lg031"></a>
## 2、kubectl 子命令详解
kebectl的子命令非常丰富，涵盖了对kubernetes集群的主要操作，包括资源对象的创建、删除、查看、修改、配置、运行等，详细的子命令如表2.10所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635005855762-2f07c16f-5b41-486d-aaec-61fb72c3fa62.webp#clientId=u098d7bce-c336-4&from=paste&id=u598b9b2e&originHeight=3739&originWidth=480&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6b3e17aa-b5fd-4b7e-b004-97240d02106&title=)
<a name="fdGZO"></a>
## 3、kubectl参数列表
Kubectl命令行的公共启动参数如下所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635005855917-dc462fb4-138c-4fbf-a7f1-8124fcdde402.webp#clientId=u098d7bce-c336-4&from=paste&id=ufeec9feb&originHeight=877&originWidth=900&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue6982f40-f3fd-4cef-a6cf-f6b4f514a1c&title=)
<a name="bJiVE"></a>
## 4、Kubectl 输出格式
kubectl命令可以用多种格式对结果进行显示，输出的格式通过-o参数指定：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635005855893-3564a765-f132-4387-85f2-9199f2f74859.webp#clientId=u098d7bce-c336-4&from=paste&id=u235d984d&originHeight=311&originWidth=900&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7445f21f-1edf-42de-8352-35420bb8118&title=)
<a name="dIyB7"></a>
## 5、kubectl操作示例
<a name="K26GQ"></a>
### 1、根据yaml配置文件一次性创建service和rc
```bash
kubectl create -f my-service.yaml -f my-rc.yaml
```
<a name="gRJxx"></a>
### 2、根据目录下所有.yaml、.yml、.json文件的定义进行创建操作
```bash
kubectl create -f <directory>
```
<a name="bKqET"></a>
### 3、查看所有Pod列表
```bash
kubectl get pods
```
<a name="oOJXZ"></a>
### 4、查看rc和service列表
```bash
kubectl get rc,service
```
<a name="LKaQe"></a>
### 5、显示Node的详细信息
```bash
kubectl describe nodes <node-name>
```
<a name="vdxsI"></a>
### 6、显示Pod的详细信息
```bash
kubectl describe pods/<pod-name>
```
<a name="SoS0F"></a>
### 7、显示由RC管理的Pod信息
```bash
kubectl describe pods <rc-name>
```
<a name="pIlHD"></a>
### 8、删除基于pod.yaml文件定义的Pod
```bash
kubectl delete -f pod.yaml
```
<a name="erG9u"></a>
### 9、删除所有包含某个label的Pod和Service
```bash
kubectl delete pods,services -l name=<label-name>
```
<a name="eG2gn"></a>
### 10、删除所有Pod
```bash
kubectl delete pods --all
```
<a name="yHYWN"></a>
### 11、在Pod的容器里执行date命令，默认使用Pod中的第1个容器执行
```bash
kubectl exec <pod-name> date
```
<a name="CwdOG"></a>
### 12、指定Pod中某个容器执行date命令
```bash
kubectl exec <pod-name> -c <container-name> date
```
<a name="vn1Id"></a>
### 13、以bash方式登陆到Pod中的某个容器里
```bash
kubectl exec -it <pod-name> -c <container-name> /bin/bash
```
<a name="dwHHS"></a>
### 14、查看容器输出到stdout的日志
```bash
kubectl logs <pod-name>
```
<a name="bQr8d"></a>
### 15、跟踪查看容器的日志，相当于`tail -f`命令的结果
```bash
kubectl logs -f <pod-name> -c <container-name>
```
