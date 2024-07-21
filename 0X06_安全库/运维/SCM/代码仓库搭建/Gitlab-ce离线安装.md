<a name="WiRKd"></a>
## 1、下载跟Linux版本相关的.rpm包
地址：[https://packages.gitlab.com/gitlab/gitlab-ce](https://packages.gitlab.com/gitlab/gitlab-ce)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1696692521964-e9d420a4-61a7-4ef1-bbca-34aeaeb34ddd.png#averageHue=%23fafafa&clientId=uc45f3649-80d3-4&from=paste&height=707&id=u9f0bb267&originHeight=1768&originWidth=3799&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=517069&status=done&style=none&taskId=uc92f7fc2-e8f3-4d5f-9e7b-2d78b4a5499&title=&width=1519.6)
<a name="tPWWf"></a>
## 2、将下载的rpm包上传到机器
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1696692903997-c27e567e-792e-4f53-81a2-a0e25d1fa3d7.png#averageHue=%23393939&clientId=uc45f3649-80d3-4&from=paste&height=93&id=uc8ba5daf&originHeight=233&originWidth=2331&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=213761&status=done&style=none&taskId=u73d0abdc-f182-4eae-a9ff-d2b028af1ee&title=&width=932.4)
<a name="RHqul"></a>
## 3、使用rpm安装
`rpm -ivh ~~gitlab-ce-15.6.2-ce.0.el7.x86_64.rpm~~` 此处解压的包名需要换成下载的名称<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1696693460055-b1b732cd-5631-408c-b81d-ec7f8e8b48a2.png#averageHue=%23353535&clientId=uc45f3649-80d3-4&from=paste&height=777&id=ub3ecc302&originHeight=1942&originWidth=2770&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1945621&status=done&style=none&taskId=uaa29bcb4-96b2-4c0b-90a2-75afc5b30d0&title=&width=1108)
```bash
rpm -ivh gitlab-ce-16.3.5-ce.0.el7.x86_64.rpm
warning: gitlab-ce-16.3.5-ce.0.el7.x86_64.rpm: Header V4 RSA/SHA1 Signature, key ID f27eab47: NOKEY
Preparing...                          ################################# [100%]
Updating / installing...
   1:gitlab-ce-16.3.5-ce.0.el7        ################################# [100%]
It looks like GitLab has not been configured yet; skipping the upgrade script.

       *.                  *.
      ***                 ***
     *****               *****
    .******             *******
    ********            ********
   ,,,,,,,,,***********,,,,,,,,,
  ,,,,,,,,,,,*********,,,,,,,,,,,
  .,,,,,,,,,,,*******,,,,,,,,,,,,
      ,,,,,,,,,*****,,,,,,,,,.
         ,,,,,,,****,,,,,,
            .,,,***,,,,
                ,*,.



     _______ __  __          __
    / ____(_) /_/ /   ____ _/ /_
   / / __/ / __/ /   / __ `/ __ \
  / /_/ / / /_/ /___/ /_/ / /_/ /
  \____/_/\__/_____/\__,_/_.___/


Thank you for installing GitLab!
GitLab was unable to detect a valid hostname for your instance.
Please configure a URL for your GitLab instance by setting `external_url`
configuration in /etc/gitlab/gitlab.rb file.
Then, you can start your GitLab instance by running the following command:
  sudo gitlab-ctl reconfigure

For a comprehensive list of configuration options please see the Omnibus GitLab readme
https://gitlab.com/gitlab-org/omnibus-gitlab/blob/master/README.md

Help us improve the installation experience, let us know how we did with a 1 minute survey:
https://gitlab.fra1.qualtrics.com/jfe/form/SV_6kVqZANThUQ1bZb?installation=omnibus&release=16-3
```
根据启动成功后的提示语修改相应的配置。
<a name="eUfBs"></a>
## 4、安装完成后修改配置文件
（1）`vim /etc/gitlab/gitlab.rb`<br />（2）找到 external_url 'http://gxitlab.example.com'<br />（3）替换为你自己的IP和端口。 样例：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1696693979141-29bbd4b1-f784-433a-af9d-d0fbc216048e.png#averageHue=%232f2f2f&clientId=uc45f3649-80d3-4&from=paste&height=664&id=u0ade61f7&originHeight=1660&originWidth=3325&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=2047250&status=done&style=none&taskId=uf8af5258-9384-4bae-96e3-4eca4284342&title=&width=1330)
<a name="we0gL"></a>
## 5、重置并启动
第一步刷新配置：`gitlab-ctl reconfigure` （需等待一会，3-5分钟）<br />第二步重新启动： `gitlab-ctl restart` （需等待一会，3-5分钟）
<a name="DAp1x"></a>
### 刷新配置：`gitlab-ctl reconfigure`时遇到报错
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1696733224862-c020d89a-7b19-40bf-bb9a-ab991feece63.png#averageHue=%23363636&clientId=u80ddc938-ba2c-4&from=paste&height=704&id=u48348c5a&originHeight=1760&originWidth=3325&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=2146093&status=done&style=none&taskId=ud3c4b997-dee5-41af-a124-14ae8516589&title=&width=1330)
```
[2023-10-08T10:21:32+08:00] INFO: file[/var/opt/gitlab/gitlab-kas/authentication_secret_file] sending restart action to runit_service[gitlab-kas] (delayed)
Recipe: gitlab-kas::enable
  * runit_service[gitlab-kas] action restart

    ================================================================================
    Error executing action `restart` on resource 'runit_service[gitlab-kas]'
    ================================================================================

    Mixlib::ShellOut::ShellCommandFailed
    ------------------------------------
    Expected process to exit with [0], but received '1'
    ---- Begin output of /opt/gitlab/embedded/bin/sv restart /opt/gitlab/service/gitlab-kas ----
    STDOUT: fail: /opt/gitlab/service/gitlab-kas: runsv not running
    STDERR:
    ---- End output of /opt/gitlab/embedded/bin/sv restart /opt/gitlab/service/gitlab-kas ----
    Ran /opt/gitlab/embedded/bin/sv restart /opt/gitlab/service/gitlab-kas returned 1

    Cookbook Trace: (most recent call first)
    ----------------------------------------
    /opt/gitlab/embedded/cookbooks/cache/cookbooks/runit/libraries/helpers.rb:136:in `safe_sv_shellout!'
    /opt/gitlab/embedded/cookbooks/cache/cookbooks/runit/libraries/helpers.rb:164:in `restart_service'
    /opt/gitlab/embedded/cookbooks/cache/cookbooks/runit/libraries/provider_runit_service.rb:368:in `block in <class:RunitService>'

    Resource Declaration:
    ---------------------
    suppressed sensitive resource output

    Compiled Resource:
    ------------------
    suppressed sensitive resource output

    System Info:
    ------------
    chef_version=17.10.0
    platform=centos
    platform_version=7.6.1810
    ruby=ruby 3.0.6p216 (2023-03-30 revision 23a532679b) [x86_64-linux]
    program_name=/opt/gitlab/embedded/bin/cinc-client
    executable=/opt/gitlab/embedded/bin/cinc-client


Running handlers:
[2023-10-08T10:21:32+08:00] ERROR: Running exception handlers
There was an error running gitlab-ctl reconfigure:

runit_service[gitlab-kas] (gitlab-kas::enable line 148) had an error: Mixlib::ShellOut::ShellCommandFailed: Expected process to exit with [0], but received '1'
---- Begin output of /opt/gitlab/embedded/bin/sv restart /opt/gitlab/service/gitlab-kas ----
STDOUT: fail: /opt/gitlab/service/gitlab-kas: runsv not running
STDERR:
---- End output of /opt/gitlab/embedded/bin/sv restart /opt/gitlab/service/gitlab-kas ----
Ran /opt/gitlab/embedded/bin/sv restart /opt/gitlab/service/gitlab-kas returned 1

Running handlers complete
[2023-10-08T10:21:32+08:00] ERROR: Exception handlers complete
```
解决办法：
```bash
chmod 0755 /var/opt/gitlab/postgresql
systemctl restart gitlab-runsvdir
```
之后再重启Gitlab
```bash
gitlab-ctl reconfigure
gitlab-ctl restart
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1696734244118-51d86527-25b0-4e6d-b7d8-9993130b1d6f.png#averageHue=%233b3b3b&clientId=u80ddc938-ba2c-4&from=paste&height=634&id=ube5686df&originHeight=1586&originWidth=2372&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1443364&status=done&style=none&taskId=u8d569801-7629-4423-b6be-a9591f4af2a&title=&width=948.8)<br />可以发现所有相关的服务都在正常运行。
<a name="vG2CP"></a>
## 6、访问
访问地址为第四步在配置文件中配置的IP和端口<br />需要关闭防火墙或者开放填写的端口。关闭防火墙命令：`systemctl stop firewalld`
:::warning
在启动Gitlab的时候访问是会提示502 Whoops, GitLab is taking too much time to respond. <br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1696734297033-f6cfde63-8ea0-42af-b78f-77387ee4008d.png#averageHue=%23fdf7f5&clientId=u80ddc938-ba2c-4&from=paste&height=626&id=ub6a75aad&originHeight=1564&originWidth=2128&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=138124&status=done&style=none&taskId=uab4124a6-48bd-4740-85ce-fb742b5eca1&title=&width=851.2)<br />开始以为是服务出了问题，实际这是一个正常的过程，此问题说明Gitlab正在启动，消耗内存中，还没有启动完成！这时不要去修改端口，启动等，等待一下即可。
:::
账号为：root<br />密码为：`cat /etc/gitlab/initial_root_password` 文件中的Password
> 注意：初始化密码文件会在24小时后被清理，及时查看初始化密码。

![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1696696038008-78f76583-4912-4f82-a0ed-7beb0b5fc54c.png#averageHue=%23373737&clientId=uf8508b4f-08dd-4&from=paste&height=155&id=u428a82c2&originHeight=387&originWidth=3300&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=469419&status=done&style=none&taskId=ue8f5fc01-1751-470b-a110-7c46be207f7&title=&width=1320)
<a name="uLpBR"></a>
## 7、卸载步骤

1. 停止Gitlab服务：
```bash
sudo gitlab-ctl stop
```

2. 卸载Gitlab：
```bash
sudo rpm -e gitlab-ce
```

3. 查看Gitlab进程：
```bash
ps -ef | grep gitlab
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1696730770459-b3077ddb-c01c-4b1b-8799-7ca0954104d2.png#averageHue=%233b3b3b&clientId=udc942f90-59d8-4&from=paste&height=519&id=u1a52e789&originHeight=1298&originWidth=3319&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1684663&status=done&style=none&taskId=ua5ecdb4f-9e40-4d72-b354-855992170b0&title=&width=1327.6)<br />杀掉第一个守护进程（就是带有好多.............的进程）:
```bash
kill -9 <进程ID>
```

4. 再次查看Gitlab进程是否存在：
```bash
ps -ef | grep gitlab
```

5. 删除Gitlab文件：
```bash
find / -name *gitlab* | xargs rm -rf
```
