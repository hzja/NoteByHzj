[https://www.runoob.com/sql/sql-orderby.html](https://www.runoob.com/sql/sql-orderby.html)<br />找个时间好好研究下sql语法，然后研究利用python写个自动化过waf脚本。

登陆<br />mysql -u用户名 -p密码

show databases;<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623249621732-d90bdd30-3cc6-4e24-b979-4dfecd56a53a.png#align=left&display=inline&height=167&originHeight=223&originWidth=293&size=9537&status=done&style=none&width=220)<br />

use 选择对应的数据库<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623249845778-86bc09d7-6ea1-48e5-b028-fcf9924a536f.png#align=left&display=inline&height=231&originHeight=462&originWidth=634&size=37889&status=done&style=none&width=317)<br />status<br />select database(); 查看当前选择的数据库<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623295138435-174cfea1-04a9-4d94-b1c2-65c83fec3030.png#align=left&display=inline&height=213&originHeight=425&originWidth=769&size=43984&status=done&style=none&width=384.5)

show tables或show tables from database_name; 显示当前数据库中所有表的名称。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623292563304-df6480eb-cca6-4d1a-8518-735d10800c9c.png#align=left&display=inline&height=390&originHeight=390&originWidth=339&size=16229&status=done&style=none&width=339)

select * from cat; 查看该表的所有列<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623294004865-bc03b2cd-1254-4d51-b042-cee0c1f500fd.png#align=left&display=inline&height=95&originHeight=126&originWidth=440&size=7997&status=done&style=none&width=330)<br />select group_concat(column_name) from information_schema.columns where table_name='admin'; 查看该表的所有列<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623294024025-7cd711de-e837-4f8f-843f-de2de1c52ff4.png#align=left&display=inline&height=81&originHeight=162&originWidth=1191&size=17896&status=done&style=none&width=595.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623294048467-d3d673e0-b5ea-4042-b951-1e59f348a47c.png#align=left&display=inline&height=140&originHeight=187&originWidth=469&size=10901&status=done&style=none&width=352)

union 必须是相同的表才能使用<br />比如下面的admin表中有username和password字段<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623295485291-2f70e0b6-32bd-4125-a895-7e2e23f54b83.png#align=left&display=inline&height=227&originHeight=227&originWidth=753&size=17180&status=done&style=none&width=753)
