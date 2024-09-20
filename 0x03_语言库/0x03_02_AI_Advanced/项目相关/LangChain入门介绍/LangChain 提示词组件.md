<a name="JISSt"></a>
## 什么提示词
用过 ChatGPT 的小伙伴都知道，提供输入，然后 ChatGPT 会提供回答，这里提供的输入在 LLM 领域有一个专业名词就叫做提示词。<br />在使用 ChatGPT 的时候，有时候会设置很多默认的角色，并且常用的提示词有很多，比如

1. 我想让你担任 Android 开发工程师面试官。我将成为候选人，您将向我询问 Android 开发工程师职位的面试问题。我希望你只作为面试官回答。不要一次写出所有的问题。我希望你只对我进行采访。问我问题，等待我的回答。不要写解释。像面试官一样一个一个问我，等我回答。我的第一句话是“面试官你好”
2. 我想让你充当 Linux 终端。我将输入命令，您将回复终端应显示的内容。我希望您只在一个唯一的代码块内回复终端输出，而不是其他任何内容。不要写解释。除非我指示您这样做，否则不要键入命令。当我需要用英语告诉你一些事情时，我会把文字放在中括号内[就像这样]。我的第一个命令是 pwd

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686272911118-4195b394-7453-4675-9686-6bc087a88764.png#averageHue=%23eeeeee&clientId=u1a4f546e-8f0b-4&from=paste&id=u1ee6e7bc&originHeight=528&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uae46342b-d672-4b2c-9e9e-cee943c3fea&title=)<br />上面的这些内容算是一种稍微复杂的提示词，而且可以看到，这些常用的提示词结构其实都比较相似，因此为了更加的通用，可以使用一个模板来表达，比如：我希望你担任一个 {xxx}，我将会提供 {yyy}，你会按照 {zzz} 等等这种形式来组装。
<a name="klnPg"></a>
## 提示词模板
上面提到了，为了更加通用的使用提示词，LangChain 定义了一个提示词模板的组件，通过提示词模板组件可以创建提示词，方便开发者更好的使用提示词。
```python
from langchain import PromptTemplate


template = """
I want you to act as a naming consultant for new companies.
What is a good name for a company that makes {product}?
"""

prompt = PromptTemplate(
    input_variables=["product"],
    template=template,
)
prompt.format(product="colorful socks")
# -> I want you to act as a naming consultant for new companies.
# -> What is a good name for a company that makes colorful socks?
```
如上所示，先创建一个提示词模板字符串，其中用{product} 来替代需要替换的内容，接着通过构造 PromptTemplate 的时候传递 `input_variables["product"]` 参数，表示 product 是一个需要被替换的变量，最后通过 format 方法来设置真实要替换的内容。<br />此外可以构造无输入变量的提示词模板，也可以构造多输入参数的模板，如下所示
```python
from langchain import PromptTemplate

# An example prompt with no input variables
no_input_prompt = PromptTemplate(input_variables=[], template="Tell me a joke.")
no_input_prompt.format()
# -> "Tell me a joke."

# An example prompt with one input variable
one_input_prompt = PromptTemplate(input_variables=["adjective"], template="Tell me a {adjective} joke.")
one_input_prompt.format(adjective="funny")
# -> "Tell me a funny joke."

# An example prompt with multiple input variables
multiple_input_prompt = PromptTemplate(
    input_variables=["adjective", "content"], 
    template="Tell me a {adjective} joke about {content}."
)
multiple_input_prompt.format(adjective="funny", content="chickens")
# -> "Tell me a funny joke about chickens."
```
<a name="OaX71"></a>
## 序列化提示词模板
可以将制作好的提示词模板进行序列化，存放的 json 或者 yaml 文件中，然后在其他地方可以加载使用，LangChain 会根据存放文件的后缀名来写入对应格式的数据。
```python
prompt_template.save("awesome_prompt.json") # Save to JSON file

from langchain.prompts import load_prompt
loaded_prompt = load_prompt("awesome_prompt.json")

assert prompt_template == loaded_prompt
```
<a name="igcHk"></a>
## 零样本提示词和小样本提示词
说明提示词模板过后，再来了解一下什么是零样本提示词和小样本提示词。
<a name="I1j9H"></a>
### 零样本提示词
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686272911189-d15601bb-d5e5-40aa-b90d-d54d9873b349.png#averageHue=%23fcfcfc&clientId=u1a4f546e-8f0b-4&from=paste&id=u984d59d5&originHeight=653&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud6c73c9c-9142-40a2-a35a-1e575d93fac&title=)<br />如下图所示，通过直接询问 ChatGPT 一句话是积极、中性还是消极的情绪，其中并没有告诉 ChatGPT 什么样的情绪是积极的，什么样是中性以及什么样是消极的。<br />但是 ChatGPT 能够很好的回答出来，这是因为 ChatGPT 已经学习了大量网络上的内容，这种情况就是零样本提示。因为提问之前并没有给 ChatGPT 任务示例和样本。
<a name="NPO7M"></a>
### 小样本提示词
顾名思义小样本提示词表示在进行提问的时候会提供少量的小样本提示词给到 ChatGPT，让 ChatGPT 根据提供发小样本来学习进行回答。<br />之所以有小样本提示词是因为在随着跟 ChatGPT 沟通对话次数的增加的时候，有时候话题会逃逸，所以改变每次传递的上下文信息，而是只传递小样本指示词，可以让我们的话题始终保持初始状态。<br />另外一个是由于跟 ChatGPT 的对话每次都是收费的，会根据传递的上下文的长度来计费，通过这种方式也可以减少传递的内容，避免长对话内容被传递增加费用。<br />举个例子<br />User：从现在起你是一个充满哲学思维的心灵导师，当每次输入一个疑问时你需要用一句富有哲理的名言警句来回答我，并且表明作者和出处\n\n\n要求字数不少于15个字，不超过30字，每次只返回一句且不输出额外的其他信息，需要使用中文和英文双语输出\n\n\n当你准备好的时候只需要回复“我已经准备好了”（不需要输出任何其他内容）<br />我作业写不完怎么办<br />“行动是治愈恐惧的良药。” - 威廉·詹姆斯\n"Action is the antidote to fear." - William James<br />论文被拒了怎么办<br />“失败是成功之母。” - 俗语\n"Failure is the mother of success." - Chinese proverb<br />使用样例<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686272911154-b9dd8a02-2031-4c3f-a3e9-c3808a8120d4.png#averageHue=%23f8f8f7&clientId=u1a4f546e-8f0b-4&from=paste&id=u75428991&originHeight=1381&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucc4ed557-fa2f-4553-8acc-31ad6fdcf5a&title=)
<a name="oUErx"></a>
## 总结
给大家介绍了一下 LangChain 的提示词组件，有了提示词和提示词模板组件就可以更加方便的跟各种 LLMs 进行交互，并且可以更加灵活的构造属于自己的模板。
<a name="OyEit"></a>
## 参考资料
官方文档：[https://python.langchain.com/en/latest/modules/prompts.html](https://python.langchain.com/en/latest/modules/prompts.html)
