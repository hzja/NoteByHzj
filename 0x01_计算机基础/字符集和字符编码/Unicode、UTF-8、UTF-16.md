编码<br />计算机起源于美国，上个世纪，他们对英语字符与二进制位之间的关系做了统一规定，并制定了一套字符编码规则，这套编码规则被称为ASCII编码<br />ASCII 编码一共定义了128个字符的编码规则，用七位二进制表示 ( 0x00 - 0x7F ), 这些字符组成的集合就叫做 ASCII 字符集<br />随着计算机的普及，在不同的地区和国家又出现了很多字符编码，比如: 大陆的 GB2312、港台的 BIG5, 日本的 Shift JIS等等<br />由于字符编码不同，计算机在不同国家之间的交流变得很困难，经常会出现乱码的问题，比如：对于同一个二进制数据，不同的编码会解析出不同的字符<br />当互联网迅猛发展，地域限制打破之后，人们迫切的希望有一种统一的规则, 对所有国家和地区的字符进行编码，于是 Unicode 就出现了
<a name="S86up"></a>
### Unicode 简介
Unicode 是国际标准字符集，它将世界各种语言的每个字符定义一个唯一的编码，以满足跨语言、跨平台的文本信息转换<br />Unicode 字符集的编码范围是 **0x0000 - 0x10FFFF** , 可以容纳一百多万个字符， 每个字符都有一个独一无二的编码，也即每个字符都有一个二进制数值和它对应，这里的二进制数值也叫 **码点** , 比如：汉字 **"中"** 的 码点是 **0x4E2D**, 大写字母 **A** 的码点是 **0x41**, 具体字符对应的 Unicode 编码可以查询 Unicode字符编码表
<a name="a94fM"></a>
### 字符集和字符编码
字符集是很多个字符的集合，例如 GB2312 是简体中文的字符集，它收录了六千多个常用的简体汉字及一些符号，数字，拼音等字符<br />字符编码是 字符集的一种实现方式，把字符集中的字符映射为特定的字节或字节序列，它是一种规则<br />比如：Unicode 只是字符集，UTF-8、UTF-16、UTF-32 才是真正的字符编码规则
<a name="WGMhv"></a>
### Unicode 字符存储
Unicode 是一个符号集， 它只规定了每个符号的二进制值，但是符号具体如何存储它并没有规定<br />前面提到, Unicode 字符集的编码范围是 **0x0000 - 0x10FFFF**，因此需要 1 到 3 个字节来表示<br />那么，对于三个字节的 Unicode字符，计算机怎么知道它表示的是一个字符而不是三个字符呢 ？<br />如果所有字符都用三个字节表示，那么对于那些一个字节就能表示的字符来说，有两个字节是无意义的，对于存储来说，这是极大的浪费，假如 , 一个普通的文本, 大部分字符都只需一个字节就能表示，现在如果需要三个字节才能表示，文本的大小会大出三倍左右<br />因此，Unicode 出现了多种存储方式，常见的有 UTF-8、UTF-16、UTF-32，它们分别用不同的二进制格式来表示 Unicode 字符<br />UTF-8、UTF-16、UTF-32 中的 "UTF" 是 "Unicode Transformation Format" 的缩写，意思是"Unicode 转换格式"，后面的数 字表明至少使用多少个比特位来存储字符, 比如：UTF-8 最少需要8个比特位也就是一个字节来存储，对应的， UTF-16 和 UTF-32 分别需要最少 2 个字节 和 4 个字节来存储
<a name="wCccf"></a>
### UTF-8 编码
UTF-8: 是一种变长字符编码，被定义为将码点编码为 1 至 4 个字节，具体取决于码点数值中有效二进制位的数量<br />UTF-8 的编码规则:

1. 对于单字节的符号，字节的第一位设为 **0**，后面 7 位为这个符号的 Unicode 码。因此对于英语字母，UTF-8 编码和 ASCII 码是相同的, 所以 UTF-8 能兼容 ASCII 编码，这也是互联网普遍采用 UTF-8 的原因之一
2. 对于 **n** 字节的符号（ **n > 1**），第一个字节的前 **n** 位都设为 **1**，第 **n + 1** 位设为 **0**，后面字节的前两位一律设为 **10** 。剩下的没有提及的二进制位，全部为这个符号的 Unicode 码

下表是Unicode编码对应UTF-8需要的字节数量以及编码格式

| Unicode编码范围(16进制) | UTF-8编码方式(二进制) |
| --- | --- |
| 000000 - 00007F | **0**xxxxxxx ASCII码 |
| 000080 - 0007FF | **110**xxxxx **10**xxxxxx |
| 000800 - 00FFFF | **1110**xxxx **10**xxxxxx **10**xxxxxx |
| 01 0000 - 10 FFFF | **11110**xxx **10**xxxxxx **10**xxxxxx **10**xxxxxx |

表格中第一列是Unicode编码的范围，第二列是对应UTF-8编码方式，其中红色的二进制 **"1"** 和 **"0"** 是固定的前缀, 字母 **x** 表示可用编码的二进制位<br />根据上面表格，要解析 UTF-8 编码就很简单了，如果一个字节第一位是 **0** ，则这个字节就是一个单独的字符，如果第一位是 **1** ，则连续有多少个 **1** ，就表示当前字符占用多少个字节<br />下面以 **"中"** 字 为例来说明 UTF-8 的编码，具体的步骤如下图， 为了便于说明，图中左边加了 1，2，3，4 的步骤编号<br />![2021-06-19-12-15-30-420007.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624076533884-4034f69d-077e-48e8-88e7-c1f2b4c5584a.png#clientId=u4e321392-7ddf-4&from=ui&id=u9ef9ac97&originHeight=916&originWidth=836&originalType=binary&ratio=3&size=2302038&status=done&style=shadow&taskId=u93d9bd7e-47b0-4e38-b80f-d19ad8a1908)<br />首先查询 **"中"** 字的 Unicode 码 **0x4E2D**, 转成二进制, 总共有 16 个二进制位， 具体如上图 步骤1 所示<br />通过前面的 Unicode 编码和 UTF-8 编码的表格知道，Unicode 码 **0x4E2D** 对应 **000800 - 00FFFF** 的范围，所以, **"中"** 字的 UTF-8 编码 需要 **3** 个字节，即格式是 **1110**xxxx **10**xxxxxx **10**xxxxxx<br />然后从 **"中"** 字的最后一个二进制位开始，按照从后向前的顺序依次填入格式中的 **x** 字符，多出的二进制补为 **0**， 具体如上图 步骤2、步骤3 所示<br />于是，就得到了 **"中"** 的 UTF-8 编码是 **11100100** **10111000** **10101101**, 转换成十六进制就是 **0xE4B8AD**， 具体如上图 步骤4 所示
<a name="JB1v4"></a>
### UTF-16 编码
UTF-16 也是一种变长字符编码, 这种编码方式比较特殊, 它将字符编码成 2 字节 或者 4 字节<br />具体的编码规则如下:

1. 对于 Unicode 码小于 **0x10000** 的字符， 使用 **2** 个字节存储，并且是直接存储 Unicode 码，不用进行编码转换
2. 对于 Unicode 码在 **0x10000** 和 **0x10FFFF** 之间的字符，使用 **4** 个字节存储，这 **4** 个字节分成前后两部分，每个部分各两个字节，其中，前面两个字节的前 **6** 位二进制固定为 **110110**，后面两个字节的前 6 位二进制固定为 **110111**, 前后部分各剩余 10 位二进制表示符号的 Unicode 码 减去 **0x10000** 的结果
3. 大于 **0x10FFFF** 的 Unicode 码无法用 UTF-16 编码

下表是Unicode编码对应UTF-16编码格式

| Unicode编码范围(16进制) | 具体Unicode码(二进制) | UTF-16编码方式(二进制) | 字节 |
| --- | --- | --- | --- |
| 0000 0000 - 0000 FFFF | xxxxxxxx xxxxxxxx | xxxxxxxx xxxxxxxx | 2 |
| 0001 0000 - 0010 FFFF | yy yyyyyyyy xx xxxxxxxx | **110110**yy yyyyyyyy **110111**xx xxxxxxxx | 4 |

表格中第一列是Unicode编码的范围，第二列是 具体Unicode码的二进制 ( 第二行的第二列表示的是 Unicode 码 减去 **0x10000** 后的二进制 ) , 第三列是对应UTF-16编码方式，其中红色的二进制 **"1"** 和 **"0"** 是固定的前缀, 字母 **x** 和 **y** 表示可用编码的二进制位， 第四列表示 编码占用的字节数<br />前面提到过，**"中"** 字的 Unicode 码是 **4E2D**, 它小于 **0x10000**，根据表格可知，它的 UTF-16 编码占两个字节，并且和 Unicode 码相同，所以 **"中"** 字的 UTF-16 编码为 **4E2D**<br />从 Unicode字符表网站 找了一个老的南阿拉伯字母, 它的 Unicode 码是: **0x10A6F** , 可以访问 https://unicode-table.com/cn/10A6F/ 查看字符的说明, Unicode 码对应的字符如下图所示<br />![2021-06-19-12-15-30-515751.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624076548125-aa1ff67d-486e-46d5-80d3-27ecc0b27dbf.png#clientId=u4e321392-7ddf-4&from=ui&id=u882e0238&originHeight=296&originWidth=678&originalType=binary&ratio=3&size=603410&status=done&style=shadow&taskId=u34ad254d-c976-45d8-8e1c-ed54bdfe913)<br />下面以这个 老的南阿拉伯字母的 Unicode 码 **0x10A6F** 为例来说明 UTF-16 **4** 字节的编码，具体步骤如下，为了便于说明，图中左边加了 1，2，3，4 、5的步骤编号<br />![2021-06-19-12-15-30-622464.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624076560133-d2fe4fb6-d001-465d-b763-67f22fb18c56.png#clientId=u4e321392-7ddf-4&from=ui&id=uc2245e6c&originHeight=943&originWidth=994&originalType=binary&ratio=3&size=2817599&status=done&style=shadow&taskId=u770daaa6-d603-4be9-839d-d40840ccdc1)<br />首先把 Unicode 码 **0x10A6F** 转成二进制, 对应上图的 步骤 1<br />然后把 Unicode 码 **0x10A6F** 减去 **0x10000**, 结果为 **0xA6F** 并把这个值转成二进制 **00 00000010** **10 01101111**，对应上图的 步骤 2<br />然后 从二进制 **00 00000010** **10 01101111** 的最后一个二进制为开始，按照从后向前的顺序依次填入格式中的 **x** 和 **y** 字符，多出的二进制补为 **0**， 对应上图的 步骤 3、 步骤 4<br />于是，就计算出了 Unicode 码 **0x10A6F** 的 UTF-16 编码是 **11011000 00000010** **11011110 01101111** , 转换成十六进制就是 **0xD802DE6F**， 对应上图的 步骤 5
<a name="sCSIN"></a>
### UTF-32 编码
UTF-32 是固定长度的编码，始终占用 4 个字节，足以容纳所有的 Unicode 字符，所以直接存储 Unicode 码即可，不需要任何编码转换。虽然浪费了空间，但提高了效率。
<a name="wHHp7"></a>
### UTF-8、UTF-16、UTF-32 之间如何转换
前面介绍过，UTF-8、UTF-16、UTF-32 是 Unicode 码表示成不同的二进制格式的编码规则，同样，通过这三种编码的二进制表示，也能获得对应的 Unicode 码，有了字符的 Unicode 码，按照上面介绍的 UTF-8、UTF-16、UTF-32 的编码方法 就能转换成任一种编码了
<a name="AzCRZ"></a>
### UTF 字节序
最小编码单元是多字节才会有字节序的问题存在，UTF-8 最小编码单元是一字节，所以 它是没有字节序的问题，UTF-16 最小编码单元是 2 个字节，在解析一个 UTF-16 字符之前，需要知道每个编码单元的字节序<br />比如：前面提到过，**"中"** 字的 Unicode 码是 **4E2D**, **"ⵎ"** 字符的 Unicode 码是 **2D4E**， 当收到一个 UTF-16 字节流 **4E2D** 时，计算机如何识别它表示的是字符 **"中"** 还是 字符 **"ⵎ"** 呢 ?<br />所以，对于多字节的编码单元，需要有一个标记显式的告诉计算机，按照什么样的顺序解析字符，也就是字节序，字节序分为 大端字节序 和 小端字节序<br />小端字节序简写为 LE( Little-Endian ), 表示 低位字节在前，高位字节在后, 高位字节保存在内存的高地址端，而低位字节保存在内存的低地址端<br />大端字节序简写为 BE( Big-Endian ), 表示 高位字节在前，低位字节在后，高位字节保存在内存的低地址端，低位字节保存在在内存的高地址端<br />下面以 **0x4E2D** 为例来说明大端和小端，具体参见下图:<br />![2021-06-19-12-15-30-708280.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624076572583-89064c8c-2b4e-4248-8c65-eee3b7ac92c9.png#clientId=u4e321392-7ddf-4&from=ui&id=u29e41429&originHeight=450&originWidth=912&originalType=binary&ratio=3&size=1233724&status=done&style=shadow&taskId=u72d645e6-a5e9-42f5-9cba-cced49cb84c)<br />数据是从高位字节到低位字节显示的，这也更符合人们阅读数据的习惯，而内存地址是从低地址向高地址增加<br />所以，字符**0x4E2D** 数据的高位字节是 **4E**，低位字节是 **2D**<br />按照大端字节序的高位字节保存内存低地址端的规则，**4E** 保存到低内存地址 **0x10001** 上，**2D** 则保存到高内存地址 **0x10002** 上<br />对于小端字节序，则正好相反，数据的高位字节保存到内存的高地址端，低位字节保存到内存低地址端的，所以 **4E** 保存到高内存地址 **0x10002** 上，**2D** 则保存到低内存地址 **0x10001** 上
<a name="NFwh4"></a>
### BOM
BOM 是 byte-order mark 的缩写，是 "字节序标记" 的意思, 它常被用来当做标识文件是以 UTF-8、UTF-16 或 UTF-32 编码的标记<br />在 Unicode 编码中有一个叫做 "零宽度非换行空格" 的字符 ( ZERO WIDTH NO-BREAK SPACE ), 用字符 **FEFF** 来表示<br />对于 UTF-16 ，如果接收到以 **FEFF** 开头的字节流， 就表明是大端字节序，如果接收到 **FFFE**， 就表明字节流 是小端字节序<br />UTF-8 没有字节序问题，上述字符只是用来标识它是 UTF-8 文件，而不是用来说明字节顺序的。"零宽度非换行空格" 字符 的 UTF-8 编码是 **EF BB BF**, 所以如果接收到以 **EF BB BF** 开头的字节流，就知道这是UTF-8 文件<br />下面的表格列出了不同 UTF 格式的固定文件头

| UTF编码 | 固定文件头 |
| --- | --- |
| UTF-8 | EF BB BF |
| UTF-16LE | FF FE |
| UTF-16BE | FE FF |
| UTF-32LE | FF FE 00 00 |
| UTF-32BE | 00 00 FE FF |

根据上面的 固定文件头，下面列出了 **"中"** 字在文件中的存储 ( 包含文件头 )

| 编码 | 固定文件头 |
| --- | --- |
| **Unicode 编码** | 0X004E2D |
| **UTF-8** | **EF BB BF** 4E 2D |
| **UTF-16BE** | **FE FF** 4E 2D |
| **UTF-16LE** | **FF FE** 2D 4E |
| **UTF-32BE** | **00 00 FE FF** 00 00 4E 2D |
| **UTF-32LE** | **FF FE 00 00** 2D 4E 00 00 |

<a name="Iw6AE"></a>
### 常见的字符编码的问题
<a name="EzWf2"></a>
#### Redis 中文key的显示
有时候需要向redis中写入含有中文的数据，然后在查看数据，但是会看到一些其他的字符，而不是写入的中文<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624076651897-454f5c52-1de6-419f-9af1-22adfef141fd.png#clientId=u4e321392-7ddf-4&from=paste&height=246&id=u6aa15f25&originHeight=738&originWidth=1431&originalType=binary&ratio=3&size=346555&status=done&style=none&taskId=uabce2a0c-7cc2-4503-bb9b-a4dca5ec1ff&width=477)<br />上图中，向redis 写入了一个 "中" 字，通过 get 命令查看的时候无法显示写入的 "中" 字<br />这时候加一个 `--raw` 参数，重新启动 redis-cli 即可，也即 执行 `redis-cli --raw` 命令启动redis客户端，具体的如下图所示<br />![2021-06-19-12-15-30-901751.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624076799814-696a7e54-a59a-48a9-917e-6a19a9ff17ee.png#clientId=u4e321392-7ddf-4&from=ui&id=u21c87b61&originHeight=134&originWidth=455&originalType=binary&ratio=3&size=183422&status=done&style=none&taskId=ua1838c21-ab98-4ab6-9539-ffdea6f93b0)
<a name="xG1Tu"></a>
#### MySQL 中的 utf8 和 utf8mb4
MySQL 中的 "utf8" 实际上不是真正的 UTF-8， "utf8" 只支持每个字符最多 3 个字节, 对于超过 3 个字节的字符就会出错, 而真正的 UTF-8 至少要支持 4 个字节<br />MySQL 中的 "utf8mb4" 才是真正的 UTF-8<br />下面以 test 表为例来说明, 表结构如下:
```sql
mysql> show create table test\G
*************************** 1. row ***************************
       Table: test
Create Table: CREATE TABLE `test` (
  `name` char(32) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8
1 row in set (0.00 sec)
```
向 test 表分别插入 **"中"** 字 和 Unicode 码为 **0x10A6F** 的字符，这个字符需要从 https://unicode-table.com/cn/10A6F/ 直接复制到 MySQL 控制台上，手工输入会无效，具体的执行结果如下图:<br />![2021-06-19-12-15-30-994470.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624076856070-24c87e21-d39e-4b51-80a0-ac104401e034.png#clientId=u4e321392-7ddf-4&from=ui&id=u9274218d&originHeight=128&originWidth=1073&originalType=binary&ratio=3&size=412904&status=done&style=none&taskId=ud8072ba7-934b-442c-9829-e6275e56f5e)<br />从上图可以看出，插入 **"中"** 字 成功，插入 **0x10A6F** 字符失败，错误提示无效的字符串，**\xF0\X90\XA9\xAF** 正是 **0x10A6F** 字符的 UTF-8 编码，占用 **4** 个字节, 因为 MySQL 的 utf8 编码最多只支持 **3** 个字节，所以插入会失败<br />把 test 表的字符集改成 utf8mb4 , 排序规则 改成 utf8bm4_unicode_ci, 具体如下图所示：<br />![2021-06-19-12-15-31-107169.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624076868847-7b50f278-642a-479b-83a3-5bf0caa65d5d.png#clientId=u4e321392-7ddf-4&from=ui&id=u6607c0e6&originHeight=257&originWidth=1046&originalType=binary&ratio=3&size=808103&status=done&style=none&taskId=u2bf0c4ee-4927-4d60-b17b-73ffdb5ec91)<br />字符集和排序方式修改之后，再次插入 **0x10A6F** 字符， 结果是成功的，具体执行结果如下图所示<br />![2021-06-19-12-15-31-222876.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624076878389-82f7cb57-496d-4447-b005-b7c25cc01a03.png#clientId=u4e321392-7ddf-4&from=ui&id=u3227dd5d&originHeight=140&originWidth=643&originalType=binary&ratio=3&size=270713&status=done&style=none&taskId=u9c210660-bd62-4367-b535-564ea66a7cf)<br />上图中，set names utf8mb4 是为了测试方便，临时修改当前会话的字符集，以便保持和 服务器一致，实际解决这个问题需要修改 my.cnf 配置中 服务器和客户端的字符集
