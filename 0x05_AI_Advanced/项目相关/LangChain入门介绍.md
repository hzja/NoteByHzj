大模型这么火，有没有想过如何基于大模型去创建自己的应用呢？作为技术人相信很多小伙伴们都有过这种想法，但是不知道该如何着手。<br />其实业内已经有很多成熟的框架来帮助开发人员进行大模型应用开发的了，对于开发人员来说不需要理解很深层次的理论就可以进行实操了。<br />最近就在学习大模型开发框架 LangChain，基于官方文档和一些网上的资料来进行个人实践学习，欢迎小伙伴们一起学习。
<a name="LsCav"></a>
## LangChain 是什么
首先 LangChain 是一个框架，这个框架是用来让开发者进行 LLMs （大语言模型）应用开发的。<br />可以理解是为各种 LLM 开发的脚手架，将 LLM 的各个组件进行封装和链接。把 LLMs 相关的组件“链接”在一起，简化 LLMs 应用的开发难度，方便开发者快速地开发复杂的 LLMs 应用。<br />举一个不是很恰当的栗子，从 Java 工程师的角度来看 LangChain 更像是 Spring 或者 SpringBoot 这种框架，帮助开发人员更快的进行应用开发。
<a name="Juom7"></a>
## LangChain 框架组件
一个 LangChain 应用是通过很多个组件实现的，LangChain 主要支持 6 种组件：

- Models：模型，各种类型的模型和模型集成，比如 GPT-4 等大语言模型，LangChain 将各家公司的大模型进行了抽象，封装了通用的 API，只要使用对应的 API 就可以完成对各个公司大模型的调用；
- Prompts：提示，包括 Prompts 管理、Prompts 优化和 Prompts 序列化，大语言模型中提示词是很重要的，不管是聊天机器人还是 AI 绘画，都少不了提示词；
- Memory：记忆，用来保存和模型交互时的上下文状态，模型本身是不具备上下文记忆的，所以当跟模型进行交互的时候，是需要传递聊天内容上下文的；
- Indexes：索引，用来结构化文档，以便和模型交互；
- Chains：链，一系列对各种组件的调用；
- Agents：代理，决定模型采取哪些行动，执行并且观察流程，直到完成为止；
<a name="igibi"></a>
### Models
LangChain 为 GPT4 这些第三方的模型提供通用接口，目前支持三种类型的模型

- LLMs：LLMs（大语言模型）接收文本字符作为输入，返回的也是文本字符
- 聊天模型
   - 聊天模型基于 LLMs，不同的是它接收聊天消息作为输入，返回的也是聊天消息
   - 聊天消息是一种特定格式的数据，LangChain 中支持四种消息 AIMessage，HumanMessage，SystemMessage ，ChatMessage，需要按照它们的角色把数据传递给模型，这部分在后面再详细解释。
- 文本嵌入模型 ：文本嵌入模型接收文本作为输入，返回的是浮点数列表
<a name="LPrxb"></a>
### Prompts
通常作为输入传递给模型的信息被称为 Prompts 提示，Prompts 可以是文本字符，也可以是文件、图片甚至是视频，LangChain 目前只支持字符形式的 Prompts。<br />Prompts 一般不是硬编码的形式写在代码里，而是由**模板**和**用户输入**来生成，LangChain 提供多个类和方法来构建 Prompts 。

- 提示模板
   - 提示模板是一种生成 Prompts 的方式，包含一个带有可替换内容的模板，从用户那获取一组参数并生成 Prompts；
   - 提示模板用来生成 LLMs 的 Prompts ，最简单的使用场景，比如“我希望你扮演一个代码专家的角色，告诉我这个方法的原理{code}。
- 聊天提示模板
   - 聊天模型接收聊天消息作为输入，再次强调**聊天消息和普通字符是不一样的**，聊天提示模板的作用就是为聊天模型生成提示；
- 示例选择器
   - 示例选择器是一个高级版的数据筛选器，举个例子，在实现一个提示模板时，有这么一组原始数据：
```python
examples = [
    {
        "input": "happy",
        "output": "sad"
    },
    {
        "input": "tall",
        "output": "short"
    },
    {
        "input": "energetic",
        "output": "lethargic"
    },
    {
        "input": "sunny",
        "output": "gloomy"
    },
    {
        "input": "windy",
        "output": "calm"
    }
];
```

- 可以实现一个示例选择器，也可以使用内置的产品需求是根据用户提供的参数来筛选数据，参数是 number 类型，
- 输出解析器
   - 由于模型返回的是文本字符，输出解析器可以把文本转换成结构化数据；
<a name="pVLxs"></a>
### Memory
模型是无状态的，不保存上一次交互时的数据，回想一下 OpenAI 的 API 服务，它是没有上下文概念的，而 ChatGPT 是额外实现了上下文功能。为了提供上下文的功能，LangChain 提供了 memory 组件，用来在对话过程中存储数据。
<a name="O9Krc"></a>
### Indexes
为了更好的和 LLMs 进行交互，需要将一些数据进行存储，在 LLM 领域常用的索引存储对应的是向量数据库 VectorStores。对应数据的存储通常会被称为索引，而对于数据的查询通常会被称为检索。在大部分情况下针对与 LLM 的场景都是一些非结构化的数据需要索引和查询，所以 LangChain 提供了下面的一些组件用于处理。

- 文档加载器 Document Loaders
   - 负责加载各个来源的文档，比如 cvs，音频，视频，document 等；
- 文本分割器 Text Splitters
   - 有时候需要把大的文档分割成小片段来和模型交互，比如 GPT3.5 只支持 4,096 token 的输入，文本分割器负责这个工作
- 检索器 Retrievers
   - 此接口公开了一个 `get_relevant_documents` 方法，该方法接受一个查询（一个字符串）并返回一个文档列表。
- 向量数据库 Vectorstores
   - 最常见的索引类型是为每个文档创建嵌入（使用嵌入模型），vectorstore 存储文档和关联的嵌入，并且提供查询相关文档的快速方式。
<a name="KaraH"></a>
### Chains
对于一些简单的应用 可以使用单个 LLM 就完成，但是对于很多复杂的应用就需要链接多个 LLM 来完成，LangChain 提供了 Chains 的标准接口，以及一些常见的 Chain 实现，方便使用。
<a name="tFWjT"></a>
### Agents
有些应用并不是一开始就确定调用哪些模型，而是依赖于用户输入，代理就提供了一套工具，根据用户的输入来决定调用这些工具种的哪一个。LangChain 提供了下面的组件：

- 工具：用来方便模型和其他资源交互
- 代理：围绕模型的包装器，接收用户输入，决定模型的行为
- 工具集：解决特定问题的工具集合
- 代理执行器：代理和一组工具，调用代理
<a name="P1izy"></a>
## 总结
LangChain 是一个让开发人员更方便进行 LLM 应用开发的一套框架，集成了很多家公司的 LLM。将这些 LLM 进行抽象，提供了很多组件和扩展能力，支持更复杂的 LLM 应用。<br />参考地址<br />LangChain 官文地址：[https://python.langchain.com/en/latest/index.html](https://python.langchain.com/en/latest/index.html)
