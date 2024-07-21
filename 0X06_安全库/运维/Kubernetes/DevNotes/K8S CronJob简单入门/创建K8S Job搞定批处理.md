K8S<br />Kubernetes jobs主要是针对短时和批量的工作负载。它是为了结束而运行的，而不是像deployment、replicasets、replication controllers和DaemonSets等其他对象那样持续运行。<br />![2021-05-03-11-41-06-548980.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620013624725-f43c65ef-e524-4696-a925-85211830928a.png#clientId=u52025e55-fee6-4&from=ui&id=u1d122722&originHeight=608&originWidth=1080&originalType=binary&ratio=1&size=66808&status=done&style=none&taskId=u0068ebb0-c1c8-4c9c-accc-896b38e6818)<br />这里介绍如何创建Kubernetes jobs和cronjobs，以及一些小技巧。<br />Kubernetes Jobs会一直运行到Job中指定的任务完成。也就是说，如果pods给出退出代码0，那么Job就会退出。而在正常的Kubernetes中，无论退出代码是什么，deployment对象在终止或出现错误时都会创建新的pod，以保持deployment的理想状态。<br />在job运行过程中，如果托管pod的节点发生故障，Job pod将被自动重新安排到另一个节点。
<a name="YGc3K"></a>
## ‍‍‍‍‍‍‍Kubernetes Jobs用例
对于Kubernetes Jobs最好的用例实践是：

1. **批处理任务**：比如想每天运行一次批处理任务，或者在指定日程中运行。它可能是像从存储库或数据库中读取文件那样，将它们分配给一个服务来处理文件。
2. **运维/ad-hoc任务**：比如想要运行一个脚本/代码，该脚本/代码会运行一个数据库清理活动，甚至备份一个Kubernetes集群。
<a name="oxVy9"></a>
## 如何创建Kubernetes Job
在本例中，将使用Ubuntu 容器来运行一个带有for循环的shell脚本，并根据传递给容器的参数来呼应消息。这个参数是一个数字，决定shell脚本循环应该运行多少次。<br />例如，如果传递了参数100，那么shell脚本将呼应消息100次然后容器将会退出。<br />可以访问以下链接查看Dockerfile和shell脚本：<br />[https://github.com/devopscube/Kubernetes-jobs-example/tree/master/Docker](https://github.com/devopscube/Kubernetes-jobs-example/tree/master/Docker)<br />先从一个简单设置的job开始。
<a name="a8qQj"></a>
### Step1：使用自定义的Docker镜像创建一个job.yaml文件，命令参数为100
100将会作为参数传递给docker ENTRYPOINT脚本。
```yaml
apiVersion: batch/v1 
kind: Job 
metadata:   
  name: kubernetes-job-example   
  labels:     
    jobgroup: jobexample 
spec:   
  template:     
    metadata:       
      name: kubejob       
      labels:         
        jobgroup: jobexample     
    spec:       
      containers:       
      - name: c         
        image: devopscube/kubernetes-job-demo:latest         
        args: ["100"]       
      restartPolicy: OnFailure
```
<a name="LSQzg"></a>
### Step2 ：使用kubectl创建一个job.yaml文件的job
```bash
kubectl apply -f job.yam
```
<a name="cSFZf"></a>
### Step3：使用kubectl检查job的状态
```bash
kubectl get jobs
```
<a name="kdUHg"></a>
### Step4：使用kubectl获取pod列表
```bash
kubectl get po
```
<a name="ZAA6G"></a>
### Step5：使用kubectl获取job pod 日志
使用在输出中看到的Pod名称替换原本的Pod名称。
```bash
kubectl logs kubernetes-job-example-bc7s9 -f
```
应该看到如下输出：<br />![2021-05-03-11-41-06-860309.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1620013438171-88bae39b-83df-427d-a57a-5c9f561fa93e.gif#clientId=u52025e55-fee6-4&from=ui&id=u9e3419ae&originHeight=723&originWidth=1080&originalType=binary&ratio=1&size=412958&status=done&style=none&taskId=ua17d553a-777e-46b1-a721-adc4a8c7bd8)
<a name="pEEMm"></a>
## 并行运行多Job pods
当一个job被部署后，可以让它在多个Pod上并行运行。例如，在一个job中如果想要运行6个 pods，同时并行运行2个pods，需要添加以下2个参数到job manifets中：
```bash
completions: 6
parallelism: 2
```
以下是带有那些参数的manifest：
```yaml
apiVersion: batch/v1
kind: Job
metadata:
  name: kubernetes-parallel-job
  labels:
    jobgroup: jobexample
spec:
  completions: 5
  parallelism: 2
  template:
    metadata:
      name: kubernetes-parallel-job
      labels:
        jobgroup: jobexample
    spec:
      containers:
      - name: c
        image: devopscube/kubernetes-job-demo:latest
        args: ["100"]
      restartPolicy: OnFailure
```
<a name="gz86j"></a>
## 为Kubernetes Job生成随机名称
不能从一个job manifest文件中创建多个job，因为Kubernetes会报错，说存在一个同名的job。为了规避这个问题，可以在元数据中添加 generateName 名称参数。<br />例如：
```yaml
apiVersion: batch/v1
kind: Job
metadata:
  generateName: kube-job-
  labels:
    jobgroup: jobexample
```
在上方示例中，每次运行该manifest，job将以kube-job-作为前缀，后面跟着一个随机字符串来创建。
<a name="MJyv6"></a>
## 如何创建Kubernetes CronJob
如果想按照特定的时间表运行批处理job，例如，每2个小时运行一次。可以用cron表达式创建一个Kubernetes cronjob。Job会按照在job中提到的时间表自动启动。<br />下面将介绍如何指定一个cron计划，可以使用crontab生成器（https://crontab-generator.org/）来生成自己的时间计划。
```yaml
schedule: "0,15,30,45 * * * *"
```
下图显示了Kubernetes cronjob schedule语法。
```java
# ┌───────────── 分钟 (0 - 59)
# │ ┌───────────── 小时 (0 - 23)
# │ │ ┌───────────── 月的某天 (1 - 31)
# │ │ │ ┌───────────── 月份 (1 - 12)
# │ │ │ │ ┌───────────── 周的某天 (0 - 6) （周日到周一；在某些系统上，7 也是星期日）
# │ │ │ │ │                                   
# │ │ │ │ │
# │ │ │ │ │
# * * * * *
```
| 输入 | 描述 | 相当于 |
| --- | --- | --- |
| `@yearly` (or `@annually`) | 每年 1 月 1 日的午夜运行一次 | 0 0 1 1 * |
| `@monthly` | 每月第一天的午夜运行一次 | 0 0 1 * * |
| `@weekly` | 每周的周日午夜运行一次 | 0 0 * * 0 |
| `@daily` (or `@midnight`) | 每天午夜运行一次 | 0 0 * * * |
| `@hourly` | 每小时的开始一次 | 0 * * * * |

来源：[kubernetes.io](https://kubernetes.io)<br />如果以cronjob的形式每15分钟运行一次之前的job，manifest应该如下所示。创建一个名为cron-job.yaml的文件，并复制以下manifest：
```yaml
apiVersion: batch/v1beta1
kind: CronJob
metadata:
    name: kubernetes-cron-job
spec:
  schedule: "0,15,30,45 * * * *"
  jobTemplate:
    spec:
      template:
        metadata:
          labels:
            app: cron-batch-job
        spec:
          restartPolicy: OnFailure
          containers:
          - name: kube-cron-job
            image: devopscube/kubernetes-job-demo:latest
            args: ["100"]
```
使用kubectl部署cronjob。
```bash
kubectl create -f cron-job.yaml
```
列出cronjobs：
```bash
kubectl get cronjobs
```
可以列出cronjob pod并从处于运行状态或完成状态的pods中获取日志来检查Cronjob日志。
<a name="nAdAS"></a>
## 手动运行Kubernetes CronJob
在某些情况下，可能希望以临时的方式执行cronjob。可以通过从现有的cronjob创建一个job来实现。<br />例如，如果想手动触发一个cronjob，应该这样做：
```bash
kubectl create job --from=cronjob/kubernetes-cron-job manual-cron-job
```
`--from=cronjob/kubernetes-cron-job`将复制cronjob模板并创建一个名为manual-cron-job的job。
<a name="jkXfP"></a>
## Kubernetes Job的关键参数
根据需求，还可以使用kubernetes jobs/cronjobs的几个关键参数：

1. `**failedJobHistoryLimit**`** & **`**successfulJobsHistoryLimit**`：根据提供的保留数量删除失败和成功的job历史记录。当尝试列出job时，这对于减少所有失败的条目非常有用。例如：
2. `**backoffLimit**`：如果Pod失败，重试的总次数。
3. `**activeDeadlineSeconds**`：如果想对cronjob的运行时间进行硬性限制，可以使用此参数。例如，如果想只运行1分钟的cronjob，可以将其设置为60。
