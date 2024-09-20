Linux
<a name="aupAL"></a>
## 一、压缩管理
<a name="qwr8q"></a>
### 1、bunzip2
<a name="fChjx"></a>
#### 1.1 功能说明
.bz2 文件的解压缩程序。
<a name="ChWsQ"></a>
#### 1.2 语法
```bash
bunzip2 [-fkLsvV][.bz2 压缩文件]
```
<a name="UPgZO"></a>
#### 1.3 补充说明
bunzip2 可解压缩.bz2 格式的压缩文件。bunzip2 实际上是 bzip2 的符号连接，执行 bunzip2 与 bzip2 -d 的效果相同。
<a name="WxMiZ"></a>
#### 1.4 参数
-f 或–force 解压缩时，若输出的文件与现有文件同名时，预设不会覆盖现有的文件。若要覆盖，请使用此参数。<br />-k 或–keep 在解压缩后，预设会删除原来的压缩文件。若要保留压缩文件，请使用此参数。<br />-s 或–small 降低程序执行时，内存的使用量。<br />-v 或–verbose 解压缩文件时，显示详细的信息。<br />-l,–license,-V 或–version 显示版本信息。
<a name="EkucP"></a>
### 2、bzip2
<a name="By7Y9"></a>
#### 2.1 功能说明
.bz2 文件的压缩程序。
<a name="ogRBg"></a>
#### 2.2 语法
```bash
bzip2 [-cdfhkLstvVz][–repetitive-best][–repetitive-fast][- 压缩等级][要压缩的文件]
```
<a name="d2Sqr"></a>
#### 2.3 补充说明
bzip2 采用新的压缩演算法，压缩效果比传统的 LZ77/LZ78 压缩演算法来得好。若没有加上任何参数，bzip2 压缩完文件后会产生.bz2 的压缩文件，并删除原始的文件。
<a name="UyuMk"></a>
#### 2.4 参数
-c 或–stdout 将压缩与解压缩的结果送到标准输出。<br />-d 或–decompress 执行解压缩。<br />-f 或–force bzip2 在压缩或解压缩时，若输出文件与现有文件同名，预设不会覆盖现有文件。若要覆盖，请使用此参数。<br />-h 或–help 显示帮助。<br />-k 或–keep bzip2 在压缩或解压缩后，会删除原始的文件。若要保留原始文件，请使用此参数。<br />-s 或–small 降低程序执行时内存的使用量。<br />-t 或–test 测试.bz2 压缩文件的完整性。<br />-v 或–verbose 压缩或解压缩文件时，显示详细的信息。<br />-z 或–compress 强制执行压缩。<br />-L,–license,<br />-V 或–version 显示版本信息。<br />–repetitive-best 若文件中有重复出现的资料时，可利用此参数提高压缩效果。<br />–repetitive-fast 若文件中有重复出现的资料时，可利用此参数加快执行速度。<br />-压缩等级 压缩时的区块大小。
<a name="KustV"></a>
### 3、cpio
<a name="pdNht"></a>
#### 3.1 功能说明
备份文件。
<a name="ySV0E"></a>
#### 3.2 语法
```bash
cpio [-0aABckLovV][-C <输入/输出大小>][-F <备份档>][-H <备份格式>][-O <备份档>][–block-size=<区块大小>][–force-local][–help][–quiet][–version]
# 或 
cpio [-bBcdfikmnrsStuvV][-C <输入/输出大小>][-E <范本文件>][-F <备份档>][-H <备份格式>][-I <备份档 >][-M < 回传信息 >][-R < 拥有者 ><:/.>< 所属群组 >][–block-size=< 区块大小 >][–force-local][–help][–no-absolute-filenames][–no-preserve-owner][–only-verify-crc][–quiet][–sparse][–version][范本样式…]
# 或 
cpio [-0adkiLmpuvV][-R < 拥有者 ><:/.>< 所属群组 >][–help][–no-preserve-owner][–quiet][–sparse][–version][目的目]
```
<a name="zcB7U"></a>
#### 3.3 补充说明
cpio 是用来建立，还原备份档的工具程序，它可以加入，解开 cpio 或 tra 备份档内的文件。
<a name="eczPA"></a>
#### 3.4 参数
-0 或–null 接受新增列控制字符，通常配合 find 指令的"-print0"参数使用。<br />-a 或–reset-access-time 重新设置文件的存取时间。<br />-A 或–append 附加到已存在的备份档中，且这个备份档必须存放在磁盘上，而不能放置于磁带机里。<br />-b 或–swap 此参数的效果和同时指定"-sS"参数相同。<br />-B 将输入/输出的区块大小改成 5210 Bytes。<br />-c 使用旧 ASCII 备份格式。<br />-C<区块大小>或–io-size=<区块大小> 设置输入/输出的区块大小，单位是 Byte。<br />-d 或–make-directories 如有需要 cpio 会自行建立目录。<br />-E<范本文件>或–pattern-file=<范本文件> 指定范本文件，其内含有一个或多个范本样式，让 cpio 解开符合范本条件的文件，格式为每列一个范本样式。<br />-f 或–nonmatching 让 cpio 解开所有不符合范本条件的文件。<br />-F<备份档>或–file=<备份档> 指定备份档的名称，用来取代标准输入或输出，也能借此通过网络使用另一台主机的保存设备存取备份档。<br />-H<备份格式> 指定备份时欲使用的文件格式。<br />-i 或–extract 执行 copy-in 模式，还原备份档。<br />-l<备份档> 指定备份档的名称，用来取代标准输入，也能借此通过网络使用另一台主机的保存设备读取备份档。<br />-k 此参数将忽略不予处理，仅负责解决 cpio 不同版本间的兼容性问题。<br />-l 或–link 以硬连接的方式取代复制文件，可在 copy-pass 模式下运用。<br />-L 或–dereference 不建立符号连接，直接复制该连接所指向的原始文件。<br />-m 或 preserve-modification-time 不去更换文件的更改时间。<br />-M<回传信息>或–message=<回传信息> 设置更换保存媒体的信息。<br />-n 或–numeric-uid-gid 使用"-tv"参数列出备份档的内容时，若再加上参数"-n"，则会以用户识别码和群组识别码替代拥有者和群组名称列出文件清单。<br />-o 或–create 执行 copy-out 模式，建立备份档。<br />-O<备份档> 指定备份档的名称，用来取代标准输出，也能借此通过网络 使用另一台主机的保存设备存放备份档。<br />-p 或–pass-through 执行 copy-pass 模式，略过备份步骤，直接将文件复制到目的目录。<br />-r 或–rename 当有文件名称需要更动时，采用互动模式。<br />-R<拥有者><:/.><所属群组>或<br />----owner<拥有者><:/.><所属群组> 在 copy-in 模式还原备份档，或 copy-pass 模式复制文件时，可指定这些备份，复制的文件的拥有者与所属群组。<br />-s 或–swap-bytes 交换每对字节的内容。<br />-S 或–swap-halfwords 交换每半个字节的内容。<br />-t 或–list 将输入的内容呈现出来。<br />-u 或–unconditional 置换所有文件，不论日期时间的新旧与否，皆不予询问而直接覆盖。<br />-v 或–verbose 详细显示指令的执行过程。<br />-V 或–dot 执行指令时，在每个文件的执行程序前面加上".“号<br />–block-size=<区块大小> 设置输入/输出的区块大小，假如设置数值为 5，则区块大小为 2500，若设置成 10，则区块大小为 5120，依次类推。<br />–force-local 强制将备份档存放在本地主机。<br />–help 在线帮助。<br />–no-absolute-filenames 使用相对路径建立文件名称。<br />–no-preserve-owner 不保留文件的拥有者，谁解开了备份档，那些文件就归谁所有。<br />-only-verify-crc 当备份档采用 CRC 备份格式时，可使用这项参数检查备份档内的每个文件是否正确无误。<br />–quiet 不显示复制了多少区块。<br />–sparse 倘若一个文件内含大量的连续 0 字节，则将此文件存成稀疏文件。<br />–version 显示版本信息。
<a name="YJYvP"></a>
### 4、gunzip
<a name="g6NJf"></a>
#### 4.1 功能说明
解压文件。
<a name="euyPp"></a>
#### 4.2 语法
```bash
gunzip [-acfhlLnNqrtvV][-s <压缩字尾字符串>][文件…] 或 gunzip [-acfhlLnNqrtvV][-s <压缩字尾字符串>][目录]
```
<a name="qKtxh"></a>
#### 4.3 补充说明
gunzip 是个使用广泛的解压缩程序，它用于解开被 gzip 压缩过的文件，这些压缩文件预设最后的扩展名为“.gz”。事实上 gunzip 就是 gzip 的硬连接，因此不论是压缩或解压缩，都可通过 gzip 指令单独完成。
<a name="KjuPJ"></a>
#### 4.4 参数
-a 或–ascii 使用 ASCII 文字模式。<br />-c 或–stdout 或–to-stdout 把解压后的文件输出到标准输出设备。<br />-f 或-force 强行解开压缩文件，不理会文件名称或硬连接是否存在以及该文件是否为符号连接。<br />-h 或–help 在线帮助。<br />-l 或–list 列出压缩文件的相关信息。<br />-L 或–license 显示版本与版权信息。<br />-n 或–no-name 解压缩时，若压缩文件内含有远来的文件名称及时间戳记，则将其忽略不予处理。<br />-N 或–name 解压缩时，若压缩文件内含有原来的文件名称及时间戳记，则将其回存到解开的文件上。<br />-q 或–quiet 不显示警告信息。<br />-r 或–recursive 递归处理，将指定目录下的所有文件及子目录一并处理。<br />-S<压缩字尾字符串>或–suffix<压缩字尾字符串> 更改压缩字尾字符串。<br />-t 或–test 测试压缩文件是否正确无误。<br />-v 或–verbose 显示指令执行过程。<br />-V 或–version 显示版本信息。
<a name="JjQe6"></a>
### 5、gzip
<a name="rvbOl"></a>
#### 5.1 功能说明
压缩文件。
<a name="V1G6A"></a>
#### 5.2 语法
```bash
gzip [-acdfhlLnNqrtvV][-S <压缩字尾字符串>][-<压缩效率>][–best/fast][文件…] 或 gzip [-acdfhlLnNqrtvV][-S <压缩字尾字符串>][-<压缩效率>][–best/fast][目录]
```
<a name="o7egw"></a>
#### 5.3 补充说明
gzip 是个使用广泛的压缩程序，文件经它压缩过后，其名称后面会多出”.gz"的扩展名。
<a name="NxniX"></a>
#### 5.4 参数
-a 或–ascii 使用 ASCII 文字模式。<br />-c 或–stdout 或–to-stdout 把压缩后的文件输出到标准输出设备，不去更动原始文件。<br />-d 或–decompress 或----uncompress 解开压缩文件。<br />-f 或–force 强行压缩文件。不理会文件名称或硬连接是否存在以及该文件是否为符号连接。<br />-h 或–help 在线帮助。<br />-l 或–list 列出压缩文件的相关信息。<br />-L 或–license 显示版本与版权信息。<br />-n 或–no-name 压缩文件时，不保存原来的文件名称及时间戳记。<br />-N 或–name 压缩文件时，保存原来的文件名称及时间戳记。<br />-q 或–quiet 不显示警告信息。<br />-r 或–recursive 递归处理，将指定目录下的所有文件及子目录一并处理。<br />-S<压缩字尾字符串>或----suffix<压缩字尾字符串> 更改压缩字尾字符串。<br />-t 或–test 测试压缩文件是否正确无误。<br />-v 或–verbose 显示指令执行过程。<br />-V 或–version 显示版本信息。<br />-<压缩效率> 压缩效率是一个介于 1－9 的数值，预设值为"6"，指定愈大的数值，压缩效率就会愈高。<br />–best 此参数的效果和指定"-9"参数相同。<br />–fast 此参数的效果和指定"-1"参数相同。
<a name="THh35"></a>
### 6、split
<a name="Ef8Up"></a>
#### 6.1 功能说明
切割文件。
<a name="HbjI9"></a>
#### 6.2 语法
```bash
split [–help][–version][-<行数>][-b <字节>][-C <字节>][-l <行数>][要切割的文件][输出文件名]
```
<a name="Jra8B"></a>
#### 6.3 补充说明
split 可将文件切成较小的文件，预设每 1000 行会切成一个小文件。
<a name="pNWpA"></a>
#### 6.4 参数
-<行数>或-l<行数> 指定每多少行就要切成一个小文件。<br />-b<字节> 指定每多少字就要切成一个小文件。<br />-C<字节> 与-b 参数类似，但切割时尽量维持每行的完整性。<br />–help 显示帮助。<br />–version 显示版本信息。<br />[输出文件名] 设置切割后文件的前置文件名，split 会自动在前置文件名后再加上编号。
<a name="gsU5G"></a>
### 7、tar
<a name="WEBO4"></a>
#### 7.1 功能说明
备份文件。
<a name="n4wqA"></a>
#### 7.2 语法
```bash
tar [-ABcdgGhiklmMoOpPrRsStuUvwWxzZ][-b <区块数目>][-C <目的目录>][-f <备份文件>][-F <Script 文件>][-K <文件>][-L <媒体容量>][-N <日期时间>][-T <范本文件>][-V <卷册名称>][-X <范本文件>]
[-<设备编号><存储密度>][–after-date=<日期时间>] [–atime-preserve]
[–backuup=<备份方式>] [–checkpoint][–concatenate][–confirmation][–delete]
[–exclude=<范本样式>] [–force-local][–group=<群组名称>]
[–help][–ignore-failed-read][–new-volume-script=<Script 文件>]
[–newer-mtime][–no-recursion][–null][–numeric-owner][–owner=<用户名称>]
[–posix][–erve][–preserve-order][–preserve-permissions][–record-size=<区块数目>]
[–recursive-unlink][–remove-files][–rsh-command=<执行指令>]
[–same-owner][–suffix=<备份字尾字符串>][–totals][–use-compress-program=<执行指令>]
[–version][–volno-file=<编号文件>][文件或目录…]
```
<a name="oaBUk"></a>
#### 7.3 补充说明
tar 是用来建立，还原备份文件的工具程序，它可以加入，解开备份文件内的文件。
<a name="NmgrF"></a>
#### 7.4 参数
-A 或–catenate 新增温暖件到已存在的备份文件。<br />-b<区块数目>或–blocking-factor=<区块数目> 设置每笔记录的区块数目，每个区块大小为 12Bytes。<br />-B 或–read-full-records 读取数据时重设区块大小。<br />-c 或–create 建立新的备份文件。<br />-C<目的目录>或–directory=<目的目录> 切换到指定的目录。<br />-d 或–diff 或–compare 对比备份文件内和文件系统上的文件的差异。<br />-f<备份文件>或–file=<备份文件> 指定备份文件。<br />-F<Script 文件>或–info-script=<Script 文件> 每次更换磁带时，就执行指定的 Script 文件。<br />-g 或–listed-incremental 处理 GNU 格式的大量备份。<br />-G 或–incremental 处理旧的 GNU 格式的大量备份。<br />-h 或–dereference 不建立符号连接，直接复制该连接所指向的原始文件。<br />-i 或–ignore-zeros 忽略备份文件中的 0 Byte 区块，也就是 EOF。<br />-k 或–keep-old-files 解开备份文件时，不覆盖已有的文件。<br />-K<文件>或–starting-file=<文件> 从指定的文件开始还原。<br />-l 或–one-file-system 复制的文件或目录存放的文件系统，必须与 tar 指令执行时所处的文件系统相同，否则不予复制。<br />-L<媒体容量>或-tape-length=<媒体容量> 设置存放每体的容量，单位以 1024 Bytes 计算。<br />-m 或–modification-time 还原文件时，不变更文件的更改时间。<br />-M 或–multi-volume 在建立，还原备份文件或列出其中的内容时，采用多卷册模式。<br />-N<日期格式>或–newer=<日期时间> 只将较指定日期更新的文件保存到备份文件里。<br />-o 或–old-archive 或–portability 将资料写入备份文件时使用 V7 格式。<br />-O 或–stdout 把从备份文件里还原的文件输出到标准输出设备。<br />-p 或–same-permissions 用原来的文件权限还原文件。<br />-P 或–absolute-names 文件名使用绝对名称，不移除文件名称前的"/“号。<br />-r 或–append 新增文件到已存在的备份文件的结尾部分。<br />-R 或–block-number 列出每个信息在备份文件中的区块编号。<br />-s 或–same-order 还原文件的顺序和备份文件内的存放顺序相同。<br />-S 或–sparse 倘若一个文件内含大量的连续 0 字节，则将此文件存成稀疏文件。<br />-t 或–list 列出备份文件的内容。<br />-T<范本文件>或–files-from=<范本文件> 指定范本文件，其内含有一个或多个范本样式，让 tar 解开或建立符合设置条件的文件。<br />-u 或–update 仅置换较备份文件内的文件更新的文件。<br />-U 或–unlink-first 解开压缩文件还原文件之前，先解除文件的连接。<br />-v 或–verbose 显示指令执行过程。<br />-V<卷册名称>或–label=<卷册名称> 建立使用指定的卷册名称的备份文件。<br />-w 或–interactive 遭遇问题时先询问用户。<br />-W 或–verify 写入备份文件后，确认文件正确无误。<br />-x 或–extract 或–get 从备份文件中还原文件。<br />-X<范本文件>或–exclude-from=<范本文件> 指定范本文件，其内含有一个或多个范本样式，让 ar 排除符合设置条件的文件。<br />-z 或–gzip 或–ungzip 通过 gzip 指令处理备份文件。<br />-Z 或–compress 或–uncompress 通过 compress 指令处理备份文件。<br />-<设备编号><存储密度> 设置备份用的外围设备编号及存放数据的密度。<br />–after-date=<日期时间> 此参数的效果和指定”-N"参数相同。<br />–atime-preserve 不变更文件的存取时间。<br />–backup=<备份方式>或–backup 移除文件前先进行备份。<br />–checkpoint 读取备份文件时列出目录名称。<br />–concatenate 此参数的效果和指定"-A"参数相同。<br />–confirmation 此参数的效果和指定"-w"参数相同。<br />–delete 从备份文件中删除指定的文件。<br />–exclude=<范本样式> 排除符合范本样式的问家。<br />–group=<群组名称> 把加入设备文件中的文件的所属群组设成指定的群组。<br />–help 在线帮助。<br />–ignore-failed-read 忽略数据读取错误，不中断程序的执行。<br />–new-volume-script=<Script 文件> 此参数的效果和指定"-F"参数相同。<br />–newer-mtime 只保存更改过的文件。<br />–no-recursion 不做递归处理，也就是指定目录下的所有文件及子目录不予处理。<br />–null 从 null 设备读取文件名称。<br />–numeric-owner 以用户识别码及群组识别码取代用户名称和群组名称。<br />–owner=<用户名称> 把加入备份文件中的文件的拥有者设成指定的用户。<br />–posix 将数据写入备份文件时使用 POSIX 格式。<br />–preserve 此参数的效果和指定"-ps"参数相同。<br />–preserve-order 此参数的效果和指定"-A"参数相同。<br />–preserve-permissions 此参数的效果和指定"-p"参数相同。<br />–record-size=<区块数目> 此参数的效果和指定"-b"参数相同。<br />–recursive-unlink 解开压缩文件还原目录之前，先解除整个目录下所有文件的连接。<br />–remove-files 文件加入备份文件后，就将其删除。<br />–rsh-command=<执行指令> 设置要在远端主机上执行的指令，以取代 rsh 指令。<br />–same-owner 尝试以相同的文件拥有者还原问家你。<br />–suffix=<备份字尾字符串> 移除文件前先行备份。<br />–totals 备份文件建立后，列出文件大小。<br />–use-compress-program=<执行指令> 通过指定的指令处理备份文件。<br />–version 显示版本信息。<br />–volno-file=<编号文件> 使用指定文件内的编号取代预设的卷册编号。
<a name="HLHfN"></a>
### 8、unzip
<a name="hZqDD"></a>
#### 8.1 功能说明
解压缩 zip 文件
<a name="mjRPP"></a>
#### 8.2 语法
```bash
unzip [-cflptuvz][-agCjLMnoqsVX][-P <密码>][.zip 文件][文件][-d <目录>][-x <文件>] 或 unzip [-Z]
```
<a name="ogqXm"></a>
#### 8.3 补充说明
unzip 为.zip 压缩文件的解压缩程序。
<a name="UR769"></a>
#### 8.4 参数
-c 将解压缩的结果显示到屏幕上，并对字符做适当的转换。<br />-f 更新现有的文件。<br />-l 显示压缩文件内所包含的文件。<br />-p 与-c 参数类似，会将解压缩的结果显示到屏幕上，但不会执行任何的转换。<br />-t 检查压缩文件是否正确。<br />-u 与-f 参数类似，但是除了更新现有的文件外，也会将压缩文件中的其他文件解压缩到目录中。<br />-v 执行是时显示详细的信息。<br />-z 仅显示压缩文件的备注文字。<br />-a 对文本文件进行必要的字符转换。<br />-b 不要对文本文件进行字符转换。<br />-C 压缩文件中的文件名称区分大小写。<br />-j 不处理压缩文件中原有的目录路径。<br />-L 将压缩文件中的全部文件名改为小写。<br />-M 将输出结果送到 more 程序处理。<br />-n 解压缩时不要覆盖原有的文件。<br />-o 不必先询问用户，unzip 执行后覆盖原有文件。<br />-P<密码> 使用 zip 的密码选项。<br />-q 执行时不显示任何信息。<br />-s 将文件名中的空白字符转换为底线字符。<br />-V 保留 VMS 的文件版本信息。<br />-X 解压缩时同时回存文件原来的 UID/GID。<br />[.zip 文件] 指定.zip 压缩文件。<br />[文件] 指定要处理.zip 压缩文件中的哪些文件。<br />-d<目录> 指定文件解压缩后所要存储的目录。<br />-x<文件> 指定不要处理.zip 压缩文件中的哪些文件。<br />-Z unzip -Z 等于执行 zipinfo 指令。
<a name="uSt4H"></a>
### 9、zgrep
<a name="g0Vvw"></a>
#### 9.1 功能说明
这个命令的功能是在压缩文件中寻找匹配的正则表达式，用法和 grep 命令相同，只不过操作的对象是压缩文件。如果用户想看看在某个压缩文件中有没有某一句话，便可用 zgrep 命令。
<a name="neDxs"></a>
#### 9.2 语法
```bash
zgrep [options] [files]
```
[options]同 grep 对应选项。
<a name="p4Owg"></a>
### 10、zip
<a name="t0yBB"></a>
#### 10.1 功能说明
压缩文件。
<a name="HjM2y"></a>
#### 10.2 语法
```bash
zip [-options][-b <工作目录>][-ll][-n <字尾字符串>][-t <日期时间>][-<压缩效率>][压缩文件][文件…][-i <范本样式>][-x <范本样式>]
```
<a name="gguiW"></a>
#### 10.3 补充说明
zip 是个使用广泛的压缩程序，文件经它压缩后会另外产生具有".zip"扩展名的压缩文件。
<a name="FcQxo"></a>
#### 10.4 参数
-A 调整可执行的自动解压缩文件。<br />-b<工作目录> 指定暂时存放文件的目录。<br />-c 替每个被压缩的文件加上注释。<br />-d 从压缩文件内删除指定的文件。<br />-D 压缩文件内不建立目录名称。<br />-f 此参数的效果和指定"-u"参数类似，但不仅更新既有文件，如果某些文件原本不存在于压缩文件内，使用本参数会一并将其加入压缩文件中。<br />-F 尝试修复已损坏的压缩文件。<br />-g 将文件压缩后附加在既有的压缩文件之后，而非另行建立新的压缩文件。<br />-h 在线帮助。<br />-i<范本样式> 只压缩符合条件的文件。<br />-j 只保存文件名称及其内容，而不存放任何目录名称。<br />-J 删除压缩文件前面不必要的数据。<br />-k 使用 MS-DOS 兼容格式的文件名称。<br />-l 压缩文件时，把 LF 字符置换成 LF+CR 字符。<br />-ll 压缩文件时，把 LF+CR 字符置换成 LF 字符。<br />-L 显示版权信息。<br />-m 将文件压缩并加入压缩文件后，删除原始文件，即把文件移到压缩文件中。<br />-n<字尾字符串> 不压缩具有特定字尾字符串的文件。<br />-o 以压缩文件内拥有最新更改时间的文件为准，将压缩文件的更改时间设成和该文件相同。<br />-q 不显示指令执行过程。<br />-r 递归处理，将指定目录下的所有文件和子目录一并处理。<br />-S 包含系统和隐藏文件。<br />-t<日期时间> 把压缩文件的日期设成指定的日期。<br />-T 检查备份文件内的每个文件是否正确无误。<br />-u 更换较新的文件到压缩文件内。<br />-v 显示指令执行过程或显示版本信息。<br />-V 保存 VMS 操作系统的文件属性。<br />-w 在文件名称里假如版本编号，本参数仅在 VMS 操作系统下有效。<br />-x<范本样式> 压缩时排除符合条件的文件。<br />-X 不保存额外的文件属性。<br />-y 直接保存符号连接，而非该连接所指向的文件，本参数仅在 UNIX 之类的系统下有效。<br />-z 替压缩文件加上注释。<br />-$ 保存第一个被压缩文件所在磁盘的卷册名称。<br />-<压缩效率> 压缩效率是一个介于 1-9 的数值。
<a name="KOCGq"></a>
## 二、进程管理
<a name="h9vEy"></a>
### 1、crontab
<a name="ruy1O"></a>
#### 1.1 功能说明
设置计时器。
<a name="mySpS"></a>
#### 1.2 语法
```bash
crontab [-u <用户名称>][配置文件] 或 crontab [-u <用户名称>][-elr]
```
<a name="Wxai3"></a>
#### 1.3 补充说明
cron 是一个常驻服务，它提供计时器的功能，让用户在特定的时间得以执行预设的指令或程序。只要用户会编辑计时器的配置文件，就可以使用计时器的功能。其配置文件格式如下：<br />Minute Hour Day Month DayOFWeek Command
<a name="Lw6AK"></a>
#### 1.4 参数
-e 编辑该用户的计时器设置。<br />-l 列出该用户的计时器设置。<br />-r 删除该用户的计时器设置。<br />-u<用户名称> 指定要设定计时器的用户名称。
<a name="TNsCD"></a>
### 2、kill
<a name="aGxMa"></a>
#### 2.1 功能说明
删除执行中的程序或工作。
<a name="MWEAx"></a>
#### 2.2 语法
kill [-s <信息名称或编号>][程序] 或 kill [-l <信息编号>]
<a name="YuZZ9"></a>
#### 2.3 补充说明
kill 可将指定的信息送至程序。预设的信息为 SIGTERM(15)，可将指定程序终止。若仍无法终止该程序，可使用 SIGKILL(9)信息尝试强制删除程序。程序或工作的编号可利用 ps 指令或 jobs 指令查看。
<a name="VGlFg"></a>
#### 2.4 参数
-l <信息编号> 若不加<信息编号>选项，则-l 参数会列出全部的信息名称。<br />-s <信息名称或编号> 指定要送出的信息。<br />[程序] [程序]可以是程序的 PID 或是 PGID，也可以是工作编号。
<a name="DGh0m"></a>
### 3、nohup
<a name="YipTj"></a>
#### 3.1 功能说明
不挂断地运行命令。该命令可以在退出帐户/关闭终端之后继续运行相应的进程。
<a name="qGRSn"></a>
#### 3.2 语法
```bash
nohup Command [ Arg … ] [ & ]
```
nohup 命令运行由 Command 参数和任何相关的 Arg 参数指定的命令，忽略所有挂断（SIGHUP）信号。在注销后使用 nohup 命令运行后台中的程序。要运行后台中的 nohup 命令，添加&（ 表示“and”的符号）到命令的尾部。<br />无论是否将 nohup 命令的输出复位向到终端，输出都将附加到当前目录的 nohup.out 文件中。如果当前目录的 nohup.out 文件不可写，输出复位向到 $HOME/nohup.out 文件中。如果没有文件能创建或打开以用于追加，那么 Command 参数指定的命令不可调用。如果标准错误是一个终端，那么把指定的命令写给标准错误的所有输出作为标准输出复位向到相同的文件描述符。<br />退出状态：该命令返回下列出口值：<br />126 可以查找但不能调用 Command 参数指定的命令。<br />127 nohup 命令发生错误或不能查找由 Command 参数指定的命令。<br />否则，nohup 命令的退出状态是 Command 参数指定命令的退出状态。<br />如果使用nohup命令提交作业，那么在缺省情况下该作业的所有输出都被复位向到一个名为nohup.out的文件中，除非另外指定了输出文件：<br />nohup command > myout.file 2>&1 &在上面的例子中，输出被复位向到myout.file 文件中。
<a name="GmKTZ"></a>
#### 3.3 例子
有个 test.php 需要在后台运行，并且希望在后台能够定期运行，那么就使用 nohup：
```bash
nohup /root/test.php &
(nohup sh make.sh &)
```
<a name="rb0pB"></a>
### 4、ps
<a name="Cui0F"></a>
#### 4.1 功能说明
报告程序状况。
<a name="RMSXS"></a>
#### 4.2 语法
```bash
ps [-aAcdefHjlmNVwy][acefghLnrsSTuvxX][-C <指令名称>][-g <群组名称>][-G <群组识别码>][-p <程序识别码>][p <程序识别码>][-s <阶段作业>][-t <终端机编号>][t <终端机编号>][-u <用户识别码>][-U <用户识别码 >][U < 用户名称 >][-< 程序识别码>][–cols < 每列字符数 >][–columns < 每列字符数>][–cumulative][–deselect][–forest][–headers][–help][–info][–lines <显示列数>][–no-headers][–group <群组名称>][-Group <群组识别码>][–pid <程序识别码>][–rows <显示列数>][–sid <阶段作业>][–tty <终端机编号>][–user <用户名称>][–User <用户识别码>][–version][–width <每列字符数>]
```
<a name="VcbHX"></a>
#### 4.3 补充说明
ps 是用来报告程序执行状况的指令，您可以搭配 kill 指令随时中断，删除不必要的程序。<br />4.4 参数<br />-a 显示所有终端机下执行的程序，除了阶段作业领导者之外。<br />a 显示现行终端机下的所有程序，包括其他用户的程序。<br />-A 显示所有程序。<br />-c 显示 CLS 和 PRI 栏位。<br />c 列出程序时，显示每个程序真正的指令名称，而不包含路径，参数或常驻服务的标示。<br />-C<指令名称> 指定执行指令的名称，并列出该指令的程序的状况。<br />-d 显示所有程序，但不包括阶段作业领导者的程序。<br />-e 此参数的效果和指定"A"参数相同。<br />e 列出程序时，显示每个程序所使用的环境变量。<br />-f 显示 UID,PPIP,C 与 STIME 栏位。<br />f 用 ASCII 字符显示树状结构，表达程序间的相互关系。<br />-g<群组名称> 此参数的效果和指定"-G"参数相同，当亦能使用阶段作业领导者的名称来指定。<br />g 显示现行终端机下的所有程序，包括群组领导者的程序。<br />-G<群组识别码> 列出属于该群组的程序的状况，也可使用群组名称来指定。<br />h 不显示标题列。<br />-H 显示树状结构，表示程序间的相互关系。<br />-j 或 j 采用工作控制的格式显示程序状况。<br />-l 或 l 采用详细的格式来显示程序状况。<br />L 列出栏位的相关信息。<br />-m 或 m 显示所有的执行绪。<br />n 以数字来表示 USER 和 WCHAN 栏位。<br />-N 显示所有的程序，除了执行 ps 指令终端机下的程序之外。<br />-p<程序识别码> 指定程序识别码，并列出该程序的状况。<br />p<程序识别码> 此参数的效果和指定"-p"参数相同，只在列表格式方面稍有差异。<br />r 只列出现行终端机正在执行中的程序。<br />-s<阶段作业> 指定阶段作业的程序识别码，并列出隶属该阶段作业的程序的状况。<br />s 采用程序信号的格式显示程序状况。<br />S 列出程序时，包括已中断的子程序资料。<br />-t<终端机编号> 指定终端机编号，并列出属于该终端机的程序的状况。<br />t<终端机编号> 此参数的效果和指定"-t"参数相同，只在列表格式方面稍有差异。<br />-T 显示现行终端机下的所有程序。<br />-u<用户识别码> 此参数的效果和指定"-U"参数相同。<br />u 以用户为主的格式来显示程序状况。<br />-U<用户识别码> 列出属于该用户的程序的状况，也可使用用户名称来指定。<br />U<用户名称> 列出属于该用户的程序的状况。<br />v 采用虚拟内存的格式显示程序状况。<br />-V 或 V 显示版本信息。<br />-w 或 w 采用宽阔的格式来显示程序状况。<br />x 显示所有程序，不以终端机来区分。<br />X 采用旧式的 Linux i386 登陆格式显示程序状况。<br />-y 配合参数"-l"使用时，不显示 F(flag)栏位，并以 RSS 栏位取代 ADDR 栏位 。<br />-<程序识别码> 此参数的效果和指定"p"参数相同。<br />–cols<每列字符数> 设置每列的最大字符数。<br />–columns<每列字符数> 此参数的效果和指定"–cols"参数相同。<br />–cumulative 此参数的效果和指定"S"参数相同。<br />–deselect 此参数的效果和指定"-N"参数相同。<br />–forest 此参数的效果和指定"f"参数相同。<br />–headers 重复显示标题列。<br />–help 在线帮助。<br />–info 显示排错信息。<br />–lines<显示列数> 设置显示画面的列数。<br />–no-headers 此参数的效果和指定"h"参数相同，只在列表格式方面稍有差异。<br />–group<群组名称> 此参数的效果和指定"-G"参数相同。<br />–Group<群组识别码> 此参数的效果和指定"-G"参数相同。<br />–pid<程序识别码> 此参数的效果和指定"-p"参数相同。<br />–rows<显示列数> 此参数的效果和指定"–lines"参数相同。<br />–sid<阶段作业> 此参数的效果和指定"-s"参数相同。<br />–tty<终端机编号> 此参数的效果和指定"-t"参数相同。<br />–user<用户名称> 此参数的效果和指定"-U"参数相同。<br />–User<用户识别码> 此参数的效果和指定"-U"参数相同。<br />–version 此参数的效果和指定"-V"参数相同。<br />–widty<每列字符数> 此参数的效果和指定"-cols"参数相同。
<a name="G8UqY"></a>
### 5、renice
<a name="CYYZU"></a>
#### 5.1 功能说明
调整优先权。
<a name="GAyho"></a>
#### 5.2 语法
```bash
renice [优先等级][-g <程序群组名称>…][-p <程序识别码>…][-u <用户名称>…]
```
<a name="aO1a0"></a>
#### 5.3 补充说明
renice 指令可重新调整程序执行的优先权等级。预设是以程序识别码指定程序调整其优先权，亦可以指定程序群组或用户名称调整优先权等级，并修改所有隶属于该程序群组或用户的程序的优先权。等级范围从-20–19，只有系统管理者可以改变其他用户程序的优先权，也仅有系统管理者可以设置负数等级。
<a name="DuNqo"></a>
#### 5.4 参数
-g <程序群组名称> 使用程序群组名称，修改所有隶属于该程序群组的程序的优先权。<br />-p <程序识别码> 改变该程序的优先权等级，此参数为预设值。<br />-u <用户名称> 指定用户名称，修改所有隶属于该用户的程序的优先权。
<a name="CaEGH"></a>
### 6、top
<a name="NipiU"></a>
#### 6.1 功能说明
显示，管理执行中的程序。
<a name="gV36i"></a>
#### 6.2 语法
```bash
top [bciqsS][d <间隔秒数>][n <执行次数>]
```
<a name="GvMrp"></a>
#### 6.3 补充说明
执行 top 指令可显示目前正在系统中执行的程序，并通过它所提供的互动式界面，用热键加以管理。
<a name="g3Zja"></a>
#### 6.4 参数
b 使用批处理模式。<br />c 列出程序时，显示每个程序的完整指令，包括指令名称，路径和参数等相关信息。<br />d<间隔秒数> 设置 top 监控程序执行状况的间隔时间，单位以秒计算。<br />i 执行 top 指令时，忽略闲置或是已成为 Zombie 的程序。<br />n<执行次数> 设置监控信息的更新次数。<br />q 持续监控程序执行的状况。<br />s 使用保密模式，消除互动模式下的潜在危机。<br />S 使用累计模式，其效果类似 ps 指令的"-S"参数。
<a name="n8lMd"></a>
### 7、who
<a name="hijpH"></a>
#### 7.1 功能说明
显示目前登入系统的用户信息。
<a name="jSfxp"></a>
#### 7.2 语法
```bash
who [-Himqsw][–help][–version][am i][记录文件]
```
<a name="uGHrA"></a>
#### 7.3 补充说明
执行这项指令可得知目前有那些用户登入系统，单独执行 who 指令会列出登入帐号，使用的终端机，登入时间以及从何处登入或正在使用哪个 X 显示器。
<a name="laREA"></a>
#### 7.4 参数
-H 或–heading 显示各栏位的标题信息列。<br />-i 或-u 或–idle 显示闲置时间，若该用户在前一分钟之内有进行任何动作，将标示成"."号，如果该用户已超过 24 小时没有任何动作，则标示出"old"字符串。<br />-m 此参数的效果和指定"am i"字符串相同。<br />-q 或–count 只显示登入系统的帐号名称和总人数。<br />-s 此参数将忽略不予处理，仅负责解决 who 指令其他版本的兼容性问题。<br />-w 或-T 或–mesg 或–message 或–writable 显示用户的信息状态栏。<br />–help 在线帮助。<br />–version 显示版本信息。
<a name="yk19p"></a>
## 三、网络管理
<a name="EcDg6"></a>
### 1、Curl
Curl 是 Linux 下一个很强大的 http 命令行工具，其功能十分强大。
<a name="MDnI3"></a>
#### 1.1 读取网页
```bash
$ curl  http://www.linuxidc.com
```
<a name="o4rKS"></a>
#### 1.2 保存网页
```bash
$ curl  http://www.linuxidc.com > page.html
$ curl -o page.html  http://www.linuxidc.com
```
<a name="XzlhG"></a>
#### 1.3 使用的 proxy 服务器及其端口：`-x`
```bash
$ curl -x 123.45.67.89:1080 -o page.html  http://www.linuxidc.com
```
<a name="emxNZ"></a>
#### 1.4 使用 cookie 来记录 session 信息
```bash
$ curl -x 123.45.67.89:1080 -o page.html -D cookie0001.txt  http://www.linuxidc.com
```
这个 option: -D 是把 http 的 response 里面的 cookie 信息存到一个特别的文件中去，这样，当页面被存到 page.html 的同时，cookie 信息也被存到了 cookie0001.txt 里面了。
<a name="Atj5V"></a>
#### 1.5 下一次访问的时候，继续使用上次留下的 cookie 信息
使用 option 来把上次的 cookie 信息追加到 http request 里面去：-b
```bash
$ curl -x 123.45.67.89:1080 -o page1.html -D cookie0002.txt -b cookie0001.txt  http://www.linuxidc.com
```
<a name="qoLFV"></a>
#### 1.6 浏览器信息
```bash
$ curl -A “Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)” -x 123.45.67.89:1080 -o page.html -D cookie0001.txt  http://www.linuxidc.com
```
76
<a name="j11ct"></a>
#### 1.7 referer
```bash
$ curl -A “Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)” -x 123.45.67.89:1080 -e “mail.linuxidc.com” -o page.html -D cookie0001.txt  http://www.linuxidc.com
```
这样就可以骗对方的服务器，是从 mail.linuxidc.com 点击某个链接过来的
<a name="HHYuz"></a>
#### 1.8 下载文件
```bash
$ curl -o 1.jpg  http://cgi2.tky.3web.ne.jp/~zzh/screen1.JPG">http://cgi2.tky.3web.ne.jp/~zzh/screen1.JPG
$ curl -O  http://cgi2.tky.3web.ne.jp/~zzh/screen1.JPG
```
-O 可以按照服务器上的文件名，自动存在本地
```bash
$ curl -O  http://cgi2.tky.3web.ne.jp/~zzh/screen[1-10].JPG
```
<a name="hiB6w"></a>
#### 1.9 批量下载
```bash
$ curl -O  http://cgi2.tky.3web.ne.jp/~{zzh,nick}/[001-201].JPG
```
这样产生的下载，就是
```bash
~zzh/001.JPG
~zzh/002.JPG
…
~zzh/201.JPG
~nick/001.JPG
~nick/002.JPG
…
~nick/201.JPG
```
<a name="POIpv"></a>
#### 1.10 自定义文件名的下载
```bash
curl -o #2_#1.jpg  http://cgi2.tky.3web.ne.jp/~{zzh,nick}/[001-201].JPG
```
这样，自定义出来下载下来的文件名，就变成了这样：<br />原来：~zzh/001.JPG —-> 下载后：001-zzh.JPG 原来：~nick/001.JPG —-> 下载后：001-nick.JPG<br />这样一来就不怕文件重名了
<a name="cM9rX"></a>
#### 1.11 断点续传
```bash
$ curl -c -O  http://cgi2.tky.3wb.ne.jp/~zzh/screen1.JPG
```
分块下载，使用这个 option 就可以了：`-r`<br />举例说明<br />比如有一个  http://cgi2.tky.3web.ne.jp/~zzh/zhao1.MP3 要下载，就可以用这样的命令：
```bash
$ curl -r 0-10240 -o "zhao.part1" http:/cgi2.tky.3web.ne.jp/~zzh/zhao1.MP3 &
$ curl -r 10241-20480 -o "zhao.part1" http:/cgi2.tky.3web.ne.jp/~zzh/zhao1.MP3 &
$ curl -r 20481-40960 -o "zhao.part1" http:/cgi2.tky.3web.ne.jp/~zzh/zhao1.MP3 &
$ curl -r 40961- -o "zhao.part1" http:/cgi2.tky.3web.ne.jp/~zzh/zhao1.MP3
```
这样就可以分块下载了。不过需要自己把这些破碎的文件合并起来如果用 UNIX 或苹果，用 `cat zhao.part* > zhao.MP3` 就可以如果用的是 Windows，用 `copy /b` 来解决吧。
<a name="g050N"></a>
#### 1.12 浏览 FTP
```bash
$ curl -u name:passwd  ftp://ip:port/path/file
```
或者大家熟悉的
```bash
$ curl  ftp://name:passwd@ip:port/path/file
```
<a name="rS6TB"></a>
#### 1.13 FTP 上传
上传的 option 是 -T<br />比如向 ftp 传一个文件：
```bash
$ curl -T localfile -u name:passwd ftp://upload_site:port/path/
```
<a name="DCapl"></a>
#### 1.14 HTTP 上传
```bash
$ curl -T localfile  http://cgi2.tky.3web.ne.jp/~zzh/abc.cgi
```
注意，这时候，使用的协议是 HTTP 的 PUT method
<a name="let8n"></a>
#### 1.15 POST 模式读取网页
POST 模式的 option 则是 -d<br />比如，
```bash
$ curl -d "user=nickwolfe&password=12345"  http://www.linuxidc.com/login.cgi
```
<a name="UnT6d"></a>
#### 1.16 POST 模式下的文件上传
比如
```html
<form method="POST" enctype="multipar/form-data" action="http://cgi2.tky.3web.ne.jp/~zzh/up_file.cgi">
  <input type=file name=upload>
  <input type=submit name=nick value=“go”>
</form>
```
这样一个 HTTP 表单，要用 curl 进行模拟，就该是这样的语法：
```bash
$ curl -F upload=@localfile -F nick=go  http://cgi2.tky.3web.ne.jp/~zzh/up_file.cgi
```
<a name="D4270"></a>
### 2、finger
<a name="D0x7s"></a>
#### 2.1 功能说明
查找并显示用户信息。
<a name="gXyKU"></a>
#### 2.2 语法
```bash
finger [-lmsp][帐号名称…]
```
<a name="PHhU4"></a>
#### 2.3 补充说明
finger 指令会去查找，并显示指定帐号的用户相关信息，包括本地与远端主机的用户皆可，帐号名称没有大小写的差别。单独执行 finger 指令，它会显示本地主机现在所有的用户的登陆信息，包括帐号名称，真实姓名，登入终端机，闲置时间，登入时间以及地址和电话。
<a name="gxLcU"></a>
#### 2.4 参数
-l 列出该用户的帐号名称，真实姓名，用户专属目录，登入所用的 Shell，登入时间，转信地址，电子邮件状态，还有计划文件和方案文件内容。<br />-m 排除查找用户的真实姓名。<br />-s 列出该用户的帐号名称，真实姓名，登入终端机，闲置时间，登入时间以及地址和电话。<br />-p 列出该用户的帐号名称，真实姓名，用户专属目录，登入所用的 Shell，登入时间，转信地址，电子邮件状态，但不显示该用户的计划文件和方案文件内容。
<a name="ec1nD"></a>
### 3、ftp
<a name="ybwAq"></a>
#### 3.1 功能说明
设置文件系统相关功能。
<a name="cFQIy"></a>
#### 3.2 语法
```bash
ftp [-dignv][主机名称或 IP 地址]
```
<a name="Kdrhk"></a>
#### 3.3 补充说明
FTP 是 ARPANet 的标准文件传输协议，该网络就是现今 Internet 的前身。
<a name="n2ey6"></a>
#### 3.4 参数
-d 详细显示指令执行过程，便于排错或分析程序执行的情形。<br />-i 关闭互动模式，不询问任何问题。<br />-g 关闭本地主机文件名称支持特殊字符的扩充特性。<br />-n 不使用自动登陆。<br />-v 显示指令执行过程。
<a name="ZWEcE"></a>
### 4、ifconfig
<a name="F9KnY"></a>
#### 4.1 功能说明
显示或设置网络设备。
<a name="xUBER"></a>
#### 4.2 语法
```bash
ifconfig [网络设备][down up -allmulti -arp -promisc][add<地址>][del<地址>][<hw<网络设备类型><硬件地址>][io_addr<I/O地址>][irq<IRQ地址>][media<网络媒介类型>][mem_start<内存地址>][metric<数目>][mtu<字节>][netmask<子网掩码>][tunnel<地址>][-broadcast<地址>][-pointopoint<地址>][IP 地址]
```
<a name="PuPfV"></a>
#### 4.3 补充说明
ifconfig 可设置网络设备的状态，或是显示目前的设置。<br />如果提示找不到命令，可用/sbin/ifconfig。
<a name="OR9y6"></a>
#### 4.4 参数
add<地址> 设置网络设备 IPv6 的 IP 地址。<br />del<地址> 删除网络设备 IPv6 的 IP 地址。<br />down 关闭指定的网络设备。<br /><hw<网络设备类型><硬件地址> 设置网络设备的类型与硬件地址。<br />io_addr<I/O 地址> 设置网络设备的 I/O 地址。<br />irq<IRQ 地址> 设置网络设备的 IRQ。<br />media<网络媒介类型> 设置网络设备的媒介类型。<br />mem_start<内存地址> 设置网络设备在主内存所占用的起始地址。<br />metric<数目> 指定在计算数据包的转送次数时，所要加上的数目。<br />mtu<字节> 设置网络设备的 MTU。<br />netmask<子网掩码> 设置网络设备的子网掩码。<br />tunnel<地址> 建立 IPv4 与 IPv6 之间的隧道通信地址。<br />up 启动指定的网络设备。<br />-broadcast<地址> 将要送往指定地址的数据包当成广播数据包来处理。<br />-pointopoint<地址> 与指定地址的网络设备建立直接连线，此模式具有保密功能。<br />-promisc 关闭或启动指定网络设备的 promiscuous 模式。<br />[IP 地址] 指定网络设备的 IP 地址。<br />[网络设备] 指定网络设备的名称。
<a name="YvNwk"></a>
### 5、ip
<a name="GeOvP"></a>
#### 5.1 功能说明
ip 是 iproute2 软件包里面的一个强大的网络配置工具，它能够替代一些传统的网络管理工具。例如：ifconfig、<br />route 等。
<a name="PC4vV"></a>
#### 5.2 语法
```bash
ip [选项] [动作] [指令]
```
<a name="L38tD"></a>
#### 5.3 参数
动作 := { link | addr | route | rule | neigh | tunnel | maddr | mroute | monitor }<br />选项 := { -V[ersion] | -s[tatistics] | -r[esolve] |-f[amily] { inet | inet6 | ipx | dnet | link } | -o[neline] }
<a name="FpoS6"></a>
#### 5.4 装置介面 (device) 的相关设定：`ip link`
ip link 可以设定与装置 (device) 有关的相关设定，包括 MTU 以及该网路介面的 MAC 等等， 当然也可以启动 (up) 或关闭 (down) 某个网路介面！整个语法是这样的：
```bash
[root@linux ~]# ip [-s] link show <== 单纯的查阅该装置相关的资讯
[root@linux ~]# ip link set [device] [动作与参数]
```
参数：<br />show：仅显示出这个装置的相关内容，如果加上 -s 会显示更多统计数据；<br />set ：可以开始设定项目， device 指的是 eth0, eth1 等等介面代号；<br />动作与参数：包括有底下的这些动作：<br />up|down ：启动 (up) 或关闭 (down) 某个介面，其他参数使用预设的乙太网路；<br />address ：如果这个装置可以更改 MAC 的话，用这个参数修改！<br />name ：给予这个装置一个特殊的名字；<br />mtu ：就是最大传输单元！<br />范例一：显示出所有的介面资讯
```bash
[root@linux ~]# ip link show
1: lo: <LOOPBACK,UP,10000> mtu 16436 qdisc noqueue
link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: eth0: <BROADCAST,MULTICAST,UP,10000> mtu 1500 qdisc pfifo_fast qlen 1000
link/ether 00:50:fc:22:9a:cb brd ff:ff:ff:ff:ff:ff
3: sit0: <NOARP> mtu 1480 qdisc noop
link/sit 0.0.0.0 brd 0.0.0.0
[root@linux ~]# ip -s link show eth0
2: eth0: <BROADCAST,MULTICAST,UP,10000> mtu 1500 qdisc pfifo_fast qlen 1000
link/ether 00:50:fc:22:9a:cb brd ff:ff:ff:ff:ff:ff
RX: bytes packets errors dropped overrun mcast
484011792 2247372 0 0 0 0
TX: bytes packets errors dropped carrier collsns
2914104290 2867753 0 0 0 0
```
使用 ip link show 可以显示出整个装置介面的硬体相关资讯，如上所示，包括网卡位址(MAC)、MTU 等等，比较有趣的应该是那个 sit0 的介面了，那个 sit0 的介面是用在 IPv4 及 IPv6 的封包转换上的， 对于仅使用 IPv4 的网路是没有作用的。lo 及 sit0 都是主机内部所自行设定的。而如果加上 -s 的参数后，则这个网路卡的相关统计资讯就会被列出来， 包括接收 (RX) 及传送 (TX) 的封包数量等等，详细的内容与 ifconfig 所输出的结果相同的。<br />范例二：启动、关闭与设定装置的相关资讯
```bash
[root@linux ~]# ip link set eth0 up
```
启动 eth0 这个装置介面；
```bash
[root@linux ~]# ip link set eth0 down
```
关闭简单的要命～
```bash
[root@linux ~]# ip link set eth0 mtu 1000
```
更改 MTU 的值，达到 1000 bytes，单位就是 bytes ！<br />更新网路卡的 MTU 使用 ifconfig 也可以达成！没啥了不起，不过，如果是要更改『网路卡代号、 MAC位址的资讯』的话，那可就得使用 ip 罗～不过，设定前得要先关闭该网路卡，否则会不成功。如下所示：<br />范例三：修改网路卡代号、MAC 等参数
```bash
[root@linux ~]# ip link set eth0 name vbird
```
SIOCSIFNAME: Device or resource busy<br />因为该装置目前是启动的，所以不能这样做设定。应该要这样做：
```bash
[root@linux ~]# ip link set eth0 down <==关闭介面
[root@linux ~]# ip link set eth0 name vbird <==重新设定
[root@linux ~]# ip link show <==观察一下
2. vbird: <BROADCAST,MILTICASE> mtu 900 qdisc pfifo_fast qlen 1000
link/ehter 00:40:d0:13:c3:46 brd ff:ff:ff:ff:ff:ff
```
怕了吧！连网路卡代号都可以改变！不过，玩玩后记得改回来！<br />因为 ifcfg-eth0 还是使用原本的装置代号！避免有问题，要改回来
```bash
[root@linux ~]# ip link set vbird name eth0 <==介面改回来
[root@linux ~]# ip link set eth0 address aa:aa:aa:aa:aa:aa
[root@linux ~]# ip link show eth0
```
如果网路卡支援硬体位址 (MAC) 可以更改的话，那么上面这个动作就可以更改网路卡位址了！厉害吧！不过，还是那句老话，测试完之后请立刻改回来！<br />在这个装置的硬体相关资讯设定上面，包括 MTU, MAC 以及传输的模式等等，都可以在这里设定。有趣的是那个 address 的项目，那个项目後面接的可是硬体位址 (MAC) 而不是 IP 喔！很容易搞错！切记切记！更多的硬体参数可以使用 man ip 查阅一下与 ip link 有关的设定。
<a name="IvPRj"></a>
#### 5.5 关于额外的 IP 相关设定：ip address
如果说 ip link 是与 OSI 七层协定 的第二层资料连阶层有关的话，那么 ip address (ip addr) 就是与第三层网路层有关的参数！主要是在设定与 IP 有关的各项参数，包括 netmask, broadcast 等等。
```bash
[root@linux ~]# ip address show <==就是查阅 IP 参数！
[root@linux ~]# ip address [add|del] [IP 参数] [dev 装置名] [相关参数]
```
参数：<br />show ：单纯的显示出介面的 IP 资讯；<br />add|del ：进行相关参数的增加 (add) 或删除 (del) 设定，主要有：<br />IP 参数：主要就是网域的设定，例如 192.168.100.100/24 之类的设定喔；<br />dev ：这个 IP 参数所要设定的介面，例如 eth0, eth1 等等；<br />相关参数：主要有底下这些：<br />broadcast：设定广播位址，如果设定值是 + 表示『让系统自动计算』<br />label ：亦即是这个装置的别名，例如 eth0:0 就是了！<br />scope ：这个介面的领域，通常是这几个大类：<br />global ：允许来自所有来源的连线；<br />site ：仅支援 IPv6 ，仅允许本主机的连线；<br />link ：仅允许本装置自我连线；<br />host ：仅允许本主机内部的连线；<br />所以当然是使用 global 罗！预设也是 global ！<br />范例一：显示出所有的介面之 IP 参数：
```bash
[root@linux ~]# ip address show
1: lo: <LOOPBACK,UP,10000> mtu 16436 qdisc noqueue
link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
inet 127.0.0.1/8 scope host lo
inet6 ::1/128 scope host
valid_lft forever preferred_lft forever
2: eth0: <BROADCAST,MULTICAST,UP,10000> mtu 1500 qdisc pfifo_fast qlen 1000
link/ether 00:50:fc:22:9a:cb brd ff:ff:ff:ff:ff:ff
inet 192.168.1.2/24 brd 192.168.1.255 scope global eth0
inet6 fe80::250:fcff:fe22:9acb/64 scope link
valid_lft forever preferred_lft forever
3: sit0: <NOARP> mtu 1480 qdisc noop
link/sit 0.0.0.0 brd 0.0.0.0
```
看到上面那个特殊的字体吗？没错！那就是 IP 参数！也是 ip address 最主要的功能。底下进一步来新增虚拟的网路介面试看看：<br />范例二：新增一个介面，名称假设为 eth0:vbird
```bash
[root@linux ~]# ip address add 192.168.50.50/24 broadcast + \
dev eth0 label eth0:vbird
[root@linux ~]# ip address show eth0
2: eth0: mtu 1500 qdisc pfifo_fast qlen 1000
link/ether 00:40:d0:13:c3:46 brd ff:ff:ff:ff:ff:ff
inet 192.168.1.100/24 brd 192.168.1.255 scope global eth0
inet 192.168.50.50/24 brd 192.168.50.255 scope global eth0:vbird
inet6 fe80::240:d0ff:fe13:c346/64 scope link
valid_lft forever preferred_lft forever
```
看到上面的特殊字体了吧？多出了一行新的介面，且名称是 eth0:vbird<br />至于那个 `broadcast +` 也可以写成 `broadcast 192.168.50.255` ！
```bash
[root@linux ~]# ifconfig
eth0:vbir Link encap:Ethernet HWaddr 00:40:D0:13:C3:46
inet addr:192.168.50.50 Bcast:192.168.50.255 Mask:255.255.255.0
UP BROADCAST RUNNING MULTICAST MTU:1500 Metric:1
Interrupt:5 Base address:0x3e00
```
如果使用 ifconfig 就能够看到这个怪东西了！<br />范例三：将刚刚的界面删除
```bash
[root@linux ~]# ip address del 192.168.50.50/24 dev eth0
```
<a name="ixAx0"></a>
#### 5.6 关于路由的相关设定：`ip route`
`ip route` 的功能几乎与 route 这个指令差不多，但是，他还可以进行额外的参数设计，例如 MTU 的规划等等，相当的强悍！
```bash
[root@linux ~]# ip route show <==单纯的显示出路由的设定而已
[root@linux ~]# ip route [add|del] [IP 或网域] [via gateway] [dev 装置]
```
参数：<br />show ：单纯的显示出路由表，也可以使用 list ；<br />add|del ：增加 (add) 或删除 (del) 路由的意思。<br />IP 或网域：可使用 192.168.50.0/24 之类的网域或者是单纯的 IP ；<br />via ：从那个 gateway 出去，不一定需要；<br />dev ：由那个装置连出去，这就需要了！<br />mtu ：可以额外的设定 MTU 的数值喔！
<a name="fMHC4"></a>
##### 范例一：显示出目前的路由资料
```bash
[root@linux ~]# ip route show
192.168.1.0/24 dev eth0 proto kernel scope link src 192.168.1.2
169.254.0.0/16 dev eth1 scope link
default via 192.168.1.254 dev eth1
```
如上表所示，最简单的功能就是显示出目前的路由资讯，其实跟 route 这个指令相同！指示必须要注意几个小东西：<br />proto：此路由的路由协定，主要有 redirect, kernel, boot, static, ra 等， 其中 kernel 指的是直接由核心判断自动设定。<br />scope：路由的范围，主要是 link ，亦即是与本装置有关的直接连线。<br />再来看一下如何进行路由的增加与删除吧！
<a name="JAHZb"></a>
##### 范例二：增加路由，主要是本机直接可沟通的网域
```bash
[root@linux ~]# ip route add 192.168.5.0/24 dev eth0
```
针对本机直接沟通的网域设定好路由，不需要透过外部的路由器
```bash
[root@linux ~]# ip route show
192.168.5.0/24 dev eth0 scope link
…以下省略…
```
<a name="pbdQ6"></a>
##### 范例三：增加可以通往外部的路由，需透过 router ！
```bash
[root@linux ~]# ip route add 192.168.10.0/24 via 192.168.5.100 dev eth0
[root@linux ~]# ip route show
192.168.5.0/24 dev eth0 scope link
…其他省略…
192.168.10.0/24 via 192.168.5.100 dev eth0
```
仔细看，因为有 192.168.5.0/24 的路由存在 (网卡直接联系)，<br />所以才可以将 192.168.10.0/24 的路由丢给 192.168.5.100<br />那部主机来帮忙传递喔！与之前提到的 route 指令是一样的限制！
<a name="Emm4q"></a>
##### 范例四：增加预设路由
```bash
[root@linux ~]# ip route add default via 192.168.1.2 dev eth0
```
那个 192.168.1.2 就是预设路由器 (gateway) 的意思！<br />真的记得，只要一个预设路由就 OK ！
<a name="O6Acm"></a>
##### 范例五：删除路由
```bash
[root@linux ~]# ip route del 192.168.10.0/24
[root@linux ~]# ip route del 192.168.5.0/24
```
<a name="hl4g2"></a>
### 6、netstat
<a name="UBnPZ"></a>
#### 6.1 功能说明
显示网络状态。
<a name="Lks6K"></a>
#### 6.2 语法
```bash
netstat [-acCeFghilMnNoprstuvVwx][-A<网络类型>][–ip]
```
<a name="UNM1G"></a>
#### 6.3 补充说明
利用 netstat 指令可得知整个 Linux 系统的网络情况。
<a name="GwJnW"></a>
#### 6.4 参数
-a 或–all 显示所有连线中的 Socket。<br />-A<网络类型>或–<网络类型> 列出该网络类型连线中的相关地址。<br />-c 或–continuous 持续列出网络状态。<br />-C 或–cache 显示路由器配置的快取信息。<br />-e 或–extend 显示网络其他相关信息。<br />-F 或–fib 显示 FIB。<br />-g 或–groups 显示多重广播功能群组组员名单。<br />-h 或–help 在线帮助。<br />-i 或–interfaces 显示网络界面信息表单。<br />-l 或–listening 显示监控中的服务器的 Socket。<br />-M 或–masquerade 显示伪装的网络连线。<br />-n 或–numeric 直接使用 IP 地址，而不通过域名服务器。<br />-N 或–netlink 或–symbolic 显示网络硬件外围设备的符号连接名称。<br />-o 或–timers 显示计时器。<br />-p 或–programs 显示正在使用 Socket 的程序识别码和程序名称。<br />-r 或–route 显示 Routing Table。<br />-s 或–statistice 显示网络工作信息统计表。<br />-t 或–tcp 显示 TCP 传输协议的连线状况。<br />-u 或–udp 显示 UDP 传输协议的连线状况。<br />-v 或–verbose 显示指令执行过程。<br />-V 或–version 显示版本信息。<br />-w 或–raw 显示 RAW 传输协议的连线状况。<br />-x 或–unix 此参数的效果和指定"-A unix"参数相同。<br />–ip 或–inet 此参数的效果和指定"-A inet"参数相同。
<a name="iVINx"></a>
### 7、nslookup
<a name="nEZUh"></a>
#### 7.1 功能说明
nslookup 命令的功能是查询一台机器的 IP 地址和其对应的域名。使用权限所有用户。它通常需要一台域名服务器来提供域名服务。如果用户已经设置好域名服务器，就可以用这个命令查看不同主机的 IP 地址对应的域名。
<a name="SWFCb"></a>
#### 7.2 语法
```bash
nslookup ［IP 地址/域名］
```
<a name="UtHXD"></a>
#### 7.3 例子
（1）在本地计算机上使用 nslookup 命令
```bash
$ nslookup
Default Server: name.cao.com.cn
Address: 192.168.1.9
```
在符号“>”后面输入要查询的 IP 地址域名，并回车即可。如果要退出该命令，输入“exit”，并回车即可。<br />（2）使用 nslookup 命令测试 named<br />输入下面命令：
```bash
nslookup
```
然后就进入交换式 nslookup 环境。如果 named 正常启动，则 nslookup 会显示当前 DNS 服务器的地址和域名，否则表示 named 没能正常启动。<br />下面简单介绍一些基本的 DNS 诊断。

- 检查正向 DNS 解析，在 nslookup 提示符下输入带域名的主机名，如 hp712.my.com，nslookup 应能显示该主机名对应的 IP 地址。如果只输入 hp712，nslookup 会根据/etc/resolv.conf 的定义，自动添加 my.com 域名，并回答对应的 IP 地址。
- 检查反向 DNS 解析，在 nslookup 提示符下输入某个 IP 地址，如 192.22.33.20，nslookup 应能回答该 IP 地址所对应的主机名。
- 检查 MX 邮件地址记录在 nslookup 提示符下输入：set q=mx然后输入某个域名，输入 my.com 和 mail.my.com，nslookup 应能够回答对应的邮件服务器地址，即support.my.com 和 support2.my.com。
- 检查 TXT 记录：set type=txt然后输入某个域名，nslookup 会显示对应的记录。
<a name="N0aAl"></a>
###  8、ping
<a name="IJwdj"></a>
#### 8.1 功能说明
检测主机。
<a name="xdPzi"></a>
#### 8.2 语法
```bash
ping [-dfnqrRv][-c<完成次数>][-i<间隔秒数>][-I<网络界面>][-l<前置载入>][-p<范本样式>][-s<数据包大小>][-t<存活数值>][主机名称或 IP 地址]
```
<a name="Qu4Gq"></a>
#### 8.3 补充说明
执行 ping 指令会使用 ICMP 传输协议，发出要求回应的信息，若远端主机的网络功能没有问题，就会回应该信息，因而得知该主机运作正常。
<a name="lRRTR"></a>
#### 8.4 参数
-d 使用 Socket 的 SO_DEBUG 功能。<br />-c<完成次数> 设置完成要求回应的次数。<br />-f 极限检测。<br />-i<间隔秒数> 指定收发信息的间隔时间。<br />-I<网络界面> 使用指定的网络界面送出数据包。<br />-l<前置载入> 设置在送出要求信息之前，先行发出的数据包。<br />-n 只输出数值。<br />-p<范本样式> 设置填满数据包的范本样式。<br />-q 不显示指令执行过程，开头和结尾的相关信息除外。<br />-r 忽略普通的 Routing Table，直接将数据包送到远端主机上。<br />-R 记录路由过程。<br />-s<数据包大小> 设置数据包的大小。<br />-t<存活数值> 设置存活数值 TTL 的大小。<br />-v 详细显示指令的执行过程。
<a name="Ic98Q"></a>
###  9、rcp
<a name="HbBxq"></a>
#### 9.1 功能说明
远端复制文件或目录。
<a name="MUozG"></a>
#### 9.2 语法
```bash
rcp [-pr][源文件或目录][目标文件或目录] 或 rcp [-pr][源文件或目录…][目标文件]
```
<a name="GBFhQ"></a>
#### 9.3 补充说明
rcp 指令用在远端复制文件或目录，如同时指定两个以上的文件或目录，且最后的目的地是一个已经存在的目录，则它灰把前面指定的所有文件或目录复制到该目录中。
<a name="MhN11"></a>
#### 9.4 参数
-p 保留源文件或目录的属性，包括拥有者，所属群组，权限与时间。<br />-r 递归处理，将指定目录下的文件与子目录一并处理。
<a name="eFS7O"></a>
### 10、route
<a name="ky4x8"></a>
#### 10.1 功能说明
route 表示手工产生、修改和查看路由表。
<a name="NEfr7"></a>
#### 10.2 语法
```bash
route [-add][-net|-host] targetaddress [-netmask Nm][dev]If]
route [－delete][-net|-host] targetaddress [gw Gw] [-netmask Nm] [dev]If]
```
<a name="wZCPI"></a>
#### 10.3 参数
-add：增加路由。<br />-delete：删除路由。<br />-net：路由到达的是一个网络，而不是一台主机。<br />-host：路由到达的是一台主机。<br />-netmask Nm：指定路由的子网掩码。<br />gw：指定路由的网关。<br />[dev]If：强迫路由链指定接口。
<a name="aQnrt"></a>
#### 10.4 例子
```bash
route add -net 202.96.96.0 -netmask 255.255.255.0 gw 192.168.1.10 dev eth0
```
表示到 202.96.96.0/255.255.255.0 这个网络的信息通过第一块网卡发送，默认网关的地址是192.168.1.10
<a name="bc2Hv"></a>
### 11、tcpdump
<a name="AjEqZ"></a>
#### 11.1 功能说明
倾倒网络传输数据。
<a name="SfvNR"></a>
#### 11.2 语法
```bash
tcpdump [-adeflnNOpqStvx][-c<数据包数目>][-dd][-ddd][-F<表达文件>][-i<网络界面>][-r<数据包文件>][-s<数据包大小>][-tt][-T<数据包类型>][-vv][-w<数据包文件>][输出数据栏位]
```
<a name="fhZMf"></a>
#### 11.3 补充说明
执行 tcpdump 指令可列出经过指定网络界面的数据包文件头，在 Linux 操作系统中，必须是系统管理员。
<a name="T67Z9"></a>
#### 11.4 参数
-a 尝试将网络和广播地址转换成名称。<br />-c<数据包数目> 收到指定的数据包数目后，就停止进行倾倒操作。<br />-d 把编译过的数据包编码转换成可阅读的格式，并倾倒到标准输出。<br />-dd 把编译过的数据包编码转换成 C 语言的格式，并倾倒到标准输出。<br />-ddd 把编译过的数据包编码转换成十进制数字的格式，并倾倒到标准输出。<br />-e 在每列倾倒资料上显示连接层级的文件头。<br />-f 用数字显示网际网络地址。<br />-F<表达文件> 指定内含表达方式的文件。<br />-i<网络界面> 使用指定的网络截面送出数据包。<br />-l 使用标准输出列的缓冲区。<br />-n 不把主机的网络地址转换成名字。<br />-N 不列出域名。<br />-O 不将数据包编码最佳化。<br />-p 不让网络界面进入混杂模式。<br />-q 快速输出，仅列出少数的传输协议信息。<br />-r<数据包文件> 从指定的文件读取数据包数据。<br />-s<数据包大小> 设置每个数据包的大小。<br />-S 用绝对而非相对数值列出 TCP 关联数。<br />-t 在每列倾倒资料上不显示时间戳记。<br />-tt 在每列倾倒资料上显示未经格式化的时间戳记。<br />-T<数据包类型> 强制将表达方式所指定的数据包转译成设置的数据包类型。<br />-v 详细显示指令执行过程。<br />-vv 更详细显示指令执行过程。<br />-x 用十六进制字码列出数据包资料。<br />-w<数据包文件> 把数据包数据写入指定的文件。
<a name="nUsrv"></a>
### 12、telnet
<a name="WPbjO"></a>
#### 12.1 功能说明
远端登入。
<a name="EcJbq"></a>
#### 12.2 语法
```bash
telnet [-8acdEfFKLrx][-b<主机别名>][-e<脱离字符>][-k<域名>][-l<用户名称>][-n<记录文件>][-S<服务类型>][-X<认证形态>][主机名称或 IP 地址<通信端口>]
```
<a name="TPL1r"></a>
#### 12.3 补充说明
执行 telnet 指令开启终端机阶段作业，并登入远端主机。
<a name="FGCRV"></a>
#### 12.4 参数
-8 允许使用 8 位字符资料，包括输入与输出。<br />-a 尝试自动登入远端系统。<br />-b<主机别名> 使用别名指定远端主机名称。<br />-c 不读取用户专属目录里的.telnetrc 文件。<br />-d 启动排错模式。<br />-e<脱离字符> 设置脱离字符。<br />-E 滤除脱离字符。<br />-f 此参数的效果和指定"-F"参数相同。<br />-F 使用 Kerberos V5 认证时，加上此参数可把本地主机的认证数据上传到远端主机。<br />-k<域名> 使用 Kerberos 认证时，加上此参数让远端主机采用指定的领域名，而非该主机的域名。<br />-K 不自动登入远端主机。<br />-l<用户名称> 指定要登入远端主机的用户名称。<br />-L 允许输出 8 位字符资料。<br />-n<记录文件> 指定文件记录相关信息。<br />-r 使用类似 rlogin 指令的用户界面。<br />-S<服务类型> 设置 telnet 连线所需的 IP TOS 信息。<br />-x 假设主机有支持数据加密的功能，就使用它。<br />-X<认证形态> 关闭指定的认证形态。
<a name="KlLXP"></a>
### 13、traceroute
<a name="aR0Pz"></a>
#### 13.1 功能说明
显示数据包到主机间的路径。
<a name="BnGV4"></a>
#### 13.2 语法
```bash
traceroute [-dFlnrvx][-f<存活数值>][-g<网关>…][-i<网络界面>][-m<存活数 值>][-p<通信端口>][-s<来源地址>][-t<服务类型>][-w<超时秒数>][主 机名称或 IP 地址][数据包大小]
```
<a name="srwfO"></a>
#### 13.3 补充说明
traceroute 指令可以追踪网络数据包的路由途径，预设数据包大小是 40Bytes，用户可另行设置。
<a name="vRoPv"></a>
#### 13.4 参数
-d 使用 Socket 层级的排错功能。<br />-f<存活数值> 设置第一个检测数据包的存活数值 TTL 的大小。<br />-F 设置勿离断位。<br />-g<网关> 设置来源路由网关，最多可设置 8 个。<br />-i<网络界面> 使用指定的网络界面送出数据包。<br />-I 使用 ICMP 回应取代 UDP 资料信息。<br />-m<存活数值> 设置检测数据包的最大存活数值 TTL 的大小。<br />-n 直接使用 IP 地址而非主机名称。<br />-p<通信端口> 设置 UDP 传输协议的通信端口。<br />-r 忽略普通的 Routing Table，直接将数据包送到远端主机上。<br />-s<来源地址> 设置本地主机送出数据包的 IP 地址。<br />-t<服务类型> 设置检测数据包的 TOS 数值。<br />-v 详细显示指令的执行过程。<br />-w<超时秒数> 设置等待远端主机回报的时间。<br />-x 开启或关闭数据包的正确性检验。
