Linux<br />最糟糕的密码不是弱密码，而是根本没有密码。作为系统管理员，必须确保每个用户帐户都有一个强密码。接下来将简要的解释如何在 Linux 中查找密码为空的帐户。<br />在进入主题之前，快速回顾一下Shadow文件及其用途。
<a name="acMyH"></a>
## 1、目录

- Shadow文件介绍
- 查找Linux上所有没有设置密码的账户
- 查找特定状态的密码状态
- 在Linux中，如何设置账户密码
- 在Linux中，如何对账户上锁
- 在Linux中，如何对账户进行解锁操作
- 总结
<a name="kPynP"></a>
## 2、Shadow文件
在 RHEL 系统中，用户密码经过哈希处理并存储在名为 /etc/shadow 的安全文件中。Shadow密码文件包含用户帐户的用户身份验证信息和密码过期策略(password aging)的详细信息。<br />Shadow文件归 root 用户所有，且只有超级用户才能读取。可以使用以下命令验证Shadow文件的所有权和权限：
```bash
#  ls  -l /etc/shadow ---------- 1 root root 618 Apr  7 07:52 /etc/shadow
```
下面给出了影子文件中示例行的典型结构：
```bash
user1:$6$5ps/XV21$EFmQ463GJZnsdF/:19089:0:99999:7:::
```
Shadow文件有九个字段，每个字段间采用冒号分隔。<br />接下来快速浏览一下每个字段。

- 字段1（登录名）- 标识了一个登录帐号，同文件/etc/passwd中的相同。
- 字段2（加密后密码） - 包含用户对应的采用散列加密方式加密后的密码。如果此字段开头有一个感叹号 (!)，则表示该用户帐户已被锁定。如果此字段为空，则该用户没有密码。
- 字段3（上次更改）- 此字段显示最后一次修改密码的时间。如果此字段包含 0，则用户在下次登录时将被强制更改密码。
- 字段4（最短天数）- 此字段显示在允许用户更改密码之前必须经过的最短天数（mindays）。可以使用带有 `-m` 选项的 chage 命令来更改此字段的值。
- 字段5（最大天数）- 显示用户密码过期前密码有效的最大天数 (maxdays)。如果该字段为 0，则表示此功能已禁用。可以使用带有 -M 选项的 chage 命令来更改该字段的值。
- 字段6（警告）- 表示用户在密码过期前收到更改密码警告的天数（警告日）。可以使用带有 `-W` 选项的 chage 命令或带有 -w 选项的 passwd 命令来更改此值。
- 字段7（密码过期）- 定义用户能够使用过期密码登录的最大允许天数。这可以使用带有 `-I` 标志的 chage 命令或带有 `-i` 标志的 passwd 命令来更改。
- 字段8（帐户到期） - 定义用户的帐户将到期且不再可用的天数。可以使用带有 `-E` 选项的 chage 命令更改此字段的值。
- 字段 9（保留）- 该字段保留供将来使用。

如上所述，加密后的密码存储在Shadow文件中每个条目的第二个字段中，就在用户名之后。<br />因此，如果影子文件中的第二个字段为空，则用户没有密码。下面展示一个查找所有无密码用户帐户的示例。
<a name="VW59H"></a>
## 3、查找所有没有密码的账户
要检测所有没有密码的本地用户帐户，只需以 root 用户身份运行以下命令：
```bash
# awk -F: '$2 == "" { print $1, "has empty password!. Please set a strong password ASAP!!" }' /etc/shadow
```
下面是上述命令的输出示例：
> ostechnix has empty password!. Please set a strong password ASAP!!

还可以使用 getent 命令，同时结合 grep 和 cut 命令来识别 Linux 中的无密码的本地用户帐户，其命令如下所示：
```bash
# getent shadow | grep -Po '^[^:]*(?=::)'
```
也可以采用下面的命令：
```bash
# getent shadow | grep '^[^:]*::' | cut -d: -f1
```
以上所有命令将仅列出密码为空的本地用户帐户。如果要同时列出所有密码为空的帐户，下面的两个命令都可以实现该功能：
```bash
# getent shadow | grep -Po '^[^:]*(?=:.?:)'# getent shadow | grep '^[^:]*:.\?:' | cut -d: -f1
```
![图 1：查找无密码的账户](https://cdn.nlark.com/yuque/0/2022/png/396745/1652093732196-1ac0ae11-19a5-4f47-9771-1f4c5327a628.png#clientId=u42178511-3404-4&from=paste&id=uae401cfa&originHeight=585&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ucc44cda2-68da-441a-8837-3aea9d6dde5&title=%E5%9B%BE%201%EF%BC%9A%E6%9F%A5%E6%89%BE%E6%97%A0%E5%AF%86%E7%A0%81%E7%9A%84%E8%B4%A6%E6%88%B7 "图 1：查找无密码的账户")
<a name="qwkkN"></a>
## 4、查看特定账户的密码状态
上述命令将列出所有没有密码的帐户。还可以使用带有 `-S` 标志的 passwd 命令检查特定用户帐户的密码状态。
```bash
# passwd -S ostechnix
```
下面是一个上述命令的输出示例：
> **ostechnix** **NP** 2022-04-07 0 99999 7 -1 (**Empty password**.)

passwd命令将指示给定用户帐户的密码状态。可能的值是：

- LK – 该帐户被锁定。
- NP - 该帐户没有密码。
- PS – 该帐户有一个可用的密码。

注意：在基于 Debian 的系统中，密码状态将分别用L、N、P来标识。
<a name="DT2QC"></a>
## 5、在Linux中设置账户密码
可以作为无密码用户登录，但并不推荐！必须设置至少包含 8 个字符的强密码，且密码中要包括大写字母、小写字母、特殊字符和数字。<br />要在 Linux 中为用户帐户设置密码，请以 root 用户身份执行passwd 命令，如下所示：<br />作为根用户：
```bash
# passwd ostechnix
```
使用上述命令时，请将ostechnix 替换为自己的用户名。<br />现在用passwd命令来检查帐户的密码状态：
```bash
# passwd -S ostechnix
```
输出示例如下：
> ostechnix PS 2022-04-07 0 99999 7 -1 (Password set, SHA512 crypt.)

![图 2：在Linux中设置账户密码](https://cdn.nlark.com/yuque/0/2022/png/396745/1652093732186-26d3d69f-83bb-41e9-8f3d-3382c29b593f.png#clientId=u42178511-3404-4&from=paste&id=u9113e04e&originHeight=156&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u6250c18d-cb64-4863-bfd4-19f673e9b3b&title=%E5%9B%BE%202%EF%BC%9A%E5%9C%A8Linux%E4%B8%AD%E8%AE%BE%E7%BD%AE%E8%B4%A6%E6%88%B7%E5%AF%86%E7%A0%81 "图 2：在Linux中设置账户密码")
<a name="FOyuY"></a>
### 在Linux中锁定账户
有时，想要锁定一个没有密码的账户。如果是这样，首先如上所述找到密码为空的用户，以root用户的身份执行带有 `-l` 标志的 passwd 命令来锁定账户，其命令如下所示：
```bash
# passwd -l ostechnix
```
下面是上述命令的输出示例：
> Locking password for user ostechnix.
> passwd: Success

现在再来检查下帐户的状态：
```bash
# passwd -S ostechnix
```
其输出示例如下：
> ostechnix **LK** 2022-04-07 0 99999 7 -1 (**Password locked.**)

从输出情况可以看出用户已被锁定，且不能再登录系统了。<br />还可以使用带有 -L（大写 L）标志的 usermod 命令来锁定用户。
```bash
# usermod -L ostechnix
```
<a name="nOXWK"></a>
### 在Linux中解锁账户
要在 Linux 中解锁无密码用户，请以root身份执行 passwd 命令或带有`-p`的usermod命令，其命令如下：
```bash
# passwd ostechnix
```
输入两次密码以解锁密码。<br />使用 usermod 命令解锁用户密码为空的用户是不可能的，可以使用 `usermod -p` 设置密码来解锁用户的密码。
```bash
# usermod -p <password-here> ostechnix
```
<a name="aC9eE"></a>
## 6、总结    
解释了什么是shadow文件以及该文件在 Linux 中的用途。然后，讨论了在 Linux 中查找所有没有密码帐户的各种命令。最后，学习了如何为用户设置密码，以及如何在 Linux 中锁定和解锁用户。

