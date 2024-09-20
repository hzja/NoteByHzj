Java Arrays<br />给一连串数字要排序，可能觉得小case，如果这里有一个图书类别，图书类的成员变量有：书名、价格、作者等等，要求根据图书价格升序排序打印所有图书信息，会怎么做呢？<br />咱们可以在图书类里面定义一个compare 方法
```java
int compare(Book book){undefined
    return  price-book.price
}
```
如果返回值大于零 说明调用者的 price 要更大 ，小于零说明方法体里的参数要更大。<br />咱们再建一个MyArrays类，定义一个排序方法给咱们的book排序。<br />思路有了，下面咱们开始写代码吧，为了达到扩展效果，以便于别人都可以使用，咱们先建一个MyCompareble接口。
```java
public interface MyComparable {
    //>0调用者大   <0参数大 ==0一样大
    public int compare(Object a);
}
```
因为希望所有类都可以实现这个接口并使用其方法，这里参数类型是用了Object，Object是所有类的父类，object对象可以指向任意对象。这里让Book类实现这个接口：
```java
public class Book implements MyComparable{
	private String bookName;
	private int price;
	public Book() {
		super();
	}
	public Book(String bookName, int price) {
		super();
		this.bookName = bookName;
		this.price = price;
	}
	public String getBookName() {
		return bookName;
	}
	public void setBookName(String bookName) {
		this.bookName = bookName;
	}
	public int getPrice() {
		return price;
	}
	public void setPrice(int price) {
		this.price = price;
	}
	@Override
	public String toString() {
		return "Book [bookName=" + bookName + ", price=" + price + "]";
	}
	@Override
	public int compare(Object a) {
		Book b=(Book)a;
		return this.price-b.price;
	}
	
}
```
已经实现了MyCompareble接口并重写了 `compare`方法，再建个MyArrays类，并写好排序方法:
```java
public class MyArrays {
    //排序,Object是所有类的父类，object对象可以指向任意对象
    public static void sort(Object[] objs){
        for(int j=0;j<objs.length-1;j++){
            for(int i=objs.length-1;i>j;i--){
                MyComparable a=(MyComparable)objs[i];
                MyComparable b=(MyComparable)objs[i-1];
                if(a.compare(b)<0){
                    //都能用这段代码
                    Object temp=objs[i];
                    objs[i]=objs[i-1];
                    objs[i-1]=temp;
                }
            }
        }
    }
}
```
功能已经完善了，来测试下:
```java
public class Demo {
    public static void main(String[] args) {
        Book[] books=new Book[]{
                new Book("C",23),
                new Book("Java",109),
                new Book("Other",10)
        };
        MyArrays.sort(books);
        display(books);
 
    }
 
    public static void display(Object[] objs){
        for(int i=0;i<objs.length;i++){
            System.out.println(objs[i]+"");
        }
    }
}
```
