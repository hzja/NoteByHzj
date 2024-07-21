<a name="pKRI1"></a>
## 命令执行
<a name="FDwVC"></a>
### linux
```
;  --在 shell 中，是”连续指令”
&  --不管第一条命令成功与否，都会执行第二条命令
&&  --第一条命令成功，第二条才会执行
|  --第一条命令的结果，作为第二条命令的输入
||  --第一条命令失败，第二条才会执行
```
;可作为连接符<br /># ls;ping baidu.com -c 2<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627217993397-8a852849-1ea9-4de8-9841-caa04db76847.png#height=183&id=KuAmV&originHeight=183&originWidth=738&originalType=binary&ratio=1&size=91635&status=done&style=none&width=738)<br /># a=who;b=ami;$a$b<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627225750847-6b716803-c525-4931-b473-5e04e43f8b41.png#height=55&id=PI2Jh&originHeight=55&originWidth=255&originalType=binary&ratio=1&size=13992&status=done&style=none&width=255)<br />${IFS}可替换空格<br /># cat ${IFS}${IFS} test.txt <br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627220905826-659f7aaa-39e7-4f68-a4b4-5f65fada9e84.png#height=64&id=Byw60&originHeight=64&originWidth=277&originalType=binary&ratio=1&size=11246&status=done&style=none&width=277)

cat可以用tac替换，其实就是内容反过来了。

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627221361984-b0d0de9e-e972-4f41-816d-788ddfbcb340.png#height=65&id=aETRq&originHeight=65&originWidth=230&originalType=binary&ratio=1&size=11888&status=done&style=none&width=230)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627221419185-c4adb6ff-ebe1-4f89-b784-a6b88ff0b153.png#height=72&id=D6Oaj&originHeight=72&originWidth=254&originalType=binary&ratio=1&size=14293&status=done&style=none&width=254)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627221524030-ed3722bf-51f1-4f71-8ef5-18cf8e852c29.png#height=253&id=P6Isr&originHeight=253&originWidth=241&originalType=binary&ratio=1&size=42384&status=done&style=none&width=241)

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627224602177-4e65580e-11e3-4de3-8780-f5df240a8c60.png#height=156&id=g9ytr&originHeight=156&originWidth=321&originalType=binary&ratio=1&size=32115&status=done&style=none&width=321)

<a name="Fi8aI"></a>
### windows
双引号可以让空格保持为一个整体<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627226267679-2af3915d-f52f-4982-baf0-c20676d15361.png#height=77&id=VyNJn&originHeight=77&originWidth=274&originalType=binary&ratio=1&size=4368&status=done&style=none&width=274)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627226298772-b92a7801-13b8-44c7-ae97-f12a93917352.png#height=92&id=za5lT&originHeight=92&originWidth=294&originalType=binary&ratio=1&size=5634&status=done&style=none&width=294)

^也可以作为连接符<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627226562237-01f5bac5-e53d-4395-a35e-070bf7539340.png#height=104&id=fWng9&originHeight=104&originWidth=266&originalType=binary&ratio=1&size=5514&status=done&style=none&width=266)

|  --第一条命令的结果，作为第二条命令的输入<br />||  --第一条命令失败，第二条才会执行<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627226786678-cf716e0f-3144-4e45-84ae-3c3230d8db97.png#height=407&id=GDc0R&originHeight=542&originWidth=488&originalType=binary&ratio=1&size=85225&status=done&style=none&width=366)

逗号、分号可以替代空格使用<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627227430259-cbf79ab4-f3bc-4254-93f3-9757d51f699c.png#height=88&id=R9zXW&originHeight=88&originWidth=303&originalType=binary&ratio=1&size=4490&status=done&style=none&width=303)

组合拳<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627227448915-65a784b6-69e0-4775-8375-8bdd42b7304d.png#height=178&id=EB8QV&originHeight=178&originWidth=393&originalType=binary&ratio=1&size=9697&status=done&style=none&width=393)

cmd /c中间的空格是可以插入任意字符的<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627227747029-c49ce8e6-49a1-4b81-9a5d-86b2b89fb7fb.png#height=94&id=znCHf&originHeight=94&originWidth=507&originalType=binary&ratio=1&size=7156&status=done&style=none&width=507)<br />组合拳<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627227801628-b9601588-62a3-4356-b95a-40e5bfe5a0d5.png#height=91&id=RpmK0&originHeight=91&originWidth=580&originalType=binary&ratio=1&size=5845&status=done&style=none&width=580)

<a name="74Gze"></a>
## CTF题目链接
[https://adworld.xctf.org.cn/](https://adworld.xctf.org.cn/)<br />[https://www.ichunqiu.com/battalion?t=1&r=68487](https://www.ichunqiu.com/battalion?t=1&r=68487)<br />[https://buuoj.cn/login?next=%2Fchallenges%3F](https://buuoj.cn/login?next=%2Fchallenges%3F)
