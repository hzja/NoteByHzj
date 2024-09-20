HFS 是一个轻量级的HTTP 服务工具，3.0版本前进提供Windows平台安装包，3.0版本开提供Linux和macOS平台的安装包。HFS更适合在局域网环境中搭建文件共享服务或者安装配置源服务器。
<a name="g0Pw9"></a>
## 非守护进程的方式运行 HFS （Ubuntu 22.04）

1. 创建 HTTP 的根目录 /home/budgie/mirrorsHTTP
```bash
mkdir -vp /home/budgie/mirrorsHTTP
```

2. 把软件包 hfs-linux.zip 解压到自定义目录 /usr/bin/hfsFolder
3. 把二进制文件 hfs 软连接至/usr/bin/
```bash
ln -s /usr/bin/hfsFolder/hfs /usr/bin/
```

4. 执行 hfs， 而后不退出当前会话并输入 help、 再输入 config port 8011
5. 访问 http://localhost:8090/~/admin/ ， 添 加 共 享 文 件 夹 路 径/home/budgie/mirrorsHTTP
6. 查看点前在运行的 hfs 服务的配置文件 $HOME/.hfs/config.yaml

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701999134289-cc56ba43-f307-4a5f-8482-e78a9efa43a2.png#averageHue=%23272a31&clientId=u48069b88-b177-4&from=paste&id=ub526a27f&originHeight=251&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u527db81b-7dc2-45cb-b589-15efd8d125f&title=)

7. 在浏览器访问 http://192.168.136.136:8090/ 即可看到共享文件夹

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701999157255-066656b6-0095-4d28-b4d9-c6455eea1c09.png#averageHue=%23100f0d&clientId=u48069b88-b177-4&from=paste&id=u2102863f&originHeight=342&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubf8a507b-b761-4530-ab10-7a3e3a23a72&title=)
<a name="ppqLL"></a>
## 以守护进程的方式运行 HFS （Ubuntu 22.04）

1. 创建 HTTP 的根目录 /home/budgie/mirrorsHTTP
```bash
mkdir -vp /home/budgie/mirrorsHTTP
```

2. 把软件包 hfs-linux.zip 解压到自定义目录 /usr/bin/hfsFolder
3. 把二进制文件 hfs 软连接至/usr/bin/
```bash
ln -s /usr/bin/hfsFolder/hfs /usr/bin/
```

4. 执行 hfs ， 而后不退出当前会话并输入 help、 再输入 config port 8011
5. 访 问 http://localhost:8090/~/admin/ ， 添 加 共 享 文 件 夹 路 径/home/budgie/mirrorsHTTP
6. 查看点前在运行的 hfs 服务的配置文件 $HOME/.hfs/config.yaml

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701999196895-8b7cd0bc-e1df-4385-80c3-75d1292adebf.png#averageHue=%23272a31&clientId=u48069b88-b177-4&from=paste&id=u81fb4427&originHeight=253&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u935668aa-5568-480b-a07b-89c319f5ccf&title=)

7. 把 nodejs 的二进制包解压至 /usr/bin/hfsFolder/
```bash
tar -xf /home/budgie/Downloads/node-v20.9.0-linux-x64.tar.xz -C /usr/bin/hfsFolder/
```

8. 把二进制文件 node 、 nmp 软连接至/usr/bin/
```bash
ln -s /usr/bin/hfsFolder/node-v20.9.0-linux-x64/bin/node /usr/bin/
ln -s /usr/bin/hfsFolder/node-v20.9.0-linux-x64/bin/npm /usr/bin/
```

9. 执行 `node -version` 查看 node、 npm、 npx 的全局执行结果（如不可使其全局命令， 则应在 /etc/profile 中暴露其全局环境变量）
10. 执 行 `/usr/bin/hfsFolder/node-v20.9.0-linux-x64/bin/npx -y hfs@latest` 查看是否有报错
11. 编写脚本文件/etc/systemd/system/hfs.service 将 hfs 配置为守护进程服务

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701999219269-f2d4b48e-e63a-46a6-84e5-d427352429ff.png#averageHue=%23262930&clientId=u48069b88-b177-4&from=paste&id=u06e5a2c8&originHeight=382&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u027afe46-a039-458a-84c3-5f9c42e7266&title=)

12. 运行以下命令启动 hfs 服务
```bash
systemctl daemon-reload && systemctl enable hfs && systemctl start hfs
&& systemctl status hfs
```

13. 在浏览器访问 http://192.168.136.136:8090/ 即可看到共享文件夹

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701999243193-831cb1b7-111d-4107-993b-0b66a3dd6986.png#averageHue=%2311100e&clientId=u48069b88-b177-4&from=paste&id=u9c11d6ed&originHeight=340&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uff7e2326-4a3f-4a5c-9e7d-65a2b9cda08&title=)
<a name="JoV4E"></a>
## Windows上运行可参考：
[开源跨平台的局域网文件互传工具](https://www.yuque.com/fcant/app/pyp3rrcdtsetmos1?view=doc_embed&inner=BtFhz)
