<a name="bzYLp"></a>
## 一、思维导图
WAF拦截会出现在安全测试的各个层面，掌握各个层面的分析和绕过技术最为关键。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326204926-e8d811bb-9c42-47d0-a0a4-8b7befbd8586.png#clientId=u23974680-8831-4&from=paste&id=u9cebd452&originHeight=846&originWidth=1256&originalType=url&ratio=1&status=done&style=none&taskId=u02a92b12-21f7-48ed-aa0f-3e81515ed10)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515200623.png)
<a name="KZtXp"></a>
## 二、演示案例
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326204937-8d9e415b-9deb-47ce-89a2-794a03aa60b6.png#clientId=u23974680-8831-4&from=paste&id=u166585f1&originHeight=270&originWidth=797&originalType=url&ratio=1&status=done&style=none&taskId=u94fc0cf0-c56c-43af-89c4-d6b00194bce)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515212428.png)
<a name="fk1Yq"></a>
### 1、Safedog-未开CC
CC就是DDOS攻击的一种，默认是不开启的。<br />判断有没有WAF可以直接在路径上报错显示。
<a name="vSlID"></a>
#### ①用目录扫描工具扫
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326204934-7ad0976c-4ae6-4704-ba10-7cb4265fb686.png#clientId=u23974680-8831-4&from=paste&id=uc1a56a02&originHeight=564&originWidth=1090&originalType=url&ratio=1&status=done&style=none&taskId=u386980a5-34e1-475f-8e1f-eb16129f101)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515213342.png)<br />扫出来的目录全是假的。<br />使用抓取进程的抓包工具抓包<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326204979-8b9c6140-fa6c-48eb-a3a6-34b692b2c0e9.png#clientId=u23974680-8831-4&from=paste&id=u9e9ae90e&originHeight=571&originWidth=1290&originalType=url&ratio=1&status=done&style=none&taskId=ubbfeee5c-4c13-4f6a-8d69-aed29b8b20a)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515213616.png)<br />可以很明显的看出不同，在请求方法上就不同。可以修改为Get方式。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326204945-eeaa993c-a5c5-482d-9625-1cf635286ae8.png#clientId=u23974680-8831-4&from=paste&id=u1c15266d&originHeight=157&originWidth=413&originalType=url&ratio=1&status=done&style=none&taskId=u4a9882f8-a03e-43c0-b532-6f471cae3d9)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515213649.png)<br />这回就没有误报了。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326205770-55f7987e-2580-43e8-9aac-1fbd252d71ab.png#clientId=u23974680-8831-4&from=paste&id=ua57aa5ad&originHeight=235&originWidth=1092&originalType=url&ratio=1&status=done&style=none&taskId=ufc888d5e-1c86-4221-8f1c-5c9b870c75c)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515213723.png)
<a name="cn7Ta"></a>
### 2、Safedog-开启CC
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326205749-7c0e26ae-c7d0-4ff6-a9f8-0b3af5783107.png#clientId=u23974680-8831-4&from=paste&id=u700862c1&originHeight=224&originWidth=703&originalType=url&ratio=1&status=done&style=none&taskId=u3092413b-a864-4b8f-95bf-d83b4329b09)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515213845.png)<br />开启CC之后，再次使用工具，并且访问网站。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326206038-7e52935d-9ed6-48bd-90db-61ad0ebed26f.png#clientId=u23974680-8831-4&from=paste&id=u168b4a16&originHeight=379&originWidth=1032&originalType=url&ratio=1&status=done&style=none&taskId=u74b65360-fae0-4086-a379-1ea1b159537)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515213935.png)<br />访问速度过快了。可以设置延迟扫描。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326205866-d3e3d576-5e04-4e2a-b7bf-d65bb86839de.png#clientId=u23974680-8831-4&from=paste&id=u9dfed6d3&originHeight=207&originWidth=591&originalType=url&ratio=1&status=done&style=none&taskId=uf1bfca5b-4e3b-4a10-ae7a-ee6ffb73f19)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515214035.png)<br />或者也可以通过爬虫的白名单来搞<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326205969-354b814f-bf47-4e74-9bba-d077157ba354.png#clientId=u23974680-8831-4&from=paste&id=u0940cc5b&originHeight=470&originWidth=707&originalType=url&ratio=1&status=done&style=none&taskId=ufb873daf-d267-4b75-8747-e31637a2714)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515214404.png)<br />模拟搜索引擎请求头User-Agent就可以了。进行扫描<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326206683-859cd490-a7b0-4c7b-abfa-0de47eabe169.png#clientId=u23974680-8831-4&from=paste&id=ubd187935&originHeight=610&originWidth=1107&originalType=url&ratio=1&status=done&style=none&taskId=u5b698e3d-584d-450f-a275-9a3cf328c70)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515214715.png)<br />发现没有任何的结果。
<a name="mFNxH"></a>
#### ①使用python脚本来访问
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326206668-71a85ebe-cce2-427e-9008-7eac91ecacdd.png#clientId=u23974680-8831-4&from=paste&id=ubae8e222&originHeight=584&originWidth=1243&originalType=url&ratio=1&status=done&style=none&taskId=u953e578e-d54f-4a36-8dce-1231a9f7ce3)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515215148.png)<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326206773-b5d0d5ad-955c-4423-973b-d758655f2fd7.png#clientId=u23974680-8831-4&from=paste&id=u41101475&originHeight=644&originWidth=1105&originalType=url&ratio=1&status=done&style=none&taskId=u43726180-3306-4ca9-9231-c3981f4c896)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515215337.png)<br />php_b.txt是一个字典
```bash
import requests
import time

headers = {
    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9',
    'Accept-Encoding': 'gzip, deflate, br',
    'Accept-Language': 'zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6',
    'Cache-Control': 'max-age=0',
    'Connection': 'keep-alive',
    'Cookie': 'PHPSESSID=4d6f9bc8de5e7456fd24d60d2dfd5e5a',
    'sec-ch-ua': '"Chromium";v="92", " Not A;Brand";v="99", "Microsoft Edge";v="92"',
    'sec-ch-ua-mobile': '?0',
    'Sec-Fetch-Dest': 'document',
    'Sec-Fetch-Mode': 'navigate',
    'Sec-Fetch-Site': 'none',
    'Sec-Fetch-User': '?1',
    'Upgrade-Insecure-Requests': '1',
    'User-Agent': 'Mozilla/5.0 (compatible; Baiduspider-render/2.0; +http://www.baidu.com/search/spider.html)'
}

for paths in open('php_b.txt', encoding='utf-8'):
    url = "http://127.0.0.1/pikachu"
    paths = paths.replace('\n', '')
    urls = url + paths
    proxy = {
        'http': '127.0.0.1:7777'
    }
    try:
        code = requests.get(urls, headers=headers, proxies=proxy).status_code
        # time.sleep(3)
        print(urls + '|' + str(code))
    except Exception as err:
        print('connect error')
        time.sleep(3)

```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630328874661-b35b9dbc-b83c-486d-b98c-d62f98df5bc2.png#clientId=u23974680-8831-4&from=paste&height=213&id=u3f5baff4&originHeight=425&originWidth=1514&originalType=binary&ratio=1&size=81190&status=done&style=none&taskId=u448e075e-4105-4d06-b68d-82fef142bbf&width=757)<br />抓包分析：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326206723-1ba9300b-4a28-4c1c-9faf-ff1b7b7812df.png#clientId=u23974680-8831-4&from=paste&id=u5baf3955&originHeight=416&originWidth=1373&originalType=url&ratio=1&status=done&style=none&taskId=u87505efb-ca31-471e-ae18-229ce877072)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515215450.png)<br />如果不用自定义头：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326206880-ab5da53a-1d02-4b99-8faa-0a0bbc843910.png#clientId=u23974680-8831-4&from=paste&id=u0b4c7b50&originHeight=126&originWidth=682&originalType=url&ratio=1&status=done&style=none&taskId=ucf4e08e4-54e5-445f-ab90-f33e1b28070)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515215557.png)<br />可以直接在脚本里使用爬虫引擎的请求头，效果很好。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326207652-bd2efd59-6274-42d1-a671-b83d781493eb.png#clientId=u23974680-8831-4&from=paste&id=ue1888f7b&originHeight=757&originWidth=1320&originalType=url&ratio=1&status=done&style=none&taskId=u3a714dfb-d1b9-4ad9-a3f6-620e578135b)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515215849.png)
<a name="sRChS"></a>
#### ②代理池
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326207810-177b347a-4498-499c-bdc3-37504ed7fdee.png#clientId=u23974680-8831-4&from=paste&id=u3647daa6&originHeight=576&originWidth=1015&originalType=url&ratio=1&status=done&style=none&taskId=ufbd10d6a-d100-49ff-a011-782726fb788)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515220116.png)
<a name="MKcHT"></a>
### 3、阿里云
阿里云-无法模拟搜索引擎爬虫绕过，只能采用代理池或者延时。
<a name="xwv15"></a>
### 4、阿里云+宝塔付费安全服务
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326207629-5507a733-1f30-4910-bb13-2d4f0f3711e1.png#clientId=u23974680-8831-4&from=paste&id=u561db83c&originHeight=536&originWidth=518&originalType=url&ratio=1&status=done&style=none&taskId=ue7a9a6c3-ddb9-4e75-b163-a248edeeeab)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515221326.png)<br />通过延时来搞：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326207693-dc95c9f2-bdab-4146-938b-23e9d530c175.png#clientId=u23974680-8831-4&from=paste&id=u5d6c50e2&originHeight=562&originWidth=1133&originalType=url&ratio=1&status=done&style=none&taskId=ucc2d01de-454a-47af-91c8-ad486537dd5)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515222025.png)<br />宝塔里的日志：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630326207747-1db4d678-5fcc-44aa-be1b-67c3d6edd303.png#clientId=u23974680-8831-4&from=paste&id=u284b77d9&originHeight=489&originWidth=815&originalType=url&ratio=1&status=done&style=none&taskId=ue1f0c7e3-0dee-40ef-a6bd-589f5cd432f)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515222053.png)<br />可以优化日志，因为宝塔检测出一分钟内访问敏感文件了。比如说code.php.bak 可以优化成code.php.bak .访问的还是原来的。

