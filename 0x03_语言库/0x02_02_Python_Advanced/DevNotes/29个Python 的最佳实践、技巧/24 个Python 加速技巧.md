Python
<a name="nLZpS"></a>
### 一、分析代码运行时间
<a name="j7jxp"></a>
#### 第1式：测算代码运行时间
平凡方法<br />![](./img/1640520665290-d2e242e6-c14f-4c38-8bcb-3b538504a1c8.webp)<br />快捷方法（jupyter环境）<br />![](./img/1640520665179-045c7542-e24d-4dfb-99d4-cc86255918f3.webp)
<a name="R6vBP"></a>
#### 第2式：测算代码多次运行平均时间
平凡方法<br />![](./img/1640520665368-dadba013-e54f-4a74-aa12-c3a0aab9a894.webp)<br />快捷方法（jupyter环境）<br />![](./img/1640520665314-b4f8bc7a-5da4-491d-949f-53fc3a185c68.webp)
<a name="EpLG6"></a>
#### 第3式：按调用函数分析代码运行时间
平凡方法<br />![](./img/1640520665033-91276eab-13f3-4241-9e1a-78aa754c5add.webp)<br />![](./img/1640520666636-c4c42171-a148-4742-8561-01dc8fec3a9f.webp)<br />快捷方法（jupyter环境）<br />![](./img/1640520667023-ead22041-e180-4af7-88aa-197c1449b598.webp)
<a name="EfwHu"></a>
#### 第4式：按行分析代码运行时间
平凡方法<br />![](./img/1640520666929-1b1b1cc4-bdb7-47ff-a51a-bf4e54450ff2.webp)<br />![](./img/1640520667010-54c98786-8c29-45f2-8903-c42f0d8d0087.webp)<br />![](./img/1640520667395-0ce475af-a1db-4a09-8553-92b09e5f2393.webp)<br />![](./img/1640520667660-e394e977-63e1-493d-b615-3e1a74fc1b12.webp)<br />快捷方法（jupyter环境）<br />![](./img/1640520668816-adb6e016-43f5-4e95-8371-d4ef1c068b8a.webp)
<a name="B3iNm"></a>
### 二、加速你的查找
<a name="jg4Kj"></a>
#### 第5式：用`set`而非`list`进行查找
低速方法<br />![](./img/1640520668185-ed780505-1508-46d3-9a67-b01376939204.webp)<br />![](./img/1640520668591-c9e9213c-e64e-453e-a41c-bbaf337da3b5.webp)<br />高速方法<br />![](./img/1640520668658-adcc9e02-cd51-4017-b22f-5b4dbc8ab46f.webp)
<a name="r1as1"></a>
#### 第6式：用`dict`而非两个`list`进行匹配查找
低速方法<br />![](./img/1640520668678-73c5aac5-8c77-464f-8fbe-3f82dfef0ccf.webp)<br />![](./img/1640520669303-5a28079c-912a-4657-8e3f-1fecf738dc74.webp)<br />高速方法<br />![](./img/1640520669262-658dbc71-fa0e-4c1f-af8f-b5bf2e8260c6.webp)
<a name="BEOue"></a>
### 三、加速你的循环
<a name="RZsZx"></a>
#### 第7式：优先使用`for`循环而不是`while`循环
低速方法<br />![](./img/1640520669554-75e829fc-5a29-43c3-ab55-2a662d602267.webp)<br />高速方法<br />![](./img/1640520669442-6c25896b-faeb-4060-b979-b43b6ad49dca.webp)
<a name="uwmDM"></a>
#### 第8式：在循环体中避免重复计算
低速方法<br />![](./img/1640520669678-74e88778-0c56-425d-902f-94666f728421.webp)<br />高速方法<br />![](./img/1640520670187-5d722716-520b-44cd-9bee-e2459d9c8d36.webp)
<a name="pdGwR"></a>
### 四、加速你的函数
<a name="pA0Rs"></a>
#### 第9式：用循环机制代替递归函数
低速方法<br />![](./img/1640520670014-6c2a29c0-3e45-4649-9232-04138ef62107.webp)<br />高速方法<br />![](./img/1640520671162-ecd4c494-7941-4726-b386-3d80ecfee367.webp)
<a name="x8DLd"></a>
#### 第10式：用缓存机制加速递归函数
低速方法<br />![](./img/1640520670235-5c90a66d-d540-490b-af28-1e124bbf9223.webp)<br />高速方法<br />![](./img/1640520670324-a276daa9-01db-4a53-9a52-1c79d0472881.webp)
<a name="BKxTM"></a>
#### 第11式：用numba加速Python函数
低速方法<br />![](./img/1640520670650-7d689b6a-b484-4e34-8268-ffc0061f2740.webp)<br />高速方法<br />![](./img/1640520671044-448d4a14-a84f-4648-adc3-266aebe0d3ff.webp)
<a name="mpDN6"></a>
### 五、使用标准库函数进行加速
<a name="f8sTW"></a>
#### 第12式：使用`collections.Counter`加速计数
低速方法<br />![](./img/1640520670960-ad6c16d0-d042-4ef0-8c22-0f4a87ca37e0.webp)<br />高速方法

![](./img/1640520670821-3b64a858-56b0-43f7-905b-ae3d0c700c24.webp)
<a name="JonvG"></a>
#### 第13式：使用`collections.ChainMap`加速字典合并
低速方法<br />![](./img/1640520671427-4b8dc70d-57ea-47ad-9af3-b2adb90613a0.webp)<br />![](./img/1640520671690-2926f195-fd07-49fb-88ea-252271fbb4ac.webp)<br />高速方法<br />![](./img/1640520671795-dd900b5d-cd93-4f2f-87c1-d1cdcdd7228a.webp)
<a name="c2rtr"></a>
### 六，使用numpy向量化进行加速
<a name="MN1bL"></a>
#### 第14式：使用np.array代替list
低速方法<br />![](./img/1640520671738-8eee9c2b-460b-49c2-8d3d-bc67e6d6bc04.webp)<br />高速方法<br />![](./img/1640520672314-b28b2dc5-2f25-4bc9-abcd-8775f5f6e80b.webp)
<a name="SO5en"></a>
#### 第15式：使用`np.ufunc`代替`math.func`
低速方法<br />![](./img/1640520672053-bd079478-1365-4a41-bc3c-125f83aba987.webp)<br />高速方法<br />![](./img/1640520672393-79aa1118-6885-40e6-96ca-5867c4f8767c.webp)
<a name="OZCSc"></a>
#### 第16式：使用`np.where`代替if
低速方法<br />![](./img/1640520672555-7233df3a-e38a-45e3-8999-8d6306ff2afc.webp)<br />![](./img/1640520672664-62c7f78f-6958-4be5-81a9-3ae0d853565a.webp)<br />高速方法<br />![](./img/1640520672828-c3f390d2-6dbf-4e85-b8c1-55da98925c07.webp)
<a name="Mo8yC"></a>
### 七、加速你的Pandas
<a name="FoAGp"></a>
#### 第17式：使用`np.ufunc`函数代替`applymap`
低速方法<br />![](./img/1640520673114-474bbfb9-f820-4626-9847-a77849088208.webp)<br />高速方法<br />![](./img/1640520673066-ce6dcfe4-1df2-41ad-86d4-d9ec91f766b5.webp)
<a name="gJ73B"></a>
#### 第18式：使用预分配存储代替动态扩容
低速方法<br />![](./img/1640520673268-8830010c-4001-41f9-88df-fbcc1ab0c367.webp)<br />高速方法<br />![](./img/1640520673174-f66e22a0-d7bf-40ef-b27d-fd2f088d0eb9.webp)
<a name="RS75d"></a>
#### 第19式：使用csv文件读写代替excel文件读写
低速方法

![](./img/1640520673704-82141d45-d6d7-47d5-90ec-8aa0f97b1d75.webp)<br />高速方法<br />![](./img/1640520673773-ed222cba-3bfd-48d6-abde-fbea5eff1fb8.webp)
<a name="ehmSx"></a>
#### 第20式：使用pandas多进程工具pandarallel
低速方法<br />![](./img/1640520674245-a8e44489-edce-4c16-a392-4fbdfa7c1722.webp)<br />![](./img/1640520675719-89ba5749-15e2-4461-bcfb-7c7549e08fd0.webp)<br />高速方法<br />![](./img/1640520674969-63baf049-b01e-4764-936e-63792aeb198d.webp)
<a name="UgLH8"></a>
### 八、使用Dask进行加速
<a name="DAVPo"></a>
#### 第21式：使用dask加速dataframe
低速方法<br />![](./img/1640520674856-dfda3e52-73ce-40b7-a302-a4f69956654c.webp)<br />高速方法<br />![](./img/1640520674955-39598833-7fea-4b66-a682-d0727d2b947f.webp)
<a name="xp3fG"></a>
#### 第22式：使用`dask.delayed`进行加速
低速方法<br />![](./img/1640520674964-a75ded64-deee-4f9f-b2bd-c21ef4ac0af5.webp)<br />![](./img/1640520675767-36068d07-341b-4209-94ec-8e5a2cf9245a.webp)<br />高速方法<br />![](./img/1640520675809-959fb98a-633c-4762-af06-a61507d4047c.webp)
<a name="A4O8Q"></a>
### 九、应用多线程多进程加速
<a name="HuV3Y"></a>
#### 第23式：应用多线程加速IO密集型任务
低速方法<br />![](./img/1640520675735-db7f6519-269c-4649-87c9-48b083944990.webp)<br />高速方法<br />![](./img/1640520675964-bd8a8402-94c5-4fd1-92c1-27d3426c9e9d.webp)
<a name="J6tQQ"></a>
#### 第24式：应用多进程加速CPU密集型任务
低速方法<br />![](./img/1640520676761-73aa2b46-fd2d-4ad2-96d1-79ecdf663884.webp)<br />高速方法<br />![](./img/1640520677249-89eab3af-a598-4754-b73a-46b27adea812.webp)
