<a name="TNzDY"></a>
## 1、解决 Linux 下 unzip 乱码的问题
```python
import os
import sys
import zipfile
import argparse

s = '\x1b[%d;%dm%s\x1b[0m'       

def unzip(path):

    file = zipfile.ZipFile(path,"r")
    if args.secret:
        file.setpassword(args.secret)

    for name in file.namelist():
        try:
            utf8name=name.decode('gbk')
            pathname = os.path.dirname(utf8name)
        except:
            utf8name=name
            pathname = os.path.dirname(utf8name)

        #print s % (1, 92, '  >> extracting:'), utf8name
        #pathname = os.path.dirname(utf8name)
        if not os.path.exists(pathname) and pathname != "":
            os.makedirs(pathname)
        data = file.read(name)
        if not os.path.exists(utf8name):
            try:
                fo = open(utf8name, "w")
                fo.write(data)
                fo.close
            except:
                pass
    file.close()

def main(argv):
    ######################################################
    # for argparse
    p = argparse.ArgumentParser(description='解决unzip乱码')
    p.add_argument('xxx', type=str, nargs='*', \
        help='命令对象.')
    p.add_argument('-s', '--secret', action='store', \
        default=None, help='密码')
    global args
    args = p.parse_args(argv[1:])
    xxx = args.xxx

    for path in xxx:
        if path.endswith('.zip'):
            if os.path.exists(path):
                print s % (1, 97, '  ++ unzip:'), path
                unzip(path)
            else:
                print s % (1, 91, '  !! file doesn\'t exist.'), path
        else:
            print s % (1, 91, '  !! file isn\'t a zip file.'), path

if __name__ == '__main__':
    argv = sys.argv
    main(argv)
```
<a name="VmQhW"></a>
## 2、统计当前根目录代码行数
```python
# coding=utf-8
import os
import time
# 设定根目录
basedir = './'
filelists = []
# 指定想要统计的文件类型
whitelist = ['cpp', 'h']
#遍历文件, 递归遍历文件夹中的所有
def getFile(basedir):
    global filelists
    for parent,dirnames,filenames in os.walk(basedir):
        for filename in filenames:
            ext = filename.split('.')[-1]
            #只统计指定的文件类型，略过一些log和cache文件
            if ext in whitelist:
                filelists.append(os.path.join(parent,filename))
#统计一个的行数
def countLine(fname):
    count = 0
    # 把文件做二进制看待,read.
    for file_line in open(fname, 'rb').readlines():
        if file_line != '' and file_line != '\n': #过滤掉空行
            count += 1
    print (fname + '----' , count)
    return count
if __name__ == '__main__' :
    startTime = time.clock()
    getFile(basedir)
    totalline = 0
    for filelist in filelists:
        totalline = totalline + countLine(filelist)
    print ('total lines:',totalline)
    print ('Done! Cost Time: %0.2f second' % (time.clock() - startTime))
```
<a name="Qrsit"></a>
## 3、扫描当前目录和所有子目录并显示大小
```python
import os
import sys      
try:
    directory = sys.argv[1]   
except IndexError:
    sys.exit("Must provide an argument.")

dir_size = 0   
fsizedicr = {'Bytes': 1,
             'Kilobytes': float(1) / 1024,
             'Megabytes': float(1) / (1024 * 1024),
             'Gigabytes': float(1) / (1024 * 1024 * 1024)}
for (path, dirs, files) in os.walk(directory):      
    for file in files:                              
        filename = os.path.join(path, file)
        dir_size += os.path.getsize(filename)       

fsizeList = [str(round(fsizedicr[key] * dir_size, 2)) + " " + key for key in fsizedicr] 

if dir_size == 0: print ("File Empty") 
else:
  for units in sorted(fsizeList)[::-1]: 
      print ("Folder Size: " + units)
```
<a name="BaSWG"></a>
## 4、将源目录240天以上的所有文件移动到目标目录
```python
import shutil
import sys
import time
import os
import argparse

usage = 'python move_files_over_x_days.py -src [SRC] -dst [DST] -days [DAYS]'
description = 'Move files from src to dst if they are older than a certain number of days.  Default is 240 days'

args_parser = argparse.ArgumentParser(usage=usage, description=description)
args_parser.add_argument('-src', '--src', type=str, nargs='?', default='.', help='(OPTIONAL) Directory where files will be moved from. Defaults to current directory')
args_parser.add_argument('-dst', '--dst', type=str, nargs='?', required=True, help='(REQUIRED) Directory where files will be moved to.')
args_parser.add_argument('-days', '--days', type=int, nargs='?', default=240, help='(OPTIONAL) Days value specifies the minimum age of files to be moved. Default is 240.')
args = args_parser.parse_args()

if args.days < 0:
  args.days = 0

src = args.src  # 设置源目录
dst = args.dst  # 设置目标目录
days = args.days # 设置天数
now = time.time()  # 获得当前时间

if not os.path.exists(dst):
  os.mkdir(dst)

for f in os.listdir(src):  # 遍历源目录所有文件
    if os.stat(f).st_mtime < now - days * 86400:  # 判断是否超过240天
        if os.path.isfile(f):  # 检查是否是文件
            shutil.move(f, dst)  # 移动文件
```
<a name="uEHbe"></a>
## 5、扫描脚本目录，并给出不同类型脚本的计数
```python
import os                                  
import shutil                                
from time import strftime                        

logsdir="c:\logs\puttylogs"                      
zipdir="c:\logs\puttylogs\zipped_logs"              
zip_program="zip.exe"                        

for files in os.listdir(logsdir):                    
  if files.endswith(".log"):                    
    files1=files+"."+strftime("%Y-%m-%d")+".zip"    
    os.chdir(logsdir)                         
    os.system(zip_program + " " +  files1 +" "+ files)  
    shutil.move(files1, zipdir)                   
    os.remove(files)
```
<a name="W73IZ"></a>
## 6、下载Leetcode的算法题
```python
import sys
import re
import os
import argparse
import requests
from lxml import html as lxml_html

try:
    import html
except ImportError:
    import HTMLParser
    html = HTMLParser.HTMLParser()

try:
    import cPickle as pk
except ImportError:
    import pickle as pk

class LeetcodeProblems(object):
    def get_problems_info(self):
        leetcode_url = 'https://leetcode.com/problemset/algorithms'
        res = requests.get(leetcode_url)
        if not res.ok:
            print('request error')
            sys.exit()
        cm = res.text
        cmt = cm.split('tbody>')[-2]
        indexs = re.findall(r'<td>(\d+)</td>', cmt)
        problem_urls = ['https://leetcode.com' + url \
                        for url in re.findall(
                            r'<a href="(/problems/.+?)"', cmt)]
        levels = re.findall(r"<td value='\d*'>(.+?)</td>", cmt)
        tinfos = zip(indexs, levels, problem_urls)
        assert (len(indexs) == len(problem_urls) == len(levels))
        infos = []
        for info in tinfos:
            res = requests.get(info[-1])
            if not res.ok:
                print('request error')
                sys.exit()
            tree = lxml_html.fromstring(res.text)
            title = tree.xpath('//meta[@property="og:title"]/@content')[0]
            description = tree.xpath('//meta[@property="description"]/@content')
            if not description:
                description = tree.xpath('//meta[@property="og:description"]/@content')[0]
            else:
                description = description[0]
            description = html.unescape(description.strip())
            tags = tree.xpath('//div[@id="tags"]/following::a[@class="btn btn-xs btn-primary"]/text()')
            infos.append(
                {
                    'title': title,
                    'level': info[1],
                    'index': int(info[0]),
                    'description': description,
                    'tags': tags
                }
            )

        with open('leecode_problems.pk', 'wb') as g:
            pk.dump(infos, g)
        return infos

    def to_text(self, pm_infos):
        if self.args.index:
            key = 'index'
        elif self.args.title:
            key = 'title'
        elif self.args.tag:
            key = 'tags'
        elif self.args.level:
            key = 'level'
        else:
            key = 'index'

        infos = sorted(pm_infos, key=lambda i: i[key])

        text_template = '## {index} - {title}\n' \
            '~{level}~  {tags}\n' \
            '{description}\n' + '\n' * self.args.line
        text = ''
        for info in infos:
            if self.args.rm_blank:
                info['description'] = re.sub(r'[\n\r]+', r'\n', info['description'])
            text += text_template.format(**info)

        with open('leecode problems.txt', 'w') as g:
            g.write(text)

    def run(self):
        if os.path.exists('leecode_problems.pk') and not self.args.redownload:
            with open('leecode_problems.pk', 'rb') as f:
                pm_infos = pk.load(f)
        else:
            pm_infos = self.get_problems_info()

        print('find %s problems.' % len(pm_infos))
        self.to_text(pm_infos)

def handle_args(argv):
    p = argparse.ArgumentParser(description='extract all leecode problems to location')
    p.add_argument('--index', action='store_true', help='sort by index')
    p.add_argument('--level', action='store_true', help='sort by level')
    p.add_argument('--tag', action='store_true', help='sort by tag')
    p.add_argument('--title', action='store_true', help='sort by title')
    p.add_argument('--rm_blank', action='store_true', help='remove blank')
    p.add_argument('--line', action='store', type=int, default=10, help='blank of two problems')
    p.add_argument('-r', '--redownload', action='store_true', help='redownload data')
    args = p.parse_args(argv[1:])
    return args

def main(argv):
    args = handle_args(argv)
    x = LeetcodeProblems()
    x.args = args
    x.run()

if __name__ == '__main__':
    argv = sys.argv
    main(argv)
```
<a name="JlRwv"></a>
## 7、将 Markdown 转换为 HTML
```python
import sys
import os

from bs4 import BeautifulSoup
import markdown


class MarkdownToHtml:

    headTag = '<head><meta charset="utf-8" /></head>'

    def __init__(self,cssFilePath = None):
        if cssFilePath != None:
            self.genStyle(cssFilePath)

    def genStyle(self,cssFilePath):
        with open(cssFilePath,'r') as f:
            cssString = f.read()
        self.headTag = self.headTag[:-7] + '<style type="text/css">{}</style>'.format(cssString) + self.headTag[-7:]

    def markdownToHtml(self, sourceFilePath, destinationDirectory = None, outputFileName = None):
        if not destinationDirectory:
            # 未定义输出目录则将源文件目录(注意要转换为绝对路径)作为输出目录
            destinationDirectory = os.path.dirname(os.path.abspath(sourceFilePath))
        if not outputFileName:
            # 未定义输出文件名则沿用输入文件名
            outputFileName = os.path.splitext(os.path.basename(sourceFilePath))[0] + '.html'
        if destinationDirectory[-1] != '/':
            destinationDirectory += '/'
        with open(sourceFilePath,'r', encoding='utf8') as f:
            markdownText = f.read()
        # 编译出原始 HTML 文本
        rawHtml = self.headTag + markdown.markdown(markdownText,output_format='html5')
        # 格式化 HTML 文本为可读性更强的格式
        beautifyHtml = BeautifulSoup(rawHtml,'html5lib').prettify()
        with open(destinationDirectory + outputFileName, 'w', encoding='utf8') as f:
            f.write(beautifyHtml)


if __name__ == "__main__":
    mth = MarkdownToHtml()
    # 做一个命令行参数列表的浅拷贝，不包含脚本文件名
    argv = sys.argv[1:]
    # 目前列表 argv 可能包含源文件路径之外的元素（即选项信息）
    # 程序最后遍历列表 argv 进行编译 markdown 时，列表中的元素必须全部是源文件路径
    outputDirectory = None
    if '-s' in argv:
        cssArgIndex = argv.index('-s') +1
        cssFilePath = argv[cssArgIndex]
        # 检测样式表文件路径是否有效
        if not os.path.isfile(cssFilePath):
            print('Invalid Path: '+cssFilePath)
            sys.exit()
        mth.genStyle(cssFilePath)
        # pop 顺序不能随意变化
        argv.pop(cssArgIndex)
        argv.pop(cssArgIndex-1)
    if '-o' in argv:
        dirArgIndex = argv.index('-o') +1
        outputDirectory = argv[dirArgIndex]
        # 检测输出目录是否有效
        if not os.path.isdir(outputDirectory):
            print('Invalid Directory: ' + outputDirectory)
            sys.exit()
        # pop 顺序不能随意变化
        argv.pop(dirArgIndex)
        argv.pop(dirArgIndex-1)
    # 至此，列表 argv 中的元素均是源文件路径
    # 遍历所有源文件路径
    for filePath in argv:
        # 判断文件路径是否有效
        if os.path.isfile(filePath):
            mth.markdownToHtml(filePath, outputDirectory)
        else:
            print('Invalid Path: ' + filePath)
```
<a name="UDZzR"></a>
## 8、文本文件编码检测与转换
```python
import sys
import os
import argparse
from chardet.universaldetector import UniversalDetector

parser = argparse.ArgumentParser(description = '文本文件编码检测与转换')
parser.add_argument('filePaths', nargs = '+',
                   help = '检测或转换的文件路径')
parser.add_argument('-e', '--encoding', nargs = '?', const = 'UTF-8',
                   help = '''
目标编码。支持的编码有：
ASCII, (Default) UTF-8 (with or without a BOM), UTF-16 (with a BOM),
UTF-32 (with a BOM), Big5, GB2312/GB18030, EUC-TW, HZ-GB-2312, ISO-2022-CN, EUC-JP, SHIFT_JIS, ISO-2022-JP,
ISO-2022-KR, KOI8-R, MacCyrillic, IBM855, IBM866, ISO-8859-5, windows-1251, ISO-8859-2, windows-1250, EUC-KR,
ISO-8859-5, windows-1251, ISO-8859-1, windows-1252, ISO-8859-7, windows-1253, ISO-8859-8, windows-1255, TIS-620
''')
parser.add_argument('-o', '--output',
                   help = '输出目录')
# 解析参数，得到一个 Namespace 对象
args = parser.parse_args()
# 输出目录不为空即视为开启转换, 若未指定转换编码，则默认为 UTF-8
if args.output != None:
    if not args.encoding:
        # 默认使用编码 UTF-8
        args.encoding = 'UTF-8'
    # 检测用户提供的输出目录是否有效
    if not os.path.isdir(args.output):
        print('Invalid Directory: ' + args.output)
        sys.exit()
    else:
        if args.output[-1] != '/':
            args.output += '/'
# 实例化一个通用检测器
detector = UniversalDetector()
print()
print('Encoding (Confidence)',':','File path')
for filePath in args.filePaths:
    # 检测文件路径是否有效，无效则跳过
    if not os.path.isfile(filePath):
        print('Invalid Path: ' + filePath)
        continue
    # 重置检测器
    detector.reset()
    # 以二进制模式读取文件
    for each in open(filePath, 'rb'):
        # 检测器读取数据
        detector.feed(each)
        # 若检测完成则跳出循环
        if detector.done:
            break
    # 关闭检测器
    detector.close()
    # 读取结果
    charEncoding = detector.result['encoding']
    confidence = detector.result['confidence']
    # 打印信息
    if charEncoding is None:
        charEncoding = 'Unknown'
        confidence = 0.99
    print('{} {:>12} : {}'.format(charEncoding.rjust(8),
        '('+str(confidence*100)+'%)', filePath))
    if args.encoding and charEncoding != 'Unknown' and confidence > 0.6:
        # 若未设置输出目录则覆盖源文件
        outputPath = args.output + os.path.basename(filePath) if args.output else filePath
        with open(filePath, 'r', encoding = charEncoding, errors = 'replace') as f:
            temp = f.read()
        with open(outputPath, 'w', encoding = args.encoding, errors = 'replace') as f:
            f.write(temp)
```
