C++ STL 函数符
<a name="K9VNs"></a>
### 定义函数符
<a name="DCkDK"></a>
#### [functor.h](https://github.com/Fcscanf/Pro1/commit/9d8e49d2dbeda2e2038f10b9aaa714200b3b5596#diff-f0753f3583f8457039ab3ae3ad7e54a62350fd9d53a15031a6a3ce9542fe7a81)
```cpp
#ifndef PRO1_FUNCTOR_H
#define PRO1_FUNCTOR_H

template <class T>
class TooBig {
private:
    T cutoff;
public:
    TooBig(const T & t):cutoff(t){}
    bool operator()(const T & v){ return v > cutoff; }
};

#endif //PRO1_FUNCTOR_H
```
<a name="oDKbY"></a>
#### [functor.cpp](https://github.com/Fcscanf/Pro1/commit/9d8e49d2dbeda2e2038f10b9aaa714200b3b5596#diff-520f3912e8197b6c45d8a5a2903893cfa609be0264ab17fd1d78d1257f6360f8)
```cpp
#include "functor.h"
```
<a name="Sl3kk"></a>
### 函数符的使用
<a name="CsxOu"></a>
#### [main.cpp](https://github.com/Fcscanf/Pro1/commit/9d8e49d2dbeda2e2038f10b9aaa714200b3b5596#diff-608d8de3fba954c50110b6d7386988f27295de845e9d7174e40095ba5efcf1bb)
```cpp
#include <iostream>
#include "module17_stl/functor/functor.h"

using namespace std;

// 函数符
void useFunctor(){
    TooBig<int> f100(100);
    list<int> yadayada;
    list<int> etcetera;
    int vals[10] = {50, 100, 90, 180, 60, 210, 415, 88, 188, 201};
    yadayada.insert(yadayada.begin(), vals, vals + 10);
    etcetera.insert(etcetera.begin(), vals, vals + 10);
    ostream_iterator<int, char> out(cout, " ");
    cout << "Original list:\n";
    copy(yadayada.begin(), yadayada.end(), out);
    cout << endl;
    copy(etcetera.begin(), etcetera.end(), out);
    cout << endl;
    yadayada.remove_if(f100);
    etcetera.remove_if(TooBig<int>(200));
    cout << "Trimmed lists:\n";
    copy(yadayada.begin(), yadayada.end(), out);
    cout << endl;
    copy(etcetera.begin(), etcetera.end(), out);
    cout << endl;
}

int main() {
    useFunctor();
    return 0;
}
```
程序输出
```cpp
Original list:
50 100 90 180 60 210 415 88 188 201
50 100 90 180 60 210 415 88 188 201
Trimmed lists:
50 100 90 60 88
50 100 90 180 60 88 188
```
<a name="ScWPn"></a>
### 自适应函数符和函数适配器
```cpp
#include <iostream>
#include <functional>

using namespace std;

// funadap-自适应函数符和函数适配器
void Shows(double);
const int LIM = 5;
void Shows(double v){
    cout.width(6);
    cout << v << '  ';
}
void funadap(){
    double arr1[LIM] = {36, 39, 42, 45, 48};
    double arr2[LIM] = {25, 27, 29, 31, 33};
    vector<double> gr8(arr1, arr1 + LIM);
    vector<double> m8(arr2, arr2 + LIM);
    cout.setf(ios_base::fixed);
    cout.precision(1);
    cout << "gr8: \t";
    for_each(gr8.begin(), gr8.end(), Shows);
    cout << endl;
    cout << "m8: \t";
    for_each(m8.begin(), m8.end(), Shows);
    cout << endl;
    vector<double > sum(LIM);
    transform(gr8.begin(), gr8.end(), m8.begin(), sum.begin(), plus<double>());
    cout << "sum: \t";
    for_each(sum.begin(), sum.end(), Shows);
    cout << endl;

    vector<double > prod(LIM);
    transform(gr8.begin(), gr8.end(), prod.begin(), bind1st(multiplies<double>(), 2.5));
    cout << "prod:\t";
    for_each(prod.begin(), prod.end(), Shows);
    cout << endl;
}

int main() {
    funadap();
    return 0;
}
```
程序输出
```cpp
gr8: 	  36.08224  39.08224  42.08224  45.08224  48.08224
m8: 	  25.08224  27.08224  29.08224  31.08224  33.08224
sum: 	  61.08224  66.08224  71.08224  76.08224  81.08224
prod:	  90.08224  97.58224 105.08224 112.58224 120.08224
```
