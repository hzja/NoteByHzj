# Clion

### 报错内容
```bash
CreateProcess error=2, 系统找不到指定的文件。
Check settings

Make execution failed with exit code = 127
```
![image.png](./img/1625099939108-e5488c1a-b8b6-4018-aa83-d64cbbff786d.png)

### 解决办法
#### 显式的配置make，不使用Clion自动检测的
![image.png](./img/1569403403793-260f3f93-3d1a-48db-a8cf-b69ac135af82.png)

#### 不配置make会在环境检测报错
:::danger
注意：不显式的配置make的路径，加载Makefile项目时会有问题

![image.png](./img/1625032221097-aee46744-319e-46c0-ae70-c5c8e6a85ba2.png)

目前该bug尚未修复，具体可见[issue](https://youtrack.jetbrains.com/issue/CPP-21816)：

[https://youtrack.jetbrains.com/issue/CPP-21816](https://youtrack.jetbrains.com/issue/CPP-21816)

![image.png](./img/1625032345413-211b6b8c-117a-4f0a-9d39-f435e03490d6.png)
:::
