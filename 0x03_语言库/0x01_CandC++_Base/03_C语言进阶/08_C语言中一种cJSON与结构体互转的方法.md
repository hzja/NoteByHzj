# C

## 前言
json是目前最为流行的文本数据传输格式，特别是在网络通信上广泛应用，随着物联网的兴起，在嵌入式设备上，也需要开始使用json进行数据传输，那么，如何快速简洁地用C语言进行json的序列化和反序列化呢？

当前，应用最广泛的C语言json解析库当属cJSON，但是，使用cJSON读json进行序列化和反序列化，需要根据key一个一个进行处理，会导致代码冗余，逻辑性不强，哪有没有更好的方法呢？


## 思路
在Android平台，一般会使用gson等工具解析json，这些工具将json直接映射成对象，在C语言上使用对象的概念，需要借助结构体，然而，最大的问题在于，C语言没有高级语言具有的反射机制，直接从json映射到结构体对象几乎是不可能的。<br />怎么解决呢，既然C语言没有反射机制，那么可以自己定义一套类似于反射的机制，这里将其称之为结构体数据模型，在数据模型中，需要准确地描述结构体的特征，包括结构体各成员的名称，类型，在结构体中的偏移。<br />有了这些，可以在解析josn的时候，将解析得到的数据直接写入到对应的内存里面去，或者是在序列化的时候，直接从对应的内存中读取数据，进行处理。
<a name="dUD1A"></a>
## 实现
CSON正是采用上面说到的思路，使用数据模型对结构体进行描述，然后基于cJSON，根据数据模型进行解析，将解析得到的数据直接写入到对应的内存区域，从而实现从json到结构体对象的映射。<br />CSON最基本的数据模型定义如下：
```c
typedef struct cson_model
{
    CsonType type;                      /**< 数据类型 */
    char *key;                          /**< 元素键值 */
    short offset;                       /**< 元素偏移 */
} CsonModel;
```
通过type描述结构体成员的数据类型，key描述该成员在json中对应的字段，offset描述该结构体成员在结构体中的偏移，CSON在解析json的时候，根据type调用相应的cJSON API并传递key作为参数，得到解析出的数据，然后根据offset将数据写入到对应的内存空间。<br />比如说这样一个结构体：
```c
struct project
{
    int id;
    char *name;
}
```
该结构体包含两个成员，对于成员id，使用数据模型对其进行描述
```c
{.type=CSON_TYPE_CHAR, key="id", offset=0}
```
对于结构体的每个成员，都进行数据模型的定义，就可以得到一个完整的结构体数据模型，CSON会根据这个模型，进行解析。<br />因为是通过直接写内存的方式，所以在写不同类型的量到内存中时，会多次用到强制转型，导致CSON中赋值的代码都类似于：
```c
*(int *)((int)obj + model[i].offset) = (int)csonDecodeNumber(json, model[i].key);
```
当然，上面说到的数据模型，只适用于基本数据类型的数据，对于子结构体，链表，数组等，需要对数据模型的定义进行扩充，有兴趣的朋友可以直接阅读CSON源码。
<a name="WCfXH"></a>
## CSON使用实例
<a name="YkLKv"></a>
### 声明结构体：
```c
/** 项目结构体 */
struct project
{
    int id;
    char *name;
};

/** 仓库结构体 */
struct hub
{
    int id;
    char *user;
    struct project *cson;
};
```
<a name="zwvZ5"></a>
### 定义数据模型：
对每一个需要使用cson的结构体，都需要定义相对应的数据模型
```c
/** 项目结构体数据模型 */
CsonModel projectModel[] =
{
    CSON_MODEL_OBJ(struct project),
    CSON_MODEL_INT(struct project, id),
    CSON_MODEL_STRING(struct project, name),
};

/** 仓库结构体数据模型 */
CsonModel hubModel[] =
{
    CSON_MODEL_OBJ(struct hub),
    CSON_MODEL_INT(struct hub, id),
    CSON_MODEL_STRING(struct hub, user),
    CSON_MODEL_STRUCT(struct hub, cson, projectModel, sizeof(projectModel)/sizeof(CsonModel))
};
```
<a name="W9pyy"></a>
### 使用CSON解析：
只需要定义好数据模型，就可以使用CSON读json进行序列化和反序列化
```c
void csonDemo(void)
{
    char *jsonDemo = "{\"id\": 1, \"user\": \"Letter\", \"cson\": {\"id\": 2, \"name\": \"cson\"}}";

    /** 解析json */
    struct hub *pHub = csonDecode(jsonDemo, hubModel, sizeof(hubModel)/sizeof(CsonModel));
    printf("hub: id: %d, user: %s, project id: %d, project name: %s\r\n",
           pHub->id, pHub->user, pHub->cson->id, pHub->cson->name);

    /** 序列化对象 */
    char *formatJson = csonEncodeFormatted(pHub, hubModel, sizeof(hubModel)/sizeof(CsonModel));
    printf("format json: %s\r\n", formatJson);

    /** 释放结构体对象 */
    csonFree(pHub, hubModel, sizeof(hubModel)/sizeof(CsonModel));

    /** 释放序列化生成的json字符串 */
    csonFreeJson(formatJson);
}
```
<a name="R3kqI"></a>
### 运行结果：
```
hub: id: 1, user: Letter, project id: 2, project name: cson
format json: {
        "id":   1,
        "user": "Letter",
        "cson": {
                "id":   2,
                "name": "cson"
        }
}
```
可以看到，无论是解析json，还是序列化结构体到json，在使用CSON的情况下，都只需要一行代码就可以解决，同样的操作，在使用原生cJSON的情况下，可能需要多次判断，解析元素。
<a name="rABuh"></a>
## CSON地址
[https://github.com/NevermindZZT/cson](https://github.com/NevermindZZT/cson)
