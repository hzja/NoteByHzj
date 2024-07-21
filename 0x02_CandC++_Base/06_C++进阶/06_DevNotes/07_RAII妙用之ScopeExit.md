## RAII
Resource Acquisition Is Initialization，**资源获取即初始化**，将资源的生命周期与一个对象的生命周期绑定，举例来说就是，把一些资源封装在类中，在构造函数请求资源，在析构函数中释放资源且绝不抛出异常，而一个对象在生命周期结束时会自动调用析构函数，即资源的生命周期与一个对象的生命周期绑定。

## RAII的应用
见如下代码：
```cpp
std::mutex mutex;
void func() {}
void NoRAII() {
    mutex.lock();
    func();
    if (xxx) {
        mutex.unlock();// 多次需要调用unlock()，还有可能忘记调用unlock导致一直持有锁
        return;
    }
    ...
    mutex.unlock();
}
void RAII() { // 不需要显式调用unlock
    std::lock_guard<std::mutex> lock(mutex);
    func();
    if (xxx) {
        return;
    }
    ...
    return;
}
```
RAII的应用非常多，C++的STL基本都遵循RAII规范，典型的如vector, string, lock_guard, unique_lock, shared_ptr, unique_ptr等，这里不会介绍这些STL的使用，相信大家也都会使用，如果有相关需求可以留言。
## RAII的巧用
boost中的ScopeExit是个很高级的特性，利用RAII特性，可以在作用域结束时自动关闭已经打开的资源或做某些清理操作，类似于unique_ptr，但又比unique_ptr方便，不需要自定义delete函数。<br />举例: 如果没有ScopeExit
```cpp
void test () {
    char *test = new char[100];
    if (a) {
        delete[] test; // count 1
        return;
    }
    xxx;
    if (b) {
        delete[] test; // count 2
        return;
    }
    ...
    delete[] test; // count 3
}
```
<a name="yJr15"></a>
## 使用了ScopeExit
```cpp
void test () {
    char *test = new char[100];
    std::ofstream ofs("test.txt");
    ScopeExit {
        delete[] test; // 在test函数生命周期结束后自动执行delete[]操作
      ofs.close(); // 在生命周期结束后自动关闭文件，这里只是举个不恰当例子，ofstream自动生命周期结束后就会关闭
    };
    if (a) {
        return;
    }
    xxx;
    if (b) {
        return;
    }
    ...
}
```
当然，正常C++代码不鼓励使用裸指针，可以使用智能指针来申请资源，这里只是举个例子，使用ScopeExit也可以用于处理文件资源的关闭等等。<br />两者代码比较后优劣程度显而易见，不使用ScopeExit需要在return前多次做资源清理操作，而使用了ScopeExit则只需做一次声明后在作用域结束后会自动进行相关的资源清理操作，方便而且不易出错。
<a name="L5Zpi"></a>
## ScopeExit实现
这里参考boost使用C++11实现了一套ScopeExit机制
```cpp
class ScopeExit {
   public:
    ScopeExit() = default;

    ScopeExit(const ScopeExit&) = delete;
    void operator=(const ScopeExit&) = delete;

    ScopeExit(ScopeExit&&) = default;
    ScopeExit& operator=(ScopeExit&&) = default;

    template <typename F, typename... Args>
    ScopeExit(F&& f, Args&&... args) {
        func_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    }

    ~ScopeExit() {
        if (func_) {
            func_();
        }
    };

   private:
    std::function<void()> func_;
};

#define _CONCAT(a, b) a##b
#define _MAKE_SCOPE_(line) ScopeExit _CONCAT(defer, line) = [&]()

#undef SCOPE_GUARD
#define SCOPE_GUARD _MAKE_SCOPE_(__LINE__)
```
使用方式如下:
```cpp
void test () {
    char *test = new char[100];
    std::ofstream ofs("test.txt");
    SCOPE_GUARD{
        delete[] test;
        ofs.close();
    };
    if (a) {
        return;
    }
    ...
    if (b) {
        return;
    }
    ...
}
```
<a name="activity-name"></a>
## RAII妙用之计算函数耗时
平时编程过程中不可避免的需要考虑程序的性能，其中，最主要的也是最常见的性能就是“函数耗时”，基本上每个开发者都有打印某个函数耗时的需求，平时打印函数时间是否使用的是如下方式：
```cpp
void Func() {
...
}

int CalTime() {
    int begin = GetCurrentTime(); // 伪代码
    Func();
    int end = GetCurrentTime();
    cout << "func time is " << end - begin << " s" << endl;
}
```
想计算某个函数的耗时就在函数前后获取时间之后再算差值，这种方式不仅写法不好看，还麻烦。<br />这里可以利用RAII方式，把函数的生命周期和一个对象绑定，对象创建时候执行函数，对象生命周期结束析构时候函数执行完毕，这样对象存活的时间就是函数的耗时，见代码：
```cpp
#pragma once

#include <sys/time.h>

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using llong = long long;
using namespace std::chrono;
using std::cout;
using std::endl;

namespace wzq {
namespace timer {
class TimerLog {
  public:
   TimerLog(const std::string tag) { // 对象构造时候保存开始时间
       m_begin_ = high_resolution_clock::now();
       m_tag_ = tag;
  }

   void Reset() { m_begin_ = high_resolution_clock::now(); }

   llong Elapsed() {
       return static_cast<llong>(
           duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - m_begin_).count());
  }

   ~TimerLog() { // 对象析构时候计算当前时间与对象构造时候的时间差就是对象存活的时间
       auto time =
           duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_begin_).count();
       std::cout << "time { " << m_tag_ << " } " << static_cast<double>(time) << " ms" << std::endl;
  }


  private:
   std::chrono::time_point<std::chrono::high_resolution_clock> m_begin_;
   std::string m_tag_;
};

}  // namespace timer
}  // namespace wzq
```
使用方式：
```cpp
void TestTimerLog() {
   auto func = [](){
       for (int i = 0; i < 5; ++i) {
           cout << "i " << i << endl;
           std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
  };
  {
       wzq::timer::TimerLog t("func");
       func();
  }
}
```
程序输出：
```cpp
i 0
i 1
i 2
i 3
i 4
time { func } 5 ms
```
这样就可以很方便的打印函数时间，但是这里每次都需要定义一个对象，貌似也不太方便，可以考虑加个宏，如下：
```cpp
#define CAL_SCOPE_TIME(x) wzq::timer::TimerLog t(x)
```
再如下使用：
```cpp
void TestTimerLog() {
    auto func = [](){
        for (int i = 0; i < 5; ++i) {
            cout << "i " << i << endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    };
    {
        CAL_SCOPE_TIME("func time");
        func();
    }
}
```
是不是更方便，当然也有可能要计算多个交叉函数的耗时。
```cpp
void TestTime() {
    func1();
    func2();
    func3();
    func4();
}
```
这里如果想计算func1()+func2()+func3()耗时，也想计算func2()+func3()+func4()耗时，使用上述RAII貌似就不太方便了，这里可以再写两个宏。
```cpp
#define CAL_TIME_BEGIN(x) auto begin_##x = wzq::timer::TimerLog::Now();

#define CAL_TIME_END(x) \
	cout << "time { " << #x << " } " << wzq::timer::TimerLog::DiffMs(begin_##x, wzq::timer::TimerLog::Now()) << "ms" << endl;
```
就可以如下使用，尽管不是特别方便，但是也比最开始介绍的方便一些。
```cpp
void TestTime() {
    CAL_TIME_BEGIN(func123)
    func1();
    CAL_TIME_BEGIN(func234)
    func2();
    func3();
    CAL_TIME_END(func123)
    func4();
    CAL_TIME_END(func234)
}
```
这样就会输出想要的耗时结果。
<a name="lpf9a"></a>
## 完整代码
```cpp
#pragma once

#include <sys/time.h>

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using llong = long long;
using namespace std::chrono;
using std::cout;
using std::endl;

#define CAL_SCOPE_TIME(x) wzq::timer::TimerLog t(x)

#define CAL_TIME_BEGIN(x) auto begin_##x = wzq::timer::TimerLog::Now();

#define CAL_TIME_END(x) \
    cout << "time { " << #x << " } " << wzq::timer::TimerLog::DiffMs(begin_##x, wzq::timer::TimerLog::Now()) << "ms" << endl;

namespace wzq {
namespace timer {
class TimerLog {
   public:
    TimerLog(const std::string tag) {
        m_begin_ = high_resolution_clock::now();
        m_tag_ = tag;
    }

    void Reset() { m_begin_ = high_resolution_clock::now(); }

    llong Elapsed() {
        return static_cast<llong>(
            duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - m_begin_).count());
    }

    static std::chrono::time_point<std::chrono::high_resolution_clock> Now() { return high_resolution_clock::now(); }

    static llong DiffUs(std::chrono::time_point<std::chrono::high_resolution_clock> before,
                        std::chrono::time_point<std::chrono::high_resolution_clock> after) {
        return static_cast<llong>(duration_cast<std::chrono::microseconds>(after - before).count());
    }

    static llong DiffMs(std::chrono::time_point<std::chrono::high_resolution_clock> before,
                        std::chrono::time_point<std::chrono::high_resolution_clock> after) {
        return static_cast<llong>(duration_cast<std::chrono::milliseconds>(after - before).count());
    }

    ~TimerLog() {
        auto time =
            duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_begin_).count();
        std::cout << "time { " << m_tag_ << " } " << static_cast<double>(time) << " ms" << std::endl;
    }

    static llong GetCurrentMs() {
        struct timeval time;
        gettimeofday(&time, NULL);
        return static_cast<llong>(time.tv_sec * 1000) + static_cast<llong>(time.tv_usec / 1000);
    }

    static void ShowCurTime() {
        time_t now = time(0);
        char* dt = ctime(&now);
        cout << "cur time is " << dt << endl;
        cout << "cur ms is " << GetCurrentMs() << endl;
    }

    static struct timeval GetCurrentTimeofDay() {
        struct timeval time;
        gettimeofday(&time, NULL);
        return time;
    }

   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_begin_;
    std::string m_tag_;
};

}  // namespace timer
}  // namespace wzq
```
