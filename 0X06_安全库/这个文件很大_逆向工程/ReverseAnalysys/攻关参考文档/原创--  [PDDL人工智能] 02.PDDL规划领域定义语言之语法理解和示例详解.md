# 原创
：  [PDDL人工智能] 02.PDDL规划领域定义语言之语法理解和示例详解

# [PDDL人工智能] 02.PDDL规划领域定义语言之语法理解和示例详解

<mark>**智能规划来源于人工智能对理性行为研究，针对一个具体行为的实施目的，开展动作规划以模拟或指导行为的实施，是AI开展研究的关键问题。本文主要介绍PDDL规划领域定义语言的基础语法知识和示例详解，希望文章对您有所帮助。**</mark>

> 
该系列文章主要介绍PDDL人工智能知识，由于自己也是初学者，所以欢迎大家批评指正，一起加油喔~


#### 文章目录

**前文赏析：**

---


## 一.PDDL简介

<mark>**PDDL：Planning Domain Deﬁnition Language**</mark><br/> 规划领域定义语言，是一种解决人工智能规划问题的比较前沿的方法。PDDL常用于解决智能规划问题，例如机器人将一个屋子的球搬运到另一间屋子里，通过合理的组织流程建造一间房子等。PDDL的规划任务（planning task）由五部分组成。

定义规划任务的三要素：

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


## 二.PDDL基础语法知识

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


## 三.入门示例详解

### 1.domain.pddl

<mark>规划域描述整个规划世界的基础构成信息，包括实体类型、实体实例化、谓词逻辑状态、实体状态等。</mark>

示例代码如下：

```
(define (domain gripper-strips)
    (:requirements :negative-preconditions)
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

---


### 2.question.pddl

<mark>问题域描述在这样的规划世界中一个具体问题的条件与目标。</mark>

示例代码如下：

```
(define (problem solve)
    (:domain gripper-strips)
    (:objects 
        rooma roomb 
        ball1 ball2 
        left right
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

---


### 3.运行结果

运行结果如下图所示：

```
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
Total time: 4.47035e-10
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
Total time: 4.47035e-10
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
Planner found 1 plan(s) in 0.689secs.

```

另一种运行结果如下图所示：

---


### 4.逻辑解释

下面介绍该实例的智能规划逻辑。

**前提**：假设存在两个房间（Rooma和Roomb）、两个球（Ball1和Ball2）机器人拥有左手和右手，且机器人和球初始位置是Rooma房间。

**问题**：如何将两个球从房间Rooma移动到Roomb？

第一步：机器人左手提起Ball1。

第二步：机器人移动至Roomb。

第三步：机器人释放Ball1。

第四步：机器人移动至Rooma。

第五步：机器人右手拿起Ball2。

第六步：机器人移动至Roomb。

第七步：机器人右手释放Ball2。

至此，完成移动任务。PDDL显示的逻辑路线如下图所示：

另一种方法如下图所示：机器人捡起两个球移动再释放。

---


## 四.总结

写到这里，本文就介绍完毕，希望对您有所帮助。

大学之道在明明德，<br/> 在亲民，在止于至善。<br/> 这周又回答了很多博友的问题，有大一学生的困惑，有论文的咨询，也有老乡和考博的疑问，还有无数博友奋斗路上的相互勉励。虽然自己早已忙成狗，但总忍不住去解答别人的问题。最后那一句感谢和祝福，永远是我最大的满足。虽然会花费我一些时间，但也挺好的，无所谓了，跟着心走。不负遇见，感恩同行。莫愁前路无知己，继续加油。晚安娜和珞。

(By:Eastmount 2023-01-12 夜于贵阳 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )

---


**参考文献：**

---

