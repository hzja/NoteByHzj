数据库命令，进入某个数据库<br />sqlite3.exe   xxx.db

查看数据表<br />sqlite> .tables<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1685463409697-b253e320-d56a-44b4-81f5-6d83a7a2591e.png#averageHue=%23181613&clientId=ub6197461-fcbc-4&from=paste&height=54&id=u4c4548dc&originHeight=47&originWidth=222&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=2604&status=done&style=none&taskId=u1f50db04-ed3b-442e-9115-ed829565a24&title=&width=253.60000610351562)

查看userinfo表结构<br />sqlite> .schema userinfo<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1685463380311-4c17df41-0fbf-483f-9a48-00e0800ebc3a.png#averageHue=%23151311&clientId=ub6197461-fcbc-4&from=paste&height=112&id=uf78c7f06&originHeight=140&originWidth=444&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=13477&status=done&style=none&taskId=u3e617d32-577d-4c27-995e-bad6ee8a8e6&title=&width=355.2)

查看userinfo表的所有值<br />sqlite> .dump userinfo<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1685463339317-e9c608b1-2f92-4772-a653-c33cd1c8183d.png#averageHue=%23141210&clientId=ub6197461-fcbc-4&from=paste&height=198&id=u640b4452&originHeight=247&originWidth=730&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=36552&status=done&style=none&taskId=u582437b5-6a2e-405e-9f4f-a9f0bee1169&title=&width=584)

删除userinfo表中uid为4的字段内容<br />sqlite> DELETE FROM userinfo where uid=4
