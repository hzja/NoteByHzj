C++ STL 分配器
<a name="WBTtq"></a>
### 分配器
```cpp
#include <iostream>
#include <vector>

using namespace std;

// 分配器
const int NUM = 5;
void useVector(){
    vector<int> rating(NUM);
    vector<double > titles(NUM);
    cout << "You will do ecactly as told. You will enter\n" << NUM << "book titles and your rating(0-10).\n";
    for (int i = 0; i < NUM; i++) {
        cout << "Enter title # " << i + 1 << ": ";
        cin >> titles[i];
        cout << "Enter your rating (0-10): ";
        cin >> rating[i];
        cin.get();
    }
    cout << "Thank you. You entered the following: \n" << "Rating\tBook\n";
    for (int i = 0; i < NUM; i++) {
        cout << rating[i] << "\t" << titles[i] << endl;
    }
}

int main() {
    useVector();
    return 0;
}
```
<a name="iNnKv"></a>
### STL函数的用法
```cpp
#include <iostream>
#include <algorithm>

using namespace std;

// STL函数的用法
struct Review{
    string title;
    int rating;
};

bool operator<(const Review & r1, const Review & r2);
bool worseThan(const Review & r1, const Review & r2);
bool FillReview(Review & rr);
void ShowReview(const Review & rr);

bool operator<(const Review & r1, const Review & r2){
    if (r1.title < r2.title)
        return true;
    else if (r1.title == r2.title && r1.rating < r2.rating)
        return false;
    else
        return false;
}

bool worseThan(const Review & r1, const Review & r2){
    if (r1.rating < r2.rating)
        return true;
    else
        return false;
}

bool FillReview(Review & rr){
    cout << "Enter book title(quit to quit): ";
    getline(cin, rr.title);
    if (rr.title == "quit")
        return false;
    cout << "Enter book rating: ";
    cin >> rr.rating;
    if (!cin)
        return false;
    cin.get();
    return true;
}

void ShowReview(const Review & rr){
    cout << rr.rating << "\t" << rr.title << endl;
}


void stlFunction(){
    vector<Review> books;
    Review temp;
    while (FillReview(temp))
        books.push_back(temp);
    cout << "Thanks ,you entered the following " << books.size() << " rating:\n" << "Rating\tBook\n";
    for_each(books.begin(), books.end(), ShowReview);
    sort(books.begin(), books.end());
    cout << "Sorted by title : \nRating\tBook\n";
    for_each(books.begin(), books.end(), ShowReview);
    sort(books.begin(), books.end(), worseThan);
    cout << "Sorted by rating : \nRating\tBook\n";
    for_each(books.begin(), books.end(), ShowReview);
    cout << "Bye.\n";
}

int main() {
    stlFunction();
    return 0;
}
```
<a name="G5no2"></a>
### 迭代器的使用
```cpp
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;

// 迭代器
void useIterator(){
    int casts[10] = {6, 7, 2, 9, 4, 11, 8, 7, 10, 5};
    vector<int > dice(10);
    // copy from to vector
    copy(casts, casts + 10, dice.begin());
    cout << "Let the dice be cast!\n";
    // create an ostream iterator
    ostream_iterator<int ,char> out_iter(cout, " ");
    // copy from vector to output
    copy(dice.begin(), dice.end(), out_iter);
    cout << endl;
    cout << "Implict use of reverse iterator.\n";
    copy(dice.rbegin(), dice.rend(), out_iter);
    cout << endl;
    cout << endl;
    cout << "Implicit use of reverse iterator.\n";
    vector<int >::reverse_iterator ri;
    for (ri = dice.rbegin(); ri != dice.rend(); ++ri)
        cout << *ri << ' ';
    cout << endl;
}

// insert
void iteratorInsert(){
    string s1[4] = {"fine", "fish", "fashion", "fate"};
    string s2[2] = {"busy", "bats"};
    string s3[2] = {"silly", "singers"};
    vector<string> words(4);
    copy(s1, s1 + 4, words.begin());
    ostream_iterator<string, char> out(cout, " ");
    copy(words.begin(), words.end(), out);
    cout << endl;

    // construct anonymous back_insert_iterator object
    copy(s2, s2 + 2, back_insert_iterator<vector<string>>(words));
    copy(words.begin(), words.end(), out);
    cout << endl;

    // construct anonymous back_insert_iterator object
    copy(s3, s3 + 2, insert_iterator<vector<string>>(words, words.begin()));
    copy(words.begin(), words.end(), out);
    cout << endl;
}

int main() {
    useIterator();
    iteratorInsert();
    return 0;
}
```
<a name="sw9Bs"></a>
### List相关操作
```cpp
#include <iostream>
#include <algorithm>
#include <iterator>
#include <list>

using namespace std;

// List的相关操作
void useList(){
    int stuff[5] = {1, 2, 4, 8, 6};
    list<int> two;
    two.insert(two.begin(), stuff, stuff + 5);
    int more[6] = {6, 4, 2, 4, 6, 5};
    list<int> three(two);
    three.insert(three.end(), more, more + 6);

    cout << "List two: ";
    ostream_iterator<int , char> out(cout, " ");
    copy(two.begin(), two.end(), out);
    cout << endl << "List three: ";
    copy(three.begin(), three.end(), out);
    three.remove(2);
    cout << endl << "List three minus 2s: ";
    copy(three.begin(), three.end(), out);
    three.splice(three.begin(), two);
    cout << endl << "List three after splice: ";
    three.unique();
    cout << endl << "List three after unique: ";
    copy(three.begin(), three.end(), out);
    three.sort();
    three.unique();
    cout << endl << "List three after sort & unique: ";
    copy(three.begin(), three.end(), out);
    two.sort();
    three.merge(two);
    cout << endl << "Sorted two merged into three: ";
    copy(three.begin(), three.end(), out);
    cout << endl;
}

int main() {
    useList();
    return 0;
}
```
程序输出
```
List two: 1 2 4 8 6
List three: 1 2 4 8 6 6 4 2 4 6 5
List three minus 2s: 1 4 8 6 6 4 4 6 5
List three after splice:
List three after unique: 1 2 4 8 6 1 4 8 6 4 6 5
List three after sort & unique: 1 2 4 5 6 8
Sorted two merged into three: 1 2 4 5 6 8
```
<a name="zQ7RX"></a>
### Set相关操作
```cpp
#include <iostream>
#include <algorithm>
#include <iterator>
#include <set>

using namespace std;

// Set的相关操作
void useSet(){
    const int N = 6;
    string s1[N] = {"buffon", "thinkers", "for", "heavy", "can", "for"};
    string s2[N] = {"metal", "any", "food", "elegant", "deliver", "for"};

    set<string> A (s1, s1 + N);
    set<string> B (s2, s2 + N);
    ostream_iterator<string, char> out(cout, "");
    cout << "Set A: ";
    copy(A.begin(), A.end(), out);
    cout << endl;
    cout << "Set B: ";
    copy(B.begin(), B.end(), out);
    cout << endl;

    cout << "Union of A and B: \n";
    set_union(A.begin(), A.end(), B.begin(), B.end(), out);
    cout << endl;

    cout << "Intersection of A and B:\n";
    set_intersection(A.begin(), A.end(), B.begin(), B.end(), out);
    cout << endl;

    cout << "Difference of A and B:\n";
    set_difference(A.begin(), A.end(), B.begin(), B.end(), out);
    cout << endl;

    set<string> C;
    cout << "Set C:\n";
    set_union(A.begin(), A.end(), B.begin(), B.end(), insert_iterator<set<string>>(C, C.begin()));
    copy(C.begin(), C.end(), out);
    cout << endl;

    string s3("grungy");
    C.insert(s3);
    cout << "Set C after insertion: \n";
    copy(C.begin(), C.end(), out);
    cout << endl;

    cout << "Showing a range:\n";
    copy(C.lower_bound("ghost"), C.upper_bound("spook"), out);
    cout << endl;
}

int main() {
    useSet();
    return 0;
}
```
程序输出
```
Set A: buffoncanforheavythinkers
Set B: anydeliverelegantfoodformetal
Union of A and B:
anybuffoncandeliverelegantfoodforheavymetalthinkers
Intersection of A and B:
for
Difference of A and B:
buffoncanheavythinkers
Set C:
anybuffoncandeliverelegantfoodforheavymetalthinkers
Set C after insertion:
anybuffoncandeliverelegantfoodforgrungyheavymetalthinkers
Showing a range:
grungyheavymetal
```
<a name="bac780ff"></a>
### 容器-multimap相关操作
```cpp
#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

// multimap的相关操作
typedef int KeyType;
typedef pair<const KeyType, string> Pairs;
typedef multimap<KeyType , string> MapCode;
void useMultimap(){
    MapCode codes;

    codes.insert(Pairs(415, "San Francisco"));
    codes.insert(Pairs(510, "Oakland"));
    codes.insert(Pairs(718, "Brooklyn"));
    codes.insert(Pairs(718, "Staten Island"));
    codes.insert(Pairs(415, "San Rafael"));
    codes.insert(Pairs(510, "Berkeley"));

    cout << "Number if cities with area code 415: " << codes.count(415) << endl;
    cout << "Number if cities with area code 718: " << codes.count(718) << endl;
    cout << "Number if cities with area code 510: " << codes.count(510) << endl;
    cout << "Area Code City\n";
    MapCode::iterator it;
    for (it = codes.begin(); it != codes.end(); ++it){
        cout << "   " << (*it).first << "   " << (*it).second << endl;
    }
    pair<MapCode::iterator, MapCode::iterator> range = codes.equal_range(718);
    cout << "Cities with area code 718: \n";
    for (it = range.first; it != range.second; ++it)
        cout << (*it).second << endl;
}

int main() {
    useMultimap();
    return 0;
}
```
程序输出
```
Number if cities with area code 415: 2
Number if cities with area code 718: 2
Number if cities with area code 510: 2
Area Code City
415   San Francisco
415   San Rafael
510   Oakland
510   Berkeley
718   Brooklyn
718   Staten Island
Cities with area code 718:
Brooklyn
Staten Island
```
<a name="TiDBT"></a>
### STL和String类
```cpp
#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

// STL和String类
void stlString(){
    string letters;
    cout << "Enter the letter grouping (q to quit): ";
    while (cin >> letters && letters != "q"){
        cout << "Permutations of " << letters << endl;
        sort(letters.begin(), letters.end());
        cout << letters << endl;
        while (next_permutation(letters.begin(), letters.end()))
            cout << letters << endl;
        cout << "Enter next sequence (q to quit): ";
    }
    cout << "Done.\n";
}

int main() {
    stlString();
    return 0;
}
```
程序输出
```
Enter the letter grouping (q to quit): wed
Permutations of wed
dew
dwe
edw
ewd
wde
wed
Enter next sequence (q to quit): q
Done.
```
<a name="6cHxA"></a>
### 函数和容器方法
```cpp
#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

// 函数和容器方法
void View(int);
void View(int v){
    cout << v << ' ';
}
const int LIMS = 10;
void funList(){
    int ar[LIMS] = {4, 5, 4, 2, 2, 3, 4, 8, 1, 4};
    list<int> la(ar, ar + LIMS);
    list<int> lb(la);

    cout << "Original list contents: \n\t";
    for_each(la.begin(), la.end(), View);
    cout << endl;
    la.remove(4);
    cout << "After using the remove() method: \n";
    cout << "la: \t";
    for_each(la.begin(), la.end(), View);
    cout << endl;
    list<int>::iterator last;
    last = remove(lb.begin(), lb.end(), 4);
    cout << "After using the remove() function: \n";
    cout << "lb: \t";
    for_each(lb.begin(), lb.end(), View);
    cout << endl;
    lb.erase(last, lb.end());
    cout << "After using the erase() method: \n";
    cout << "lb: \t";
    for_each(lb.begin(), lb.end(), View);
    cout << endl;
}

int main() {
    funList();
    return 0;
}
```
程序输出
```
Original list contents:
4 5 4 2 2 3 4 8 1 4
After using the remove() method:
la: 	5 2 2 3 8 1
After using the remove() function:
lb: 	5 2 2 3 8 1 4 8 1 4
After using the erase() method:
lb: 	5 2 2 3 8 1
```
<a name="fkg4T"></a>
### 使用STL
```cpp
#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

// 使用STL
char toLower(char ch){ return tolower(ch); }
string & ToLower(string & st);
void display(const string & s);

string & ToLower(string & st){
    transform(st.begin(), st.end(), st.begin(), toLower);
    return st;
}

void display(const string & s){
    cout << s << " ";
}

void useSTL(){
    vector<string> words;
    cout << "Enter words (enter q to quit): \n";
    string input;
    while (cin >> input && input != "q")
        words.push_back(input);
    cout << "You entered the following words:\n";
    for_each(words.begin(), words.end(), display);
    cout << endl;
    // place words in set, converting to lowercase
    set<string> wordset;
    transform(words.begin(), words.end(), insert_iterator<set<string>>(wordset, wordset.begin()), ToLower);
    cout << "\nAlphabetic list of words: \n";
    for_each(wordset.begin(), wordset.end(), display);
    cout << endl;

    // place word and frequency in map
    map<string, int> wordmap;
    set<string>::iterator si;
    for (si = wordset.begin(); si != wordset.end(); si++)
        wordmap[*si] = count(words.begin(), words.end(), *si);

    // display map contents
    cout << "\nWord frequency: \n";
    for (si = wordset.begin(); si != wordset.end(); si++)
        cout << *si << ": " << wordmap[*si] << endl;
}

int main() {
    useSTL();
    return 0;
}
```
程序输出
```
Enter words (enter q to quit):
The dog saw the cat thought the cat fat the cat thought the cat perfact
q
You entered the following words:
The dog saw the cat thought the cat fat the cat thought the cat perfact

Alphabetic list of words:
cat dog fat perfact saw the thought

Word frequency:
cat: 4
dog: 1
fat: 1
perfact: 1
saw: 1
the: 5
thought: 2
```
<a name="z2RBZ"></a>
### 其他库的使用-Vector、valarray
```cpp
#include <iostream>
#include <algorithm>
#include <iterator>
#include <valarray>

using namespace std;

// 其他STL库的使用
void valvect(){
    vector<double > data;
    double temp;
    cout << "Enter numbers(<= 0 to quit):\n";
    while (cin >> temp && temp > 0)
        data.push_back(temp);
    sort(data.begin(), data.end());
    int size = data.size();
    valarray<double > numbers(size);
    int i;
    for (int i = 0; i < size; i++) {
        numbers[i] = data[i];
    }
    valarray<double > sq_rts(size);
    sq_rts = sqrt(numbers);
    valarray<double > result(size);
    result = numbers + 2.0 * sq_rts;
    cout.setf(ios_base::fixed);
    cout.precision(4);
    for (int i = 0; i < size; i++) {
        cout.width(8);
        cout << numbers[i] << ": ";
        cout.widen(8);
        cout << result[i] << endl;
    }
    cout << "Done!\n";
}

int main() {
    valvect();
    return 0;
}
```
程序输出
```
Enter numbers(<= 0 to quit):
5 21.2 6 8 2 10 14.4 0
2.0000: 4.8284
5.0000: 9.4721
6.0000: 10.8990
8.0000: 13.6569
10.0000: 16.3246
14.4000: 21.9895
21.2000: 30.4087
Done!
```
<a name="0db918ae"></a>
### slice的使用
```cpp
#include <iostream>
#include <algorithm>
#include <iterator>
#include <valarray>

using namespace std;

// slice的使用
const int SIZE = 12;
typedef valarray<int> vint;
void Show_s(const vint & v, int cols);

void Show_s(const vint & v, int cols){
    int lim = v.size();
    for (int i = 0; i < lim; i++) {
        cout.width(3);
        cout << v[i];
        if (i % cols == cols - 1)
            cout << endl;
        else
            cout << ' ';
    }
    if (lim % cols != 0)
        cout << endl;
}

void useSlice(){
    vint valint(SIZE);
    for (int i = 0; i < SIZE; i++) {
        valint[i] = rand() % 10;
    }
    cout << "Original array: \n";
    Show_s(valint, 3);
    vint vcol(valint[slice(1, 4, 3)]);
    cout << "Second column: \n";
    Show_s(vcol, 1);
    valint[slice(2, 4, 3)] = 10;
    cout << "Set last column to 10 :\n";
    Show_s(valint, 3);
    cout << "Set first column to sum of next two:\n";
    valint[slice(0, 4, 3)] = vint(valint[slice(1, 4, 3)]) + vint(valint[slice(2, 4, 3)]);
    Show_s(valint, 3);
}

int main() {
    useSlice();
    return 0;
}
```
程序输出
```
Original array:
3   3   2
9   0   8
2   6   6
9   1   1
Second column:
3
0
6
1
Set last column to 10 :
3   3  10
9   0  10
2   6  10
9   1  10
Set first column to sum of next two:
13   3  10
10   0  10
16   6  10
11   1  10
```
<a name="gm5lU"></a>
### 输出输入-write
```cpp
#include <iostream>
#include <algorithm>
#include <iterator>
#include <valarray>

using namespace std;

void useWrite(){
    const char * state1 = "Florida";
    const char * state2 = "Kansas";
    const char * state3 = "Euphoria";
    int len = strlen(state2);
    cout << "Increasing loop index: \n";
    for (int i = 0; i < len; i++) {
        cout.write(state2, i);
        cout << endl;
    }
    cout << "Decreasing loop index: \n";
    for (int i = len; i > 0; i--)
        cout.write(state2, i) << endl;

    cout << "Exceeding string length:\n";
    cout.write(state2, len + 5) << endl;
}

int main() {
    useWrite();
    return 0;
}
```
程序输出
```
Increasing loop index:

K
Ka
Kan
Kans
Kansa
Decreasing loop index:
Kansas
Kansa
Kans
Kan
Ka
K
Exceeding string length:
Kansas Euph
```
