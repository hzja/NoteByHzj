# C++11 async

C++11中增加了async，如它的名字一样，这个关键字就是用来创建异步操作的，c++11中有个更常用的异步操作，叫做线程thread，那么thread和async有什么区别呢？以及async的优势是什么？应该怎么使用？


### C++11 使用 std::async创建异步程序
C++11中增加了线程，可以非常方便的创建线程，它的基本用法是这样的：
```cpp
void f(int n);
std::thread t(f, n + 1);
t.join();
```
但是线程毕竟是属于比较低层次的东西，有时候使用有些不便，比如希望获取线程函数的返回结果的时候，就不能直接通过 `thread.join()`得到结果，这时就必须定义一个变量，在线程函数中去给这个变量赋值，然后join，最后得到结果，这个过程是比较繁琐的。

c++11还提供了异步接口`std::async`，通过这个异步接口可以很方便的获取线程函数的执行结果。`std::async`会自动创建一个线程去调用线程函数，它返回一个`std::future`，这个future中存储了线程函数返回的结果，当需要线程函数的结果时，直接从future中获取，非常方便。

其实std::async提供的便利可不仅仅是这一点，它首先解耦了线程的创建和执行，可以在需要的时候获取异步操作的结果；其次它还提供了线程的创建策略（比如可以通过延迟加载的方式去创建线程），可以以多种方式去创建线程。在介绍`async`具体用法以及为什么要用`std::async`代替线程的创建之前，先看看`std::future`、`std::promise`和 `std::packaged_task`。


### std::future
std::future是一个非常有用也很有意思的东西，简单说std::future提供了一种访问异步操作结果的机制。从字面意思来理解， 它表示未来，因为一个异步操作是不可能马上就获取操作结果的，只能在未来某个时候获取，但是可以以同步等待的方式来获取结果，可以通过查询future的状态（future_status）来获取异步操作的结果。future_status有三种状态：

- deferred：异步操作还没开始
- ready：异步操作已经完成
- timeout：异步操作超时
```cpp
//查询future的状态
std::future_status status;
do {
    status = future.wait_for(std::chrono::seconds(1));
    if (status == std::future_status::deferred) {
        std::cout << "deferred\n";
    } else if (status == std::future_status::timeout) {
        std::cout << "timeout\n";
    } else if (status == std::future_status::ready) {
        std::cout << "ready!\n";
} while (status != std::future_status::ready);
```
获取future结果有三种方式：get、wait、wait_for，其中get等待异步操作结束并返回结果，`wait`只是等待异步操作完成，没有返回值，`wait_for`是超时等待返回结果。

### std::promise
std::promise为获取线程函数中的某个值提供便利，在线程函数中给外面传进来的promise赋值，当线程函数执行完成之后就可以通过promis获取该值了，值得注意的是取值是间接的通过promise内部提供的future来获取的。它的基本用法：
```cpp
std::promise<int> pr;
std::thread t([](std::promise<int>& p){
    p.set_value_at_thread_exit(9);
},std::ref(pr));
std::future<int> f = pr.get_future();
auto r = f.get();
```
### std::packaged_task
std::packaged_task它包装了一个可调用的目标（如function, lambda expression, bind expression, or another function object）,以便异步调用，它和promise在某种程度上有点像，promise保存了一个共享状态的值，而`packaged_task`保存的是一 个函数。它的基本用法：
```cpp
std::packaged_task<int()> task([](){ return 7; });
std::thread t1(std::ref(task)); 
std::future<int> f1 = task.get_future(); 
auto r1 = f1.get();
```


### std::promise、std::packaged_task和std::future的关系

看了`std::async`相关的几个对象`std::future`、`std::promise`和`std::packaged_task`，其中 `std::promise`和`std::packaged_task`的结果最终都是通过其内部的future返回出来的，看看他们之间的关系到底是怎样的，`std::future`提供了一个访问异步操作结果的机制，它和线程是一个级别的属于低层次的对象，在它之上高一层的是`std::packaged_task`和`std::promise`，他们内部都有future以便访问异步操作结果，`std::packaged_task`包装的是一个异步操作，而`std::promise`包装的是一个值，都是为了方便异步操作的，因为有时需要获取线程中的某个值，这时就用`std::promise`，而有时需要获一个异步操作的返回值，这时就用`std::packaged_task`。

那 `std::promise`和`std::packaged_task`之间又是什么关系呢？说他们没关系也没关系，说他们有关系也有关系，都取决于如何使用他们了，可以将一个异步操作的结果保存到`std::promise`中。


### 为什么要用`std::async`代替线程的创建
`std::async`是为了让开发者的少费点脑子的，它让这三个对象默契的工作。大概的工作过程是这样的：`std::async`先将异步操作用`std::packaged_task`包 装起来，然后将异步操作的结果放到`std::promise`中，这个过程就是创造未来的过程。外面再通过`future.get/wait`来获取这个未来的结果！

现在来看看`std::async`的原型

`async(std::launch::async | std::launch::deferred, f, args...)` 第一个参数是线程的创建策略，有两种策略，默认的策略是立即创建线程：

`std::launch::async`：在调用async就开始创建线程。

`std::launch::deferred`：延迟加载方式创建线程。调用async时不创建线程，直到调用了`future`的`get`或者`wait`时才创建线程。

第二个参数是线程函数，第三个参数是线程函数的参数。



### `std::async`基本用法
```cpp
std::future<int> f1 = std::async(std::launch::async, []() {
    return 8;
    });
cout << f1.get() << endl; //output: 8
std::future<void> f2 = std::async(std::launch::async, []() {
    cout << 8 << endl;
    //return 8;
    });
f2.wait(); //output: 8
std::future<int> future = std::async(std::launch::async, []() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 8;
    });
std::cout << "waiting...\n";
//Test12();
std::future_status status;
Sleep(3000);
do {
    status = future.wait_for(std::chrono::seconds(1));
    if (status == std::future_status::deferred) {
        std::cout << "deferred\n";
    }
    else if (status == std::future_status::timeout) {
        std::cout << "timeout\n";
    }
    else if (status == std::future_status::ready) {
        std::cout << "ready!\n";
    }
} while (status != std::future_status::ready);
std::cout << "result is " << future.get() << '\n';
```
可能的结果：waiting... timeout timeout ready! result is 8

### 总结
`std::async`是更高层次上的异步操作，它的存在可以使开发者不用关注线程创建内部细节，就能方便的获取异步执行状态和结果，还可以指定线程创建策略，应该用`std::async`替代线程的创建，让它成为做异步操作的首选。
