Linux
<a name="tueLo"></a>
## 1、`sl` 命令
会看到一辆火车从屏幕右边开往左边……
<a name="olUg4"></a>
### 安装
```bash
$ sudo apt-get install sl
```
<a name="oOeZM"></a>
### 运行
```bash
$ sl
```
<a name="UyRiZ"></a>
### 命令有 `-a l F e` 几个选项
```bash
-a An accident seems to happen. You'll feel pity for people who cry for help.

-l shows little one.

-F It flies.

-e Allow interrupt by Ctrl+C.
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623742157064-82e7e9b7-f309-4436-b309-38a160468f8b.png#clientId=u947048c4-6947-4&from=paste&height=583&id=uc20d5b76&originHeight=1750&originWidth=3323&originalType=binary&ratio=3&rotation=0&showTitle=false&size=1960019&status=done&style=none&taskId=u6ecfee1a-dd49-42d0-bed0-420798225a1&title=&width=1107.6666666666667)<br />可以给别人来个恶作剧，他一敲`ls`，不知道的肯定很有效果
```bash
$alias ls=sl
```
<a name="HyX8g"></a>
## 2、`fortune` 命令
输出一句话，有笑话，名言什么的 (还有唐诗宋词`sudo apt-get install fortune-zh`)
<a name="pDXxA"></a>
### 安装
```bash
$sudo apt-get install fortune
```
<a name="cty1e"></a>
### 运行
```bash
$fortune
```
![2021-05-09-16-55-47-574635.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620550967833-9b9179d2-06d1-4a5d-a02d-b4b131cc545b.png#clientId=u69ed2538-5813-4&from=ui&id=ua705f211&originHeight=217&originWidth=709&originalType=binary&ratio=1&rotation=0&showTitle=false&size=38298&status=done&style=none&taskId=ufedfc48d-f6de-45e8-930c-7a8329de073&title=)
<a name="lDdpE"></a>
## 3、`cowsay` 命令
用ASCII字符打印牛，羊等动物，还有个`cowthink`
<a name="D2K3M"></a>
### 安装
```bash
$sudo apt-get install cowsay
```
<a name="J11XN"></a>
### 运行
```bash
$cowsay "I am not a cow, hahaha"
```
![2021-05-09-16-55-47-668382.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620550986391-3df9ebc2-6836-4802-87d1-e4749aaa3900.png#clientId=u69ed2538-5813-4&from=ui&id=ufe3389b5&originHeight=161&originWidth=641&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9372&status=done&style=none&taskId=u9b9c0932-f1be-46cb-990f-b6894ac84b1&title=)<br />`$cowsay -l` 查看其它动物的名字，然后 `-f` 跟上动物名，如
```bash
$cowsay -f tux "坑爹啊"
```
![2021-05-09-16-55-47-748172.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620550995855-05c7606e-20f2-4db4-b7dc-61f50c0e3eb0.png#clientId=u69ed2538-5813-4&from=ui&id=u792bd532&originHeight=244&originWidth=601&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12133&status=done&style=none&taskId=ue6e1e053-dc6c-4f8c-94e2-84236ca6c7f&title=)<br />还可以让`cowsay`说出`fortune`的内容，就像这样：
```bash
$fortune | cowsay
```
![2021-05-09-16-55-47-844912.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551010985-fb882910-8ea9-4446-859f-adfe9c437392.png#clientId=u69ed2538-5813-4&from=ui&id=u6e6940d8&originHeight=184&originWidth=661&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26706&status=done&style=none&taskId=uccad576a-5f51-4dc7-af00-095e6ed7c9e&title=)
<a name="hAOJr"></a>
## 4、`cmatrix` 命令
这个很酷！《黑客帝国》那种矩阵风格的动画效果
<a name="QBZUo"></a>
### 安装
```bash
$ sudo apt-get install cmatrix
```
<a name="iCj8u"></a>
### 运行
```bash
$ cmatrix
```
![2021-05-09-16-55-47-956617.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551023061-0229522b-b36d-4ee4-9a0c-eb2338fbef41.png#clientId=u69ed2538-5813-4&from=ui&id=u17fbade9&originHeight=462&originWidth=720&originalType=binary&ratio=1&rotation=0&showTitle=false&size=36587&status=done&style=none&taskId=u0890fcfc-ebe5-4b03-8ba0-87a6012e6dc&title=)
<a name="YPgXx"></a>
## 5、`figlet `、`toilet`命令
艺术字生成器，由ASCII字符组成，把文本显示成标题栏。此外还有`banner`这个命令<br />安装
```bash
$sudo apt-get install figlet

$sudo apt-get install toilet
```
运行
```bash
$figlet fuck you !

$toilet i love you
```
![2021-05-09-16-55-48-027423.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551035900-d39c683c-a5ef-4c1a-822f-e58737e1951f.png#clientId=u69ed2538-5813-4&from=ui&id=u644d228b&originHeight=285&originWidth=658&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15175&status=done&style=none&taskId=u47a515e8-1299-411f-b9e1-f9cf140fc8e&title=)<br />`toilet` 还可以添加颜色，里面的选项请自己 man 一下
```bash
$ toilet -f mono12 -F gay a girl
```
![2021-05-09-16-55-48-094250.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551045388-172c9b39-3fea-4650-9430-46ffc00598b2.png#clientId=u69ed2538-5813-4&from=ui&id=u02476187&originHeight=187&originWidth=578&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5542&status=done&style=none&taskId=u16b7c08d-f990-41f4-a36b-6b93ec32fa4&title=)
<a name="xWAJB"></a>
## 6、`oneko` 命令
桌面上出现一直喵星人，跟着鼠标跑，不动了它就睡觉。
<a name="sE7mu"></a>
### 安装
```bash
$sudo apt-get install oneko
```
<a name="DhRPn"></a>
### 运行
```bash
$ oneko
```
要关掉这家伙，按ctrl+c 结束<br />![2021-05-09-16-55-48-157078.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551055744-3e6e9814-6387-4e82-9faf-dc9152e4e284.png#clientId=u69ed2538-5813-4&from=ui&id=ud776089b&originHeight=281&originWidth=677&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5289&status=done&style=none&taskId=ud130129d-2a3e-4ad9-9d59-57d87e44c7a&title=)
<a name="anrex"></a>
## 7、xeyes 命令
在屏幕上出现一双眼睛，盯着你的鼠标指针<br />安装运行同上<br />![2021-05-09-16-55-48-241851.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551065223-360bb28d-9707-438e-a573-26911066886d.png#clientId=u69ed2538-5813-4&from=ui&id=u34d26e00&originHeight=208&originWidth=466&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17119&status=done&style=none&taskId=u0f507c21-875d-469d-868d-1186d90d81e&title=)
<a name="aiTMq"></a>
## 8、yes 命令
输出无穷无尽的字符，按ctrl+c结束，如
```bash
$ yes 我很NB
```
<a name="bw482"></a>
## 9、`cal 9 1752`
`cal`是打印日历，不过这个是很奇葩的一个月，<br />![2021-05-09-16-55-48-335598.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551073261-58af9750-5a33-44a5-a73f-ea422613f8f9.png#clientId=u69ed2538-5813-4&from=ui&id=u26cbc57b&originHeight=124&originWidth=270&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9045&status=done&style=none&taskId=u5f59ab2e-7b67-40ca-a1a0-141a3e5497e&title=)
<a name="AzmrJ"></a>
## 10、shred
覆盖搞乱文件，就是文档粉碎，把私藏的大片种子和电影销毁，不被恢复出来，就靠它了
<a name="iYpaN"></a>
## 11、factor
分解因数，这个……小学生可以用一下<br />![2021-05-09-16-55-48-415385.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551090605-0d6eeb0e-0abc-4271-ace5-c5e041f59bdc.png#clientId=u69ed2538-5813-4&from=ui&id=u3d03aaa7&originHeight=40&originWidth=338&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3953&status=done&style=none&taskId=u504a248b-b02c-4ec1-95f6-e0eac4399d4&title=)
<a name="Jf6oN"></a>
## 12、asciiquarium 水族馆
<a name="QIM42"></a>
### 安装
```bash
yum install perl-Curses perl-ExtUtils-MakeMaker perl-Data-Dumper -y
cd /tmp
wget http://search.cpan.org/CPAN/authors/id/K/KB/KBAUCOM/Term-Animation-2.6.tar.gz
tar -zxvf Term-Animation-2.6.tar.gz
cd Term-Animation-2.6/
perl Makefile.PL && make && make test
make install
cp asciiquarium /usr/local/bin
chmod 0755 /usr/local/bin/asciiquarium
```
<a name="mFQ1b"></a>
### 运行
```bash
> asciiquarium
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1645146547262-224802b0-3efe-4b3b-b38c-9320d6d13463.gif#clientId=u0185a38f-b721-4&from=paste&id=u092dfc17&originHeight=562&originWidth=1038&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u429cd03d-0381-4347-8e99-5ce79838dab&title=)

<a name="eW3An"></a>
## 13、asciiview 图片转换字符画
asciiview 命令可以将任意图片转换成字符画的形式输出，安装方法如下：
```bash
sudo apt-get install aview imagemagick
asciiview hepburn.pen
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645146581764-fde1a492-fdcd-4d55-a382-d264700d6772.webp#clientId=u0185a38f-b721-4&from=paste&id=u05fa38fd&originHeight=697&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u22e5dfb0-ecfa-4b4b-8915-b3bedb7f8a8&title=)
<a name="MKniC"></a>
## 14、ninvaders 太空入侵者
<a name="gi0FP"></a>
### 安装
```bash
sudo apt-get install ninvaders
```
<a name="RPHpT"></a>
### 运行
```bash
> ninvaders
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1645146660905-59964622-0ed6-4a94-9e49-b813514182ba.gif#clientId=u0185a38f-b721-4&from=paste&id=u21913244&originHeight=497&originWidth=814&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub20ef299-84a8-4769-b0ce-35cdc216e2e&title=)
<a name="ocsib"></a>
## 15、bastet 俄罗斯方块
<a name="MRHVs"></a>
### 安装
```bash
sudo apt-get install bastet
```
<a name="ZTiiT"></a>
### 执行
```bash
> bastet
```
<a name="L3f7N"></a>
## 16、挨个敲下面这一堆，aptitude 没有的可以安装上
```bash
aptitude moo

aptitude -v moo

aptitude -vv moo

aptitude -vvv moo

aptitude -vvvv moo

aptitude -vvvvv moo

aptitude -vvvvvv moo

aptitude -vvvvvvv moo
```
<a name="ZvXsV"></a>
## 17、高大上仪表盘blessed-contrib
```bash
sudo apt-get install npm
sudo apt install nodejs-legacy
git clone https://github.com/yaronn/blessed-contrib.git
cd blessed-contrib
npm install
node ./examples/dashboard.js
```
高大上黑客仪表盘<br />![2021-05-09-16-55-48-531076.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551106780-3e4ccffe-52fa-4b45-897b-18ab3311717a.png#clientId=u69ed2538-5813-4&from=ui&id=ube0180a9&originHeight=482&originWidth=1000&originalType=binary&ratio=1&rotation=0&showTitle=false&size=82218&status=done&style=shadow&taskId=u061e1b5d-7467-4cd1-baae-9890a5f78ab&title=)
<a name="EpxI8"></a>
## 18、高大上仪表盘hollywood
Dustin Kirkland 利用一个长途飞行的时间，编写了这个炫酷、有趣但也没什么实际作用的软件。  <br />在其它Linux发行版中，可以通过以下命令安装并运行。 
```bash
sudo apt-add-repository ppa:hollywood/ppa
sudo apt-get install hollywood
sudo apt-get install byobu
hollywood
```
![2021-05-09-16-55-48-641780.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551118941-b643d2bc-8f65-4caf-a556-1020a02eb8c9.png#clientId=u69ed2538-5813-4&from=ui&id=u71816a68&originHeight=482&originWidth=1000&originalType=binary&ratio=1&rotation=0&showTitle=false&size=84885&status=done&style=none&taskId=uec2ec3a3-e403-4068-b0e5-084b55a08b1&title=)
<a name="poPFu"></a>
## 19、追逐鼠标的小猫oneko
在桌面的命令行界面输入  
```bash
sudo apt-get install oneko
oneko
```
然后输入oneko，即可看到效果。 <br />![2021-05-09-16-55-49-163385.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1620551131723-3dccedd2-9f71-485e-875d-2edf0c4288d6.gif#clientId=u69ed2538-5813-4&from=ui&id=u955a88f1&originHeight=491&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1505640&status=done&style=none&taskId=u3977f89a-9c17-4e5b-b0c1-5591065c5a9&title=)
<a name="ykvq2"></a>
## 20、ASCII艺术框：box命令 
```bash
sudo apt-get install boxes
echo "Tongji Univerisity" | boxes
echo "Tongji Univerisity" | boxes -d dog
fortune | boxes -d cat | lolcat
```
![2021-05-09-16-55-49-274090.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551142955-3369a427-803b-4c06-9c0e-4e95405eb79e.png#clientId=u69ed2538-5813-4&from=ui&id=u2ffc9893&originHeight=625&originWidth=775&originalType=binary&ratio=1&rotation=0&showTitle=false&size=54268&status=done&style=none&taskId=u6b868666-ecf9-4ac7-b2d7-edd4b39a485&title=)
<a name="G4oN6"></a>
## 21、燃起字符串大火`aafire`在命令行界面输入 
```bash
sudo apt-get install libaa-bin
aafire
```
然后输入 `aafire`，即可看到效果 <br />![2021-05-09-16-55-49-408731.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551155072-c7e4e610-300d-4b26-9bd4-0a883c058b94.png#clientId=u69ed2538-5813-4&from=ui&id=u9cab0a1e&originHeight=481&originWidth=1000&originalType=binary&ratio=1&rotation=0&showTitle=false&size=99039&status=done&style=none&taskId=udd34b121-7ccb-48bf-8d8c-f16f1e55482&title=)<br />`aafire`字符串大火
<a name="SLGD9"></a>
## 22、图片转字符串
这条命令在树莓派上运行会出问题，建议在云主机或虚拟机上运行。
```bash
sudo apt-get install aview imagemagick
wget http://labfile.oss.aliyuncs.com/courses/1/Linus.png
asciiview Linus.png
```
![2021-05-09-16-55-49-592238.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620551173386-3c5b3f42-6af4-4e9b-916f-db55941efe61.png#clientId=u69ed2538-5813-4&from=ui&id=u3914e22d&originHeight=691&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=358758&status=done&style=none&taskId=u63394b86-d0c3-4600-bdf6-2a30ec66332&title=)
<a name="kpQF1"></a>
## 23、从删库到跑路 sudo rm -rf /* 
```bash
sudo rm -rf /*
sudo：获取root管理员权限
rm：remove，即删除
-rf：r表示递归删除，即删除所有的子目录，f表示不需要再进行确认
/：根目录
*：所有文件
```
![2021-05-09-16-55-49-968234.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1620551183219-02e6f952-62ce-414c-af52-d9853de548d6.gif#clientId=u69ed2538-5813-4&from=ui&id=u9e63e8b1&originHeight=225&originWidth=400&originalType=binary&ratio=1&rotation=0&showTitle=false&size=961027&status=done&style=none&taskId=u45157c34-cfd0-4ca0-821d-a49e43fcf1a&title=)
