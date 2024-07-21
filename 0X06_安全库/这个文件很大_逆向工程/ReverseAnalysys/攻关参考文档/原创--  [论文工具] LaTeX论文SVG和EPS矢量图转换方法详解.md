# 原创
：  [论文工具] LaTeX论文SVG和EPS矢量图转换方法详解

# [论文工具] LaTeX论文SVG和EPS矢量图转换方法详解

<font color="red">**祝大家中秋国庆双节快乐！**</font><br/> 回过头来，我们在编程过程中，经常会遇到各种各样的问题。然而，很多问题都无法解决，网上夹杂着各种冗余的回答，也缺乏系统的实战技巧归纳。为更好地从事科学研究和编程学习，后续将总结各类常见实战技巧，希望对您有所帮助。

本文主要介绍LaTeX论文SVG和EPS矢量图转换方法总结，包括Visio、Excel、Matplotlib等常见方法转换，总体而言是将图片转换为SVG，再转EPS矢量图和生成PDF文件，最终在LaTeX中显示。本文一方面作为自己的学习笔记，另一方面希望能帮助初学者解决实际问题，且看且珍惜！

#### 文章目录

---


## 一.非矢量图模糊问题

假设存在如下图所示的图像，在LaTeX中会调用 \includegraphics 显示图像，通常期刊或会议会要求为矢量图，并且格式为EPS。

然而，如果不是矢量图，其图片放大后边缘会形成锯齿状，即使是EPS格式也可能是错误转换的矢量图形式，如下图所示，其图片内容比较模糊。

那么，如何解决矢量图转换的问题呢？因为我们在画图或编程生成实验效果图过程中，通常会利用Visio、Excel或编写Python代码。本文主要介绍常见的EPS矢量图转换方法，其核心流程为：

后续内容包括：

**此外，大家尤其需要注意：（1）不要直接用PNG转EPS矢量图，因为大部分转换会失败，即使是EPS格式的图像；（2）部分在线网站提供SVG转EPS矢量图的过程，大家可以使用，但需要在最终版PDF论文文件中，放大看是否模糊；（3）部分LaTeX还需要PDF格式的图像文件，但LaTeX会自动将EPS转换为“XX-eps-converted-to”格式，如果不行选择在线网站转换。**

---


## 二.Visio转矢量图EPS至LaTeX

通常在英文论文撰写中，我们会利用Visio绘制框架图或示例图，如何将其转换为EPS矢量图呢？

第一步，利用Visio绘制框架图或示意图，通常为VSD格式。

第二步，将图像导出为SVG格式（而非PNG格式）。

第三步，利用在线网站将SVG图像转换为EPS矢量图。强烈推荐下列这个网站，其它网站效果较差。

输出结果如下图所示，矢量图放在大字体也是清晰的。

第四步，尝试在线将EPS矢量图转换成PDF文件。

第五步，将图像按照指定名字命名，然后在LaTeX中显示。

```
\begin{figure}[!ht]
	\centering
	\includegraphics[width=0.45\textwidth]{5.eps}
	\caption{An example of obfuscation and deobfuscation.}
	\label{fig5}
\end{figure}

```

显示如下图所示：

最终完成LaTeX矢量图及显示，放大也非常清晰。

---


## 三.Matplotlib绘制图转矢量图EPS至LaTeX

在实验过程中，我们通常会撰写Python代码绘制实验效果图，其中画图会调用Matplotlib库实现。此外，由于深度学习代码绘制图像（如ROC曲线）较为复杂，我们仅撰写Matplotlib绘制图的代码，如下所示：

```
from matplotlib import pyplot
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

#解决中文显示问题
plt.rcParams['font.sans-serif'] = ['Times New Roman']
plt.rcParams['axes.unicode_minus'] = False
plt.rcParams.update({'font.size': 16})

x = ['1','2','3','4','5','6','7','8','9','10']

#Accuacy
CNN = [0.7315, 0.8271, 0.9069, 0.9304, 0.9424, 0.9464, 0.9480, 0.9493, 0.9509, 0.9517]
BiLSTM = [0.8245, 0.8957, 0.9322, 0.9497, 0.9588, 0.9603, 0.9608, 0.9612, 0.9616, 0.9617]
BiGRU = [0.8110, 0.8823, 0.9221, 0.9454, 0.9548, 0.9582, 0.9588, 0.9595, 0.9598, 0.9604]
Transformer = [0.8512, 0.9111, 0.9438, 0.9563, 0.9611, 0.9619, 0.9625, 0.9630, 0.9631, 0.9631]
Our = [0.9318, 0.9606, 0.9700, 0.9706, 0.9710, 0.9712, 0.9711, 0.9714, 0.9712, 0.9715]

plt.xlim(-0.5, 10)
plt.plot(x, CNN, marker='o', c='r', mec='r', mfc='w', linestyle=":",label='CNN')
plt.plot(x, BiLSTM, marker='D', ms=5,linestyle=":", label='BiLSTM')
plt.plot(x, BiGRU, marker='*', ms=8, linestyle="-.", label='BiGRU')
plt.plot(x, Transformer, marker='x', ms=6,linestyle="--", label='Transformer')
plt.plot(x, Our, marker='.', c='b', mec='b', ms=10, label='Our Method')
plt.legend(loc="lower right", fontsize="14")
plt.xticks(x, x, rotation=1)
plt.gca().yaxis.set_major_formatter(ticker.FormatStrFormatter('%.4f'))
plt.margins(0)
plt.subplots_adjust(bottom=0.10)
plt.xlabel('Epoch')
plt.ylabel("Accuracy")
plt.ylim(0.7000, 1.0000)  
plt.savefig('result-acc.svg', format='svg', dpi=1000)
plt.show()

```

第一步，撰写Python绘制图像代码，生成如下图所示的结果。

第二步，大家可以导出SVG或EPS矢量图，如下图所示。

第三步，利用AI软件导出PDF格式文件。

第四步，重命名EPS和PDF，并利用LaTeX显示图像。

```
\begin{figure}[!ht]
	\centering
	\includegraphics[width=0.5\textwidth]{6.eps}
	\caption{The accuracy of different methods.}
	\label{fig6}
\end{figure}

```

最终显示效果如下图所示，请放大清晰。

然而，导出为EPS图像时其留白界面较多，导致最终显示图像效果较差。一方面，读者可以尝试AI编辑EPS图像时调整背景大小；另一方面，Matplotlib导出图像时可以选择SVG图像，再转换为指定的EPS和PDF文件，可以选择在线转换或AI工具转换。

最终修改后的效果如下图所示，总之推荐的顺序还是为：

---


## 四.Excel转矢量图EPS至LaTeX

在实验过程中，部分读者习惯用Excel绘制实验结果，我们同样需要将Excel转换为矢量图。

第一步，利用Excel绘制实验效果图，如下图所示。然而，Office缺乏将Excel转换为矢量图的功能，在线网站转换的效果也非常差。那么，如何解决呢？

第二步，由于Excel和Visio之间可以相互转换（相同家族产品），因此，我们将Excel绘制的图像复制到Visio中。

为更饱满地显示图像，按住Ctrl拖动鼠标能将图像调整为更饱满。

第三步，在Visio中将图片导出为SVG矢量图。

第四步，再利用前面两个部分（在线网站或AI工具）将SVG图转换为EPS矢量图或PDF文件，最终显示效果如下图所示。

---


## 五.AI和PS转矢量图EPS至LaTeX

在前面的内容中，我们已经看到了AI软件转EPS矢量图的过程。同样，PS也具有相同的功能，但再次强调，不推荐大家直接将PNG图片转换为矢量图，建议SVG图片转换。在这里，我们尝试将PNG图像转换为矢量图，看看其模糊的效果。

输出结果如下图所示，图像比较模糊。

---


## 六.总结

写到这里，希望这篇文章对您有所帮助，后续也将继续更新内容。还有种在线绘制实验结果图的，以后有机会再介绍。需要注意：

> 
月是故乡明，伟大祖国母亲生日快乐。​见过各地山水，最清澈的还是家乡施秉的杉木河；吃过很多酸汤，最正宗的还是家乡黔东南的红酸白酸。难得这两天回老家看看，看到了家乡的发展和变化；看到了自己零几年的读书笔记；看到了小学中学的奖状和证件，值得纪念的是全县中考第一和那两张全国物理和数学竞赛获奖；看到了父亲为自己包好的各类文学书，以及父亲九几年手写的书法和绘画，并刻在自家的桌椅上（图3），父爱母爱伟大。<br/><br/> ​回首，人生犹如贵州连绵的青山绿水，总是起起伏伏，从小学到初中是上升期；紧接着到高中又直线下降；到大学初学程序又波折起伏；硕士毕业初为老师又缓慢下降；到发改借调又短暂回升；读博又起伏波折，如今又直线下降。然而，家乡的大山总能让我们养成质朴的性格，正如女神总笑我去哪都要背着个电脑，陪伴之余还是要敲敲代码，看看这平凡的世界。脚踏实地，知行合一，见笑见笑，爱你们喔！祝大家双节快乐。 <br/><br/> <img alt="在这里插入图片描述" height="360" src="https://img-blog.csdnimg.cn/f430b4f1ed53451d86c993bcd871f824.jpeg#pic_center" width="500"/><br/> <img alt="在这里插入图片描述" height="360" src="https://img-blog.csdnimg.cn/08e6713aebdb455ab0aebb9171eaea1c.jpeg#pic_center" width="500"/>


(By:Eastmount 2023-10-06 夜于黔东南 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )
