# 原创
：  [论文工具] LaTeX常见错误及解决方法汇总笔记（eps2pdf失败 | 特殊符号 | 参考文献未显示或越界）

# [论文工具] LaTeX常见错误及解决方法汇总笔记（eps2pdf失败 | 特殊符号 | 参考文献未显示或越界）

<font color="red">**祝大家中秋国庆双节快乐！**</font><br/> 回过头来，我们在编程过程中，经常会遇到各种各样的问题。然而，很多问题都无法解决，网上夹杂着各种冗余的回答，甚至需要看自己多年前的博客来解决。为更好地从事科学研究和编程学习，后续将撰写各类常见错误及解决方法的汇总笔记，希望对您有所帮助。

本文主要介绍LaTeX常见错误，包括Package pdftex.def Error: File `figures//4-eps-converted-to.pdf’ not found: using draft setting、参考文献未显示及越界问题、特殊符号引用总结等，并且将长期更新和完善。本文采用的工具为TeXLive 2022，操作系统为Windows 11。

#### 文章目录

---


## 一.TexLive2022安装

TeXLive安装过程比较简单，可以从清华大学开源软件镜像站下载资源，如下图所示：

第一步，解压IOS文件并以管理员权限运行“install-tl-windows.bat”文件。

第二步，傻瓜式安装TeX Live，包括安装路径等。

第三步，点击“Advanced”中的“Customize”，选择要安装的内容，保留常见的语言。

接着是傻瓜式安装。

第四步，右键“我的电脑”-&gt;“属性”，点击“高级系统设置”，添加TeXLive的系统环境变量。

然后在运行（Win+R键）中输入CMD，输入“latex -v”查看版本信息，即安装成功。

第五步，尽管TeXLive自带Texworks editor，但很多读者习惯使用其它编辑器，如VS Code、TeXstudio等，因此介绍如何安装TeXstudio。下载后傻瓜式安装即可。

第六步，运行TeXstudio并在“选项”中设置TeXstudio，首先显示所有行号。

其次，在“构建”中设置编译器为“PdfLaTeX”。如果是中文，可以设置为“XeLaTeX”。

运行结果如下图所示：

---


## 二.Package pdftex.def Error: File `figures//4-eps-converted-to.pdf’ not found: using draft setting.

**背景知识：**<br/> 在使用Texlive对要提交的论文排版，通常要求图片为eps格式，并且LaTeX通常能自动转换为PDF文件（xx-eps-converted-to.pdf）。然而，eps图片在转换为PDF文件时会报错，如何解决该问题呢？

<img alt="在这里插入图片描述" height="300" src="https://img-blog.csdnimg.cn/c0ace21304e0456db0494b56064e4290.png#pic_center" width="550"/><br/> ·<br/> 引用代码：

```
\begin{figure}[!ht]
	\centering
	\includegraphics[width=0.3\textwidth]{4.eps}
	\caption{OFDM system}
	\label{fig8}
\end{figure}

```

**错误提示：**

**解决方法：**

第一步，检查图片命名规则。

第二步，增加宏包，注意顺序。

```
\usepackage{graphicx}
\usepackage{epstopdf}
\graphicspath{{figures/}}

```

第三步，用命令行编译。

```
pdflatex --shell-escape 具体文件名.tex

```

在需要编译tex文件夹下输入cmd，即可打开命令行。

然而仍然报错，提示“epstopdf 不是可执行文件”，怀疑是环境安装问题（最后一个错误）。

第四步，epstopdf需要安装ghostscript才能转换，重新下载Ghostscript和gswin32c.exe并进行安装，然而仍然失败。下载地址如下：

安装后的文件如下所示：

可以看到TexLive以及添加了相关程序路径。

第五步，欲哭无泪 o(╥﹏╥)o，卸载软件重新安装还是报错。

第六步，本地转换PDF文件仍然报错，确认是下列问题导致。

> 
GPL Ghostscript 9.55.0: **** Could not open temporary file ‘’ **** Unable to open the initial device, quitting. epstopdf ($Id: epstopdf.pl 48681 2018-09-16 23:03:58Z karl $) 2.28 !!! Error: Writing to gswin32c failed, error code 1<br/> 网上有反馈该报错由于中文路径或用户名为中文导致，然后作者电脑均为英文路径。


第七步，最后只能在线手动将EPS图片转为PDF文件，等以后再解决该问题，难受。

下面补充另一个常见错误及其解决方法。

> 
epstopdf ($Id: epstopdf.pl 66407 2023-03-06 23:44:49Z karl $) 2.31<br/> <mark>!!! Error: Input filename ‘fig/Fig1.eps’ not allowed in restricted mode.</mark><br/> Package pdftex.def Error: File `fig/XXX-eps-converted-to.pdf’ not found: using draft setting. …


该错误属于restricted相关的系统Bug，需要修改texmf-dist\scripts\epstopdf\epstopdf.pl文件代码，如下图所示，将“$restricted = 1 if $::opt_restricted;”修改为“$restricted = 0 if $::opt_restricted;”。

<img alt="在这里插入图片描述" height="200" src="https://img-blog.csdnimg.cn/bd48472a5705471ba24e40edc024dfa1.png#pic_center" width="550"/><br/> <img alt="在这里插入图片描述" height="200" src="https://img-blog.csdnimg.cn/411d9a32ec204fcc904e0e923345c276.png#pic_center" width="550"/>

如果该文件没有写入权限，则修改下权限即可。

> 
参考资料：



---


## 三.Texlive参考文献Bib未显示

在Texlive中，通常有两种格式的引用参考文献。

<mark>（1）第一种方式直接在main.tex文件中撰写，以thebibliography的形式。</mark>

```
\begin{thebibliography}{49}

\bibitem{b1} Microsoft, ``What is PowerShell? - PowerShell | Microsoft Docs,'' 
    Website: https://docs.microsoft.com/en-us/powershell/scripting/overview, 2022.

\bibitem{b2} D. Hendler, S. Kels, et al., ``AMSI-Based Detection of Malicious PowerShell 
    Code Using Contextual Embeddings,'' in 15th ACM Asia Conference on Computer and 
    Communications Security (AsiaCCS). ACM, 2020, pp. 679-693.

\bibitem{b49} M. Ring, D. Schlor, et al., ``Malware detection on windows audit logs using 
  LSTMs,'' Computers \&amp; Security, vol.109, 2021, p. 102389. 

\end{thebibliography}

```

<mark>（2）第二种方式将参考文献写在myref.bib，以 \bibliography{myref} 的形式添加。</mark>

```
@misc{b1,
  title = {What is PowerShell? - PowerShell | Microsoft Docs},
  url = {https://docs.microsoft.com/en-us/powershell/scripting/overview},
  author = {Microsoft},
  year = {2022}
}

@inproceedings{b2,
  title={Amsi-based detection of malicious powershell code using contextual embeddings},
  author={Hendler, Danny and Kels, Shay and Rubin, Amir},
  booktitle={Proceedings of the 15th ACM Asia Conference on Computer and Communications Security (AsiaCCS)},
  pages={679--693},
  year={2020},
  organization = {ACM}
}

@article{b49,
  title={Malware detection on windows audit logs using LSTMs},
  author={Ring, Markus and Schl{\"o}r, Daniel and Wunderlich, Sarah and Landes, Dieter and Hotho, Andreas},
  journal={Computers \&amp; Security},
  volume={109},
  pages={102389},
  year={2021},
  publisher={Elsevier}
}

```

参考文献BIB格式可以通过谷歌学术下载。同样，ChatGPT能够修改对应的格式，如百度的文心一言。参考文献格式推荐：

然后在生成参考文献时，**总会遇到文献未显示的现象**，如下图所示。

解决方法通常包括三个步骤：

> 
参考资料：



---


## 四.特殊符号引用总结

**1.半圆**<br/> 导入引用包和代码：

```
\usepackage{tikz}
\newcommand*\emptycirc[1][1ex]{\tikz\draw (0,0) circle (#1);} 
\newcommand*\halfcirc[1][1ex]{%
	\begin{tikzpicture}
	\draw[fill] (0,0)-- (90:#1) arc (90:270:#1) -- cycle ;
	\draw (0,0) circle (#1);
	\end{tikzpicture}}
\newcommand*\fullcirc[1][1ex]{\tikz\fill (0,0) circle (#1);} 

```

使用如下：

```
\fullcirc
\halfcirc
\emptycirc

```

> 
参考文献：https://blog.csdn.net/weixin_43846270/article/details/108202031


**2.勾叉**<br/> 导入引用包和代码：

```
\usepackage{pifont}       % \ding{xx}
\usepackage{bbding}       % \Checkmark,\XSolid,... (需要和pifont宏包共同使用)

```

使用如下：

```
\checkmark
\Checkmark
\CheckmarkBold
\XSolid
\XSolidBold
\XSolidBrush

```

其它参见的符号参考Xovee老师的文章，详见参考文献。

> 
参考文献：



**3.纸牌**

引用代码如下：

```
$\clubsuit$
$\spadesuit$
$\heartsuit$
$\diamondsuit$

```

其它常见的符号推荐如下所示的博客。

---


## 五.参考文献超链接太长越界

在LaTeX中，通常引用在线文章时，会出现参考文献太长的情况，尤其是URL。如下图所示：

```
@misc{30,
  title = {Practical behavioral profiling of PowerShell scripts through static analysis (Part2)},
  url = {https://unit42.paloaltonetworks.com/practical-behavioral-profiling-of-powershell-scripts-through-static-analysis-part-2},
  author = {Jeff White},
  year = {2017}
}

```

解决方法是导入如下包即可。

```
\usepackage{url}
\def\UrlBreaks{\do\A\do\B\do\C\do\D\do\E\do\F\do\G\do\H\do\I\do\J
\do\K\do\L\do\M\do\N\do\O\do\P\do\Q\do\R\do\S\do\T\do\U\do\V
\do\W\do\X\do\Y\do\Z\do\[\do\\\do\]\do\^\do\_\do\`\do\a\do\b
\do\c\do\d\do\e\do\f\do\g\do\h\do\i\do\j\do\k\do\l\do\m\do\n
\do\o\do\p\do\q\do\r\do\s\do\t\do\u\do\v\do\w\do\x\do\y\do\z
\do\.\do\@\do\\\do\/\do\!\do\_\do\|\do\;\do\&gt;\do\]\do\)\do\,
\do\?\do\'\do+\do\=\do\#}

```

导入后的效果如下图所示，而且对齐美观。

另一种解决方法说是调用breakurl包实现，而URL通常利用“-”连接。

```
\usepackage{hyperref}
\usepackage[hyphenbreaks]{breakurl}

```

---


## 六.表格越界垂直居中和竖直显示问题

```
\begin{table*}[!ht]
	\centering
	\caption{Related Work Comparison.}
	\resizebox{\textwidth}{!}{
	\begin{tabular}{m{0.5cm}&lt;{\centering}cm{3.5cm}&lt;{\centering}ccccccccc} \hline

      ...
		\end{tabular}}
	\label{tab2}
\end{table*}

```

（持续更新中）

---


## 总结

写到这里，希望这篇文章对您有所帮助，后续也将继续更新内容，并将一些LaTeX使用技巧进行总结，比如表格、算法、图片等排版内容。

> 
月是故乡明，伟大祖国母亲生日快乐。​见过各地山水，最清澈的还是家乡施秉的杉木河；吃过很多酸汤，最正宗的还是家乡黔东南的红酸白酸。难得这两天回老家看看，看到了家乡的发展和变化；看到了自己零几年的读书笔记；看到了小学中学的奖状和证件，值得纪念的是全县中考第一和那两张全国物理和数学竞赛获奖；看到了父亲为自己包好的各类文学书，以及父亲九几年手写的书法和绘画，并刻在自家的桌椅上（图3），父爱母爱伟大。<br/><br/> ​回首，人生犹如贵州连绵的青山绿水，总是起起伏伏，从小学到初中是上升期；紧接着到高中又直线下降；到大学初学程序又波折起伏；硕士毕业初为老师又缓慢下降；到发改借调又短暂回升；读博又起伏波折，如今又直线下降。然而，家乡的大山总能让我们养成质朴的性格，正如女神总笑我去哪都要背着个电脑，陪伴之余还是要敲敲代码，看看这平凡的世界。脚踏实地，知行合一，见笑见笑，爱你们喔！祝大家双节快乐。 <br/><br/> <img alt="在这里插入图片描述" height="360" src="https://img-blog.csdnimg.cn/f430b4f1ed53451d86c993bcd871f824.jpeg#pic_center" width="500"/><br/> <img alt="在这里插入图片描述" height="360" src="https://img-blog.csdnimg.cn/08e6713aebdb455ab0aebb9171eaea1c.jpeg#pic_center" width="500"/>


(By:Eastmount 2023-10-03 夜于黔东南 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )
