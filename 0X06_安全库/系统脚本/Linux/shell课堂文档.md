1、课程简介

	课程共计两阶段八部分。
	
	第一阶段：shell编程
		1.1 shell快速入门
			运维(了解)、shell简介
		1.2 shell基础
			shell脚本、三种变量
		1.3 shell进阶
			测试语句、表达式、linux常见符号、linux常用命令
		1.4 shell流程控制语句
			简单流程控制、复杂流程控制
	
	第二阶段：代码发布
		2.1 代码部署
			部署定义、方式、流程
		2.2 服务器环境部署
		2.3 手工方式部署代码
		2.4 脚本方式部署代码
		
	时间安排：
		第一天： 1.1-1.3 
		第二天： 1.4-2.2
		第三天： 2.4

		
第 1 章 SHELL 快速入门

	1.1 运维
	1.2 shell简介	
		1.2.1 什么是shell
			命令解释器
		1.2.2 shell的分类
			图形界面shell和命令行shell(bash)
		1.2.3 shell 脚本	
			使用：
				手工
				脚本 
					#!/bin/bash
					# linshi
					echo $SHELL
					cat /etc/shells	

第 2 章 SHELL 基础知识	

	2.1 shell脚本	
		2.1.1 创建脚本
			脚本创建工具：各种编辑工具都可
			脚本命名：有意义
			脚本内容：可执行
			注释：
				单行： #
				多行：:<<字符 ... 字符
				
		2.1.2 脚本执行	
			1	/bin/bash /path/to/script-name
				特点： 生产强烈推荐		*****
			2	/path/to/script-name
				特点：执行权限			****
			3	source script-name
				特点：环境一致性		***
				
		2.1.2 脚本开发规范
			1、脚本命名要有意义，文件后缀是.sh
			2、脚本文件首行是而且必须是脚本解释器
			#!/bin/bash
			3、脚本文件解释器后面要有脚本的基本信息等内容
			脚本文件中尽量不用中文注释;
					尽量用英文注释，防止本机或切换系统环境后中文乱码的困扰
			常见的注释信息：脚本名称、脚本功能描述、脚本版本、脚本作者、联系方式等
			4、脚本文件常见执行方式：bash 脚本名
			5、脚本内容执行：从上到下，依次执行
			6、代码书写优秀习惯;
				1）成对内容的一次性写出来,防止遗漏。
			如：()、{}、[]、''、``、""
				2）[]中括号两端要有空格,书写时即可留出空格[    ],然后再退格书写内容。
				3）流程控制语句一次性书写完，再添加内容
			7、通过缩进让代码易读;(即该有空格的地方就要有空格)	
	
	2.2 变量	
		2.2.1 什么是变量
			变量包括两部分：
				变量名+变量值
		2.2.2 本地变量
			普通变量：
				1 	变量名=变量值
					特点：整体无特殊字符
				2	变量名='变量值'
					特点：原字符输出
				3	变量名="变量值"
					特点：看情况
				示例：
					echo $SHELL
					echo $itcast
					itcast=nihao666
					echo $itcast
					itcast='nihao777'
					echo $itcast
					itcast="nihao888"
					echo $itcast
					echo $itcast1
					itcast1=nihao 666
					echo $itcast1
					itcast1='nihao 777'
					echo $itcast1
					itcast1="nihao 888"
					echo $itcast1
					echo $itcast
					itcast2='dan-$itcast'
					echo $itcast2
					itcast2="shuang-$itcast"
					echo $itcast2
				
			命令变量
				1、	变量名=`命令`
					关键点：反引号
				2	变量名=$(命令)
					生产推荐
				示例：
					echo $dir
					pwd
					dir=`pwd`
					echo $dir
					dir1=$(pwd)
					echo $dir1		
				
				
		2.2.3 全局变量	
			查看：env
			定义：
				方法一：
					变量=值
					export 变量
				方法二：（最常用）
					export 变量=值
				示例：
					env | grep SHELL
					SHENGWEISHUJI=qiangge
					echo $SHENGWEISHUJI
					env | grep SHENG
					export SEHNGWEISHUJI
					env | grep SHENG
					export SHENGWEISHUJI
					env | grep SHENG
					export SHENGZHANG=longge
					echo $SHENGZHANG
					env | grep SHENG	
					
		2.2.4 变量查看和取消	
			查看：
				标准："${变量名}"
				其他：${变量名}、$变量名、"$变量名"
				示例：
					echo $SHENGZHANG
					echo "$SHENGZHANG"
					echo ${SHENGZHANG}
					echo "${SHENGZHANG}"
			取消：
				unset 变量名
				示例：
					env | grep SHENG
					unset SHENGWEISHUJI
					env | grep SHENG
					unset SHENGZHANG
					env | grep SHENG
					echo $itcast
					unset itcast
					echo $itcast
					echo $itcast1
					unset itcast1
					echo $itcast1
			
		2.2.5 shell内置变量	
			和脚本文件有关
				$0		当前执行文件的文件名
				$n		第n个位置的参数内容			
				$#		参数总数量
				$?		上一条指令的状态返回值
			示例：
				#!/bin/bash
				# wenjianming $0
				echo "dangqianwenjianming shi : get_name.sh"
				echo "dangqianwenjianming shi : $0"
				
				#!/bin/bash
				# huoqu weizhi canshu: $n
				echo "di 1 ge weizhi d canshu shi : $1"
				echo "di 2 ge weizhi d canshu shi : $2"
				echo "di 3 ge weizhi d canshu shi : $3"
				echo "di 4 ge weizhi d canshu shi : $4"				
			
				#!/bin/bash
				# huoqu zong shuliang: $#
				echo "dangqian jiaoben zhixing chuanrude canshuzonggeshu shi : $#"		

				echo $?
				kdalsfj
				echo $?
				ls
				echo $?	
		字符串精确截取
			格式：${变量名:起始位置:截取长度}
				关键点：起始位置从0开始
				示例：
					file=dksafslajfljdslkfjslkdajflkdsajlkfjdsa
					echo $file
					echo ${file:0:8}
					echo ${file:11:8}
					echo ${file:0-21:8}	

		默认值相关：
			有条件的默认：${变量名:-默认值}
			强制默认：${变量名+默认值}
			示例：
				#!/bin/bash
				# youtiaojianmoren: ${变量名:-默认值}
				a="$1"
				echo "nin xuanzede taocanshi: ${a:-1}"	

				#!/bin/bash
				# morenzhi changjing: ${变量名+默认值}
				a="$1"
				echo "国家法定结婚年龄(男性)是 ${a+22} 岁"				
第 3 章 SHELL 进阶	
	3.1 表达式	
		3.1.1 测试语句	
			测试语句形式
				A: test 条件表达式
				B: [ 条件表达式 ]
				关键点：表达式和[]两侧都有空格
			示例：
				[ 1 = 1 ]
				[ 123 = 123 ]
				[ a == a ]
				[ 1 = 1 ]
				echo $?
				[ 1 = 2 ]
				echo $?
				[ 1 = 2]
				[ 1 = 2] ]
				echo $?		
				
		3.1.2 条件表达式
			逻辑表达式
				命令1  &&  命令2
					特点：同林鸟
				命令1  ||  命令2
					特点：对着干
				示例：
					[ 1 = 1 ] && echo "chengli"
					[ 1 = 2 ] && echo "chengli"
					[ 1 = 1 ] || echo "buchengli"
					[ 1 = 2 ] || echo "buchengli"				
					
			文件表达式
				-f 		判断输入内容是否是一个普通文件
				-d 		判断输入内容是否是一个目录文件
				-x 		判断输入内容是否可执行
				示例：
					[ -f zhushi.sh ] && cat zhushi.sh 
					ls
					[ -d nihao ] || mkdir nihao
					ls
					[ -x moren1.sh ] || chmod +x moren1.sh		

			数值操作符
					n1 -eq n2            相等		*****
					n1 -gt n2            大于
					n1 -lt n2            小于
					n1 -ne n2            不等于		*****	

			字符串比较
				==			内容一致
				!=			内容不一致
				示例：
					[ a == a ]
					echo $?
					[ a == abc ]
					echo $?
					[ a != abc ]
					echo $?
					a=abc
					b=dec
					[ $a == $b ]
					echo $?	
					
		3.1.3 计算表达式
			1	$(( 计算表达式 ))
			2	let 计算表达式
				关键点：计算表达式是一个整体
			示例：
				echo $((3+3))
				echo $((3+39))
				a=$((3+39))
				echo $a
				a=$((a+1))
				echo $a
				let b=1+1
				echo $b
				let b=b+1
				echo $b			
	3.2 linux常见符号	
		3.2.1 重定向符号
			> 符号			覆盖
			>> 符号			追加
			示例：
				cat nihao.txt 
				echo 'nihao.txt' > nihao.txt 
				cat nihao.txt 
				echo 'nihao 3' >> nihao.txt 
				cat nihao.txt					
		3.2.2 管道符 |
			| 这个就是管道符，传递信息使用的
				关键点：从左向右传递
				示例：
					env | grep SHELL
					
		3.2.3 其他符号
			后台展示符号 & 
				& 就是将一个命令从前台转到后台执行
				示例：
					sleep 10 &
					ps | grep sleep
					sleep 10 &
					ps | grep sleep		
			全部信息符号 2>&1
				cat ceshi.sh  1>> ceshi-ok 2>> ceshi-nook
				cat ceshi-ok
				/bin/bash ceshi.sh  1>> ceshi-o 2>> ceshi-n
				cat ceshi-o
				cat ceshi-n
				/bin/bash ceshi.sh  1>> ceshi-all 2>> ceshi-all
				cat ceshi-all 
				/bin/bash ceshi.sh >> ceshi-all1 2>&1
				cat ceshi-all1
			linux系统垃圾桶：/dev/null
				/bin/bash ceshi.sh >> /dev/null 2>&1
				
				
			
	3.3 常见命令详解	
		3.3.1 grep命令详解
			格式：grep [参数] [关键字] <文件名>
			参数：
				-c 	计数
				-v 	取反
				-n	行号
			生产小技巧：
				grep -nr 关键字 路径
				
		3.3.2 sed命令详解
			格式：sed [参数] '<匹配条件> [动作]' [文件名]
			样式：
				sed -i '行号s#原内容#替换后内容#列号' [文件名]
					关键点：
						行号不写，表示所有行
						列号不写，表示第一列
				sed -i '行号s#原内容#替换后内容#g' [文件名]
			替换示例：
				sed 's#sed#SED#' sed.txt 
				cat sed.txt 
				sed -i 's#sed#SED#' sed.txt 
				cat sed.txt 
				sed -i '2s#sed#SED#2' sed.txt 
				cat sed.txt 
				sed -i 's#sed#SED#g' sed.txt 
				cat sed.txt 
				sed -i '2s#SED#sed#g' sed.txt 
				cat sed.txt 	
			增加操作
				当行增加
					sed -i '行号i\增加的内容' 文件名
				下一行增加
					sed -i '行号a\增加的内容' 文件名
			删除操作
				sed -i '行号d' 文件名
			示例：
				sed -i '1a\add-1' sed.txt 
				cat -n sed.txt 
				sed -i '1,3a\add-3' sed.txt 
				cat -n sed.txt 
				sed -i '1i\insert-1' sed.txt 
				cat -n sed.txt 
				sed -i '1,5i\insert-5' sed.txt 
				cat -n sed.txt 
				sed -i '3d' sed.txt 
				cat sed.txt 
				cat -n sed.txt 
				sed -i '1,9d' sed.txt			
		3.3.3 awk命令详解	
			格式：
				awk [参数] '[ 动作]' [文件名]
				打印：
					awk '{print $0}' awk.txt 
					awk '{print $1}' awk.txt 
					awk '{print $1,$3}' awk.txt 
					awk '{print $4,$2}' awk.txt 
					awk '{print NR,$4,$2}' awk.txt 
					awk 'NR==1 {print NR,$4,$2}' awk.txt
				格式化输入 	-F
				示例：
					awk -F ':' '{print $1,$7}' awk1.txt
				格式化输出：
					关键点；
						1、位置
						2、方式
				示例：
					awk -F ':' 'BEGIN{OFS="~~"} {print $1,$7}' awk1.txt

					
		3.3.4 find命令详解	
			格式：
				find [路径] [参数] [关键字]
				关键点：路径
			示例：
				find / -name "awk.txt"
				find /root -name "awk.txt"
				find /root -name "*.txt"
				ll /root/.mozilla/firefox/w4ameoja.default/AlternateServices.txt
				find /root -type f
				ll /root/.mozilla/firefox/w4ameoja.default/kinto.sqlite
				find /root -type d
				ll /root/.mozilla/firefox/w4ameoja.default/bookmarkbackups
				find /root -type f -name "*tx*"
				
			
			
第4章 流程控制	19
	4.1 简单流程控制语句
		4.1.1 单分支if语句
			格式：
				if [ 条件 ]
				then
					指令
				fi		
			示例：
				#!/bin/bash
				# 单if示例
				a="$1"
				if [ "${a}" == "nan" ]
				then
				  echo "男"
				fi
			
		4.1.2 双分支if语句
			格式：
				if [ 条件 ]
				then
					指令1
				else
					指令2
				fi
			示例：
				#!/bin/bash
				# 双if示例
				a="$1"
				if [ "${a}" == "nan" ]
				then
				  echo "男"
				else:
				  echo "女"
				fi		
				
		4.1.3 多分支if语句	
			格式：
				if [ 条件 ]
				then
					指令1
				elif [ 条件2 ]
				then
					指令2
				else
					指令3
				fi	
			示例：
				#!/bin/bash
				# 多if语句
				a="$1"
				if [ "${a}" == "nan" ]
				then
				  echo "男"
				elif [ "${a}" == "nv" ]
				then
				  echo "女"
				else
				  echo "请问黄哥哥..."
				fi			
		4.1.4 case选择语句	
			格式：
				case 变量名 in
				   值1)
					  指令1
						 ;;
				   值2)
					  指令2
						 ;;
				   值3)
					  指令3
						 ;;
				esac
			示例：
				#!/bin/bash
				# case生产案例脚本
				canshu="$1"
				case "${canshu}" in
				  "start")
					 echo "服务启动..."
				  ;;
				  "stop")
					echo "服务关闭..."
				  ;;
				  "restart")
					echo "服务重启..."
				  ;;
				  *)
					echo "脚本 $0 的使用方式: /bin/bash $0 [start|stop|restart]"
				  ;;
				esac			
		4.1.5 for循环语句
			格式：
				for 值 in 列表
				do
				   执行语句
				done	
			示例：
				#!/bin/bash
				# for遍历脚本
				for i in $(ls /root)
				do
				  echo "/root 目录下的文件有 : ${i}"
				done
			
		4.1.6 while循环语句	
			格式：
				while 条件
				do
				   执行语句
				done		
			示例：
				#!/bin/bash
				# while 死循环
				a=1
				while [ "${a}" -lt 5 ]
				do
				  echo "${a}"
				  a=$((a+1))
				done			
		4.1.7 until循环语句
			格式：
				until 条件
				do
				   执行语句
				done	
			示例：
				#!/bin/bash
				# until语句示例
				a=1
				until [ "${a}" -eq 5 ]
				do
				  echo "${a}"
				  let a=a+1
				done
			
	4.2 复杂流程控制语句
		4.2.1 函数基础知识	
			简单函数定义和调用
				格式：
					定义函数：
						函数名(){
							函数体
						}
					调用函数：
						函数名
				示例：
					#!/bin/bash
					# 简单函数定义和调用
					dayin(){
					  echo "你好 huang 哥"
					}
					dayin
					
			传参函数定义和调用
				格式：
					传参数
						函数名 参数 
					函数体调用参数：
						函数名(){
							函数体 $n
						}			
				示例：
					#!/bin/bash
					# 传参函数定义和调用
					dayin(){
					  echo "你好 $1 哥"
					}
					dayin 黄
					dayin 龙
					dayin jing	

			脚本传参、函数调用
				示例：
					#!/bin/bash
					# 脚本传参函数调用
					dayin(){
					  echo "你好 $1 哥"
					}
					dayin $1
			生产脚本函数写法
				#!/bin/bash
				# 生产脚本函数写法
				# 本地变量
				canshu="$1"
				dayin(){
				  echo "你好 $1 哥"
				}
				dayin "${canshu}"

					
		4.2.2 函数实践

第 5 章 代码发布	
	5.1 代码发布简介	
		5.1.1 代码发布介绍	
			代码发布
				代码 服务器 效果
		5.1.2 发布方式	
			手工+脚本
	5.2 代码发布流程	
		5.2.1 流程简介	
			获取代码
			打包代码
			传输代码
			关闭应用
			解压代码
			放置代码
			开启应用
			检查效果
			开放
		5.2.2 流程详解
			获取代码
				权限
			打包代码
				tar
			传输代码
				scp
			关闭应用
				先近后远
			解压代码
				tar
			放置代码
				先备后放
			开启应用
				先远后近
			检查效果
				浏览器+端口
			开放
		5.2.3 技术关键点
			文件的压缩和解压
				tar zcvf 压缩后的文件名  将要压缩的文件
				tar xf 压缩后的文件名
				zcat 压缩文件
				示例：
					tar zcvf scripts.tar.gz scripts/
					tar zcf scripts.tar.gz scripts/
					tar xf scripts1.tar.gz 
					zcat scripts1.tar.gz
			文件的传输
				格式：
					scp  要传输的文件		要放置的位置
					关键点：
						远程的表示方法：
						位置：远程连接的用户@远程主机:远程主机的目录路径
						文件：远程连接的用户@远程主机:远程主机的文件
				示例：
					scp scripts.tar.gz root@192.168.8.15:/root/
					scp root@192.168.8.15:/root/scripts.tar.gz ./
				
			文件的备份
				时间戳：date +%Y%m%d%H%M%S
				备份：mv scripts.tar.gz scripts.tar.gz-$(date +%Y%m%d%H%M%S)
					
第 6 章 环境部署	
	6.1 基础环境
		6.1.1 基础目录环境	
		6.1.2 主机网络环境
			主机间免密码认证
				1、本机生成密钥对					线上主机
					ssh-keygen -t rsa
				2、对端机器使用公钥文件认证			代码仓库
					挂锁
						/root/.ssh/authorized_keys
							内容是 线上主机的 id_rsa.pub 内容
					房东认证：
						ssh配置文件	/etc/ssh/sshd_config
							AuthorizedKeysFile	%h/.ssh/authorized_keys
						配置生效
							/etc/init.d/ssh restart	
				3、验证								线上主机
					拉取文件
	6.2 方案分析	
		6.2.1 需求
		6.2.2 需求分析
		6.2.3 部署方案
	6.3 项目环境部署	
		6.3.1 python虚拟环境
			安装虚拟环境软件
				apt-get install python-virtualenv -y
			虚拟环境基本操作
				创建 
					virtualenv -p /usr/bin/python3.5 venv
				进入
					source venv/bin/activate
				退出
					deactivate
				删除
					rm -rf venv		
		6.3.2 django环境
			拓展知识点：	
				python类型软件的安装流程
					普通：
						解压   安装
					特殊：
						解压    编译 	安装
						
						编译：python setup.py build		
		6.3.3 nginx环境	
			拓展知识点：
			linux中软件安装的一般流程
				解压
					tar
						解压文件，获取真正的配置文件
				配置
					configure
						根据默认的配置项或者更改配置项，生成编译配置文件(Makefile)
				编译
					make
						根据 Makefile 内容，编译生成指定的软件所需要的所有文件
				安装
					make install
						将编译生成的所有文件，转移到软件指定安装的目录下面
														--prefix
		6.3.4 nginx代理django
			关键点：
				nginx配置文件 四部分之间的关系
				代理的配置：proxy_pass
				其他：upstream rewrite
				
第 7 章 手工代码发布
	7.1 方案分析
	7.2 方案实施
		代码仓库：
			获取代码
				cd /data/codes
				sed -i 's#1.0#1.1#' django/views.py
			打包代码(方式1)
				cd /data/codes
				[ -f django.tar.gz ] && rm -f django.tar.gz
				tar zcf django.tar.gz django/
		线上主机
			打包代码(方式2)
				ssh root@192.168.8.15 "/bin/bash /data/scripts/tar_code.sh"
			传输代码
				cd /data/codes/
				[ -f django.tar.gz ] && rm -f django.tar.gz
				scp root@192.168.8.15:/data/codes/django.tar.gz ./
			关闭应用
				/data/server/nginx/sbin/nginx -s stop
				kill $(lsof -Pti :8000)
			解压代码
				cd /data/codes/
				tar xf django.tar.gz
			放置代码
				mv /data/server/itcast/test1/views.py /data/backup/views.py-$(date +%Y%m%d%H%M%S)
				mv /data/codes/django/views.py /data/server/itcast/test1/
			开启应用
				source /data/virtual/venv/bin/activate
				cd /data/server/itcast/
				python manage.py runserver >> /dev/null  2>&1 &
				deactivate
				
				/data/server/nginx/sbin/nginx
			检查效果
				netstat -tnulp | grep nginx
			开放	
第 8 章 脚本发布代码
	8.1 简单脚本编写	
		8.1.1 命令罗列
			关键点：
				命令可执行--"粘贴复制"
			示例：
				#!/bin/bash
				# 功能：打包代码
				# 脚本名：tar_code.sh
				# 作者：itcast
				# 版本：V 0.1
				# 联系方式：www.itcast.cn

				cd /data/codes
				[ -f django.tar.gz ] && rm -f django.tar.gz
				tar zcf django.tar.gz django/			
		8.1.2 固定内容变量化
			关键点：
				省事，防止手工犯错
			示例：
				#!/bin/bash
				# 功能：打包代码
				# 脚本名：tar_code.sh
				# 作者：itcast
				# 版本：V 0.2
				# 联系方式：www.itcast.cn

				# 本地变量
				CODE_DIR='/data/codes'
				PRO_FILE='django.tar.gz'
				PRO_DIR='django'

				# 主命令
				cd "${CODE_DIR}"
				[ -f "${PRO_FILE}" ] && rm -f "${PRO_FILE}"
				tar zcf "${PRO_FILE}" "${PRO_DIR}"		
		8.1.3 功能函数化	
			关键点：
				简单函数定义和调用
			代码：
				#!/bin/bash
				# 功能：打包代码
				# 脚本名：tar_code.sh
				# 作者：itcast
				# 版本：V 0.3
				# 联系方式：www.itcast.cn

				# 本地变量
				CODE_DIR='/data/codes'
				PRO_FILE='django.tar.gz'
				PRO_DIR='django'

				# 功能函数
				tar_code(){
				  cd "${CODE_DIR}"
				  [ -f "${PRO_FILE}" ] && rm -f "${PRO_FILE}"
				  tar zcf "${PRO_FILE}" "${PRO_DIR}"
				}

				# 函数调用
				tar_code

		8.1.4 远程执行
			格式：
				ssh 远程主机登录用户名@远程主机ip地址 "执行命令"
			示例：
				ssh root@192.168.8.15 "ls /data/scripts/"
				ssh root@192.168.8.15 "sed -i 's#1.4#1.5#' django/views.py"
				ssh root@192.168.8.15 "sed -i 's#1.4#1.5#' /data/codes/django/views.py"
				ssh root@192.168.8.15 "/bin/bash /data/scripts/tar_code.sh"
				ssh root@192.168.8.15 "zcat /data/codes/django.tar.gz"	
				
	8.2 大型脚本编写	
		8.2.1 脚本框架
			关键点：
				流程分析+各个击破
			代码：
				#!/bin/bash
				# 功能：打包代码	
				# 脚本名：deploy.sh	
				# 作者：itcast	
				# 版本：V 0.1	
				# 联系方式：www.itcast.cn

				# 获取代码
				get_code(){
				  echo "获取代码"
				}

				# 打包代码
				tar_code(){
				  echo "打包代码"
				}

				# 传输代码
				scp_code(){
				  echo "传输代码"
				}

				# 关闭应用
				stop_serv(){
				  echo "关闭应用"
				  echo "关闭nginx应用"
				  echo "关闭django应用"
				}

				# 解压代码
				untar_code(){
				  echo "解压代码"
				}

				# 放置代码
				fangzhi(){
				  echo "放置代码"
				  echo "备份老文件"
				  echo "放置新文件"
				}

				# 开启应用
				start_serv(){
				  echo "开启应用"
				  echo "开启django应用"
				  echo "开启nginx应用"
				}

				# 检查效果
				check(){
				  echo "检查效果"
				}

				# 部署函数
				pro_deploy(){
				  get_code
				  tar_code
				  scp_code
				  stop_serv
				  untar_code
				  fangzhi
				  start_serv
				  check
				}

				# 主函数
				main(){
				  pro_deploy
				}

				# 执行主函数
				main
			
		8.2.2 命令填充
			关键点：
				针对性粘贴复制 
		8.2.3 增加日志功能	
			需求：
				数据+记录		安全
			关键点：
				传参函数定义和调用
			代码：
				...
				# 本地变量
				LOG_FILE="/data/logs/deploy.log"

				# 日志功能
				write_log(){
				  DATE=$(date +%F)
				  TIME=$(date +%T)
				  BUZHOU="$1"
				  echo "${DATE} ${TIME} $0 : ${BUZHOU}" >> "${LOG_FILE}"
				}

				# 获取代码
				get_code(){
				  。。。
				  write_log "获取代码"
				}
				...			
				
				
		8.2.4 增加锁文件功能
			目的：
				安全- 在某一时间段内只能有一个人在执行脚本
			关键点：
				1、所有功能都用函数实现
				2、流程控制-- 双分支if语句
			代码：
				# 增加锁文件
				add_lock(){
				  echo "增加锁文件"
				  touch "${PID_FILE}"
				  write_log "增加锁文件"
				}
				# 删除锁文件
				del_lock(){
				  echo "删除锁文件"
				  rm -f "${PID_FILE}"
				  write_log "删除锁文件"
				}

				# 错误信息
				err_msg(){
				  echo "脚本 $0 正在运行，请稍候..."
				}
				...
				# 部署函数
				pro_deploy(){
				  add_lock
				  。。。
				  del_lock
				}
				# 主函数
				main(){
				if [ -f "${PID_FILE}" ]
				then
				  err_msg
				else
				  pro_deploy
				fi
				}

		8.2.5 脚本流程知识点填充
			关键点：
				脚本传参、函数调用
			代码：
				GONGNENG="$1"
				...
				# 脚本帮助信息
				usage(){
				  echo " 脚本 $0 的使用方式： /bin/bash $0 [ deploy ]"
				}
				...
				# 主函数
				main(){
				  case "$1" in
					"deploy")
					  if [ -f "${PID_FILE}" ]
					  then
						err_msg
					  else
						pro_deploy
					  fi
					;;
					*)
					  usage
					;;
				  esac
				}

				# 执行主函数 
				main "${GONGNENG}"


		8.2.6 输入参数安全优化	
	8.3脚本调试功能	
		-n	语法
		-x	追踪
	8.4 生产脚本编写总结	
		8.4.1 简单脚本编写总结	
		8.4.2 复杂脚本编写总结	
		8.4.3 注意事项：