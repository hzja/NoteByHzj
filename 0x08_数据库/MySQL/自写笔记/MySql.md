---
title: MySql
tags: []
---

视频地址:
        https://www.bilibili.com/video/BV1iq4y1u7vj/?spm_id_from=333.999.0.0&vd_source=1e325091774aa31c4dcd65d8667c69de
    
    配套资料地址:
        https://pan.baidu.com/s/1jUPQqXmk6x0YbSTvCu9ZRg
        提取码:yyds

[参考网站](https://dev.mysql.com/doc/refman/8.0/en)
        https://www.cs.usfca.edu/~galles/visualization/about.html
    
    教材:
        MySQL是怎样运行的
     
     笔记地址:
        1、https://gitee.com/zwhddup/mysql-learning/tree/master
        2、https://gitee.com/MingZii/learn_db/tree/master
    
    Mysql8.0
        https://pan.baidu.com/s/1cpSdi5UjTWrIFlXI-2IvOw
        提取码: 8m46
    
    Mysql5.7
        https://pan.baidu.com/s/1vlhd_2jMDqcbR2uEWMuyxw
        提取码: dxdw
    
    Mysql5.6
        https://pan.baidu.com/s/1pBH7Pt2BUkfusywYxRqSlQ
        提取码: k7vc
    
        https://pan.baidu.com/s/1UY_uSFbLGBSEzc77jGrtfw 
        提取码: d1ha


## 第一章 数据库概述

### 1、为什么要使用数据库

+ 持久化(persistence):把数据保存到可掉电式存储设备中以供之后使用,大多数情况下特别是企业级应用,数据持久化意味着将内存中的数据保存到硬盘上加以“固化",而持久化的实现过程大多通过各种关系数据库完成.
+ 持久化的主要作用是将内存中数据存储在关系型数据库中,当然也可存储磁盘文件、XML数据文件.
  
### 2、数据库与数据库管理系统

#### 2.1 数据库的相关概念

+ DB,数据库(Database):即存储数据的"仓库",本质是一个文件系统，保存了一系列有组织的数据.
  
+ DBMS,数据库管理系统(Database Management System):是一种操纵和管理数据库的大型软件，用于建立使用和维护数据库,对数据库进行统一管理和控制,用户可通过数据库管理系统访问数据库中表内数据.就是日常所用软件
  
+ SQL,结构化查询语言(Structured Query Languange):专门用于与数据库通信的语言
  
#### 2.2 数据库与数据库管理系统的关系

+ 数据库管理系统(DBMS)可以管理多个数据库,一般开发人员会针对每一个应用创建一个数据库。为保存应用中实体的数据一般会在数据库中创建多个表以保存程序中实体用户数据.
  
#### 2.3 常见的数据库管理系统排名(DBMS)

+ 目前互联网上常见数据库管理软件有Oracle、MySQL、MS SQL Server、DB2、PostgreSQL、Access、Sybase、Informix这几种([查看数据库最新排名](https://db-engings.com/en/ranking))
+ [对应走势图](https://db-engines.com/en/ranking_trend)
        
#### 2.4 常见数据库介绍

+ **Oracle**

    - 1942年Oracle2诞生，是第一个商用RDBMS(关系型数据库管理系统),随着Oracle软件名气越来越大公司也改名叫Oracle公司.
    - 2007年总计85亿美金收购BEA Systems.
    - 2009年总计74亿美金收购SUN,此前的2008年SUN以10亿美金收购MySQL,意味着Oracle同时拥有了MySQL的管理权,至此Oracle在数据库领域中成为绝对领导者.
    - 2013年甲骨文超越IBM成为继Microsoft后全球第二大软件公司.
    - 如今Oracle年收入达到400亿美金足以证明商用收费数据库软件的价值.

+ **SQL Server**

    - SQL Server是微软开发的大型商业数据库,诞生于1989年;C#、.net等语言常使用,与WinNT完全集成也可很好与Microsoft BackOffice产品集成.
    
+ **DB2**

    - IBM公司的数据库产品,收费,常用于银行系统中.

+ **PostgreSQL**

    - PostgreSQL稳定性极强且最符合SQL标准且开放源码,具有商业级DBMS质量,PG对数据量大的文本以及SQL处理较快

+ **SyBase**
  
    - 已经淡出历史舞台,提供了一个非常专业数据建模的工具PowerDesigner.
    
+ **SQLite**

    - 嵌入式小型数据库,应用于手机端.零配置,SQLite3不用安装、不用配置、不用启动,关闭或配置数据库实例.当系统崩溃后不用作任何恢复操作在下次使用数据库时自动恢复.

+ **informix**

    - IBM公司出品,取自Information和Unix的结合,是第一个被移植到Linux的商用数据库产品,仅运行于unix/linux平台,命令行操作.性能较高,支持集群,适应于安全性较高的系统,尤其是银行、证券系统的应用.

### 3、MySQL介绍

#### 3.1概述

+ MySQL是一个_开放源代码的关系型数据库管理系统_，由瑞典MySQL AB(创始人Michael Widenius)公司开发,迅速成为开源数据库的No.1
+ 2008被Sun收购(10亿美金),2009年Sun被Oracle收购.MariaDB应运而生(MySQL的创造者担心MySQL有闭源风险,因此创建了MySQL的分支项目MariaDB)
+ MySQL6.x版本之后分为_社区版_和_商业版_
+ MySQL是一种关联数据库管理系统,将数据保存在不同的表中而不是将所有数据放在一个大仓库内,如此增加了速度并提高了灵活性.
+ MySQL是开源的故不需支付额外费用;
+ MySQL可定制,采用了GPL(GNU General Public License)协议,故可修改源码开发属于自己的MySQL系统.
+ MySQL支持大型数据库,可处理拥有上千万条记录的大型数据库,支持5000万条记录的数据仓库,32位系统表文件最大可支持4GB,64位系统支持的最大表文件为8TB;
+ MySQL使用标准的SQL数据语言形式,并不完全标准;
+ MySQL可允许运行于多个系统并支持多种语言,包括C、C++、Python、Java、Perl、PHP和Ruby等;

#### 3.2重大历史时间

+ 2000年,ISAM转为MyISAM存储引擎，同年MySQL开放了源代码且基于GPL许可协议;
+ 2010年,MySQL5.5发布,InnoDB引擎作为MySQL默认存储引擎;
+ 2015年,MySQL5.7 GA版本发布,是稳定版本;
+ 2016年,MySQL发布里程碑版本MySQL8.0.0

#### 3.3关于MySQL 8.0

MySQL从5.7版本直接跳跃发布了8.0版本,可见是一个令人兴奋的里程碑版本;MySQL 8版本在功能上做了显著改进与增强,开发者对MySQL源代码进行了重构,最突出一点是多MySQL Optimizer优化器进行了改进,不仅速度上得到改善,还为用户带来了更好性能和更棒体验.

#### 3.4为何选择MySQL

1. 开放源代码,使用成本低;
2. 性能卓越,服务稳定;
3. 软件体积小,使用简单且易于维护;
4. 历史悠久,社区用户非常活跃,遇到问题可寻求帮助;
5. 许多互联网公司在用,经过了时间的验证;

#### 3.5 Oracle vs MySQL

+ OracLe更适合大型跨国企业使用,其对费用不敏感,但对性能要求及安全性要求更高;
+ MySQL由于体积小、速度快、总体拥有成本低,可处理上千万条记录的大型数据库,尤其是开放源码这一特点,使得很多互联网公司、中小型网站选择了MySQL作为网站数据库(Facebook、Twitter、YouTube、阿里巴巴/蚂蚁金服、去哪儿网、美团外卖、腾讯)

看到了[第四讲](https://www.bilibili.com/video/BV1iq4y1u7vj/?p=4&spm_id_from=pageDriver&vd_source=1e325091774aa31c4dcd65d8667c69de)
