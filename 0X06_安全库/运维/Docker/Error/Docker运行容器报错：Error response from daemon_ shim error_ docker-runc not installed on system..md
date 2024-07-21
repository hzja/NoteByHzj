<a name="TfF7A"></a>
## 1、完整的报错
```bash
[root@iZuligp6e1dyzfZ bin]# docker run --name zk01 -p 2180:2180 --restart always -d 7341c5373a13
635978de0e825b5b6ffa7e610e3b844ca205222b4a6e761895011f0cb311d215
/usr/bin/docker-current: Error response from daemon: shim error: docker-runc not installed on system.
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596511587357-b79b5592-c55a-4299-8ac1-cd08ceb99ff6.png#height=58&id=XCEZn&originHeight=174&originWidth=3323&originalType=binary&size=213471&status=done&style=none&width=1107.6666666666667)
<a name="wKbjc"></a>
## 2、解决步骤-创建docker-runc的软链接
```bash
[root@iZuligp6e1dyzfZ bin]# cd /usr/libexec/docker
[root@iZuligp6e1dyzfZ docker]# ln -s docker-runc-current docker-runc
```

