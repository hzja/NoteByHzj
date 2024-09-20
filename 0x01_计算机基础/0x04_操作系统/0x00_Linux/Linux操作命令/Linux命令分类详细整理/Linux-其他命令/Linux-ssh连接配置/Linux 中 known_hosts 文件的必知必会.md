Linux<br />如果经常使用 Linux 系统 ssh 到一个远程服务器，在主目录中一般会有一个 .ssh 文件夹，在这个文件夹中会有若干个文件，其中包括一个 known_hosts 文件：
```bash
$ ls -l .ssh
total 16
-rwxr-xr-x 1 abhishek abhishek  618 Aug 30 16:52 config
-rw------- 1 abhishek abhishek 1766 Nov 12  2017 id_rsa
-rw-r--r-- 1 abhishek abhishek  398 Nov 12  2017 id_rsa.pub
-rw------- 1 abhishek abhishek    1 Sep 26 15:00 known_hosts
```
这里，id_rsa 是 ssh 的私钥，id_rsa.pub 为ssh公钥，config 文件用于创建概要信息，以便进行 ssh 连接。<br />这里重点要介绍的，是上述列表中的最后一个文件，known_hosts，它是客户端 ssh 配置文件的重要组成部分。
<a name="zBFMY"></a>
### SSH中的 known_hosts 文件是什么
known_hosts 文件存储用户访问的主机的公钥。这是一个非常重要的文件，它通过将用户的身份保存到本地系统来确保用户连接到合法的服务器。这也有助于避免中间人攻击。<br />当通过SSH连接到一个新的远程服务器时，系统会提示是否要将远程主机添加到 known_hosts 文件：
```bash
The authenticity of host '194.195.118.85 (194.195.118.85)' can't be established.
ED25519 key fingerprint is SHA256:wF2qILJg7VbqEE4/zWmyMTSwy3ja7be1jTIg3WzmpeE.
This key is not known by any other names
Are you sure you want to continue connecting (yes/no/[fingerprint])?
```
选择 yes，服务器的连接信息会保存在系统中。
<a name="B7V70"></a>
### 避免中间人攻击
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675125456068-6a5c9924-ded6-4d7d-9724-4aba5cfdd0aa.png#averageHue=%23373535&clientId=u03079a39-aa1a-4&from=paste&id=ue6989940&originHeight=289&originWidth=616&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6445e33e-b47a-44d8-a264-2b9f8c41f08&title=)<br />假设经常连接某个服务器，且将该服务器的连接信息保存在了 know_hosts 文件中。<br />如果远程服务器的公钥发生了更改，系统会根据 known_hosts 文件中存储的信息记录此次更改，也会收到此更改的通知：
```bash
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ WARNING: POSSIBLE DNS SPOOFING DETECTED!
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
The RSA host key for xyz remote host has changed,and the key for the corresponding IP address xxx.yy.xxx.yy is unknown. This could either mean that DNS SPOOFING is happening or the IP address for the host and its host key have changed at the same time.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
IT IS POSSIBLE THAT SOMEONE IS DOING SOMETHING NASTY!
Someone could be eavesdropping on you right now (man-in-the-middle attack)!
It is also possible that the RSA host key has just been changed.
The fingerprint for the RSA key sent by the remote host is
69:4e:bb:70:6a:64:e3:78:07:6f:b4:00:41:07:d8:9c.
Please contact your system administrator.
Add correct host key in /home/.ssh/known_hosts to get rid of this message.
Offending key in /home/.ssh/known_hosts:1
Keyboard-interactive authentication is disabled to avoid man-in-the-middle attacks.
```
在这种情况下，可以在接受这个新密钥之前联系远程服务器的管理员，以便确保远程服务器没有受到入侵。<br />有时候，管理员可能会由于重新安装服务器而故意更改服务器（主机）的密钥。<br />无论更改的原因是什么，都需要首先从 known_hosts 文件中删除旧密钥，以便重新连接到远程服务器。下次连接到此服务器时，客户端主机将为此服务器创建新的主机密钥。
<a name="LNZlR"></a>
### 管理多个经过身份验证的用户
如前所述，一旦客户机主机成功连接到远程服务器，其 known_hosts 文件将附加服务器的公钥。<br />有时可能会希望对服务器进行身份验证，而不需要提示进行服务器密钥验证。例如，正在运行 Ansible 之类的配置管理工具，并且不希望客户端主机请求服务器密钥验证。<br />因此，如果有多个用户，可以使用三种方式绕过SSH交互提示：<br />1）手动将服务器的公钥附加到每个用户的known_hosts文件中；<br />2）通过 ssh 访问服务器时，对每个客户端都使用命令行设置参数 `-o StrictHostKeyChecking=no`（不推荐）；<br />3）在主 ssh_known_hosts 文件中注册所有主机，然后将此文件编排到所有客户端主机，然后使用 ssh-keyscan 命令：
```bash
ssh-keyscan -H -t rsa 'your-server-ip' >> /etc/ssh/ssh_known_hosts
```
下图显示了如何使用 `StrictHostKeyChecking=no` 选项：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675125456151-7c2c48a2-63b2-42b0-9c4c-25ac5deae1ec.png#averageHue=%23140502&clientId=u03079a39-aa1a-4&from=paste&id=u9252104e&originHeight=378&originWidth=890&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3dac8435-eadc-4354-bb07-4ea0df87696&title=)<br />上述方法中，第一种方法相比于另外两种方法，操作起来更加繁琐。
<a name="Go1vj"></a>
### 从 known_hosts 文件获取远程系统详细信息
获取远程系统的详细信息，不是一个简单的工作。<br />基于安全考虑，几乎所有 Linux 系统都会将 ssh 配置文件中的 HashKnownHosts 参数设置为 yes。<br />这也就意味着在 known_hosts 文件中的信息是以 hash 方式存储的，可能会看到一些随机数，但这并不能提供任何有用的信息。
```bash
$ cat .ssh/known_hosts

|1|yWIW17YIg0wBRXJ8Ktt4mcfBqsk=|cFHOrZ8VEx0vdOjau2XQr/K7B/c= ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIFR293PJnDCj59XxfqYGctrMo60ZU5IOjACZZNRp9D6f
|1|Ta7hoH/az4O3l2dwfaKh8O2jitI=|WGU5TKhMA+2og1qMKE6kmynFNYo= ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQCmrxLW436AyBGyGCggl/j2qBCr782AVIvbiTEsWNBWLcWMKYAQpTdAXnaV4bBRqnk2NJg/60XDHKC2DF1tzx6ABWN/R6vcUAbulI9H1RUWpJ1AiDmFL84MvW2UukbpN5a6Lr+DvjclVqPxJRjQKr6Vy2K9oJgGKnHVcWSIHeAlW49kCCg5fIxF8stBTqJg0cRk6uxmcYVud1vh9a7SaZGK+jFZTB75RiHAVFuitHWpljecMxJRNYy/EhmmXrrvyU8pObVXlWlDd61uwExi4uEwNSY+Do7vR1y8svnt9mjTzzyM6MhT4sOcxWmNgdmw7bU/wPpie3dSjZMQeu2mQCSM7SG28dwjSyFPpanRsZKzkh0okAaCSItoNwl6zOf6dE3zt0s5EI6BPolhFAbT3NqmXRblxb7eV8rGEPf14iguHUkg6ZQr2OUdfeN1FYNMJ8Gb9RD159Mwjl4/jPIBdnXvt7zYct3XhPKm7Wxv4K/RWZE837C7mGQh2KEahWajdq8=
|1|NonAy25kVXL24U2mx6ZNxAY5m98=|ypf0IMpf3qq3vhrvUMprssOhODs= ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBE3br/9jaZfdB+qBxiVEZBJMb4XQONwzV4tH1xeFZX/zkyws2eBHrVO9O5l9b6M6+gO6nBtCwAzzaeLOn6mo8GQ=
```
如果知道系统的主机名或IP地址，则可以从 known_hosts 获取相关信息：
```bash
ssh-keygen -l -F <server-IP-or-hostname>
```
但是，如果想要一个能够以明文形式列出的所有服务器及其详细信息的命令，那是不可能的。<br />当然，现实中会有一些专门的工具可以破译 known_hosts，但这不是普通用户可以做到的。
<a name="HdC68"></a>
### 从known_hosts中删除信息
如果想要从 known_hosts 文件中删除某个特定的主机信息，可以根据远程系统的主机名或 IP 执行这个操作：
```bash
ssh-keygen -R <server-hostname-or-ip>
```
当然也可以先识别相关服务器的信息，然后使用 rm 命令手动删除，但是这个操作要麻烦的多，远不如使用上述命令方便。<br />通过了解 ssh 相关的配置文件，有助于更好的理解系统安全相关知识。known_hosts 正是系统安全相关的重要组成部分。
