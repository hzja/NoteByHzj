本文参考了 IntelliJ IDEA 的官网，列举了IntelliJ IDEA（Windows 版）的所有快捷键。并在此基础上，为 90% 以上的快捷键提供了动图演示，能够直观的看到操作效果。<br />该快捷键共分 16 种，可以方便的按各类查找自己需要的快捷键~~
<a name="W3PoO"></a>
## 一、构建/编译
<a name="teWPb"></a>
### Ctrl + F9：构建项目
该快捷键，等同于菜单【Build】—>【Build Project】<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683266295287-1952e848-8083-4736-b29a-6a90eacdaf49.png#averageHue=%233f464d&clientId=ueefae3ee-f610-4&from=paste&id=qfsB0&originHeight=93&originWidth=1071&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue697f1e8-67af-42a8-b064-39872f05bea&title=)<br />执行该命令后，IntelliJ IDEA 会编译项目中所有类，并将编译结果输出到out目录中。IntelliJ IDEA 支持增量构建，会在上次构建的基础上，仅编译修改的类。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266295250-72da9e73-923f-472e-b515-9e71abf23be3.gif#averageHue=%23809677&clientId=ueefae3ee-f610-4&from=paste&id=SvYDe&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u72cef50a-934c-4289-a7d1-be8a82074b6&title=)
<a name="irqJI"></a>
### Ctrl + Shift + F9：重新编译当前类
该快捷键，等同于菜单【Build】—>【Recompile ‘class name’】<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683266295254-46630805-749a-4cc0-85c2-b39b788a10f1.png#averageHue=%233e454d&clientId=ueefae3ee-f610-4&from=paste&id=kwoSN&originHeight=99&originWidth=1072&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufe8e01c5-17b5-4af0-ae9d-abc531dd7a2&title=)<br />在IntelliJ IDEA 中打开要编译的类，执行该命令会编译当前类。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266295286-bba51243-5d28-48e4-ae0d-6d96f8d24a39.gif#averageHue=%23282729&clientId=ueefae3ee-f610-4&from=paste&id=VueEm&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u01ce1353-90c1-4891-b3fb-a98d7c79daf&title=)
<a name="RmYcF"></a>
## 二、文本编辑
<a name="eUhhO"></a>
### Ctrl + X：剪切
剪切选中文本，若未选中则剪切当前行。![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266295263-e0afbe60-b650-471b-a1cb-bf6bca4630db.gif#averageHue=%235f6f55&clientId=ueefae3ee-f610-4&from=paste&id=kyxEy&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud6ab3851-82fd-462d-ab97-5a7d338107f&title=)
<a name="sXmw2"></a>
### Ctrl + C：复制
复制选中文本，若未选中则复制当前行。
<a name="F6efn"></a>
### Ctrl + V：粘贴
<a name="BcNCi"></a>
### Ctrl + Alt + Shift + V：粘贴为纯文本
<a name="dPl2b"></a>
### Ctrl + Shift + V：从历史选择粘贴
从历史剪粘版中选择要粘贴的内容。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266295809-cf350224-91b5-40d5-914c-fa4f7495210b.gif#averageHue=%23687e5b&clientId=ueefae3ee-f610-4&from=paste&id=b5Yfg&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud7ce7faa-be23-4e0f-a5a1-884f9d55acd&title=)
<a name="X8Ymm"></a>
### Ctrl + D：复制行
复制光标所在行。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266295757-8d83664b-362d-4301-998f-dd4e748a8042.gif#averageHue=%23627655&clientId=ueefae3ee-f610-4&from=paste&id=yODjm&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4749afe9-58ba-401c-85f0-ccaa8c6ab76&title=)
<a name="I3aEi"></a>
### Ctrl + Shift + C：复制文件路径
复制选中文件所在路径。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266295861-ba2f9755-48bb-468c-9b15-1ddbba911d6d.gif#averageHue=%233b3f42&clientId=ueefae3ee-f610-4&from=paste&id=UcBQp&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9e53f56d-d74f-4188-8e4f-f8054948976&title=)
<a name="AzwG5"></a>
### Ctrl + Alt + Shift + C：复制引用
复制包的路径，或者类的名称。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266296010-f6da5dc4-2a7a-42b4-bef3-9db6ab2834d0.gif#averageHue=%233b3f42&clientId=ueefae3ee-f610-4&from=paste&id=x6dQU&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u12701640-e1b9-4db8-bd95-191d0ec9ed9&title=)
<a name="UgvMO"></a>
### Ctrl + S：保存全部
<a name="HZcvZ"></a>
### Ctrl + Z：撤销
撤销上一步操作内容。
<a name="GKFz8"></a>
### Ctrl + Shift + Z：重做
恢复上一步撤销内容。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266296248-95697f6a-953c-4553-9519-0998a56978a1.gif#averageHue=%233b3f41&clientId=ueefae3ee-f610-4&from=paste&id=ORl1K&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u027fab70-0dd2-4b0e-88bc-ae986adcc87&title=)
<a name="TQJMI"></a>
### Tab：缩进
<a name="fEIhf"></a>
### Shift + Tabl：取消缩进
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266296404-e0fc1eac-c11d-4223-80bc-3cec121bb291.gif#averageHue=%23627458&clientId=ueefae3ee-f610-4&from=paste&id=oegYG&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub1c82dec-45bd-4e43-bdc5-3c118c26720&title=)
<a name="XYXzM"></a>
### Ctrl + Alt + I：自动缩进行
自动缩进至规范位置。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266296492-cfe87954-bdae-4112-b4c4-e4ea36f1c2bc.gif#averageHue=%236c7968&clientId=ueefae3ee-f610-4&from=paste&id=jWoD2&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u450397df-4681-4070-b8d2-2979c0b0c35&title=)
<a name="W3Ijs"></a>
### Shift + Enter：开始新行
无论光标是否在行尾，都开始新的行。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266296519-d959e1e8-2a28-44b7-90de-21028620e976.gif#averageHue=%23667b5a&clientId=ueefae3ee-f610-4&from=paste&id=YW8Ru&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8419b15d-febd-4e6c-8c61-b374701e44a&title=)
<a name="fPeCV"></a>
### Ctrl + Alt + Enter：在当前行之前开始新行
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266296690-149059c4-ae62-46e5-b157-a08951f90293.gif#averageHue=%23333537&clientId=ueefae3ee-f610-4&from=paste&id=eWFSx&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u70cd29b2-9bf6-4627-aaea-a973bba0a36&title=)
<a name="V1Czv"></a>
### Ctrl + Y：删除行
删除当前行。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266296891-4ecf22e3-a270-405c-9c68-f867cb9b7456.gif#averageHue=%237e9071&clientId=ueefae3ee-f610-4&from=paste&id=og5Ux&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf2839c81-497e-412c-a12a-e47c7f9b288&title=)
<a name="Nowgw"></a>
### Ctrl + Shift + U：大小写转换
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266296962-0db0f473-adc0-4ce8-9cbd-92f4600f0391.gif#averageHue=%236b825b&clientId=ueefae3ee-f610-4&from=paste&id=f9wjw&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud86d39b8-2b31-485d-b3c9-c58f563e8c8&title=)
<a name="LPuwg"></a>
### Ctrl + Alt + Shift + Insert：创建临时文件
可以创建各种类型的临时文件，该临时文件不会保存到磁盘中。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266297011-a7b36a0e-83bd-4f41-b678-e6a3224196a5.gif#averageHue=%2366805d&clientId=ueefae3ee-f610-4&from=paste&id=pMD7g&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6e57a5f8-a47f-4891-b347-2054e11f261&title=)
<a name="DLWZj"></a>
### Shift + F4：在新窗口中打开
在新窗口打开当前文件。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266297167-47880e39-77eb-4933-97b1-159094424a26.gif#averageHue=%23697f5c&clientId=ueefae3ee-f610-4&from=paste&id=tNeCQ&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5a01f2a3-1570-4094-bc5f-4bf50707dcd&title=)
<a name="Q5Tz8"></a>
## 三、光标操作
<a name="d0wFm"></a>
### Ctrl + Left：左移一个单词
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266297176-0cf4ed66-cb1a-4483-9e84-9820e8cbfe53.gif#averageHue=%236c825d&clientId=ueefae3ee-f610-4&from=paste&id=qToeu&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u17e862eb-fba7-4e70-b041-18de44756d8&title=)
<a name="wg3o7"></a>
### Ctrl + Right：右移一个单词
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266297434-c928cd93-2abb-4289-83e5-43367b15e325.gif#averageHue=%2374866c&clientId=ueefae3ee-f610-4&from=paste&id=r0UE3&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc8eb3920-94b4-43d2-9bfe-3487ac2daa5&title=)
<a name="n41sj"></a>
### Home：移动至行首
<a name="jc0C1"></a>
### End：移动至行尾
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266297596-f250fac4-575a-49b8-832a-cd2a92e5e10a.gif#averageHue=%23464c42&clientId=ueefae3ee-f610-4&from=paste&id=Eop3A&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucaa7bf35-4731-4446-a79b-b760a8c0f5b&title=)
<a name="uDNgE"></a>
### Ctrl + Shift + M：移动至大括号
多次按下快捷键，可以在左右两个大括号间切换。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266297589-ac2ad3c4-89a0-47ce-8531-5759e96b61e4.gif#averageHue=%2371916d&clientId=ueefae3ee-f610-4&from=paste&id=oAZxu&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3865cebe-1073-43cc-9b11-d4e8b1f8ac1&title=)
<a name="QbAV6"></a>
### Ctrl + [：移动至代码块开始
<a name="NKYm6"></a>
### Ctrl + ]：移动至代码块末尾
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266297630-b7fc89c3-bd61-4531-943a-ed27077a2ce1.gif#averageHue=%237d9072&clientId=ueefae3ee-f610-4&from=paste&id=Pbd4q&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u274965d2-6467-46cd-b827-9dedaba88ec&title=)
<a name="oQh1q"></a>
### Alt + Down：下一个方法
<a name="Mzyig"></a>
### Alt + Up：上一个方法
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266298164-257c305a-2d77-4950-b67f-ba967c6d1129.gif#averageHue=%23444843&clientId=ueefae3ee-f610-4&from=paste&id=nyOVc&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u429fd355-1de8-45e5-8104-80299ee6eaf&title=)
<a name="Nf8RY"></a>
### Ctrl + PageUp：移动至页面顶部
<a name="aJH5U"></a>
### Ctrl + PageDown：移动至页面底部
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266298356-4bd8dd5a-7032-4fdd-a015-75721a26f53b.gif#averageHue=%236a8366&clientId=ueefae3ee-f610-4&from=paste&id=KwPTE&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7e1e084e-cfe4-463a-b635-57411593a6e&title=)
<a name="Q9ZMY"></a>
### PageUp：向上翻页
<a name="mHxGm"></a>
### PageDown：向下翻页
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266298336-9582e7f3-89f5-4e4b-8d50-fbfd7f6f9b58.gif#averageHue=%23444a42&clientId=ueefae3ee-f610-4&from=paste&id=SZQH6&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uaf9f9121-98fb-4e28-8dde-33038986fad&title=)
<a name="nwJtS"></a>
### Ctrl + Home：移动至文件开头
<a name="qxPMY"></a>
### Ctrl + End：移动至文件末尾
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266298341-aed2f0fc-712c-4d07-abe8-9252ad6b91fc.gif#averageHue=%237b8d70&clientId=ueefae3ee-f610-4&from=paste&id=XGUAC&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc3b7e4aa-1884-472e-a2c4-9424f34be5f&title=)
<a name="M4dmN"></a>
## 四、文本选择
<a name="dJtEW"></a>
### Ctrl + A：全选
<a name="aqRSi"></a>
### Shift + Left：向左选择
<a name="TZEBP"></a>
### Shift + Right：向右选择
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266298345-9fcbe04f-f591-4a7c-9f3d-dde111799fc6.gif#averageHue=%23323537&clientId=ueefae3ee-f610-4&from=paste&id=s1cUg&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufe51f690-3779-48a4-9702-b86156c03d3&title=)
<a name="NFW6w"></a>
### Ctrl + Shift + Left：向左选择一个单词
<a name="TBiUu"></a>
### Ctrl + Shift + Right：向右选择一个单词
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266298732-6757eb6d-d254-4693-bf14-f820284dac30.gif#averageHue=%235d6f53&clientId=ueefae3ee-f610-4&from=paste&id=TxZB7&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u10857907-1740-4218-8193-33a189ce5e1&title=)
<a name="TyW96"></a>
### Shift + Home：向左选择至行头
<a name="QZXRJ"></a>
### Shift + End：向右选择至行尾
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266298790-e038607b-775a-4dab-81ad-a20615209e54.gif#averageHue=%236e8066&clientId=ueefae3ee-f610-4&from=paste&id=R2zPF&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3895b567-d725-4286-a175-1dd295d6198&title=)
<a name="Llk4f"></a>
### Shift + Up：向上选择
<a name="HDgmt"></a>
### Shift + Down：向下选择
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266298824-28715584-22e0-4205-ab37-68ae562a7c34.gif#averageHue=%233b4043&clientId=ueefae3ee-f610-4&from=paste&id=slfo2&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u62b73ae4-437a-48cc-9176-907446d1611&title=)
<a name="ubJLK"></a>
### Ctrl + Shift + [：选择至代码块开头
<a name="zCseg"></a>
### Ctrl + Shift + ]：选择至代码块结尾
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266298939-0cceb79e-3c28-4c23-be10-e989f4a760a8.gif#averageHue=%23333537&clientId=ueefae3ee-f610-4&from=paste&id=ZzHrR&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua7dd4730-769e-4312-8b3a-bef1e1b4a4a&title=)
<a name="TTHH8"></a>
### Ctrl + Shift + PageUp：选择至页面顶部
<a name="eC6PJ"></a>
### Ctrl + Shift + PageDown：选择至页面底部
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266299053-8ab95b99-3a4f-44c9-b459-b19708dea0f3.gif#averageHue=%2394a590&clientId=ueefae3ee-f610-4&from=paste&id=MZECk&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8c5d848f-51ce-4cc1-b019-e08ae9bee03&title=)
<a name="hdPvr"></a>
### Shift + PageUp：向上翻页选择
<a name="RYhdf"></a>
### Shift + PageDown：向下翻页选择
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266299156-e164bcf0-8723-4da4-b7ac-929a53218061.gif#averageHue=%233a3e41&clientId=ueefae3ee-f610-4&from=paste&id=pya2Y&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua519be82-11b6-4154-8cbe-19021d23e34&title=)
<a name="oCwdQ"></a>
### Ctrl + Shift + Home：选择至文件开关
<a name="x3pt4"></a>
### Ctrl + Shift + End：选择至文件结尾
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266299280-b6b5edc0-2f98-40b2-8290-d1636fbf7cf0.gif#averageHue=%2399a896&clientId=ueefae3ee-f610-4&from=paste&id=ONgs5&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uefbe2a91-3619-4526-9fe5-7438c47fa27&title=)
<a name="BCDwp"></a>
### Ctrl + W：扩展选择
<a name="uiGEp"></a>
### Ctrl + Shift + W：收缩选择
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266299632-e84754c8-f855-4ad1-bb50-29fff6faa674.gif#averageHue=%23444a41&clientId=ueefae3ee-f610-4&from=paste&id=E3WTI&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u550a929f-277e-4eff-a3f6-e2abc39bc9f&title=)
<a name="WZphR"></a>
## 五、代码折叠
<a name="FSwRM"></a>
### Ctrl + NumPad+：展开代码块
<a name="jE9zQ"></a>
### Ctrl + NumPad-：折叠代码块
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266299321-550af56f-1ef1-46ba-98f5-fc894b3006c1.gif#averageHue=%23788c70&clientId=ueefae3ee-f610-4&from=paste&id=DsXLj&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u642ed508-950d-4849-bf7c-b18317ef163&title=)
<a name="viido"></a>
### Ctrl + Alt + NumPad+：递归展开
<a name="iXQSI"></a>
### Ctrl + Alt + NumPad-：递归折叠
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266299442-f45a98ab-e852-4f3a-9fe3-dc18c4a39ad3.gif#averageHue=%233a4043&clientId=ueefae3ee-f610-4&from=paste&id=SRm2p&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u148aff91-333e-40b4-a57b-a746526f705&title=)
<a name="pwXYC"></a>
### Ctrl + Shift + NumPad+：全部展开
<a name="AAJCY"></a>
### Ctrl + Shift + NumPad-：全部折叠
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266299683-dc757a74-5b56-4f4f-bba7-0c0702dd3fcb.gif#averageHue=%238f9d8c&clientId=ueefae3ee-f610-4&from=paste&id=Ur6cK&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u953e0a47-873f-4ab6-aaff-ce88643c68d&title=)
<a name="VXtcl"></a>
### Ctrl + .：折叠选择
<a name="tAHgU"></a>
## 六、多个插入符号和范围选择
<a name="azpWk"></a>
### Alt + Shift + Click：添加/删除插入符号
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266299728-ca864cc8-eac8-45fd-9a72-dbedc0b45406.gif#averageHue=%233c4043&clientId=ueefae3ee-f610-4&from=paste&id=kiXIt&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1a57d788-50f6-4911-b321-12885e7099c&title=)
<a name="YgSfH"></a>
### Alt + Shift + Insert：切换列选择模式
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266299776-97185b51-ca92-4743-80dd-b0f265455d88.gif#averageHue=%23444941&clientId=ueefae3ee-f610-4&from=paste&id=g48qp&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf25f1ce6-3680-4d5d-b1a0-9d90cf29e70&title=)
<a name="Xu4qM"></a>
### 双击Ctrl + Up：向上克隆插入符号
按Ctrl键两次，然后在不松开的情况下按向上箭头键。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266299994-c0f8a966-f87c-4586-9f7e-6afd4551f42e.gif#averageHue=%23809578&clientId=ueefae3ee-f610-4&from=paste&id=dFSir&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud8323e2d-bcba-4fdf-acf4-72b8936b4a6&title=)
<a name="hHeEr"></a>
### 双击Ctrl + Down：向下克隆插入符号
按Ctrl键两次，然后在不松开的情况下按向下箭头键。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266300090-60efa10e-c285-46c5-8466-6433cf18c745.gif#averageHue=%23333537&clientId=ueefae3ee-f610-4&from=paste&id=pEmns&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u979e6eb9-cce3-4132-92a1-e652b52b60c&title=)
<a name="cDBKX"></a>
### Alt + Shift + G：将插入符号添加到选择中的每一行
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266300133-6e68ccfe-a9e8-4598-b1fc-f9f9352a4252.gif#averageHue=%237a9373&clientId=ueefae3ee-f610-4&from=paste&id=pNKOS&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u042a349d-b442-431f-9230-fe32e865dca&title=)
<a name="u5pET"></a>
### Alt + J：选择单位下次出现的位置
<a name="POYDa"></a>
### Alt + Shift + J：取消最后一次选择
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266300310-da082b41-615a-4c40-b5cd-5b40a7ecbdfb.gif#averageHue=%2373866e&clientId=ueefae3ee-f610-4&from=paste&id=zyijW&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubc2e064d-26fb-4229-b79f-1402e749585&title=)
<a name="zC1Kh"></a>
### Ctrl + Alt + Shift + J：选择所有出现的位置
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266300311-1c06e35d-ca95-4cac-87c6-2fa43d00b460.gif#averageHue=%23333536&clientId=ueefae3ee-f610-4&from=paste&id=geHwl&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udef33582-49c3-479d-82dd-7c55f4c10bb&title=)
<a name="WRwoz"></a>
### Alt + Shift + Middle-Click：创建矩形选择
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266300464-12011380-d8ce-48c3-b333-8df2df4ba3de.gif#averageHue=%237e8f75&clientId=ueefae3ee-f610-4&from=paste&id=gecEA&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7ef04f14-4ed6-4c6e-b2a9-a3e8dcd2a1c&title=)
<a name="xtZSi"></a>
### Alt + Click：拖拽以创建矩形选择区
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266300569-1b38743a-7a00-4495-8aac-ee2318e37bf9.gif#averageHue=%236d8368&clientId=ueefae3ee-f610-4&from=paste&id=cwayR&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9f572224-9afe-459d-b9e1-47ff67bc3d0&title=)
<a name="GtHka"></a>
### Ctrl + Alt + Shift + Click：拖拽以创建多个矩形选择区
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266300595-b38af738-ce8b-45d6-bfb3-73a6759957be.gif#averageHue=%233d4043&clientId=ueefae3ee-f610-4&from=paste&id=wH9Gg&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u62476359-98d1-4010-860c-368bb83a570&title=)
<a name="wlCfr"></a>
## 七、辅助编码
<a name="ljpwc"></a>
### Alt + Enter：显示建议操作
该快捷键又称为“万通快捷键”，它会根据不同的语境建议不同的操作。下面这个演示只是其中的一种，还有很多种用法，你可以尝试一下。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266300798-d078230b-fdb9-46b8-a40d-9b847e217903.gif#averageHue=%233b3e42&clientId=ueefae3ee-f610-4&from=paste&id=LOY7d&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud12dfb28-1ef0-4bc1-974c-128940d2078&title=)
<a name="X7Y6B"></a>
### Ctrl + Space：代码补全
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266300817-f8daf4d7-f1d2-4b14-93d5-a25c98d6d523.gif#averageHue=%23899988&clientId=ueefae3ee-f610-4&from=paste&id=FjDMM&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4cbea3d3-3d23-4970-a0f2-b1dbe990176&title=)
<a name="m2D9i"></a>
### Ctrl + Shift + Space：类型匹配代码补全
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266300941-099b9074-8db5-4d4a-9327-a263bf09cb26.gif#averageHue=%238f9e88&clientId=ueefae3ee-f610-4&from=paste&id=AVNYm&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2c2db2f9-0dc3-46eb-aa89-c5e3d1bff8b&title=)
<a name="nQ8Pp"></a>
### Ctrl + Alt + Space：第二次代码补全
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266301185-ff53c9d5-cc25-45d1-b0a3-019a9550e7a1.gif#averageHue=%23333537&clientId=ueefae3ee-f610-4&from=paste&id=Mg2K8&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf0b3f274-ad36-4dce-9322-637a5b57927&title=)
<a name="s6Q96"></a>
### Ctrl + Shift + Enter：补全当前语句
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266301209-c137769a-48f1-4653-8a39-19f9dce6161f.gif#averageHue=%235b6c57&clientId=ueefae3ee-f610-4&from=paste&id=TTbHs&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u83d9d881-4dbb-439d-a2d0-7188422ea85&title=)
<a name="fg8bJ"></a>
### Ctrl + Alt + L：格式化代码
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266301237-42b1120b-46d3-4ac4-b528-879155b817fe.gif#averageHue=%236a815d&clientId=ueefae3ee-f610-4&from=paste&id=C0Jrp&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub99ba4ac-2019-4b81-9314-e26f1dc26af&title=)
<a name="o1Dbb"></a>
### Ctrl + P：参数信息提醒
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266301231-66d71d17-c874-4a7c-bbce-df8a7b27f0b0.gif#averageHue=%233c3f43&clientId=ueefae3ee-f610-4&from=paste&id=LXSV0&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue1bd2181-256c-45fc-8761-8e1e972d5b8&title=)
<a name="kpxPQ"></a>
### Ctrl + Q：快速文档
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266301381-25bdfaec-e90d-478f-af23-c3f83e481491.gif#averageHue=%23333637&clientId=ueefae3ee-f610-4&from=paste&id=wTTEh&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u191d6dfc-09a5-4e66-b7f5-ffe6c4c1c38&title=)
<a name="EoEk2"></a>
### Ctrl + Shift + Up：向上移动语句
<a name="KnKAy"></a>
### Ctrl + Shift + Down：向下移动语句
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266301585-825a6ec9-5f31-4e1b-a00a-ebdb23299133.gif#averageHue=%236f7f6a&clientId=ueefae3ee-f610-4&from=paste&id=Avgth&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufeed6b32-664b-438a-a90e-cbf2543a68f&title=)
<a name="gQlYE"></a>
### Ctrl + Alt + Shift + Left：向左移动元素
<a name="MXCaZ"></a>
### Ctrl + Alt + Shift + Right：向右移动元素
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266301596-fdfe643a-b94f-4b64-8b56-1209328a2ca5.gif#averageHue=%23474c44&clientId=ueefae3ee-f610-4&from=paste&id=Ptp4X&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3b299db0-b581-4122-abfc-3d42afd7c93&title=)
<a name="mNyUU"></a>
### Alt + Shift + Up：向上移动队列
<a name="px7kW"></a>
### Alt + Shift + Down：向下移动队列
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266301677-1ec2b38f-0737-46c8-9e7f-8d8a55077934.gif#averageHue=%235f7158&clientId=ueefae3ee-f610-4&from=paste&id=zcKnc&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua432e987-f848-4c6f-b926-142e5b866fc&title=)
<a name="QY7DR"></a>
### Ctrl + /：添加行注释
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266301669-8797d7d3-7fce-4f5e-a095-61e35320701c.gif#averageHue=%23708a66&clientId=ueefae3ee-f610-4&from=paste&id=dhGaL&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u18d3b810-db13-435e-a1e9-93f26353f28&title=)
<a name="BLqbC"></a>
### Ctrl + Shift + /：添加块注释
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266301860-881cefd1-4e0f-4663-ac61-c294774ce4a8.gif#averageHue=%23779a6f&clientId=ueefae3ee-f610-4&from=paste&id=i9XIh&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uba30c166-8fe4-40eb-bfec-307c5255e9a&title=)
<a name="f3BFA"></a>
### Alt + Insert：生产语句
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266302124-ecf20cb4-317f-4998-9ad8-a6b5a30fd762.gif#averageHue=%233c4038&clientId=ueefae3ee-f610-4&from=paste&id=xVhW4&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u407226af-7e71-4538-b016-042e9c69d8d&title=)
<a name="GYpRZ"></a>
## 八、上下文导航
<a name="BXUkG"></a>
### Alt + Down：跳转至下一个方法
<a name="rNXsg"></a>
### Alt + Up：跳转至上一个方法
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266302053-3e9b569d-cd89-4231-a8b1-3b09bbbe9a0f.gif#averageHue=%23606f5d&clientId=ueefae3ee-f610-4&from=paste&id=gkW59&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udefd8a77-8a8a-4ed4-9058-120d5ad695f&title=)
<a name="El3K2"></a>
### Ctrl + G：跳转到指定行
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266302074-646be63b-76d4-428a-928c-8e1c9fc18d74.gif#averageHue=%237c9377&clientId=ueefae3ee-f610-4&from=paste&id=XEXZt&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3a264759-776f-4db3-ba6a-16137e26600&title=)
<a name="KhFTv"></a>
### Ctrl + Tab：切换活动文件
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266302187-bfb0319f-b1d0-4b4b-bb04-a9058c25fc06.gif#averageHue=%238f9e8f&clientId=ueefae3ee-f610-4&from=paste&id=ewrYp&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u77617d94-e71c-4333-9781-06b7d559bf6&title=)
<a name="gSqXL"></a>
### Alt + F1：选择文件的定位
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266302315-bf42bf08-d4b3-4f97-a719-e040c9fc2e52.gif#averageHue=%233f423f&clientId=ueefae3ee-f610-4&from=paste&id=tAgVo&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u009cd6ea-4cac-4caa-9dab-be9cce76409&title=)
<a name="lO2H4"></a>
### Ctrl + E：最近的文件
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266302608-69011ad0-e21c-4feb-8585-ddf6d9eafd5b.gif#averageHue=%23333637&clientId=ueefae3ee-f610-4&from=paste&id=BzlVz&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6d896bc8-604b-405c-bdd2-2fd28115205&title=)
<a name="sqp2g"></a>
### Ctrl + Shift + Backspace：返回上次编辑位置
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266302675-287a70f9-7d0c-445b-80c6-bae3e20c08d7.gif#averageHue=%23323537&clientId=ueefae3ee-f610-4&from=paste&id=PYGFX&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufc614f6c-0f8e-4ade-9db3-b210f3a1dda&title=)
<a name="VZfLI"></a>
### Ctrl + Alt + Left：后退
<a name="WIl3q"></a>
### Ctrl + Alt + Right：前进
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266302726-4d93370a-beca-4bd1-a954-18faf2b92f55.gif#averageHue=%23323537&clientId=ueefae3ee-f610-4&from=paste&id=GnAOR&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud421f47e-86cc-411d-a750-728302ad3ea&title=)
<a name="E9Lo5"></a>
### Ctrl + Alt + Down：下一事件
<a name="PJlY0"></a>
### Ctrl + Alt + Up：上一事件
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266302671-7b26d339-cd9a-4548-b490-5319b72e4190.gif#averageHue=%235e785c&clientId=ueefae3ee-f610-4&from=paste&id=BzoxA&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u28754802-ad05-4134-b385-1c6956b5f4f&title=)
<a name="Gyj9W"></a>
### Alt + Right：选择下一个选项卡
<a name="R987n"></a>
### Alt + Left：选择下一个选项卡
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266302857-cd1bf917-b958-4958-828d-02f956376619.gif#averageHue=%236b7d68&clientId=ueefae3ee-f610-4&from=paste&id=nK4La&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub508063a-b733-43c6-a333-1d7ced12ecd&title=)
<a name="Q6WA0"></a>
### F11：切换匿名书签
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266303071-f2dac1c4-d6d3-4696-a75c-560c1ef31ed6.gif#averageHue=%2392a08f&clientId=ueefae3ee-f610-4&from=paste&id=JiFth&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8fd46b2f-4193-4b16-a1df-8ff495c3e51&title=)
<a name="uKjZB"></a>
### Ctrl + Shift + [digit]：用数字切换书签
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266303091-4819c7ed-7409-4128-aa65-61120978067f.gif#averageHue=%235d635b&clientId=ueefae3ee-f610-4&from=paste&id=eRdMz&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf829e00b-e918-4575-a7df-c50e0bbb998&title=)
<a name="Imf3Z"></a>
### Ctrl + F11：使用助词符切换书签
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266371805-a70b6bb2-471d-4c2c-b566-6196b7d2c46a.gif#averageHue=%233b4044&clientId=ueefae3ee-f610-4&from=paste&id=LcJDl&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufe35e5b6-b66f-461c-8abf-8a26926d4da&title=)
<a name="aGRiR"></a>
### Shift + F11：显示所有书签
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266371980-a41aa091-6050-4458-8899-74acf025e066.gif#averageHue=%235e635b&clientId=ueefae3ee-f610-4&from=paste&id=w5AEv&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua850f632-11b1-4321-a043-ecfd9f41301&title=)
<a name="uDQZl"></a>
### Ctrl + [digit]：用数字跳转到书签
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266371863-52b4b1f1-43de-46d8-a2cd-f4317c8ac6d4.gif#averageHue=%23434642&clientId=ueefae3ee-f610-4&from=paste&id=oiQ5Y&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf04ea568-b3c9-4b9e-8267-512114a7ead&title=)
<a name="kPZcH"></a>
### Alt + 7：显示结构窗口
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266371836-36472c7f-ef2f-495d-93ad-a25765edf4eb.gif#averageHue=%23333637&clientId=ueefae3ee-f610-4&from=paste&id=EVXo4&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufc7456e2-2a8b-4306-a99a-d0ca5ec4c4d&title=)
<a name="RI2ij"></a>
### Alt + 3：显示查找窗口
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266371950-d9345636-b8cf-4396-b8a7-9ed68db5cf01.gif#averageHue=%23688269&clientId=ueefae3ee-f610-4&from=paste&id=muYoz&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u997c4e67-d926-4e39-8130-4dee640071b&title=)
<a name="UvAsc"></a>
## 九、查找操作
<a name="UTCwq"></a>
### 双击Shift：查找所有
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266373111-85122e3e-d52f-4143-b438-919880f80eea.gif#averageHue=%23474c44&clientId=ueefae3ee-f610-4&from=paste&id=DSv3v&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc9cff87a-b336-4acc-b81a-ce647233c8d&title=)
<a name="hbhhF"></a>
### Ctrl + F：查找字符（当前文件）
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266373354-a5d3305c-7269-4be3-9ae9-cdb639cf6c80.gif#averageHue=%237d8f75&clientId=ueefae3ee-f610-4&from=paste&id=fvRAe&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u76dfbef2-e7da-4f6b-837b-a8d37768f6e&title=)
<a name="NynrF"></a>
### F3：查找下一个
<a name="uPHBq"></a>
### Shift + F3：查找上一个
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266373364-0cfa3eec-028f-4ef4-8131-2d9eda7a562a.gif#averageHue=%23496a4b&clientId=ueefae3ee-f610-4&from=paste&id=j7Mbm&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2fe4cd5b-5cbc-449e-a280-adc06ca4f5a&title=)
<a name="Rb4WK"></a>
### Ctrl + R：替换字符（当前文件）
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266373385-d2d6b583-3bb6-4a25-80cf-a99a8ca9f817.gif#averageHue=%23768a6c&clientId=ueefae3ee-f610-4&from=paste&id=K5NpL&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub02419bc-e3ab-4690-941a-1b1e9d69675&title=)
<a name="mJJUj"></a>
### Ctrl + Shift + F：查找字符（所有文件）
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266373410-a6c7ba8c-f2ca-4c64-8675-4793eb215264.gif#averageHue=%23687559&clientId=ueefae3ee-f610-4&from=paste&id=rFRMJ&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5602c486-6dd7-4b44-80f6-2b312e7e177&title=)
<a name="MgqqU"></a>
### Ctrl + Shift + R：替换字符（所有文件）
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266373640-ed744911-bab5-4012-bbd0-1680ec43201b.gif#averageHue=%2369765b&clientId=ueefae3ee-f610-4&from=paste&id=knOKE&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1752e86d-6f70-4533-8019-6d07507637b&title=)
<a name="jtVjD"></a>
### Ctrl + F3：跳转到光标处单词的下一位置
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266374221-f2315ac4-5094-4754-90ed-78409bc73f21.gif#averageHue=%23587558&clientId=ueefae3ee-f610-4&from=paste&id=IuJT6&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4296677a-37ef-4ed0-ac87-e48981bc2a2&title=)
<a name="MehvU"></a>
### Ctrl + Shift + N：查找文件并跳转
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266374375-3cc5fc02-d2c1-48ab-9dc2-7705206bbf42.gif#averageHue=%23333537&clientId=ueefae3ee-f610-4&from=paste&id=nWvaT&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u26e17799-2b5f-4e74-b6fb-c228358cb72&title=)
<a name="PKGSw"></a>
### Ctrl + F12：打开文件结构
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266374380-caec03a9-a862-41cc-a225-05687252d71d.gif#averageHue=%23333638&clientId=ueefae3ee-f610-4&from=paste&id=JCBeT&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uac0eb1fd-9753-4de0-a24e-f4ce816a100&title=)
<a name="UYH4R"></a>
### Ctrl + Alt + Shift + N：查找符号（变量、方法等）
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266374409-71bf74d4-db90-49f6-bdaf-396a21784f3e.gif#averageHue=%23333637&clientId=ueefae3ee-f610-4&from=paste&id=jNV2Z&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u33583fcf-1af3-4374-b8f2-75e473bc743&title=)
<a name="zBIXQ"></a>
### Ctrl + Shift + A：查找动作
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266374656-7706b066-a2ae-4d01-8bd3-d3e073782117.gif#averageHue=%23688463&clientId=ueefae3ee-f610-4&from=paste&id=qKhdE&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0109897e-446d-41e8-99d4-b4b87db1aed&title=)
<a name="DTN1L"></a>
## 十、符号导航
<a name="KDYyD"></a>
### Alt + F7：查找用法
<a name="yGmfK"></a>
### Ctrl + B：跳转到声明处
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266374947-517d3b8f-584c-4339-ba8f-7735cdc260cb.gif#averageHue=%23727e6f&clientId=ueefae3ee-f610-4&from=paste&id=rdW74&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubb8ca6c8-f044-4fd4-93d5-57a4a6c4a35&title=)
<a name="Fvf9N"></a>
### Ctrl + Shift + B：跳转到声明类处
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266375068-5cea4f4d-14e3-4782-85fe-04bd1cda2a0e.gif#averageHue=%236b7d61&clientId=ueefae3ee-f610-4&from=paste&id=j1oPH&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u95ddbb11-ba19-4721-bd57-00ef72b7701&title=)
<a name="c2unL"></a>
### Ctrl + Alt + F7：显示用法
<a name="uHCdc"></a>
### Ctrl + U：跳转到超级方法
<a name="UW5Zo"></a>
### Ctrl + Alt + B：跳转到实现方法
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266375136-5ab60482-2e25-40e3-87d3-7ae1d35802c7.gif#averageHue=%233b3f42&clientId=ueefae3ee-f610-4&from=paste&id=UsaTW&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u55fae479-7fc4-4242-a5be-3fd0959cfe4&title=)
<a name="qPZao"></a>
### Ctrl + Shift + F7：突出显示文件中的用法
<a name="hjybW"></a>
## 十一、代码分析
<a name="BnGoV"></a>
### Alt + Enter：显示意图操作
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266375154-50f9f98e-0669-4764-b175-90477c41fb0b.gif#averageHue=%23697559&clientId=ueefae3ee-f610-4&from=paste&id=Gr9nf&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2b48b6d1-f87b-4dd0-b4a5-38fe2ad69a2&title=)
<a name="khCFo"></a>
### Ctrl + F1：显示错误描述
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266375329-d299b7d5-43e4-4c3b-81c2-0b1562d8e38b.gif#averageHue=%235b7153&clientId=ueefae3ee-f610-4&from=paste&id=SrwQR&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6ffa1960-b7d5-4eb4-9dbb-64bb02df3a2&title=)
<a name="YkZ6i"></a>
### F2：下一个突出显示的错误
<a name="Y1rqS"></a>
### Shift + F2：上一个突出显示的错误
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266375743-c9d41dbe-49e1-496e-92c7-a834c63bb50f.gif#averageHue=%23778e71&clientId=ueefae3ee-f610-4&from=paste&id=YG8wZ&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6e55e6fa-2315-4e31-9ace-066819ad730&title=)
<a name="mbQGK"></a>
### Ctrl + Alt + Shift + I：按名称运行检查
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266375807-6e15ecbc-6205-4522-9f74-0086477425ed.gif#averageHue=%238e9f8e&clientId=ueefae3ee-f610-4&from=paste&id=UyLNa&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u990fad79-b390-48e4-805b-7eb8d624417&title=)
<a name="o902E"></a>
### Alt + 6：显示问题窗口
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266375820-0c987fbf-08ad-43c3-9bec-a58a243fd15d.gif#averageHue=%23333637&clientId=ueefae3ee-f610-4&from=paste&id=js7LU&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue5d130e4-7805-4a03-b45f-5cd378fc7a8&title=)
<a name="edWfz"></a>
### Ctrl + H：查看类的继承结构
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693755150857-ca567f11-663e-4733-87f0-f5b78f316f3a.png#averageHue=%23f9f8f6&clientId=u1e00c1e7-a39c-4&from=paste&height=802&id=ykTm5&originHeight=1203&originWidth=3033&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=358265&status=done&style=none&taskId=u0daa93a6-22fa-42d4-a2f8-c20103fdeac&title=&width=2022)
<a name="t5ElW"></a>
## 十二、运行和调试
<a name="RZRDk"></a>
### 双击Ctrl：运行所有
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266375826-b0a696de-dbee-48af-9469-055a2f3ab6d4.gif#averageHue=%23333537&clientId=ueefae3ee-f610-4&from=paste&id=E9nbg&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1619c564-5c4b-4932-b12f-59b9658afab&title=)
<a name="gON9n"></a>
### Shift + F10：运行上下文配置
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266376122-88fd7e12-24f7-44a2-a494-8fcda3a04859.gif#averageHue=%23657b59&clientId=ueefae3ee-f610-4&from=paste&id=NwWJ1&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue844c774-1bf3-439c-a903-5ba765275f6&title=)
<a name="uFVus"></a>
### Alt + Shift + F10：打开运行窗口
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266376279-e038fd88-9102-40a2-be44-91c0a80e510e.gif#averageHue=%233a3e41&clientId=ueefae3ee-f610-4&from=paste&id=xhs0R&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf1ee4352-c342-4f32-b8ff-fb278cfc1e5&title=)
<a name="Rp7FC"></a>
### Shift + F9：调试上下文配置
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266376428-61b2e686-c65c-487f-96a7-84ac6bfe286a.gif#averageHue=%236b845d&clientId=ueefae3ee-f610-4&from=paste&id=ygd25&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uea00e8d0-9204-4dfc-9cc5-7aa192c58b1&title=)
<a name="nEWfG"></a>
### Alt + Shift + F9：打开调试窗口
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266376593-4b82c56a-bfbc-42ba-ad94-26a6807c7082.gif#averageHue=%2378886d&clientId=ueefae3ee-f610-4&from=paste&id=vvS4m&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub06a906a-e945-4b69-8ac0-2973d0e255d&title=)
<a name="XGVcT"></a>
### Ctrl + Alt + F5：附加到进程
<a name="NagIF"></a>
### Ctrl + F2：停止
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266376602-0397a3aa-86e0-44e0-9613-ee39249a4edd.gif#averageHue=%2395a58f&clientId=ueefae3ee-f610-4&from=paste&id=r1jUN&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf7b4e712-b368-415e-8cbc-b38a58378e4&title=)
<a name="EtdAo"></a>
### F9：运行至下一断点
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266376884-2407d568-edae-4157-b733-a23921b2a455.gif#averageHue=%234b5656&clientId=ueefae3ee-f610-4&from=paste&id=N7OY2&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u44016b1f-0683-4d53-97b2-94b332b5a70&title=)
<a name="yAo8P"></a>
### Ctrl + Shift + F2：停止后台进程
<a name="Y15dF"></a>
### F8：跨过调用
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266376978-706979a3-9156-4cc8-b87c-50ef9a255ae0.gif#averageHue=%23617367&clientId=ueefae3ee-f610-4&from=paste&id=DHz7c&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubbd0c3ba-2d4f-4755-bfe9-6f7c38f932f&title=)
<a name="K3RG5"></a>
### Alt + Shift + F8：强制跨过调用
<a name="iwtE4"></a>
### F7：进入调用
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266377139-332d2da7-4002-4a4a-81ba-ba0a5bc8309c.gif#averageHue=%237a9173&clientId=ueefae3ee-f610-4&from=paste&id=In6ez&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uec7593fb-8245-40c7-9a97-704bbe83af4&title=)
<a name="rgu5f"></a>
### Shift + F7：智能进入调用
<a name="GFBgg"></a>
### Alt + Shift + F7：强制进入调用
<a name="RyPT7"></a>
### Shift + F8：跳出调用
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266377215-3f026604-e9e2-4a76-9bfc-02cedd506d34.gif#averageHue=%2385937f&clientId=ueefae3ee-f610-4&from=paste&id=rUulM&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u64b2fd7d-f398-42b6-bd8b-ac639689860&title=)
<a name="OBMSQ"></a>
### Alt + F9：运行至光标处
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266377225-83b18361-5035-421b-8217-f6fa30d36674.gif#averageHue=%23383c3d&clientId=ueefae3ee-f610-4&from=paste&id=SbHG7&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7261cfd1-3329-44f7-bbd3-092f9553ee6&title=)
<a name="KTl3m"></a>
### Ctrl + Alt + F9：强制运行至光标处
<a name="Cd6nv"></a>
### Alt + F10：显示执行点
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266377476-78700568-d7ff-464c-a7d5-b265e8cc8bea.gif#averageHue=%23424744&clientId=ueefae3ee-f610-4&from=paste&id=mgneP&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u73a695b2-2074-4803-9dd4-8056b24d645&title=)
<a name="tfgcm"></a>
### Alt + F8：评估表达式
<a name="XEm4J"></a>
### Ctrl + Alt + F8：快速评估表达式
<a name="Arq35"></a>
### Ctrl + F8：切换行断点
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266377703-2ecc5f78-c62a-49cd-aa6a-c808f0543964.gif#averageHue=%23546251&clientId=ueefae3ee-f610-4&from=paste&id=V3uQf&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua0a7f380-7d1b-4556-a162-53182efe661&title=)
<a name="Js3Ni"></a>
### Ctrl + Alt + Shift + F8：切换临时行断点
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266377756-ff6ef65d-79dd-449d-a00d-d20789ebaf78.gif#averageHue=%23424540&clientId=ueefae3ee-f610-4&from=paste&id=CJY2i&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud9da8c27-657f-4c41-bb39-122479a0dd7&title=)
<a name="Xliwt"></a>
### Ctrl + Shift + F8：查看断点
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266378035-14bbb78e-29f1-4ac5-accf-583c60665c94.gif#averageHue=%233d4144&clientId=ueefae3ee-f610-4&from=paste&id=zqrWx&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u86f0647b-2848-446d-b529-a3cf0e9ba97&title=)
<a name="FO4vg"></a>
### Ctrl + Shift + F8：编辑断点
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266378041-ec992e29-6d64-4d25-adc8-64fa344a3b7d.gif#averageHue=%233a3f42&clientId=ueefae3ee-f610-4&from=paste&id=HYTuH&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9ce054b7-0998-4f4f-8739-07de71e56c8&title=)
<a name="unSD8"></a>
### Alt + 4：显示运行窗口
<a name="pTnvV"></a>
### Alt + 5：显示调试窗口
<a name="qFQzO"></a>
### Alt + 8：显示服务窗口
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266378282-d790a7e2-e71d-4fa9-bb47-6f93d4c3314d.gif#averageHue=%237b9573&clientId=ueefae3ee-f610-4&from=paste&id=lxUei&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udb37a2af-c055-4909-ba71-97bb71260ef&title=)
<a name="ZHubH"></a>
## 十三、代码重构
<a name="nW8uO"></a>
### Ctrl + Alt + Shift + T：打开重构列表
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266378461-ee5b036e-2a87-4500-8fc6-cdba68a44845.gif#averageHue=%233b3f43&clientId=ueefae3ee-f610-4&from=paste&id=XI2NJ&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ude89f978-eec6-4e89-8a41-bf84ec8df00&title=)
<a name="F4Xg5"></a>
### Shift + F6：修改名称
<a name="mFegc"></a>
### Ctrl + F6：修改签名
<a name="gWKpT"></a>
### Ctrl + Alt + N：内联
<a name="L7jPU"></a>
### F6：移动
<a name="VslyA"></a>
### Ctrl + Alt + M：提取方法
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266378466-e8410d4d-c7cc-4bb3-8daa-fb839d1c8179.gif#averageHue=%23759073&clientId=ueefae3ee-f610-4&from=paste&id=jqTgM&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf153ff6c-d290-4fe5-8084-e89b1330742&title=)
<a name="pb0NM"></a>
### Ctrl + Alt + F：引入域
<a name="wshhz"></a>
### Ctrl + Alt + P：引入参数
<a name="AwD4H"></a>
### Ctrl + Alt + V：引入变量
<a name="plyO7"></a>
### Alt + Delete：安全删除
<a name="LMnSP"></a>
## 十四、全局 CVS 操作
<a name="vR2KG"></a>
### `Alt + `` ：弹出 CVS 窗口
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266378723-97ce262c-b845-4a14-a07e-8bf7b109dec4.gif#averageHue=%233c4042&clientId=ueefae3ee-f610-4&from=paste&id=Guu8O&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u755901a0-00bd-48f5-93f7-497276422a6&title=)
<a name="tWTUz"></a>
### Ctrl + K：提交
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266378680-85f05a88-4ee9-4e44-a85c-1fffc3e47a54.gif#averageHue=%23333637&clientId=ueefae3ee-f610-4&from=paste&id=f0da1&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf6fb1503-d6db-42b7-b81e-7e21f99e783&title=)
<a name="VxrAn"></a>
### Ctrl + T：更新项目
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266378996-4c2ee72e-0209-48e0-8497-1d2d1c4220ab.gif#averageHue=%23738171&clientId=ueefae3ee-f610-4&from=paste&id=ApV8m&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u258d86ad-0067-4757-94e5-41502364b94&title=)
<a name="KhAbS"></a>
### Ctrl + Alt + Z：回滚
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266378937-a850faf3-4c79-49ce-940c-fde0a24cc2f6.gif#averageHue=%233b3f43&clientId=ueefae3ee-f610-4&from=paste&id=AYM0E&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3fc12ade-fe8b-48a1-8a8d-8e505d6d840&title=)
<a name="xHhZ3"></a>
### Ctrl + Shift + K：拉取
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266379068-5e2660e3-b135-496b-a1f7-535af7b04238.gif#averageHue=%23434741&clientId=ueefae3ee-f610-4&from=paste&id=O4zdH&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5839044c-17c0-47a5-926a-fdeaba40277&title=)
<a name="U73rg"></a>
### Ctrl + Alt + Shift + Down：下一个修改
<a name="Oh4kV"></a>
### Ctrl + Alt + Shift + Up：上一个修改
<a name="etwew"></a>
### Alt + 9：显示版本控制窗口
<a name="D5kWU"></a>
### Alt + 0：显示提交窗口
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266379304-4e085efa-a7b6-4836-9cd9-22e99937df0c.gif#averageHue=%23809577&clientId=ueefae3ee-f610-4&from=paste&id=qejua&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6e850bb5-3a03-4899-9e37-d6bd4cd0de9&title=)
<a name="j6v15"></a>
## 十五、差异查看器
<a name="Sn0Kc"></a>
### F7：下一个差异
<a name="OVK0o"></a>
### Shift + F7：上一个差异
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266379212-e0f660ff-5911-4a0c-b844-c139ff651bc3.gif#averageHue=%23708263&clientId=ueefae3ee-f610-4&from=paste&id=RMogJ&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u79241baf-d378-447c-b141-435a864a34f&title=)
<a name="G6iDx"></a>
### Ctrl + Alt + R：接受左侧
<a name="eq6vL"></a>
### Ctrl + Alt + A：接受右侧
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266379565-dcdd7a06-fd38-4361-b802-b3ed3e8098fa.gif#averageHue=%23313536&clientId=ueefae3ee-f610-4&from=paste&id=ranLG&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud3d09e4f-0300-49dd-8648-7e6b3f2d7bf&title=)
<a name="fLqc5"></a>
### Ctrl + Shift + Tab：选择对面的差异窗格
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266379516-2eb7fc51-5fbd-4ab9-9be4-c3b0d954d59a.gif#averageHue=%23323537&clientId=ueefae3ee-f610-4&from=paste&id=WQzak&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf10c2550-febd-4d86-9248-b243ca4d716&title=)
<a name="gjSbi"></a>
### Ctrl + Shift + D：显示差异设置窗口
<a name="A3d8t"></a>
## 十六、工具窗口
<a name="O2sh9"></a>
### Shift + Escape：隐藏活动的工具窗口
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266379679-a89dafa7-8d23-41ba-a0ba-d27b76847e3d.gif#averageHue=%233d4143&clientId=ueefae3ee-f610-4&from=paste&id=iAhaZ&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0beb0e95-3c12-44cf-9878-8d78e5b6222&title=)
<a name="mc352"></a>
### Ctrl + Shift + F12：隐藏所有工具窗口
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266380027-acca2682-95a8-478e-bdac-41da29cf1036.gif#averageHue=%234f6059&clientId=ueefae3ee-f610-4&from=paste&id=atjBt&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u56064754-4021-45af-b1a3-a65592f0601&title=)
<a name="skBOW"></a>
### F12：跳转到最后一个工具窗口
<a name="DXrIb"></a>
### Ctrl + Alt + Shift + Left：向左延伸窗口大小
<a name="Ce5aI"></a>
### Ctrl + Alt + Shift + Right：向右延伸窗口大小
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266380143-555d1590-0b24-41ae-b5ab-54ee7d44289e.gif#averageHue=%23799170&clientId=ueefae3ee-f610-4&from=paste&id=u92dl&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud1893c35-7fef-408f-9fe3-b7f3747c763&title=)
<a name="Sx2xn"></a>
### Ctrl + Alt + Shift + Up：向顶部延伸窗口大小
<a name="bjM51"></a>
### Ctrl + Alt + Shift + Down：向底部延伸窗口大小
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266380363-3c3ad594-aa98-4670-b909-57fd29f712fa.gif#averageHue=%2347423a&clientId=ueefae3ee-f610-4&from=paste&id=O7ztH&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6cbdb3d1-0cc2-4607-9273-795d07c6099&title=)
<a name="gZzyn"></a>
### Alt + 1：显示项目窗口
<a name="fYlnm"></a>
### Alt + 2：显示书签窗口
<a name="V3Q1F"></a>
### Alt + 3：显示查找窗口
<a name="FWcXE"></a>
### Alt + 4：显示运行窗口
<a name="pQDyf"></a>
### Alt + 5：显示调试窗口
<a name="OUF40"></a>
### Alt + 6：显示问题窗口
<a name="ltIR0"></a>
### Alt + 7：显示结构窗口
<a name="uzX9u"></a>
### Alt + 8：显示服务窗口
<a name="Kzxyg"></a>
### Alt + 9：显示版本控制窗口
<a name="ZmN5f"></a>
### Alt + 0：显示提交窗口
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266380329-72c89667-9f86-43d6-9043-ea6154c5d62f.gif#averageHue=%232b2a2c&clientId=ueefae3ee-f610-4&from=paste&id=C9WF4&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud371a66a-fc5d-4d4c-8136-935d4a39451&title=)
<a name="MMksH"></a>
### Alt + F12：显示终端窗![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1683266380347-e1656856-0f49-48f2-a8e7-6479bf4f5ad2.gif#averageHue=%23343639&clientId=ueefae3ee-f610-4&from=paste&id=dKJ1J&originHeight=671&originWidth=1065&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3b66f27f-c8f1-4188-9cef-66726a02dd2&title=)
