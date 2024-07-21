K8S<br />有时候调度一个应用程序进程、一些重复的操作（如发送邮件、告警、验证等）是极为必要的。在server上，通常使用一个cron，它极易设置和维护。<br />在使用Docker的时候，可以运行crontab来完成以上操作，但当使用Kubernetes应该使用什么组件来进行上述操作呢？<br />实际上，Kubernetes的运行方式有所不同，因为在负载均衡的情况下可能有一个或多个相同服务的实例，而不管启动多少个实例crontab仅运行一次。另一方面，需要crontab为一个或多个pod的每个进程都运行一次。在Kubernetes中有一个称为CronJob的特性解决了这一问题。<br />以下示例均基于kind。
<a name="GqP0j"></a>
## 创建CronJob
```yaml
apiVersion: batch/v1beta1
kind: CronJob
metadata:
  name: my-cron-job
spec:
  schedule: "*/1 * * * *"
  jobTemplate:
    spec:
      template:
        spec:
          containers:
          - name: my-cron-job
            image: curlimages/curl
            resources:
              limits:
                cpu: "1"
                memory: "300Mi"
              requests:
                cpu: "1"
                memory: "300Mi"
            args:
            - /bin/sh
            - -c
            - date; echo "Starting an example of CronJob"; resp=$(curl -I --http2 https://www.google.com) ; echo $resp; exit 0
          restartPolicy: Never
  successfulJobsHistoryLimit: 3
  failedJobsHistoryLimit: 3
```
CronJob已经创建，它每分钟运行一个curl镜像。<br />同时，需要设置资源限制（如CPU和内存），如果将AWS、Azure或GCP实例作为args，最好的可视化方式是在Google上进行简单的curl即可。<br />这一实例永远不会重启，而且成功和失败的历史job都有一个限制，在本例中这一次数设置为3。

- `spec.successfulJobsHistoryLimit`：要保留的成功完成的cronjob的数量
- `spec.failedJobsHistoryLimit`：要保留的失败的cronjob的数量

如果想了解更多关于CronJob API的信息，强烈建议阅读以下链接中的内容：<br />[https://docs.koki.io/short/resources/cron-job](https://docs.koki.io/short/resources/cron-job)<br />现在，运行以下命令以在Kubernetes中应用CronJob。
```bash
$ kubectl apply -f cronjob.yml
```
如果没有错误发生，使用以下命令看到最近配置的cronjob：
```bash
$ kubectl get cronjob
```
使用Lens来可视化所有可用的cronjob，它对Kubernetes中的跟踪和监控都非常有用。<br />![2021-05-03-13-48-25-753390.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620021039695-a3c57ba3-7602-4b6e-8b65-b100a175fe09.png#clientId=u38d24506-2632-4&from=ui&id=ucacab85b&originHeight=378&originWidth=1080&originalType=binary&size=36235&status=done&style=none&taskId=u55e170df-aef9-4ac8-9ec0-16cdb0f568b)<br />查看日志：<br />![2021-05-03-13-48-26-271006.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620021050547-05c30d11-f178-427b-8c58-62bff51a0418.png#clientId=u38d24506-2632-4&from=ui&id=u21358d97&originHeight=439&originWidth=1080&originalType=binary&size=99055&status=done&style=none&taskId=ud704ea2f-d72b-41d9-bf90-948daa2fa1d)<br />运行以下命令即可删除这一条目：
```bash
$ kubectl delete cronjob my-cron-job
```
在本例中运行了一个简单的Cron以及一个实例。<br />CronJob有一个局限性是需要通过在每个进程中添加一行来为同一进程调度多个CronJob。但是，Kubernetes 1.8 beta中不提供CronJob，必须使用并行机制（parallelism）复制相同的CronJob。对于另一个调度，需要创建另一个cron条目。期待着将来有机会为同一进程调度多个模式。
<a name="UeG2P"></a>
## 结 论
Kubernetes CronJob非常有用并且易于学习，可访问以下链接阅读和了解有关API参数的更多信息，并运行一些测试以更好地了解其工作原理：<br />[https://docs.koki.io/short/resources/cron-job/](https://docs.koki.io/short/resources/cron-job/)
