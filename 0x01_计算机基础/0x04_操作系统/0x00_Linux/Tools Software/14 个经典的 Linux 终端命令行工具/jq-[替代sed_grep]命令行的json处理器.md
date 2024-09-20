Linux jq 
<a name="rpamY"></a>
## jq的仓库地址
[jq的仓库地址](https://stedolan.github.io/jq/)<br />[https://stedolan.github.io/jq/](https://stedolan.github.io/jq/)
<a name="YDYZM"></a>
## jq简介
jq 是 stedolan 开发的一个轻量级的和灵活的命令行JSON处理器，源码请参考 jq 项目主页。<br />jq 是一个命令行 JSON 处理器，类似于 sed 或 grep，但专门设计用于处理 JSON 数据。如果是在日常任务中会用到 JSON 的开发人员或系统管理员，那么这是工具箱中必不可少的工具。<br />jq 用于处理JSON输入，将给定过滤器应用于其JSON文本输入并在标准输出上将过滤器的结果生成为JSON。<br />最简单的过滤器是.，它将jq的输入未经修改地复制到其输出中（格式设置除外)。<br />与 grep 和 sed 等通用文本处理工具相比，jq 的主要优点是它理解 JSON 的数据结构，允许使用单个表达式创建复杂的查询。
<a name="tBSAW"></a>
## jq语法
```
jq [options] <jq filter> [file...]
jq [options] --args <jq filter> [strings...]
jq [options] --jsonargs <jq filter> [JSON_TEXTS...]
```
<a name="eqnN1"></a>
## jq参数

- -c 紧凑而不是漂亮的输出;
- -n 使用null作为单个输入值;
- -e 根据输出设置退出状态代码;
- -s 将所有输入读取（吸取）到数组中；应用过滤器;
- -r 输出原始字符串，而不是JSON文本;
- -R 读取原始字符串，而不是JSON文本;
- -C 为JSON着色;
- -M 单色（不要为JSON着色）;
- -S 在输出上排序对象的键;
- –tab 使用制表符进行缩进;
- –arg a v 将变量$a设置为value<v>;
- –argjson a v 将变量$a设置为JSON value<v>;
- –slurpfile a f 将变量$a设置为从<f>读取的JSON文本数组;
- –rawfile a f 将变量$a设置为包含<f>内容的字符串;
- –args 其余参数是字符串参数，而不是文件;
- –jsonargs 其余的参数是JSON参数，而不是文件;
- – 终止参数处理;
<a name="BjOZg"></a>
## jq的使用
举例来说，假设试图在这个 JSON 文件中查找容器的名称：
```json
{
  "apiVersion": "v1",
  "kind": "Pod",
  "metadata": {
    "labels": {
      "app": "myapp"
    },
    "name": "myapp",
    "namespace": "project1"  
  },
  "spec": {
    "containers": [
      {
        "command": [
          "sleep",
          "3000"
        ],
        "image": "busybox",
        "imagePullPolicy": "IfNotPresent",
        "name": "busybox"
      },
      {        
        "name": "nginx",
        "image": "nginx",
        "resources": {},
        "imagePullPolicy": "IfNotPresent"
      }
    ],
    "restartPolicy": "Never"  
  }
}
```
如果直接使用 grep 查找 name，则写法如下：
```bash
$ grep name k8s-pod.json
    "name": "myapp",
    "namespace": "project1"
        "name": "busybox"
        "name": "nginx",
```
grep 返回包含单词 name 的所有行。可以向 grep 添加更多选项来限制它，借助一些正则表达式操作来找到容器的名称。如果是使用 jq 获得想要的结果，则可以使用一个表达式来模拟向下导航数据结构，如下所示：
```bash
$ jq '.spec.containers[].name' k8s-pod.json
"busybox"
"nginx"
```
这个命令提供这两个容器的名称。如果只是在查找第二个容器的名称，则可以在表达式中添加数组元素索引：
```bash
$ jq '.spec.containers[1].name' k8s-pod.json
"nginx"
```
因为 jq 知道数据结构，所以即使文件格式稍有变化，它也能提供相同的结果。而只要格式稍有变化，grep 和 sed 就可能提供不同的结果。
<a name="Nvl9o"></a>
## jq实例
有一json文件(test.json)内容如下，使用jq做相关数据提出
```json
{
  "apiVersion": "v1",
  "kind": "Service",
  "metadata": {
    "annotations": {
      "meta.helm.sh/release-name": "prometheus",
      "meta.helm.sh/release-namespace": "kube-system"
    },
    "creationTimestamp": "2022-05-05T09:19:16Z",
    "labels": {
      "app": "prometheus",
      "app.kubernetes.io/managed-by": "Helm",
      "chart": "prometheus-11.12.1",
      "component": "server",
      "heritage": "Helm",
      "release": "prometheus"
    },
    "managedFields": [
      {
        "apiVersion": "v1",
        "fieldsType": "FieldsV1",
        "fieldsV1": {
          "f:metadata": {
            "f:annotations": {
              ".": {},
              "f:meta.helm.sh/release-name": {},
              "f:meta.helm.sh/release-namespace": {}
            },
            "f:labels": {
              ".": {},
              "f:app": {},
              "f:app.kubernetes.io/managed-by": {},
              "f:chart": {},
              "f:component": {},
              "f:heritage": {},
              "f:release": {}
            }
          },
          "f:spec": {
            "f:externalTrafficPolicy": {},
            "f:ports": {
              ".": {},
              "k:{\"port\":80,\"protocol\":\"TCP\"}": {
                ".": {},
                "f:name": {},
                "f:nodePort": {},
                "f:port": {},
                "f:protocol": {},
                "f:targetPort": {}
              }
            },
            "f:selector": {
              ".": {},
              "f:app": {},
              "f:component": {},
              "f:release": {}
            },
            "f:sessionAffinity": {},
            "f:type": {}
          }
        },
        "manager": "Go-http-client",
        "operation": "Update",
        "time": "2022-05-05T09:19:16Z"
      }
    ],
    "name": "prometheus-server",
    "namespace": "kube-system",
    "resourceVersion": "54247",
    "selfLink": "/api/v1/namespaces/kube-system/services/prometheus-server",
    "uid": "8eb54da5-f64e-4ea4-a5d1-2493bc1673cd"
  },
  "spec": {
    "clusterIP": "10.96.3.118",
    "externalTrafficPolicy": "Cluster",
    "ports": [
      {
        "name": "http",
        "nodePort": 31090,
        "port": 80,
        "protocol": "TCP",
        "targetPort": 9090
      }
        ],
        "selector": {
            "app": "prometheus",
            "component": "server",
            "release": "prometheus"
        },
        "sessionAffinity": "None",
        "type": "NodePort"
    },
    "status": {
        "loadBalancer": {}
    }
}
```
<a name="pSIfI"></a>
### 1、`.key`：获取一个键的值
```bash
cat test.json | jq .metadata.lebels
{
  "app": "prometheus",
  "app.kubernetes.io/managed-by": "Helm",
  "chart": "prometheus-11.12.1",
  "component": "server",
  "heritage": "Helm",
  "release": "prometheus"
}
```
<a name="Zm5Cp"></a>
### 2、`.[],[]`：获取一个数组
```bash
cat test.json | jq .metadata.managedFields[0].apiVersion
"v1"
```
<a name="LBHkw"></a>
### 3、keys：获取数组中的键
```bash
cat test.json | jq '.metadata.managedFields[0]|keys'
[
  "apiVersion",
  "fieldsType",
  "fieldsV1",
  "manager",
  "operation",
  "time"
]
```
<a name="fMwn8"></a>
### 4、`,`：使用过滤器
```bash
cat test.json | jq '.metadata.managedFields[0]|.manager,.time'
"Go-http-client"
"2022-05-05T09:19:16Z"
```
<a name="oYa6h"></a>
### 5、has：判断是否存在某个key
```bash
cat test.json | jq '.metadata.managedFields[0]|has("name")'
false
```
<a name="Rq716"></a>
### 6、`{key}`：构造对象
```bash
cat test.json | jq '.metadata.managedFields[0].manager |{"name": .}'
{
  "name": "Go-http-client"
}
```
<a name="ioD5U"></a>
### 7、length：查看元素个数
```bash
cat test.json | jq '.metadata.managedFields[0]|.manager,.time'
6
```
<a name="ykhX2"></a>
### 8、`map()`：改变数组的值
```bash
cat test.json | jq '.metadata.managedFields[0]|.manager,.time|map_values(.+"ss")'
```
<a name="Eik3m"></a>
### 9、-r：去除引号
```bash
cat test.json | jq -r '.metadata.managedFields[0]|.manager,.time'
Go-http-client
2022-05-05T09:19:16Z
```
