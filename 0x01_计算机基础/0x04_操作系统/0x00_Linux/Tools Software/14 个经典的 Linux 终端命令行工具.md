Linux
<a name="CfKos"></a>
### ag：比grep、ack更快的递归搜索文件内容。
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628131161203-6f5f53d6-c06e-4129-9e79-f6fcdaf4ce06.webp#clientId=u4b283717-6de0-4&from=paste&id=ubee85215&originHeight=474&originWidth=720&originalType=url&ratio=1&status=done&style=none&taskId=u49e19e0f-41ec-4eb4-adc2-9324905eb69)
<a name="DhM6c"></a>
### tig：字符模式下交互查看git项目，可以替代git命令。
![2021-08-05-10-38-18-579385.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628131199496-b827e2ef-669f-4b49-8238-f1912e5eb9ea.jpeg#clientId=u4b283717-6de0-4&from=ui&id=u5f117569&originHeight=392&originWidth=720&originalType=binary&ratio=1&size=60400&status=done&style=none&taskId=ua38f8cbf-c701-48fa-8dc2-d89b877841f)
<a name="wAOvv"></a>
### mycli：mysql客户端，支持语法高亮和命令补全，效果类似ipython，可以替代mysql命令。
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628131161012-3afc3285-8401-4f01-8adb-e947443811f6.webp#clientId=u4b283717-6de0-4&from=paste&id=u33bc1537&originHeight=381&originWidth=720&originalType=url&ratio=1&status=done&style=none&taskId=u5e6415e2-3020-4939-ac33-2f7b837ea63)
<a name="VdPjv"></a>
### jq: json文件处理以及格式化显示，支持高亮，可以替换python -m json.tool。
![2021-08-05-10-38-18-818379.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628131216806-26d80551-1b71-4694-ba13-399ae8f1449e.jpeg#clientId=u4b283717-6de0-4&from=ui&id=uda2e2f54&originHeight=452&originWidth=720&originalType=binary&ratio=1&size=39546&status=done&style=none&taskId=u9eace236-8b61-4930-bf65-250a4cef7cb)
<a name="Yp3Rm"></a>
### shellcheck：shell脚本静态检查工具，能够识别语法错误以及不规范的写法。
![2021-08-05-10-38-18-912380.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628131226526-52c12062-d92e-43bc-a817-4c9183677000.jpeg#clientId=u4b283717-6de0-4&from=ui&id=ud8726a69&originHeight=328&originWidth=720&originalType=binary&ratio=1&size=19102&status=done&style=none&taskId=u14f919c2-86b1-4801-a17a-bb651b5c921)
<a name="W1tkn"></a>
### fzf：命令行下模糊搜索工具，能够交互式智能搜索并选取文件或者内容，配合终端ctrl-r历史命令搜索简直完美。
![2021-08-05-10-38-19-002389.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628131240763-e034f818-458b-46ba-b20c-81c50002bb78.jpeg#clientId=u4b283717-6de0-4&from=ui&id=u391c105d&originHeight=526&originWidth=720&originalType=binary&ratio=1&size=73006&status=done&style=none&taskId=u28b7ec62-5242-4ed9-9956-56cd754cda6)
<a name="PKrNm"></a>
### PathPicker(fpp): 在命令行输出中自动识别目录和文件，支持交互式，配合git非常有用。
运行以下命令：
```bash
git diff HEAD~8 --stat | fpp
```
![2021-08-05-10-38-19-101382.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628131258305-26f57f00-eb04-4a5a-923c-307c2d31ecaf.jpeg#clientId=u4b283717-6de0-4&from=ui&id=u067ac980&originHeight=368&originWidth=720&originalType=binary&ratio=1&size=42534&status=done&style=none&taskId=uc995b4e1-f741-4319-86aa-b25b033f64e)
<a name="jiBJg"></a>
### htop: 提供更美观、更方便的进程监控工具，替代top命令。
![2021-08-05-10-38-19-240389.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628131271590-f0e94ee8-8917-4c5c-a4d0-ec1657f33eb7.jpeg#clientId=u4b283717-6de0-4&from=ui&id=u6b861442&originHeight=396&originWidth=720&originalType=binary&ratio=1&size=67107&status=done&style=none&taskId=uf2f12ad5-58f8-450b-bd63-7ade1ce3fa1)
<a name="lRlJf"></a>
### glances：更强大的 htop / top 代替者。
htop 代替 top，glances 代替 htop：<br />![2021-08-05-10-38-19-363388.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628131282332-f577dafd-a2d8-4d31-a386-d73a85881bca.jpeg#clientId=u4b283717-6de0-4&from=ui&id=u115dd775&originHeight=447&originWidth=720&originalType=binary&ratio=1&size=70896&status=done&style=none&taskId=u69ce1415-53a6-496a-93fd-f7fc464f606)<br />信息比 htop 丰富了不少，更全了，对吧？除了命令行查看外，glances 还提供页面服务，从页面上随时查看某服务器的状态。
<a name="uEQaz"></a>
### axel：多线程下载工具，下载文件时可以替代curl、wget。
```bash
axel -n 20 http://centos.ustc.edu.cn/centos/7/isos/x86_64/CentOS-7-x86_64-Minimal-1511.iso
```
![2021-08-05-10-38-19-546381.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628131310002-b429b53c-ca6e-4136-a25f-39556736920d.jpeg#clientId=u4b283717-6de0-4&from=ui&id=ue5cbf33b&originHeight=634&originWidth=720&originalType=binary&ratio=1&size=86099&status=done&style=none&taskId=u92e49542-fcbe-461c-890e-cf6c84ca129)
<a name="TFhBU"></a>
### sz/rz：交互式文件传输，在多重跳板机下传输文件非常好用，不用一级一级传输。
<a name="j6S7L"></a>
### cloc：代码统计工具，能够统计代码的空行数、注释行、编程语言。
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628131162288-8b6edaf2-6b64-444c-a777-5e17a4182554.webp#clientId=u4b283717-6de0-4&from=paste&id=u819ed176&originHeight=229&originWidth=720&originalType=url&ratio=1&status=done&style=none&taskId=u1a8d2eb2-4868-4f31-bf59-20c893be5e1)
<a name="VctpG"></a>
### tmux：终端复用工具，替代screen、nohup。
![2021-08-05-10-38-19-731383.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628131357797-5d14fbd3-626c-4148-9dc4-a4f63c6a1557.jpeg#clientId=u4b283717-6de0-4&from=ui&id=u4c07ccf9&originHeight=451&originWidth=720&originalType=binary&ratio=1&size=55274&status=done&style=none&taskId=ubaf59c77-cf1d-4459-b1af-c5a312cc143)
<a name="saBPB"></a>
### script/scriptreplay:  终端会话录制。
```bash
# 录制
script -t 2>time.txt session.typescript
# your commands
# 录制结束
exit
# 回放
scriptreplay -t time.txt session.typescript
```
<a name="RYyWR"></a>
### multitail：多重  tail。
通常不止一个日志文件要监控，怎么办？终端软件里开多个 tab 太占地方，可以试试这个工具：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628131162607-93c5ac01-bcd4-4372-90b9-7a388c60eadd.webp#clientId=u4b283717-6de0-4&from=paste&id=u8adbc646&originHeight=481&originWidth=720&originalType=url&ratio=1&status=done&style=none&taskId=uc8fcb508-7102-4226-b0af-274d4d374f1)
