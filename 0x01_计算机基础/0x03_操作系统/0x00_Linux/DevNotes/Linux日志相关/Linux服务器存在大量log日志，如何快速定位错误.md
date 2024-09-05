Linux
<a name="lgsQh"></a>
### 针对大量log日志快速定位错误地方
动态查看日志
```bash
tail -f catalina.out
```
从头打开日志文件
```bash
cat catalina.out
```
可以使用 >nanjiangtest.txt 输出某个新日志去查看
```bash
[root@yesky logs]# cat -n catalina.out |grep 717892466 >nanjiangtest.txt
```
<a name="edp3s"></a>
### tail/head简单命令使用
```bash
[root@yesky logs]# tail -n number catalina.out 查询日志尾部最后number行的日志
[root@yesky logs]# tail -n +number catalina.out 查询number行之后的所有日志
[root@yesky logs]# head -n number catalina.out 查询日志文件中的前number行日志
[root@yesky logs]# head -n -number catalina.out 查询日志文件除了最后number行的其他所有日志
```
<a name="gJ7VO"></a>
#### 第一种方式（根据关键字查找出行号）：
用grep拿到的日志很少，需要查看附近的日志。首先：`cat -n test.log | grep "关键词"` 得到关键日志的行号
```bash
[root@yesky logs]# cat -n catalina.out |grep 717892466
13230539        [11:07 17:47:11] INFO nanjiang:Edit Old Article：717892466-2020-11-07 17:47:11
13230593        [11:07 17:47:15] INFO nanjiangSave Article ID IS：717892466
13230595        717892466 article.getDisplayTime()1 = 2020-11-07 16:25:11
13230596        717892466 article.getDisplayTime()2 = 2020-11-07 16:25:11
13230601        [11:07 17:47:15] INFO 南江 10.10.10.39  edit    article 717892466       编辑文章
```
**「**`**cat -n catalina.out|tail -n +13230539|head -n 10**`**」**

- `tail -n +13230539`表示查询13230539行之后的日志
- `head -n 10`则表示在前面的查询结果里再查前10条记录
```bash
[root@yesky logs]# cat -n catalina.out |tail -n +13230539|head -n 10
13230539        [11:07 17:47:11] INFO nanjiang:Edit Old Article：717892466-2020-11-07 17:47:11
13230540        [11:07 17:47:11] INFO Takes:2 ms class com.tmg.cms.manager.dao.article.impl.ArticleContentDaoImpl       getListByArticleId      [NzE3ODkyNDY2]       [int]
13230541        [11:07 17:47:11] INFO Takes:1 ms class com.tmg.cms.manager.dao.resourceImage.impl.ResourceImageDaoImpl  load    
13230542        [11:07 17:47:11] INFO Takes:0 ms class com.tmg.cms.manager.dao.resourceImage.impl.ResourceImageDaoImpl  load    
13230543        [11:07 17:47:11] INFO Takes:1 ms class com.tmg.cms.manager.dao.resourceImage.impl.ResourceImageDaoImpl  load    
13230544        [11:07 17:47:11] INFO article.getImage3：/uploadImages/2020/312/02/3NXCRK4U3589_2.jpg
13230545        [11:07 17:47:11] INFO Takes:0 ms class com.tmg.cms.manager.dao.resourceImage.impl.ResourceImageDaoImpl  load   
13230546        [11:07 17:47:11] INFO Takes:2 ms class com.tmg.cms.manager.dao.privilege.impl.UserDaoImpl       getUserByid     
13230547        [11:07 17:47:11] INFO Takes:57 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl     selectSitemapWithoutAudit       [MQ==]  [int]
13230548        [11:07 17:47:11] INFO Takes:5 ms class com.tmg.cms.manager.dao.forbidword.impl.ForbidwordDaoImpl        getForbidwordBysiteid   [MjI=]  [int]
```
<a name="TAshb"></a>
#### 第二种方式：查看指定时间段内的日志
首先要进行范围时间段内日志查询先查看是否在当前日之内存在
```bash
grep '11:07 18:29:20' catalina.out
grep '11:07 18:31:11' catalina.out
```
时间范围内的查询
```bash
sed -n '/11:07 18:29:20/,/11:07 18:31:11/p' catalina.out 
sed -n '/11:07 18:29:/,/11:07 18:31:/p' catalina.out
```
<a name="re5o6"></a>
#### 第三种方式：查看日志中特定字符的匹配数目
```bash
[root@yesky logs]# grep '1175109632' catalina.out | wc -l
154
```
<a name="uRA37"></a>
#### 第四种方式：查询最后number行，并查找关键字“结果”
```bash
[root@yesky logs]# tail -n 20 catalina.out | grep 'INFO Takes:1'
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.config.impl.ConfigInfoDaoImpl load 
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [NTkwOTQ5] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzI0] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzI3] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzMw] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzA5NA==] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [Mzc4Mg==] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [OTM1MA==] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MTE5MDMw] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [NTQ2MzQw] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [NTg2NzYy] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzYyMjA=] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.configModule.impl.ConfigModuleDaoImpl getPersonMenuList
```
<a name="cO7cQ"></a>
#### 第五种方式：查询最后number行，并查找关键字“结果”并且对结果进行标红
```bash
[root@yesky logs]# tail -n 20 catalina.out | grep 'INFO Takes:1' --color
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.config.impl.ConfigInfoDaoImpl load 
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [NTkwOTQ5] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzI0] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzI3] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzMw] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzA5NA==] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [Mzc4Mg==] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [OTM1MA==] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MTE5MDMw] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [NTQ2MzQw] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [NTg2NzYy] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzYyMjA=] [int]
```
<a name="v3Qkt"></a>
#### 第六种方式：查询最后number行，并查找关键字“结果”并且对结果进行标红，上下扩展两行
```bash
[root@yesky logs]# tail -n 20 catalina.out | grep 'INFO Takes:1' --color -a2
[11:11 22:02:51] INFO Takes:0 ms class com.tmg.cms.manager.dao.article.impl.ArticleContentDaoImpl getArticlePageNum [NzE4MTM2ODky] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.config.impl.ConfigInfoDaoImpl load [com.tmg.cms.manager.model.config.ConfigInfo]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [NTkwOTQ5] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzI0] [int]
[11:11 22:02:51] INFO Takes:0 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzI1] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzI3] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzMw] [int]
[11:11 22:02:51] INFO Takes:0 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzAzNg==] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzA5NA==] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [Mzc4Mg==] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [OTM1MA==] [int]
[11:11 22:02:51] INFO Takes:0 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MTE0MjQ4] [int]
[11:11 22:02:51] INFO Takes:0 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MTE4MDc4] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MTE5MDMw] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [NTQ2MzQw] [int]
[11:11 22:02:51] INFO Takes:0 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [NTQ3MTIw] [int]
[11:11 22:02:51] INFO Takes:0 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [NTY4OTYx] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [NTg2NzYy] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.sitemap.impl.SitemapDaoImpl getSitemapTop [MzYyMjA=] [int]
[11:11 22:02:51] INFO Takes:1 ms class com.tmg.cms.manager.dao.configModule.impl.ConfigModuleDaoImpl getPersonMenuList [com.tmg.cms.manager.model.config.ConfigPersonMenu]
```
<a name="ZhHck"></a>
#### 第七种方式：分页查看，使用空格翻页(使用more/less)
```bash
[root@yesky logs]# tail -n 2000 catalina.out | grep 'INFO Takes:1' --color -a2 | more
[root@yesky logs]# tail -n 2000 catalina.out | grep 'INFO Takes:1' --color -a2 | less
```
<a name="HXTeT"></a>
### 附加
<a name="C0vg6"></a>
#### 1、全屏导航

- ctrl + F - 向前移动一屏
- ctrl + B - 向后移动一屏
- ctrl + D - 向前移动半屏
- ctrl + U - 向后移动半屏
<a name="Ygv4Q"></a>
#### 2、单行导航

- j - 向前移动一行
- k - 向后移动一行
<a name="Ht4Yf"></a>
#### 3、其它导航

- G - 移动到最后一行
- g - 移动到第一行
- q / ZZ - 退出 less 命令
