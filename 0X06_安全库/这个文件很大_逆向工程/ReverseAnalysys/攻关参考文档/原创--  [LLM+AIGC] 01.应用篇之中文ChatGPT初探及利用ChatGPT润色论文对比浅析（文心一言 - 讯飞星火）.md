# 原创
：  [LLM+AIGC] 01.应用篇之中文ChatGPT初探及利用ChatGPT润色论文对比浅析（文心一言 | 讯飞星火）

# [LLM+AIGC] 01.应用篇之中文ChatGPT初探及利用ChatGPT润色论文对比浅析（文心一言 | 讯飞星火）

<font color="red">近年来，人工智能技术火热发展，尤其是OpenAI在2022年11月30日发布ChatGPT聊天机器人程序，其使用了Transformer神经网络架构（GPT-3.5），能够基于在预训练阶段所见的模式、统计规律和知识来生成回答，还能根据聊天的上下文进行互动，真正像人类一样来聊天交流以及完成复杂的NLP任务。基于此，为更好地学习前沿AI知识，了解LLM和AIGC应用实战，本人在忙完博士学业后，立刻开启了《LLM+AIGC》专栏，一方面作为在线笔记记录和分享自己的学习过程，另一方面期望帮助更多初学者以及对LLM感兴趣的同学。您的关注、点赞和转发就是对秀璋最大的支持，知识无价人有情，希望我们都能在人生路上开心快乐、共同成长。</font>

该系列主要涵盖三方面：

第一篇文章主要是ChatGPT应用初试，简单介绍什么是大语言模型（LLM）和人工智能生成内容（AIGC），利用中文ChatGPT实现英文论文润色，并与传统英文润色和语法校对进行比较。基础性文章，希望对您有所帮助！且行且珍惜，继续加油，提前祝大家中秋节快乐。O(∩_∩)O

#### 文章目录

---


## 一.什么是LLM和AIGC

**大语言模型（Large Language Model，LLM）** 是指使用大量文本数据训练的深度学习模型，可以生成自然语言文本或理解语言文本的含义。大语言模型可以处理多种自然语言任务，如文本分类、问答、对话等，是通向人工智能的一条重要途径。下图是艾瑞咨询研究院绘制的大语言模型改变数字产业生态路线图，非常好地展示了大语言模型相关的技术及应用。

此外，大语言模型已渗透至各行各业，常见应用讴歌包括搜索引擎（如bing、google）、办公软件、对话式AI、智能问答、企业服务应用（如客服）、垂直领域（如医学）等。

**生成式人工智能（Artificial Intelligence Generated Content，AIGC）** 是指基于生成对抗网络、大型预训练模型等人工智能的技术方法，通过已有数据的学习和识别，以适当的泛化能力生成相关内容的技术。AIGC技术的核心思想是利用人工智能算法生成具有一定创意和质量的内容。通过训练模型和大量数据的学习，AIGC可以根据输入的条件或指导，生成与之相关的内容。

常见应用如下：

最后，给出ChatGPT的发展历史及基本思路。下图展示RLHF的（论文 TAMER：Training an Agent Manually via Evaluative Reinforcement ）基本步骤：

ChaptGPT会结合提示学习（Prompt Learning）实现，通过问题、Prompts、数据集来生成回答，整个过程通过Fine Tuning和Prompt Tuning实现对预训练模型的权重微调（GPT3.5），从而提升性能。

整个发展历程如下图所示，推荐大家阅读“快乐小码农”的博客。

> 
温馨提示：<br/> 本文主要以应用尝试为主，更多原理知识随着作者后续学习深入，再对其进行总结和归纳。该部分也仅简单描述相关问题。


---


## 二.传统英文翻译方式

假设存在一段系统安全方向需要翻译的文本，如下所示：

> 
传统APT攻击检测主要通过构建规则，利用安全人员逆向分析提取特征来实现同源分析及检测，典型技术包括特征值检测技术、校验和检测技术、启发式检测技术和主动防御技术[8-9]。然而，先前的方法过度依赖安全专家经验，规则和知识库匹配较难准确识别APT攻击的行为，其自动化和智能化程度较低，并且在面对具有混淆、加壳、逃逸和欺骗技 术的恶意软件时，会进一步降低检测的准确率。


### 1.英文翻译

常见的翻译软件包括Google、有道、bing、DeepL等，整体效果各有优劣，但准确度均存在一定误差。具体翻译如下：

**(1) 有道翻译**

> 
Traditional APT attack detection mainly realizes homology analysis and detection by constructing rules and using reverse analysis of security personnel to extract features. Typical technologies include eigenvalue detection technology, checksum detection technology, heuristic detection technology and active defense technology [8-9]. However, previous methods rely too much on the experience of security experts, rules and knowledge base matching is difficult to accurately identify APT attack behavior, its degree of automation and intelligence is low, and in the face of malware with confusion, shell, escape and deception techniques, will further reduce the detection accuracy.


**(2) Google翻译**

> 
Traditional APT attack detection mainly identifies malicious attacks or analyzes attack behaviors by constructing rules, or security personnel use reverse analysis to extract features. Typical detection techniques include characteristic value detection, checksum detection, heuristic detection and active defense technology [8-9]. However, previous methods rely too much on security expert experience, rules, and knowledge bases, making it difficult to accurately identify the behavior of APT attacks, and the degree of automation and intelligence of existing methods is low. Existing methods will further reduce the accuracy of detection in the face of malware with obfuscation, packing, evasion and deception techniques.


**(3) DeepL翻译**

<mark>**总结：** 由上图可知，在翻译过程中，某些专有名词、语序及表述均存在不准确的地方。</mark>

---


### 2.grammarly英文校对

接着我们利用Grammarly软件对英文进行校对。

修改后的内容如下图所示，除了语法校正外，也增加部分专业词汇。譬如：

> 
Traditional APT attack detection identifies malicious attacks or analyzes attack behaviors by constructing rules, or security analysts utilize reverse analysis to extract features. Typical detection techniques include characteristic value detection, checksum detection, heuristic detection, and active defense technology [8-9]. However, the previous methods rely too much on the experience of security experts, rules, and knowledge bases, making it difficult to accurately identify APT attacks’ behavior. In addition, the existing methods lack automatic and intelligent detection capabilities, and they further reduce detection accuracy when facing malware with obfuscation, packing, evasion, and deception techniques.


<mark>**总结：** 尽管相比于翻译软件，其描述取得一定改进。然而，整个描述仍然存在一些问题，比如“特征值检测技术、校验和检测技术、启发式检测技术”可以修改为“signature-based detection, checksum-based detection, heuristic detection”，再比如英文通常会说“However, these traditional methods have some limitations…”，甚至是“state-of-the-art”来表现现有先进方法。</mark>

---


## 三.中文ChatGPT小试牛刀

在利用ChatGPT进行润色之前，我们先介绍中文ChatGPT的常用方法。以最近非常火的网图为例，中文ChatGPT主要应用如下：

### 1.文心一言

来源百度，官方网址：

其运行的主界面如下图所示：

(1) 首先我们对其进行简单提问。

（2）对其进行编程提问。

生成代码如下：

```
def quick_sort(arr):  
    if len(arr) &lt;= 1:  
        return arr  
    else:  
        pivot = arr[len(arr) // 2]  
        left = [x for x in arr if x &lt; pivot]  
        middle = [x for x in arr if x == pivot]  
        right = [x for x in arr if x &gt; pivot]  
        return quick_sort(left) + middle + quick_sort(right)  
  
numbers = [9, 5, 2, 6, 1, 4]  
sorted_numbers = quick_sort(numbers)  
print(sorted_numbers)

```

（3）接下来给出一个更为复杂的问题。

生成的代码如下：

```
@echo off  
setlocal  
  
set /p folder_path=请输入文件夹路径:   
set /p project_name=请输入项目名称:   
  
set "counter=0"  
for %%F in ("%folder_path%*") do (  
    set /a "counter+=1"  
    set "file_name=%%~nxF"  
    set "extension=%%~xF"  
    set "new_file_name=%project_name%-%%~tF-!counter:~-2!!extension!"  
    ren "%%F" "!new_file_name!"  
)  
  
echo 重命名完成。  
pause

```

文心全景框架如下图所示：

此外，读者还可以尝试插件开发。

<mark>**总结：** 文心一言尽管能实现一定程度的智能问答，但也存在一些问题。比如“反向工程”应修改为更加准确的“逆向工程”，再如恶意代码分析的这几个步骤不能简单的罗列，其描述也存在不准确的问题，动态分析和静态分析可归为一类。在代码方面效果还不错，能够生成直接运行的快速排序代码，但仍然存在需要提升的地方，比如复杂逻辑的代码。</mark>

---


### 2.讯飞星火

来源科大讯飞，官方网址：

其运行的主界面如下图所示：

（1）首先我们对其进行简单提问。

（2）对其进行编程提问。

（3）接下来给出一个更为复杂的问题及输出结果。

<mark>**总结：** 讯飞回答相对简洁，同样能实现各种功能。比如，在论文润色方面，文心一言润色后相对啰嗦，而讯飞回答会直接给出答案，当然前者会告诉你为什么这样，适用于初学者理解；后者更方便解决实际问题，见人见面。</mark>

---


### 3.其它

还有其它的中文应用，除了大公司还有一些小团队或个人的尝试。比如：

---


### 4.New bing及ChatGPT推荐

当然还有两个非常重要的应用推荐，即——New bing和ChatGPT。

读者在使用过程中会遇到部分问题，需要学会独立解决喔。

---


## 四.中文ChatGPT润色尝试

最后，我们将尝试利用中文ChatGPT对前面的句子进行润色。

### 1.什么是Prompt

有人说，Prompt（提示词）是AI 2.0时代打开大模型能力的金钥匙，它能够大大的提高现有工作的小珞，在百度内部沟通会中，李彦宏预测“十年后全世界所有工作当中50%的内容恐怕都是写Prompt”。随后，衍生出提示学习（Prompt Learning）。

什么是Prompt（提示）。类似于搜索引擎，根据输入就可以给出对应的提示，利用提示让人想起具体的事情。提示学习将这种模式植入到大语言模型训练中，类似于激发人类一样来激发大语言模型的学习能力。

下图是浙大许文豪老师，非常棒地解释了什么是Prompt。如果把大语言模型具象成一个员工，那Prompt提示词则好比是你给员工下的指令，给出指令越详细，员工执行的结果就越好。

Prompt作为一种用于与人工智能模型进行交互的文本输入方式，它可以帮助用户更容易地与模型进行对话、提问或请求特定的信息。典型的应用场景如下：

常见的提示词框架如下：

---


### 2.论文润色Prompt

在英文润色中，常见的Prompt如下（后续会长期更新），参考Google和知乎（见后）。

---


### 3.润色示例

> 
I need you to act as a professional English paper teacher, please help me to improve my expression in the style of academic writing. Here is my expression: <br/><br/> Traditional APT attack detection identifies malicious attacks or analyzes attack behaviors by constructing rules, or security analysts utilize reverse analysis to extract features. Typical detection techniques include characteristic value detection, checksum detection, heuristic detection, and active defense technology [8-9]. However, the previous methods rely too much on the experience of security experts, rules, and knowledge bases, making it difficult to accurately identify APT attacks’ behavior. In addition, the existing methods lack automatic and intelligent detection capabilities, and they further reduce detection accuracy when facing malware with obfuscation, packing, evasion, and deception techniques.


文心一言润色如下：

> 
Traditional methods for detecting APT (Advanced Persistent Threat) attacks involve the use of rules-based systems or security analysts who reverse engineer the malicious software to extract features. Common techniques include signature-based detection, checksum-based detection, heuristic detection, and proactive defense measures [8-9]. However, these traditional methods have significant limitations, as they rely heavily on the experience of security experts, rules-based knowledge bases, and manual analysis processes. They often struggle to accurately identify the behavior of advanced persistent threats, particularly when facing advanced obfuscation, packing, evasion, and deception techniques used by attackers.


讯飞星火润色如下：

> 
Traditional Automatic Patching (APT) attack detection relies on constructing rules or employing reverse analysis to identify malicious attacks and analyze their behavior. Common detection techniques include characteristic value detection, checksum detection, heuristic detection, and active defense technology [8-9]. However, these methods heavily rely on the experiences of security experts, rules, and knowledge bases, making it challenging to accurately recognize APT attacks’ patterns. Furthermore, these existing approaches lack automatic and intelligent detection capabilities, which can further decrease detection accuracy when confronted with malware employing obfuscation, packing, evasion, and deception techniques.


星河超算ChatGPT润色如下：

> 
Traditional APT attack detection aims to identify malicious attacks and analyze attack behaviors through the construction of rules or the utilization of reverse analysis by security analysts. Commonly used detection techniques involve characteristic value detection, checksum detection, heuristic detection, and active defense technology [8-9]. However, these conventional methods heavily rely on the expertise of security professionals, predefined rules, and knowledge bases, thus posing challenges in accurately identifying the behavior of APT attacks. Moreover, existing approaches lack automatic and intelligent detection capabilities, which further compromises their effectiveness in detecting malware with obfuscation, packing, evasion, and deception techniques.


---


## 五.结束语

<mark>**总结：** 上述润色有一定提升，但还是存在不准确的地方，建议大家在撰写英文论文时，还是以自身为主，多阅读顶会顶刊论文，学习别人更地道的句子和专有名词。此外，撰写的时候就采用英文语气会更佳，按照每个段落来描述会更好。他山之石可以攻玉，ChatGPT尽管给大家带了众多便利，但仍然存在很多问题，如版权、安全等，切勿直接用ChatGPT来撰写论文、作业、文稿等，知识产权还需要大家共同维护，并且学术问题也是底线问题。</mark>

写到这里，文章就结束了。本文主要带领大家了解什么是LLM、AIGC和ChatGPT，并熟悉中文ChatGPT的应用，简单用论文润色进行对比。基础性文章，希望对您有所帮助。后续作者会继续深入，分享更多LLM和AIGC内容，加油，共勉！

> 
祝天下所有教师节日快乐！三尺讲台三寸笔，从小受到很多老师的教诲和解惑，师恩难忘，唯有感恩。祝长辈和老师们节日快乐，阖家幸福，身体健康，桃李满天。<br/> ​回想，自己2014年站在支教的讲台前，到2016年成为大学教师开启暑假实训，再到网上博客授课和答疑，以及完成自己的一门门课程，这已经是自己的第10个教师节。站在讲台前的那一刻，真的好想把自己所学所感分享出去。那些年，我刚成为老师，和老于成立了V创联盟，我俩自愿教60多名学生写代码，三年，下班后、周末，许许多多贵财凌晨三点都有我们的身影，办公室深夜更是灯火通明，熬夜训练，分组比赛，无数个日夜陪伴着学生编程，真想一头扎进家乡贵州的教育。很遗憾，后来两人读博V创联盟解散，随着年龄增长激情淡出；但也很幸运，那些年培养的学生出来都找到了很好的工作，拥有很棒的编程经历，更是留下深厚的友谊。这两周，还接到之前学生成为贵州这边某单位编程负责人，也有放弃稳定的事业单位选择去高校当老师的故事，以及正在读博未来争取成为老师的期盼。回首，自己给14级到18级，五个年级的学生上过课，有二十多人成为了教师，分散在贵州小学、中学、高职、大学的经历，我们亦师亦友，还挺开心的。这些年出来读博，更多是线上分享和博客分享，期待重回讲台的那一天，希望自己能记住当老师的那份美丽，记住你们脸上洋溢的笑容。不忘初心，知行合一，止于至善，砥砺前行。当教师最美的经历永远是和女神一起站在讲台前的那天，我来分享编程，你课间为我大四学生分享考公的经验。哈哈，都是青春，都是记忆，无悔，加油 <br/><br/> <img alt="在这里插入图片描述" height="360" src="https://img-blog.csdnimg.cn/884dddb999f9445187be8174b7f6b9e3.jpeg#pic_center" width="600"/>


(By:Eastmount 2023-09-18 夜于贵阳 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )

---


参考文献如下，感谢各位老师和大佬，也推荐大家阅读。
