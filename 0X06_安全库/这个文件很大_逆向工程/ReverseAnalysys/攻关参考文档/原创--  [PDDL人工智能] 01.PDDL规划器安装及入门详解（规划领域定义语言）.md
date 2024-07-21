# 原创
：  [PDDL人工智能] 01.PDDL规划器安装及入门详解（规划领域定义语言）

# [PDDL人工智能] 01.PDDL规划器安装及入门详解（规划领域定义语言）

该系列文章主要介绍PDDL人工智能知识，由于自己也是初学者，所以欢迎大家批评指正，也希望文章对您有所帮助，一起加油喔~

#### 文章目录

## 一.PDDL简介

<mark>**PDDL：Planning Domain Deﬁnition Language**</mark><br/> 规划领域定义语言，是一种解决人工智能规划问题的比较前沿的方法。PDDL常用于解决智能规划问题，例如机器人将一个屋子的球搬运到另一间屋子里，通过合理的组织流程建造一间房子等。PDDL的规划任务（planning task）由五部分组成。

PDDL指定的规划任务分为两个文件，`domain` 文件和 `problem` 文件。
1. A **domain file（域文件）** for predicates and actions.1. A **problem file（问题文件）** for objects, initial state and goal specification.
<mark>**Domain Files**</mark><br/> 域文件定义如下：

```
(define (domain &lt;domain name&gt;)
	&lt;PDDL code for predicates&gt;
	&lt;PDDL code for first action&gt;
	[...]
	&lt;PDDL code for last action&gt;
)

```

<mark>**Problem Files**</mark><br/> 问题文件定义如下：

```
(define (problem &lt;problem name&gt;)
	(:domain &lt;domain name&gt;)
	&lt;PDDL code for objects&gt;
	&lt;PDDL code for initial state&gt;
	&lt;PDDL code for goal specification&gt;
)

```

> 
推荐下列参考资料：



---


## 二.VSCode安装教程

**第一步，在VSCode中安装PDDL插件。**

安装成功后如下图所示：

**第二步，在VSCode中安装VAL语法检查。**

在VS Code安装PDDL成功后，打开PDDL Overview进行安装VAL。

安装过程如下图所示，对应“VAL tools”点击“Download”。

**第三步，安装成功之后，可以看到详细的配置信息如下：**

此时，我们可以进入后续的PDDL代码编写过程。同时，存在部分在线访问的问题，后续尝试解决。

**VSCode PDDL**<br/> VSCode是建模规划域的良好工具，其扩展将PDDL引入了一流的编程语言家族中，让其与C#、Python、JavaScript等处于同等级别。PDDL旨在通过以下功能帮助新手并为专家赋能。

当扩展在 `.pddl` 文件（通常包含域或问题定义）或选定的PDDL命令被激活时，即可配置解释器（parser）或规划器（planner）。

---


## 三.PDDL基础案例快速入门

### 1.Getting started

创建PDDL文件并运行计划程序，下面是官方最简单的入门步骤：

同时补充一个在线编辑PDD的网站。

---


### 2.创建文件

第一步，点击 `File &gt; Open Folder` 打开VS Code中一个空白文件夹。

第二步，点击 `File &gt; New File` 创建两个名为 `domain.pddl` 和 `question.pddl` 的空白文件，这两个文件将显示在资源管理器窗格中，在编辑器中并排打开它们。

第三步，打开 `domain.pddl` 文件并输入 `domain`。自动模板会插入域文件的整个结构，使用 Tab 和 Enter 键跳过占位符并进行选择。

第四步，打开 `problem.pddl` 文件并输入 `problem` 。自动模板会插入问题文件的整个结构，确保此处的（域名）与域文件中选择的名称匹配。

此时的代码如下所示，仅仅是一个PDDL规划问题的框架基础代码。

<mark>**domain.pddl**</mark>

```
;Header and description

(define (domain domain_name)

;remove requirements that are not needed
(:requirements :strips :fluents :durative-actions :timed-initial-literals :typing :conditional-effects :negative-preconditions :duration-inequalities :equality)

(:types ;todo: enumerate types and their hierarchy here, e.g. car truck bus - vehicle
)

; un-comment following line if constants are needed
;(:constants )

(:predicates ;todo: define predicates here
)

(:functions ;todo: define numeric functions here
)

;define actions here
)

```

<mark>**problem.pddl**</mark>

```
(define (problem problem_name) (:domain domain_name)
(:objects 
)

(:init
    ;todo: put the initial state's facts and numeric values here
)

(:goal (and
    ;todo: put the goal condition here
))

;un-comment the following line if metric is needed
;(:metric minimize (???))
)

```

---


### 3.调用在线规划器

由于作者不太会安装VAL工具（学习中），所以先介绍在线运行的过程。

> 
当准备在域和问题文件上运行规划器时，可以通过文件内容或编辑器选项卡的上下文菜单，或通过 `Alt + P` 快捷方式调用规划器（planner）。这个 `planning.domains` 解析器将将被使用，因此不要发送任何敏感的PDDL代码。



第一步，编写一个 `domain` 示例代码。

<mark>domain.pddl</mark>

```
(define (domain gripper-strips)
    (:predicates (room ?r)
                (ball ?b)
                (at ?b ?r)
                (at-robby ?r)
                (gripper ?g)
                (free ?g)
                (carry ?b ?g))
    (:action pick
        :parameters(?obj ?room ?gripper)
        :precondition 
            (and 
                (ball ?obj) 
                (room ?room) 
                (gripper ?gripper)
                (free ?gripper) 
                (at ?obj ?room) 
                (at-robby ?room)
            )
        :effect 
            (and 
                (not 
                    (free ?gripper)
                ) 
                (carry ?obj ?gripper) 
                (not (at ?obj ?room))
            )
        )
    (:action move
        :parameters (?from ?to)
        :precondition 
            (and 
                (room ?from) 
                (room ?to) 
                (at-robby ?from)
            )
        :effect 
            (and 
                (at-robby ?to)
                (not 
                    (at-robby ?from)
                )
            )
    )
    (:action drop
        :parameters (?room ?ball ?gripper)
        :precondition
            (and 
                (room ?room) 
                (ball ?ball) 
                (at-robby ?room) 
                (gripper ?gripper)
                (not 
                    (free ?gripper)
                ) 
                (carry ?ball ?gripper)
                (at-robby ?room))
        :effect 
            (and 
                (free ?gripper) 
                (at ?ball ?room) 
                (not
                    (carry ?ball ?gripper)
                )
            )
        )
)

```

第二步，编写一个 `problem` 示例代码。

<mark>question.pddl</mark>

```
(define (problem solve)
    (:domain gripper-strips)
    (:objects 
        rooma roomb ball1 ball2 left right
    )
    (:init 
        (room rooma)
        (room roomb) 
        (ball ball1) 
        (ball ball2)
        (gripper left) 
        (gripper right) 
        (free left) 
        (free right)
        (at ball1 rooma) 
        (at ball2 rooma) 
        (at-robby rooma)
    )
    (:goal 
        (and
            (at ball1 roomb)
            (at ball2 roomb)
        )
    )
)

```

第三步，如果右键选中直接运行或按 `alt+P` 运行planner，它会提示错误。

第四步，点击如下图所示的“Test”按钮并创建。

生成如下图所示的测试树状图，并且准备调用远程的 `planning.domains` 服务器

第五步，点击“domain”右边的三角运行按钮，生成如下图所示的测试报告。注意，右边的“Planner output”是一个规划路径的输出方案。

同时，我们可以双栏打开窗口。

<font color="red">**注意，其实我们直接在Explorer中选中PDDL文件，右键“PDDL: Run the planner and display the plan”按钮可以生成运行的规划结果。不需要创建Test，如下图所示，但仍然会进行远程连接。**</font>

至此，我们线上运行PDDL规划器的方法介绍完毕。该代码具体的含义及基础语法我们将在第四部分详细介绍。

```
Finished executing tests from VSCode/PDDL-test01/domain.ptest.json.
Planning service: http://solver.planning.domains/solve
Domain: gripper-strips, Problem: solve
 --- OK.
 Match tree built with 18 nodes.

PDDL problem description loaded: 
	Domain: GRIPPER-STRIPS
	Problem: SOLVE
	#Actions: 18
	#Fluents: 14
Landmarks found: 2
Starting search with IW (time budget is 60 secs)...
rel_plan size: 5
#RP_fluents 7
Caption
{#goals, #UNnachieved,  #Achieved} -&gt; IW(max_w)

{2/2/0}:IW(1) -&gt; [2][3][4]rel_plan size: 4
#RP_fluents 6
{2/1/1}:IW(1) -&gt; [2][3][4][5][6][7];; NOT I-REACHABLE ;;
Total time: -8.64267e-10
Nodes generated during search: 42
Nodes expanded during search: 35
IW search completed
Starting search with BFS(novel,land,h_add)...
--[4294967295 / 8]--
--[1 / 8]--
--[1 / 6]--
--[1 / 5]--
--[1 / 4]--
--[1 / 2]--
--[1 / 1]--
--[1 / 0]--
--[0 / 0]--
Total time: -1.51992e-09
Nodes generated during search: 22
Nodes expanded during search: 7
Plan found with cost: 7
BFS search completed
0.00100: (pick ball2 rooma left)
0.00200: (move rooma roomb)
0.00300: (drop roomb ball2 left)
0.00400: (move roomb rooma)
0.00500: (pick ball1 rooma left)
0.00600: (move rooma roomb)
0.00700: (drop roomb ball1 left)
☑ question.pddl (1.176 sec)
Planner found 1 plan(s) in 1.177secs.

```

> 
补充知识点<br/> 在VS Code中，如果PDDL存在错误或警告，我们是可以动态修复的，比如上述代码存在 “Undeclared requirement :negative-preconditions” 问题。<br/><br/> <img alt="在这里插入图片描述" height="350" src="https://img-blog.csdnimg.cn/20210608112845946.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="550"/><br/> 点击则可以进行修复，“Add missing requirement: negative preconditions”。<br/><br/> <img alt="在这里插入图片描述" height="250" src="https://img-blog.csdnimg.cn/20210608113350884.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="450"/><br/> 修改后如下图所示。<br/><img alt="在这里插入图片描述" height="220" src="https://img-blog.csdnimg.cn/20210608114254961.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="450"/>


---


### 4.本地规划器配置

上述方法非常依赖网络，因此官方不建议将私有代码上传运行。如果断网情况下，它会提示错误“Error: PDDL Planning Service returned code 400 Invalid header received from client”。

此时，我们需要尝试配置您自己的 PDDL 规划器，并在本地能运行代码。

<font color="red">**作者尝试本地配置规划器一直失败，希望了解的读者能评论分享下，谢谢。**</font>

第一步，点击“Show configuration”，接着会显示PDDL Overview页面，显示“AI Planning and PDDL support in VS Code”。

该页面显示的配置信息如下：

第二步，点击“Add a PDDL Planner”按钮，添加规划引擎。

选择POPF选项，选择对应的popf.exe程序（未找到）。

第三步，继续点击“Planning engine”后面的加号，添加本地请求的网址。

<mark>最后还是失败，o(╥﹏╥)o，知道的大佬救救孩子吧！</mark>

---


## 四.PDDL基础语法知识

四个球移动的任务（Gripper task with four balls）：假设有一个机器人可以在两个房间之间移动，并用两条手臂中的任何一条捡起或扔掉球。 最初，所有球和机器人都在第一个房间，我们希望移动球到第二个房间中。

接下来简单给出基本定义，由于其英语直译非常明确，这里仅给出描述，下一篇文章再进行详细的语法介绍。

<mark>**Gripper task: Objects**</mark>

<mark>**Gripper task: Predicates**</mark>

<mark>**Gripper task: Initial state**</mark>

<mark>**Gripper task: Goal specification**</mark>

<mark>**Gripper task: Movement operator**</mark>

<mark>**Gripper task: Pick-up operator**</mark>

<mark>**Gripper task: Drop operator**</mark>

**A Note on Action Effects**

---


## 五.总结

写到这里，本文就介绍完毕，希望对您有所帮助。

大学之道在明明德，<br/> 在亲民，在止于至善。<br/> 这周又回答了很多博友的问题，有大一学生的困惑，有论文的咨询，也有老乡和考博的疑问，还有无数博友奋斗路上的相互勉励。虽然自己早已忙成狗，但总忍不住去解答别人的问题。最后那一句感谢和祝福，永远是我最大的满足。虽然会花费我一些时间，但也挺好的，无所谓了，跟着心走。不负遇见，感恩同行。莫愁前路无知己，继续加油。晚安娜和珞。

---


**参考文献：**
