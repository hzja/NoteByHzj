<a name="nYhOG"></a>
## 链 Chain
所谓链顾名思义就是通过将一系列的组件联系起来形成一个链，在 LangChain 这块就是通过 Chain 将提示模板，模型或者其他链的结果链接在一起。<br />举个例子<br />下面这个 Chain 是将提示模板和模型链接在一起
```python
from langchain import PromptTemplate
from langchain.llms import OpenAI
from langchain.chains import LLMChain

template = "我的邻居姓{lastname}，他生了个儿子，给他儿子起个名字"

prompt = PromptTemplate(
    input_variables=["lastname"],
    template=template,
)
llm = OpenAI(temperature=0.9)

# 通过链将提示模板和 LLM 链接在一起
chain = LLMChain(llm = llm, 
                 prompt = prompt)
# 执行链
print(chain.run("朱"))
```
也可以将多个 Chain 链接在一起，如下所示
```python
from langchain import PromptTemplate
from langchain.llms import OpenAI
from langchain.chains import LLMChain, SimpleSequentialChain

template = "我的邻居姓{lastname}，他生了个儿子，给他儿子起个名字"

prompt = PromptTemplate(
    input_variables=["lastname"],
    template=template,
)
llm = OpenAI(temperature=0.9)

# 通过链将提示模板和 LLM 链接在一起
chain = LLMChain(llm = llm, 
                 prompt = prompt)
# 创建第二条链
second_prompt = PromptTemplate(
    input_variables=["child_name"],
    template="邻居的儿子名字叫{child_name}，给他起一个小名",
)

# 通过链将提示模板和 LLM 链接在一起
chain_two = LLMChain(llm=llm, prompt=second_prompt)

# 链接两条链 
overall_chain = SimpleSequentialChain(chains=[chain, chain_two], verbose=True)

# 执行链，只需要传入第一个参数
catchphrase = overall_chain.run("朱")
```
上一个链的执行结果可以当成参数传递给下一个链，上面的 case 执行结果如下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686877124019-f487546c-7850-4396-885d-564d08b57efc.png#averageHue=%23f3f3f3&clientId=u36683ab8-c2a4-4&from=paste&id=u8b5f364a&originHeight=795&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u10c4216e-061f-430f-bc16-6aee31d0307&title=)
<a name="CeUwv"></a>
## 代理
尽管大语言模型非常强大，它们也有一些局限性，它们不能回答实时信息，它们没有上下文的概念，导致无法保存状态，它们处理数学逻辑问题仍然非常初级，只能借助于第三方的工具来完成这些需求，比如使用搜索引擎或者数据库，LangChain 中代理的作用就是根据用户需求，来去访问这些工具。<br />ChatGPT 本身也是这样，不会保存状态，也没有联网功能，所以前段时间 OpenAI 开放 ChatGPT 插件的时候大家都说是 App Store 时刻到来。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686877123941-3af0a26b-a5b2-4aaa-b9ab-a8c2ab9071cc.png#averageHue=%23fbfbfb&clientId=u36683ab8-c2a4-4&from=paste&id=u24d789c5&originHeight=483&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u652e000b-e7de-4897-bc07-ce6fa3e808e&title=)<br />举个栗子<br />假设明天要去杭州出差，咨询一下 ChatGPT 明天去出差应该穿什么样的衣服？要知道明天穿什么衣服，需要知道明天杭州的天气怎么样，但是对于 ChatGPT 来说，它并不知道明天的天气如何，所以就需要借助第三方工具来实现了。
```python
from langchain.agents import load_tools
from langchain.agents import initialize_agent
from langchain.agents import AgentType
from langchain.llms import OpenAI


llm = OpenAI(temperature=0)
tools = load_tools(["serpapi"], llm=llm)
agent = initialize_agent(tools, llm, agent=AgentType.ZERO_SHOT_REACT_DESCRIPTION, verbose=True)

agent.run("请用中文告诉我，明天去杭州出差适合穿什么衣服?")
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686877124014-ec9545f2-5676-40d7-9575-7edc2818125d.png#averageHue=%23f7f7f7&clientId=u36683ab8-c2a4-4&from=paste&id=uf1324063&originHeight=516&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf1bdd96a-c5d5-4ebb-b639-24a5eca1ea3&title=)<br />可以看到 ChatGPT 成功的回答了我们，0616 去杭州出差要穿雨衣，因为这边写文章的时间是 0615，所以这里的明天就是 0616。在杭州的小伙伴们，可以在评论区留言，看看16 号是否是有雨。<br />这里在查询天气的时候，使用的工具是 serpapi，要使用这个工具需要先去官网注册一个账号，然后获取到一个 SERPAPI_API_KEY 就可以使用了。<br />LangChain 现在支持很多工具，如下所示，感兴趣的小伙伴也可以去试试。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1686877124045-81314f3a-f7d5-4887-973e-4efe8ccc0481.png#averageHue=%23fcfcfc&clientId=u36683ab8-c2a4-4&from=paste&id=u550e87cd&originHeight=718&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufc970b20-ab2e-4418-be30-adc720ae0d2&title=)
<a name="F6YZF"></a>
## 总结
介绍了 LangChain 的链和代理组件，通过链可以将各个组件串联在一起，方便任务的执行；通过代理可以实现大模型不具备的功能，比如联网搜索。
