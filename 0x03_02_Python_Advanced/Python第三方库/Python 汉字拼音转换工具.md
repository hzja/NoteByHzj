Python<br />将汉字转为拼音，可以用于批量汉字注音、文字排序、拼音检索文字等常见场景。<br />现在互联网上有许多拼音转换工具，基于Python的开源模块也不少，介绍一个功能特性最多的模块:  pypinyin ，它支持以下特性：

1. 根据词组智能匹配最正确的拼音。
2. 支持多音字。
3. 简单的繁体支持, 注音支持。
4. 支持多种不同拼音/注音风格。
5. 命令行工具一键转化
<a name="yUSMs"></a>
## 1、准备
开始之前，要确保Python和pip已经成功安装在电脑上。
```bash
pip install pypinyin
```
<a name="GLSyx"></a>
## 2、基本使用
最普通的拼音转化方法如下：
```python
from pypinyin import pinyin, lazy_pinyin, Style
pinyin('中心')
# [['zhōng'], ['xīn']]
```
识别多音字：
```python
from pypinyin import pinyin, lazy_pinyin, Style
pinyin('中心', heteronym=True) # 启用多音字模式
# [['zhōng', 'zhòng'], ['xīn']]
```
设置输出风格，只识别首字母：
```python
from pypinyin import pinyin, lazy_pinyin, Style
pinyin('中心', style=Style.FIRST_LETTER) # 设置拼音风格
# [['z'], ['x']]
```
修改音调输出位置，在相应字母的后面显示音调，或者拼音的最后显示音调：
```python
from pypinyin import pinyin, lazy_pinyin, Style

# TONE2 在相应字母的后面显示音调
pinyin('中心', style=Style.TONE2, heteronym=True)
# [['zho1ng', 'zho4ng'], ['xi1n']]

# TONE3 拼音的最后显示音调
pinyin('中心', style=Style.TONE3, heteronym=True)
# [['zhong1', 'zhong4'], ['xin1']]
```
不考虑多音字的情况：
```python
from pypinyin import pinyin, lazy_pinyin, Style
lazy_pinyin('中心') # 不考虑多音字的情况
# ['zhong', 'xin']
```
不使用v来代替ü:
```python
from pypinyin import pinyin, lazy_pinyin, Style
lazy_pinyin('战略', v_to_u=True) # 不使用 v 表示 ü
# ['zhan', 'lüe']
```
标记轻声：
```python
from pypinyin import pinyin, lazy_pinyin, Style
# 使用 5 标识轻声
lazy_pinyin('衣裳', style=Style.TONE3, neutral_tone_with_five=True)
# ['yi1', 'shang5']


使用命令行一键识别拼音：
python -m pypinyin 音乐
# yīn yuè
```
<a name="jcubj"></a>
## 3、高级使用
<a name="QkloZ"></a>
### 自定义拼音显示风格
可以通过 `register()` 来实现自定义拼音风格的需求：
```python
from pypinyin import lazy_pinyin

from pypinyin.style import register

@register('kiss')
def kiss(pinyin, **kwargs):
    return '😘 {0}'.format(pinyin)

  
lazy_pinyin('么么', style='kiss')
# ['😘 me', '😘 me']
```
可以见到，通过定义一个 kiss 函数，使用 register 装饰器，生成了一个新的 style，这个 style 可以直接被用于拼音的转换参数，非常方便。<br />另外，所有模块自带的 style 及其效果如下：
```python
@unique
class Style(IntEnum):
    """拼音风格"""

    #: 普通风格，不带声调。如：中国 -> ``zhong guo``
    NORMAL = 0
    #: 标准声调风格，拼音声调在韵母第一个字母上（默认风格）。如：中国 -> ``zhōng guó``
    TONE = 1
    #: 声调风格2，即拼音声调在各个韵母之后，用数字 [1-4] 进行表示。如：中国 -> ``zho1ng guo2``
    TONE2 = 2
    #: 声调风格3，即拼音声调在各个拼音之后，用数字 [1-4] 进行表示。如：中国 -> ``zhong1 guo2``
    TONE3 = 8
    #: 声母风格，只返回各个拼音的声母部分（注：有的拼音没有声母，详见 `#27`_）。如：中国 -> ``zh g``
    INITIALS = 3
    #: 首字母风格，只返回拼音的首字母部分。如：中国 -> ``z g``
    FIRST_LETTER = 4
    #: 韵母风格，只返回各个拼音的韵母部分，不带声调。如：中国 -> ``ong uo``
    FINALS = 5
    #: 标准韵母风格，带声调，声调在韵母第一个字母上。如：中国 -> ``ōng uó``
    FINALS_TONE = 6
    #: 韵母风格2，带声调，声调在各个韵母之后，用数字 [1-4] 进行表示。如：中国 -> ``o1ng uo2``
    FINALS_TONE2 = 7
    #: 韵母风格3，带声调，声调在各个拼音之后，用数字 [1-4] 进行表示。如：中国 -> ``ong1 uo2``
    FINALS_TONE3 = 9
    #: 注音风格，带声调，阴平（第一声）不标。如：中国 -> ``ㄓㄨㄥ ㄍㄨㄛˊ``
    BOPOMOFO = 10
    #: 注音风格，仅首字母。如：中国 -> ``ㄓ ㄍ``
    BOPOMOFO_FIRST = 11
    #: 汉语拼音与俄语字母对照风格，声调在各个拼音之后，用数字 [1-4] 进行表示。如：中国 -> ``чжун1 го2``
    CYRILLIC = 12
    #: 汉语拼音与俄语字母对照风格，仅首字母。如：中国 -> ``ч г``
    CYRILLIC_FIRST = 13
```
<a name="aI1Cf"></a>
### 处理特殊字符
默认情况下，对于文字中的特殊字符会不做任何处理，原样返回:
```python
pinyin('你好☆☆')
# [['nǐ'], ['hǎo'], ['☆☆']]
```
不过如果想对这些特殊字符做处理也是可以的，比如：<br />ignore  : 忽略该字符
```python
pinyin('你好☆☆', errors='ignore')
# [['nǐ'], ['hǎo']]
```
errors : 替换为去掉  \u  的 unicode 编码:
```python
pinyin('你好☆☆', errors='replace')
# [['nǐ'], ['hǎo'], ['26062606']]
```
callable 对象  : 提供一个回调函数，接受无拼音字符(串)作为参数, 支持的返回值类型:  `unicode`或 `list`或 `None`:
```python
pinyin('你好☆☆', errors=lambda x: 'star')
# [['nǐ'], ['hǎo'], ['star']]

pinyin('你好☆☆', errors=lambda x: None)
# [['nǐ'], ['hǎo']]
```
返回值类型为 list时，会自动 expend list:
```python
pinyin('你好☆☆', errors=lambda x: ['star' for _ in x])
# [['nǐ'], ['hǎo'], ['star'], ['star']]

# 指定多音字
pinyin('你好☆☆', heteronym=True, errors=lambda x: [['star', '☆'] for _ in x])
# [['nǐ'], ['hǎo'], ['star', '☆'], ['star', '☆']]
```
<a name="z0qe2"></a>
### 自定义拼音库
如果觉得模块输出效果不正确，或者想做特殊处理，可以通过  `load_single_dict()` 或  `load_phrases_dict()`  以自定义拼音库的方式修正结果：
```python
from pypinyin import lazy_pinyin, load_phrases_dict, Style, load_single_dict
hans = '桔子'
lazy_pinyin(hans, style=Style.TONE2)
# ['jie2', 'zi3']
load_phrases_dict({'桔子': [['jú'], ['zǐ']]}) # 增加 "桔子" 词组
lazy_pinyin(hans, style=Style.TONE2)
# ['ju2', 'zi3']

hans = '还没'
lazy_pinyin(hans, style=Style.TONE2)
# ['hua2n', 'me2i']
load_single_dict({ord('还'): 'hái,huán'}) # 调整 "还" 字的拼音顺序
lazy_pinyin('还没', style=Style.TONE2)
# ['ha2i', 'me2i']
```
