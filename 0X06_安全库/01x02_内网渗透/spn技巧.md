<a name="eRo5W"></a>
# SPN
[https://github.com/nullbind/Powershellery/tree/master/Stable-ish](https://github.com/nullbind/Powershellery/tree/master/Stable-ish)<br />setspn -T domain -q */*列出域中所有的spn信息<br />适用win 8
<a name="JZImS"></a>
## 定位域内PC位置
spn中直接在存在域的主机中 ping 信管中心xxx，就能获取目标内网ip位置。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1646276347425-365faca0-c66a-4de8-ab78-30bfa741a6f6.png#clientId=uce5de383-e006-4&from=paste&height=194&id=u06ad0779&originHeight=258&originWidth=764&originalType=binary&ratio=1&size=112924&status=done&style=none&taskId=u13b51727-20d2-4947-8f8c-6678dc79dcc&width=573)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1646276373285-087f37af-25f7-4050-aaa7-957884c49daa.png#clientId=uce5de383-e006-4&from=paste&height=149&id=ua1848f27&originHeight=199&originWidth=705&originalType=binary&ratio=1&size=46478&status=done&style=none&taskId=uec9d94c4-9fa2-4e55-8ef1-dd1afb55c70&width=529)

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611400553056-4bb7d4bf-5e77-4a6f-a33f-586f4c6e3ce4.png#height=426&id=hDYK9&originHeight=851&originWidth=1722&originalType=binary&ratio=1&size=388826&status=done&style=none&width=861)

<a name="ccvja"></a>
## 域内寻找邮箱服务
Exchange、POP3 还需要本地做hosts才能访问<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611400894173-1c87142f-2768-4a66-8054-765d22d88ebf.png#height=456&id=DsdMT&originHeight=608&originWidth=644&originalType=binary&ratio=1&status=done&style=none&width=483)
<a name="wXjJx"></a>
## 通过SPN探测域内存活主机
接着对拉回来的spn文件进行处理，获取spn记录前面的主机名字<br />grep "CN=" spn.txt | awk -F "," {'print $1'} |  awk -F "=" {'print $2'} > host.txt<br /> <br />接着通过批处理来获取里面的的信息，通过批处理来批量的Ping主机，如果主机有回复的话就会有答复<br />for /f "delims=" %i in (C:\Users\hyadmin\Desktop\2.txt) do @ping -w 1 -n 1 %i | findstr /c ".highsun.com |" >> C:\Users\hyadmin\Desktop\1.txt

或者

C:\Users\admin1>for /f "delims=" %i in (C:\Users\admin1\Desktop\spn.txt) do @pin<br />g -w 1 -n 1 %i | findstr /r "[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*"


![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1629358787295-2f4e817a-2949-4907-a45b-92015b650b60.png#height=365&id=bLxC0&originHeight=486&originWidth=660&originalType=binary&ratio=1&size=48620&status=done&style=none&width=495)

```
import re

def main():
    f = None
    filename=r'C:\\Users\\19307\\Desktop\\222.txt'
    try:
        f = open(filename, 'r', encoding='utf-8')
        for line in f.readlines():
            trueIp =re.findall(r"\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b",line)
            print(trueIp[0])
    except FileNotFoundError:
        print('无法打开指定的文件!')
    except LookupError:
        print('指定了未知的编码!')
    except UnicodeDecodeError:
        print('读取文件时解码错误!')
    finally:
        if f:
            f.close()

if __name__ == '__main__':
    main()
```
