介绍一下 LangChain 的第一个很重要的组件模型 Model。<br />注意这里说的模型是指 LangChain 的模型组件，而不是指类似于 OpenAI 的语言模型，之所以 LangChain 会出现模型组件就是因为业内的语言模型太多了，除了 OpenAI 这家公司的语言模型之外，还很有很多其他家的。<br />LangChain 的模型组件有三种类型，分别是 LLM 大语言模型，Chat Model 聊天模型和文本嵌入模型 Text Embedding Models。
<a name="VJ8ED"></a>
## LLM 大语言模型
LLM 作为最基本的一种模型组件，输入和输出都只支持字符串，在大部分的场景下就可以满足需求了。可以在 Colab([[https://colab.research.google.com](https://colab.research.google.com)) 上面直接写 Python 代码
<a name="zx7jV"></a>
### 举个栗子
下面是一个 case，先安装依赖，再执行下面的代码。
```bash
pip install openai
pip install langchain
```
```python
import os
# 配置OpenAI 的 API KEY
os.environ["OPENAI_API_KEY"] ="sk-xxx"
# 从 LangChain 中导入 OpenAI 的模型
from langchain.llms import OpenAI
# 三个参数分别代表OpenAI 的模型名称，执行的次数和随机性，数值越大越发散
llm = OpenAI(model_name="text-davinci-003", n=2, temperature=0.3)
llm.generate(["给我讲一个故事", "给我讲一个笑话"])
```
运行的结果如下![](https://cdn.nlark.com/yuque/0/2023/png/396745/1685668849770-35953720-3cb9-4161-a5d7-820216ea70c3.png#averageHue=%23e9e9e9&clientId=u15d2b0af-0cea-4&from=paste&id=ueb194396&originHeight=242&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u160f8a3f-ecee-4ab6-bbe8-d3f706d5e7b&title=)
<a name="f67Ac"></a>
## Chat Model 聊天模型
Chat Model 是基于 LLM 模型的，只不过 Chat Model 相对于 LLM 模型组件间输入和输出更加结构化，输入和输出的参数的类型都是 Chat Model，而不是简单的字符串。常用的 Chat Model 类型有如下几种

1. AIMessage：用来保存 LLM 的响应，以便在下次请求时把这些信息传回给 LLM。
2. HumanMessage：发送给 LLMs 的提示信息，比如“实现一个快速排序方法”
3. SystemMessage：设置 LLM 模型的行为方式和目标。你可以在这里给出具体的指示，比如“作为一个代码专家”，或者“返回 json 格式”。
4. ChatMessage：ChatMessage 可以接收任意形式的值，但是在大多数时间，应该使用上面的三种类型。
<a name="GJmdc"></a>
### 举个栗子
```python
from langchain.chat_models import ChatOpenAI
from langchain.schema import (
    AIMessage,
    HumanMessage,
    SystemMessage
)
chat = ChatOpenAI(temperature=0)

messages = [
        SystemMessage(content="返回的数据markdown 语法进行展示，代码使用代码块包裹"),
        HumanMessage(content="用 Java 实现一个二分查找算法")
]

print(chat(messages))
```
生成的内容字符串形式如下
> content='二分查找算法，也称折半查找算法，是一种在有序数组中查找特定元素的搜索算法。该算法的时间复杂度为 O(log n)。\n\n以下是 Java 实现二分查找算法的代码：\n\njava\npublic class BinarySearch {\n public static int binarySearch(int[] arr, int target) {\n int left = 0;\n int right = arr.length - 1;\n while (left <= right) {\n int mid = (left + right) / 2;\n if (arr[mid] == target) {\n return mid;\n } else if (arr[mid] < target) {\n left = mid + 1;\n } else {\n right = mid - 1;\n }\n }\n return -1;\n }\n\n public static void main(String[] args) {\n int[] arr = {1, 3, 5, 7, 9};\n int target = 5;\n int index = binarySearch(arr, target);\n if (index != -1) {\n System.out.println("目标元素 " + target + " 在数组中的下标为 " + index);\n } else {\n System.out.println("目标元素 " + target + " 不在数组中");\n }\n }\n}\n\n\n在上述代码中，binarySearch 方法接收一个有序数组和一个目标元素，返回目标元素在数组中的下标，如果目标元素不在数组中，则返回 -1。\n\n在 binarySearch 方法中，使用两个指针 left 和 right 分别指向数组的左右两端，然后在一个 while 循环中不断缩小查找范围，直到找到目标元素或者查找范围为空。在每次循环中，计算中间位置 mid，然后将目标元素与中间位置的元素进行比较，如果相等，则返回中间位置的下标；如果目标元素比中间位置的元素大，则将左指针移到中间位置的右边；如果目标元素比中间位置的元素小，则将右指针移到中间位置的左边。' additional_kwargs={} example=False

将 content 里面的内容提取出来，用 markdown 语法展示出来是这样的![](https://cdn.nlark.com/yuque/0/2023/png/396745/1685668849748-dbb917f5-679c-4c13-8c83-aa09d3f4abe5.png#averageHue=%238a8a8e&clientId=u15d2b0af-0cea-4&from=paste&id=u549ad496&originHeight=826&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6932a064-5c69-4484-939a-b3f9b80d114&title=)使用这个模型组件，可以进行一些角色的预设，然后来定制个性化的问答。
<a name="CgsMc"></a>
### Prompts template
```python
from langchain.chat_models import ChatOpenAI
from langchain.prompts import (
    ChatPromptTemplate,
    PromptTemplate,
    SystemMessagePromptTemplate,
    AIMessagePromptTemplate,
    HumanMessagePromptTemplate,
)
from langchain.schema import (
    AIMessage,
    HumanMessage,
    SystemMessage
)
system_template="你是一个把{input_language}翻译成{output_language}的助手"
system_message_prompt = SystemMessagePromptTemplate.from_template(system_template)
human_template="{text}"
human_message_prompt = HumanMessagePromptTemplate.from_template(human_template)

chat_prompt = ChatPromptTemplate.from_messages([system_message_prompt, human_message_prompt])
messages = chat_prompt.format_prompt(input_language="英语", output_language="汉语", text="I love programming.")

print(messages)

chat = ChatOpenAI(temperature=0)

print(chat(messages.to_messages()))
```
output
> messages=[SystemMessage(content='你是一个把英语翻译成汉语的助手', additional_kwargs={}), HumanMessage(content='I love programming.', additional_kwargs={}, example=False)] content='我喜欢编程。' additional_kwargs={} example=False

<a name="SIX4F"></a>
## 文本嵌入模型 Text Embedding Models
文本嵌入模型组件相对比较难理解，这个组件接收的是一个字符串，返回的是一个浮点数的列表。在 NLP 领域中 Embedding 是一个很常用的技术，Embedding 是将高维特征压缩成低维特征的一种方法，常用于自然语言处理任务中，如文本分类、机器翻译、推荐系统等。它将单词、短语、句子等文本中的离散数据映射到实数域上的向量，这些向量可以被神经网络更好地处理和学习。通过 Embedding，文本数据可以被更好地表示和理解，提高了模型的表现力和泛化能力。
<a name="yjo6R"></a>
### 举个栗子
```python
from langchain.embeddings import OpenAIEmbeddings
embeddings = OpenAIEmbeddings()
text = "hello world"

query_result = embeddings.embed_query(text)
doc_result = embeddings.embed_documents([text])

print(query_result)
print(doc_result)
```
output
> [-0.01491016335785389, 0.0013780705630779266, -0.018519161269068718, -0.031111136078834534, -0.02430146001279354, 0.007488010451197624,0.011340680532157421, 此处省略 .......

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1685668849871-05cab478-1255-470e-9c8f-ad36766a35b0.png#averageHue=%23f8f8f8&clientId=u15d2b0af-0cea-4&from=paste&id=u9c7d4616&originHeight=652&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ued6748d2-bead-4b32-a9f4-d1e0c1ec5cf&title=)
<a name="uxuBb"></a>
## 总结
介绍了一下 LangChain 的模型组件，有了模型组件就可以更加方便的跟各种 LLMs 进行交互了。
<a name="pX6qF"></a>
## 参考资料
官方文档：[https://python.langchain.com/en/latest/modules/models.html](https://python.langchain.com/en/latest/modules/models.html)
