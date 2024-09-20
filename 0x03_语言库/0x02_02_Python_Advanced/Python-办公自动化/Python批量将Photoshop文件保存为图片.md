Python
<a name="LOZRh"></a>
## 安装psd-tools包
```xml
pip install psd-tools
```
<a name="SwRGM"></a>
## 实现思路

1. 读取桌面文件找到所在的文件目录（这里可用`input()`改写复用） 
2. 再该文件下建一个文件夹printPhoto 
3. 对每个psd文件的路径进行获取（遍历）
4. 保存进行输出
<a name="rPRkn"></a>
## 代码展示
```python
from psd_tools import PSDImage
import os
psddir = 'C:/Users/Administrator/Desktop/123/'
#遍历每一个文件下的psd文件
files = os.listdir(psddir)
print(files)
#构建输出文件夹printphoto
outdir = psddir + '/' + 'printphoto'
#判断目录是否存在，不存在就新建一个
if not os.path.exists(outdir):
    print(f"{outdir}目录不存在")
    os.mkdir(outdir)
#printphoto文件夹也计数一次所以从0开始
i = 0
for f in files:
    i += 1
    fname = psddir + f
    print(fname)
    #打开每一个psd文件，除psd以外的文件可能报错需要删除可以用if写一个判断。
    psd = PSDImage.open(fname)
    photooutname = os.path.join(outdir, f.split('.')[0]+'.BMP')
    psd.composite().save(photooutname)
print(f'转化完成,一共有{i}个psd文件')
```
