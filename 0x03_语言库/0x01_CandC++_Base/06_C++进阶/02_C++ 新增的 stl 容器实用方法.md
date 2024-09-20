# C++

### 1、原位构造与容器的emplace系列函数
在介绍**emplace**和**emplace_back**方法之前，先看一段代码：
```cpp
#include <iostream>
#include <list>

class Test
{
public:
    Test(int a, int b, int c)
    {
        ma = a;
        mb = b;
        mc = c;
        std::cout << "Test constructed." << std::endl;
    }

    ~Test()
    {
        std::cout << "Test destructed." << std::endl;
    }

    Test(const Test& rhs)
    {
        if (this == &rhs)
            return;

        this->ma = rhs.ma;
        this->mb = rhs.mb;
        this->mc = rhs.mc;

        std::cout << "Test copy-constructed." << std::endl;
    }

private:
    int ma;
    int mb;
    int mc;
};


int main()
{
    std::list<Test> collections;
    for (int i = 0; i < 10; ++i)
    {
        Test t(1 * i, 2 * i, 3 * i);
        collections.push_back(t);
    }
     
    return 0;
}
```
上述代码在一个循环里面产生一个对象，然后将这个对象放入集合当中，这样的代码在实际开发中太常见了。但是这样的代码存在严重的效率问题。循环中的t对象在每次循环时，都分别调用一次构造函数、拷贝构造函数和析构函数。这个过程示意如下：

循环10次，总共调用三十次。但实际上初衷是创建一个对象t，将其直接放入集合中，而不是将t作为一个中间临时产生的对象，这样的话，总共只需要调用t的构造函数10次就可以了。C++11提供了一个在这种情形下替代push_back的方法——**emplace_back**，使用**emplace_back**，将**main**函数中的代码改写一下：

```cpp
std::list<Test> collections;
for (int i = 0; i < 10; ++i)
{       
    collections.emplace_back(1 * i, 2 * i, 3 * i);
}
```
实际执行的时候，可以发现现在，只需要调用Test类的构造函数10次，大大地提高了执行效率。

同理，在这种情形下，对于像`**std::list**`、`**std::vector**`这样的容器，其push/push_front方法在C++11中也有对应的改进方法即emplace/emplace_front方法。C++ Reference上将这里的**emplace**操作称之为“**原位构造元素**”（**EmplaceConstructible**）是非常贴切的。

| 原方法 | C++ 11 改进方法 | 方法含义 |
| --- | --- | --- |
| push/insert | emplace | 在容器指定位置原位构造元素 |
| push_front | emplace_front | 在容器首部原位构造元素 |
| push_back | emplace_back | 在容器尾部原位构造元素 |

除了使用emplace系列函数原位构造元素，也可以为Test类添加移动构造函数（Move Constructor）来复用产生的临时对象t以提高效率。
### 2、`std::map`的`try_emplace`与`insert_or_assign`方法
由于`std::map`中的元素的key是唯一的，所以在实际开发中经常会遇到这样一类需求：即往某个map中插入元素时需要先检测map中指定的key是否存在，如果不存在才做插入操作，如果存在，则直接取来使用；或者在指定key不存在时做插入操作，存在时做更新操作。

以PC版QQ为例，好友列表中每个好友都对应一个userid，当双击某个QQ好友头像时，如果与该好友的聊天对话框（这里使用ChatDialog 表示）已经创建，则直接激活显示，如果不存在，则创建并显示之。假设使用`std::map`来管理这些聊天对话框，在C++17之前，必须编写额外的逻辑去判断元素是否存在，上述逻辑可以编写成如下代码：

```cpp
class ChatDialog
{
//其他实现省略...
public:
    void activate()
    {
        //实现省略
    }
};

//用于管理所有聊天对话框的map，key是好友id，ChatDialog是聊天对话框指针
std::map<int64_t, ChatDialog*> m_ChatDialogs;

//双击好友头像后
void onDoubleClickFriendItem(int64_t userid)
{
    auto targetChatDialog = m_ChatDialogs.find(userid);
    //好友对话框不存在，则创建之，并激活
    if (targetChatDialog == m_ChatDialogs.end())
    {
        ChatDialog* pChatDialog = new ChatDialog();
        m_ChatDialogs.insert(std::pair<int64_t, ChatDialog*>(userid, pChatDialog));
        pChatDialog->activate();
    }
    //好友对话框存在，直接激活
    else
    {
        targetChatDialog->second->activate();
    }
}
```
在C++17中map提供了一个**try_emplace**这样的方法，该方法会检测指定的key是否存在，如果存在，则什么也不做。函数签名如下：
```cpp
template <class... Args>
pair<iterator, bool> try_emplace(const key_type& k, Args&&... args);

template <class... Args>
pair<iterator, bool> try_emplace(key_type&& k, Args&&... args);

template <class... Args>
iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args);

template <class... Args>
iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args);
```
上述函数签名中， 参数**k**表示需要插入的key，args参数是一个不定参数，表示构造value对象需要传给构造函数的参数，**hint**参数可以指定插入位置。

在前两种签名形式中， **try_emplace**的返回值是一个`**std::pair<T1, T2>**`**类型，其中T2**是一个bool类型表示元素是否成功插入map中，**T1**是一个map的迭代器，如果插入成功，则返回指向插入位置的元素的迭代器，如果插入失败，则返回map中已存在的相同key元素的迭代器。用**try_emplace**改写上面的代码（这里不关心插入位置，因此使用前两个签名）：

```cpp
#include <iostream>
#include <map>

class ChatDialog
{
//其他实现省略...
public:
    void activate()
    {
        //实现省略
    }
};

//用于管理所有聊天对话框的map，key是好友id，ChatDialog是聊天对话框指针
std::map<int64_t, ChatDialog*> m_ChatDialogs;

//普通版本
void onDoubleClickFriendItem(int64_t userid)
{
    auto targetChatDialog = m_ChatDialogs.find(userid);
    //好友对话框不存在，则创建之，并激活
    if (targetChatDialog == m_ChatDialogs.end())
    {
        ChatDialog* pChatDialog = new ChatDialog();
        m_ChatDialogs.insert(std::pair<int64_t, ChatDialog*>(userid, pChatDialog));
        pChatDialog->activate();
    }
    //好友对话框存在，直接激活
    else
    {
        targetChatDialog->second->activate();
    }
}

//C++17版本1
void onDoubleClickFriendItem2(int64_t userid)
{   
    //结构化绑定和try_emplace都是 C++17语法
    auto [iter, inserted] = m_ChatDialogs.try_emplace(userid);
    if (inserted)
        iter->second = new ChatDialog();   

    iter->second->activate();
}

int main()
{
    //测试用例
    //906106643 是userid
    onDoubleClickFriendItem2(906106643L);
    //906106644 是userid
    onDoubleClickFriendItem2(906106644L);
    //906106643 是userid
    onDoubleClickFriendItem2(906106643L);

    return 0;
}
```
使用了**try_emplace**改写后的代码简洁了许多。但是上述代码存在一个注意事项，由于`std::map<int64_t, ChatDialog*> m_ChatDialogs`的value是指针类型（`ChatDialog*`），而**try_emplace**第二个参数支持的是构造一个ChatDialog对象，而不是指针类型，因此，当某个userid不存在时，成功插入map后会导致相应的value为空指针。因此，利用`**inserted**`的值按需new出一个ChatDialog。当然，新的C++语言规范（C++11及后续版本）提供了灵活而强大的智能指针以后，就不应该再有任何理由去使用裸指针了，因此上述代码可以使用`std::unique_ptr`智能指针类型来重构：
```cpp
/** 
 * std::map::try_emplace用法演示
 */

#include <iostream>
#include <map>
#include <memory>

class ChatDialog
{
//其他实现省略...
public:
    ChatDialog()
    {
        std::cout << "ChatDialog constructor" << std::endl;
    }

    ~ChatDialog()
    {
        std::cout << "ChatDialog destructor" << std::endl;
    }

    void activate()
    {
        //实现省略
    }
};

//用于管理所有聊天对话框的map，key是好友id，value是ChatDialog是聊天对话框智能指针
std::map<int64_t, std::unique_ptr<ChatDialog>> m_ChatDialogs;

//C++ 17 版本2
void onDoubleClickFriendItem3(int64_t userid)
{   
    //结构化绑定和try_emplace都是 C++17语法
    auto spChatDialog = std::make_unique<ChatDialog>();
    auto [iter, inserted] = m_ChatDialogs.try_emplace(userid, std::move(spChatDialog));
    iter->second->activate();
}

int main()
{
    //测试用例
    //906106643 是userid
    onDoubleClickFriendItem3(906106643L);
    //906106644 是userid
    onDoubleClickFriendItem3(906106644L);
    //906106643 是userid
    onDoubleClickFriendItem3(906106643L);

    return 0;
}
```
上述代码将map的类型从`**std::map<int64_t, ChatDialog*>**` 改为`**std::map<int64_t, std::unique_ptr<ChatDialog>>**` ，让程序自动管理聊天对话框对象。程序在gcc/g++ 7.3下编译并运行输出如下：
```bash
[root@mydev test]# g++ -g -o test_map_try_emplace_with_smartpointer test_map_try_emplace_with_smartpointer.cpp -std=c++17
[root@mydev test]# ./test_map_try_emplace_with_smartpointer
ChatDialog constructor
ChatDialog constructor
ChatDialog constructor
ChatDialog destructor
ChatDialog destructor
ChatDialog destructor
```
上述代码中构造函数和析构函数均被调用了3次，实际上，按最原始的逻辑（上文中普通版本）ChatDialog应该只被构造和析构2次，多出来的一次是因为在**try_emplace**时，无论某个userid是否存在于map中均创建一个ChatDialog对象（这个是额外的、用不上的对象），由于这个对象并没有被用上，当出了函数 **onDoubleClickFriendItem3** 作用域后，智能指针对象**spChatDialog**被析构，进而导致这个额外的、用不上的ChatDialog对象被析构。这相当于做了一次无用功。为此，可以继续优化代码如下：
```cpp
#include <iostream>
#include <map>
#include <memory>

class ChatDialog
{
//其他实现省略...
public:
    ChatDialog()
    {
        std::cout << "ChatDialog constructor" << std::endl;
    }

    ~ChatDialog()
    {
        std::cout << "ChatDialog destructor" << std::endl;
    }

    void activate()
    {
        //实现省略
    }
};

//用于管理所有聊天对话框的map，key是好友id，value是ChatDialog是聊天对话框智能指针
std::map<int64_t, std::unique_ptr<ChatDialog>> m_ChatDialogs;

//C++ 17版本3
void onDoubleClickFriendItem3(int64_t userid)
{   
    //结构化绑定和try_emplace都是 C++17语法    
    auto [iter, inserted] = m_ChatDialogs.try_emplace(userid, nullptr);
    if (inserted)
    {
        //这样就按需创建了
        auto spChatDialog = std::make_unique<ChatDialog>();
        iter->second = std::move(spChatDialog);
    }

    iter->second->activate();
}

int main()
{
    //测试用例
    //906106643 是userid
    onDoubleClickFriendItem3(906106643L);
    //906106644 是userid
    onDoubleClickFriendItem3(906106644L);
    //906106643 是userid
    onDoubleClickFriendItem3(906106643L);

    return 0;
}
```
上述代码按照之前的裸指针版本的思路，按需创建一个智能指针对象。这样就避免了一次ChatDialog对象无用的构造和析构。再次编译程序，执行结果如下：
```
[root@mydev test]# g++ -g -o test_map_try_emplace_with_smartpointer2 test_map_try_emplace_with_smartpointer2.cpp -std=c++17
[root@mydev test]# ./test_map_try_emplace_with_smartpointer2
ChatDialog constructor
ChatDialog constructor
ChatDialog destructor
ChatDialog destructor
```
`auto [iter, inserted] = m_ChatDialogs.try_emplace(userid, nullptr);`这种函数返回值有一个布尔变量表示操作是否成功，如果成功，另外一个返回值含有函数调用成功后的数据的模式，称为ok-idiom模式，golang语言的开发者应该非常熟悉ok-idiom模式。

为了演示**try_emplace**函数支持原位构造（上文已经介绍），将map的value类型改成ChatDialog类型，当然，这里只是为了演示方便，实际开发中对于非POD类型的复杂数据类型，在stl容器中应该存储其指针或者智能指针类型，而不是对象本身。修改后的代码如下：

```cpp
#include <iostream>
#include <map>

class ChatDialog
{
//其他实现省略...
public:
    ChatDialog(int64_t userid) : m_userid(userid)
    {
        std::cout << "ChatDialog constructor" << std::endl;
    }

    ~ChatDialog()
    {
        std::cout << "ChatDialog destructor" << std::endl;
    }

    void activate()
    {
        //实现省略
    }

private:
    int64_t     m_userid;
};

//用于管理所有聊天对话框的map，key是好友id，value是ChatDialog是聊天对话框对象
std::map<int64_t, ChatDialog>   m_ChatDialogs;

//C++ 17版本4
void onDoubleClickFriendItem3(int64_t userid)
{   
    //第二个userid是传给ChatDialog构造函数的参数
    auto [iter, inserted] = m_ChatDialogs.try_emplace(userid, userid);   
    iter->second.activate();
}

int main()
{
    //测试用例
    //906106643 是userid
    onDoubleClickFriendItem3(906106643L);
    //906106644 是userid
    onDoubleClickFriendItem3(906106644L);
    //906106643 是userid
    onDoubleClickFriendItem3(906106643L);

    return 0;
}
```
上述代码中，为ChatDialog类的构造函数增加了一个userid参数，因此当调用**try_emplace**方法时，需要传递一个参数，这样**try_emplace**就会根据map中是否已存在同样的userid按需构造ChatDialog对象。程序执行结果和上一个代码示例应该是一样的：
```bash
[root@mydev test]# g++ -g -o test_map_try_emplace_with_directobject test_map_try_emplace_with_directobject.cpp -std=c++17
[root@mydev test]# ./test_map_try_emplace_with_directobject
ChatDialog constructor
ChatDialog constructor
ChatDialog destructor
ChatDialog destructor
```
上面介绍了如果map中指定的key不存在则插入，存在则使用的情形。再来介绍一下如果map中指定的key不存在则插入，存在则更新其value值的情形。C++17为此也为map容器新增了一个这样的方法**insert_or_assign**，不再像C++17标准之前，需要额外编写先判断是否存在，不存在则插入，存在则更新的代码了，这次可以直接一步到位。**insert_or_assign**的函数签名如下：
```cpp
template <class M>
pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);

template <class M>
pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);

template <class M>
iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);

template <class M>
iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);
```
其各个函数参数的含义与**try_emplace**一样，这里就不再赘述。

来看一个例子：

```cpp
int main()
{
    std::map<std::string, int> mapUsersAge{ { "Alex", 45 }, { "John", 25 } };
    mapUsersAge.insert_or_assign("Tom", 26);
    mapUsersAge.insert_or_assign("Alex", 27);

    for (const auto& [userName, userAge] : mapUsersAge)
    {
        std::cout << "userName: " << userName << ", userAge: " << userAge << std::endl;
    }
}
```
上述代码中，尝试插入名为**Tom**的用户，由于该人名在map中不存在，因此插入成功；当插入人名为**Alex**的用户时，由于map中已经存在该人名了，因此只对其年龄进行更新，**Alex**的年龄从45更新为27。程序执行结果如下：
```bash
[root@mydev test]# g++ -g -o test_map_insert_or_assign test_map_insert_or_assign.cpp -std=c++17
[root@mydev test]# ./test_map_insert_or_assign
userName: Alex, userAge: 27
userName: John, userAge: 25
userName: Tom, userAge: 26
```
这里介绍了 C++11/17 为 stl 容器新增的几个实用方法，合理利用它们会让的程序变得更简洁、更高效。其实新的标准一致在不断改进和优化已有 stl 各个容器，如果工作需要经常与这些容器打交道，建议平常留意C++新标准涉及到它们的新动态。
