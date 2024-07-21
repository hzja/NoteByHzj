密码学 AES<br />数据加密标准（Data Encryption Standard: DES）的密钥长度是56比特，因此算法的理论安全强度是256。但二十世纪中后期正是计算机飞速发展的阶段，元器件制造工艺的进步使得计算机的处理能力越来越强，DES将不能提供足够的安全性。1997年1月2号，美国国家标准技术研究所（National Institute of Standards and Technology: NIST）宣布希望征集高级加密标准（Advanced Encryption Standard: AES），用以取代DES。AES评判要求：

- NIST在征集算法的时候就提出了几项硬性要求：
- 分组加密算法：支持128位分组大小，128/192/256位密钥
- 安全性不低于3DES，但实施与执行要比3DES的更高效
- 优化过的ANSI C的实现代码
- KAT(Known-Answer tests)及MCT(Monte Carlo Tests)测试及验证
- 软件及硬件实现的便捷
- 可抵御已知攻击

AES得到了全世界很多密码工作者的响应，先后有很多人提交了自己设计的算法。第一轮共有15种算法入选，其中5种算法入围了决赛，分别是：Rijndael，Serpent，Twofish，RC6和MARS。又经过3年的验证、评测及公众讨论之后Rijndael算法最终入选。Rijndael是一个分组密码算法族，由比利时学者Joan Daemen和Vincent Rijmen所提出的，算法的名字就由两位作者的名字组合而成。其分组长度包括128比特、160比特、192比特、224比特、256比特，密钥长度也包括这五种长度，但是最终AES只选取了分组长度为128比特，密钥长度为128比特、192比特和256比特的三个版本。<br />Rijndael的优势在于集安全性、性能、效率、可实现性及灵活性与一体。Rijndael设计思想：

- 安全性（Security） 算法足够强，抗攻击
- 经济性（Efficiency） 算法运算效率高
- 密钥捷变（Key Agility） 更改密钥所引入的损失尽量小，即最小消耗的密钥扩展算法
- 适应性 （Versatility） 适用于不同的CPU架构，软件或硬件平台的实现
- 设计简单（Simplicity） 轮函数的设计精简，只是多轮迭代
<a name="yBkQP"></a>
### AES的算法原理
加密算法的一般设计准则

- 混淆 (Confusion) 最大限度地复杂化密文、明文与密钥之间的关系，通常用非线性变换算法达到最大化的混淆。
- 扩散 (Diffusion) 明文或密钥每变动一位将最大化地影响密文中的位数，通常采用线性变换算法达到最大化的扩散。

对Rijndael算法来说解密过程就是加密过程的逆向过程。下图为AES加解密的流程，从图中可以看出：1）解密算法的每一步分别对应加密算法的逆操作，2）加解密所有操作的顺序正好是相反的。正是由于这几点（再加上加密算法与解密算法每步的操作互逆）保证了算法的正确性。<br />![2021-08-23-21-53-01-832076.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629726826611-ebbee0a8-5ff8-44bb-9bbb-f02c16691875.png#clientId=uc887d4b3-36c0-4&from=ui&id=u6ee46df4&originHeight=669&originWidth=508&originalType=binary&ratio=1&size=26122&status=done&style=shadow&taskId=u941a81ce-2b1f-499b-bb2b-3712b1ca6ae)<br />Rijndael算法是基于代换-置换网络（SPN，Substitution-permutation network）的迭代算法。明文数据经过多轮次的转换后方能生成密文，每个轮次的转换操作由轮函数定义。轮函数任务就是根据密钥编排序列（即轮密码）对数据进行不同的代换及置换等操作。<br />![2021-08-23-21-53-02-484074.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629726826841-67de1aed-7f21-414a-b85f-65195d79c719.png#clientId=uc887d4b3-36c0-4&from=ui&id=cKF0C&originHeight=608&originWidth=874&originalType=binary&ratio=1&size=142325&status=done&style=shadow&taskId=u36ced131-5e15-4ce9-b36d-1fe8f86ebb2)

- 图左侧为轮函数的流程，主要包含4种主要运算操作：字节代换(SubByte)、行移位(ShiftRow)、列混合(MixColumn)、轮密钥加(AddRoundKey)。
- 图右侧为密钥编排方案，在Rijndael中称为密钥扩展算法（KeyExpansion）。
<a name="HOCl9"></a>
### 轮函数
轮函数主要包含4种运算，但不同的运算轮所做的具体运的算组合并不相同。主要区别是初始轮（Round: 0）和最后一轮（Round: Nr），所有中间轮的运算都是相同的，会依次进行4种运算，即：

- 字节代换(SubByte)
- 行移位(ShiftRow)
- 列混合(MixColumn)
- 轮密钥加(AddRoundKey)

Rijndael算法支持大于128位的明文分组，所以需要列数更多的矩阵来描述。Rijndael轮函数的运算是在特殊定义的有限域GF(256)上进行的。有限域（Finite Field）又名伽罗瓦域（Galois field），简单言之就是一个满足特定规则的集合，集合中的元素可以进行加减乘除运算，且运算结果也是属于此集合。根据Rinjdael算法的定义，加密轮数会针对不同的分组及不同的密钥长度选择不同的数值：<br />![2021-08-23-21-53-03-302078.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629726826701-abe94ddc-ee41-4471-a8ae-141cbd72b90c.png#clientId=uc887d4b3-36c0-4&from=ui&id=fgk4g&originHeight=177&originWidth=438&originalType=binary&ratio=1&size=37574&status=done&style=shadow&taskId=uacf55d99-b5bd-4ca9-b9a7-0192f3037fe)<br />AES标准只支持128位分组（Nb = 4）的情况。AES标准算法将128位的明文，以特定次序生成一个4×4的矩阵（每个元素是一个字节，8位），即初始状态（state），经由轮函数的迭代转换之后又将作为下一轮迭代的输入继续参与运算直到迭代结束。<br />轮函数拆解：字节代换（Substitute Bytes）<br />字节代换（SubBytes）是对state矩阵中的每一个独立元素于置换盒（Substitution-box，S盒）中进行查找并以此替换输入状态的操作。字节代换是可逆的非线性变换，也是AES运算组中唯一的非线性变换。字节代换逆操作也是通过逆向置换盒的查找及替换来完成的，主要功能是通过S盒完成一个字节到另外一个字节的映射。<br />![2021-08-23-21-53-04-966078.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629727355721-6fab61ae-a3f9-4535-9351-d532c575bdc4.png#clientId=uc887d4b3-36c0-4&from=ui&id=u769fc1fc&originHeight=200&originWidth=401&originalType=binary&ratio=1&size=44316&status=done&style=shadow&taskId=u1604df98-d158-49a0-b601-898f809af00)<br />S盒是由一个有限域GF(256)上的乘法求逆并串联线性仿射变换所构造出来的，不是一个随意构造的简单查询表。S因其运算复杂，众多的AES 软件及硬件实现直接使用了查找表(LUP, Look-up table)，但查询表的方式并不适合所有场景，针对特定的硬件最小化面积设计需求，则要采用优化的组合逻辑以得到同价的S盒替换。盒是事先设计好的16×16的查询表，即256个元素。S盒用于提供密码算法的混淆性。S盒的详细构造方法可以参考文献。这里直接给出构造好的结果，下图(a)为S盒，图(b)为S-1（S盒的逆）。S和S-1分别为16×16的矩阵，完成一个8比特输入到8比特输出的映射，输入的高4-bit对应的值作为行标，低4-bit对应的值作为列标。<br />![2021-08-23-21-53-05-259406.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629727355991-097d4059-4ad0-4c6f-a9d9-f149fe241228.png#clientId=uc887d4b3-36c0-4&from=ui&id=Jh03f&originHeight=302&originWidth=895&originalType=binary&ratio=1&size=227558&status=done&style=shadow&taskId=u8ca5e0ef-4496-4f5a-8d02-71de5fa4780)<br />轮函数拆解：行移位（Shift Rows）<br />行移位主要目的是实现字节在每一行的扩散，属于线性变换。行移位是一个4×4的矩阵内部字节之间的置换，用于提供算法的扩散性。<br />1) 正向行移位<br />正向行移位用于加密，其原理图如下。其中：第一行保持不变，第二行循环左移8比特，第三行循环左移16比特，第四行循环左移24比特。假设矩阵的名字为state，用公式表示如下：state’[i][j] = state[i][(j+i)%4];其中i、j属于[0,3]。<br />![2021-08-23-21-53-05-364406.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629727402548-442bae9d-db17-462c-ad53-d45843d927b6.png#clientId=uc887d4b3-36c0-4&from=ui&id=u9cfe353a&originHeight=175&originWidth=464&originalType=binary&ratio=1&size=49373&status=done&style=shadow&taskId=ufb857d6e-42eb-406a-9bd5-f8cd9b8263f)<br />2) 逆向行移位<br />逆向行移位即是相反的操作，即：第一行保持不变，第二行循环右移8比特，第三行循环右移16比特，第四行循环右移24比特。用公式表示如下：state’[i][j] = state[i][(4+j-i)%4];其中i、j属于[0,3]。<br />轮函数拆解：列混合（Mix Columns）<br />列混合是通过将state矩阵与常矩阵C相乘以达成在列上的扩散，属于代替变换。列混合是Rijndael算法中最复杂的一步，其实质是在有限域GF(256)上的多项式乘法运算。<br />![2021-08-23-21-53-05-461407.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629727402846-6461d333-94a3-41e8-a486-3a1e056ef2a0.png#clientId=uc887d4b3-36c0-4&from=ui&id=bXKaG&originHeight=206&originWidth=381&originalType=binary&ratio=1&size=42260&status=done&style=shadow&taskId=u340abde7-df3c-4076-9d79-f8c8bfdabce)<br />1) 正向列混淆<br />正向列混淆的原理图如下：<br />![2021-08-23-21-53-05-553409.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629727402866-34b9ae4f-749a-4a8b-8c3f-5032b95685e3.png#clientId=uc887d4b3-36c0-4&from=ui&id=giT0e&originHeight=163&originWidth=462&originalType=binary&ratio=1&size=5826&status=done&style=shadow&taskId=u58b02a0f-68d7-4d15-bdaf-71f9be78226)<br />根据矩阵的乘法可知，在列混淆的过程中，每个字节对应的值只与该列的4个值有关系。此处的乘法和加法都是定义在GF(28)上的，需要注意如下几点：

- 将某个字节所对应的值乘以2，其结果就是将该值的二进制位左移一位，如果原始值的最高位为1，则还需要将移位后的结果异或00011011；
- 乘法对加法满足分配率，例如：07·S0,0=(01⊕02⊕04)·S0,0= S0,0⊕(02·S0,0)(04·S0,0)
- 此处的矩阵乘法与一般意义上矩阵的乘法有所不同，各个值在相加时使用的是模28加法（异或运算）。

下面举一个例子，假设某一列的值如下图，运算过程如下：<br />![2021-08-23-21-53-05-628404.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629727402872-602bfd4d-c762-487c-92d5-7c048c6265c0.png#clientId=uc887d4b3-36c0-4&from=ui&id=vide2&originHeight=147&originWidth=251&originalType=binary&ratio=1&size=2512&status=done&style=shadow&taskId=uacb69727-548a-452c-9fab-0627947a1bd)<br />![2021-08-23-21-53-05-697404.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629727445835-2dce378e-eb07-4be1-bbfd-c1322525f717.png#clientId=uc887d4b3-36c0-4&from=ui&id=ub2d8f5d5&originHeight=221&originWidth=424&originalType=binary&ratio=1&size=3738&status=done&style=shadow&taskId=u15b4de38-a179-4a03-ab0d-e4644d663c8)<br />在计算02与C9的乘积时，由于C9对应最左边的比特为1，因此需要将C9左移一位后的值与(0001 1011)求异或。同理可以求出另外几个值。<br />2) 逆向列混淆<br />逆向列混淆的原理图如下：<br />![2021-08-23-21-53-05-783440.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629727445857-1acdc63c-405d-4923-acbe-397677d3582d.png#clientId=uc887d4b3-36c0-4&from=ui&id=QViej&originHeight=163&originWidth=462&originalType=binary&ratio=1&size=5960&status=done&style=shadow&taskId=uda223eed-c5a5-4b02-99a8-b344ee83bc4)<br />由于：<br />![2021-08-23-21-53-05-896407.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629727445862-1e38dd8b-56f5-401c-aea9-f22d07e98da1.png#clientId=uc887d4b3-36c0-4&from=ui&id=QMpxf&originHeight=81&originWidth=336&originalType=binary&ratio=1&size=4250&status=done&style=shadow&taskId=u593f24b2-028c-4846-8b19-55c44ab0cc6)<br />说明两个矩阵互逆，经过一次逆向列混淆后即可恢复原文。<br />轮函数拆解：轮密钥加（Add Round Key）<br />密钥加是将轮密钥简单地与状态进行逐比特异或。这个操作相对简单，其依据的原理是“任何数和自身的异或结果为0”。加密过程中，每轮的输入与轮子密钥异或一次；因此，解密时再异或上该轮的轮子密钥即可恢复。<br />![2021-08-23-21-53-06-068406.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629727502112-39225e3c-051f-40e4-829b-f4a6d136bbf9.png#clientId=uc887d4b3-36c0-4&from=ui&id=u8d8d885c&originHeight=325&originWidth=412&originalType=binary&ratio=1&size=62597&status=done&style=shadow&taskId=u2b86453a-27c2-485c-88d8-2c3777e3090)
<a name="CiVrA"></a>
### 密钥扩展算法（Key Expansion）
AES加解密中每轮的密钥分别由种子密钥经过密钥扩展算法得到。算法中16字节的明文、密文和轮子密钥都以一个4×4的矩阵表示。<br />密钥扩展算法是Rijndael的密钥编排实现算法，其目的是根据种子密钥（用户密钥）生成多组轮密钥。轮密钥为多组128位密钥，对应不同密钥长度，分别是11，13，15组。<br />![2021-08-23-21-53-06-185404.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629727502121-09422683-68f9-45e1-a585-5645b09e1d17.png#clientId=uc887d4b3-36c0-4&from=ui&id=U1olf&originHeight=278&originWidth=561&originalType=binary&ratio=1&size=29190&status=done&style=shadow&taskId=uf5c11376-d7af-49db-94ff-ac56f03e2ef)<br />密钥扩展的原理图如下：<br />![2021-08-23-21-53-06-461448.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629727502143-485410bc-8ad2-4e34-b85a-73b0fc9fb81c.png#clientId=uc887d4b3-36c0-4&from=ui&id=xnAUz&originHeight=467&originWidth=470&originalType=binary&ratio=1&size=8803&status=done&style=shadow&taskId=u4c29e897-316e-4c0a-8ac3-1cd69ffc1e3)<br />密钥扩展过程说明：<br />1)  将种子密钥按图(a)的格式排列，其中k0、k1、……、k15依次表示种子密钥的一个字节；排列后用4个32比特的字表示，分别记为w[0]、w[1]、w[2]、w[3]；<br />2)  按照如下方式，依次求解w[j]，其中j是整数并且属于[4,43]；<br />3)  若j%4=0,则w[j]=w[j-4]⊕g(w[j-1]),否则w[j]=w[j-4]⊕w[j-1]；<br />函数g的流程说明：

1. a)  将w循环左移8比特；
2. b)  分别对每个字节做S盒置换；
3. c)  与32比特的常量（RC[j/4],0,0,0）进行异或，RC是一个一维数组，其值如下。（RC的值只需要有10个，而此处用了11个，实际上RC[0]在运算中没有用到，增加RC[0]是为了便于程序中用数组表示。由于j的最小取值是4，j/4的最小取值则是1，因此不会产生错误。）

RC = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36}<br />动画演示加密过程：

- Enrique Zabala创建了一个AES-128加密算法的动画演示，清楚、直观地介绍了轮函数执行的过程。
- 波士顿大学的Howard Straubing做了这么一个动画来展示AES加密算法的演示。
