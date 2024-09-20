Java 接口
<a name="UNzuI"></a>
## 1、发展史
Java 编程语言已经发展了二十多年。没有其他编程语言在如此长的时间内发展得如此之快，同时还保持对其遗留代码库的支持。Java 始终尽可能地提供向后兼容性，同时与大多数当前编程语言中可用的新功能保持同步。<br />Java 编程语言主要是关于开发四种类型——**类**、**接口**、**枚举**和**注解**。枚举和注解从 Java 5 开始出现。这里谈谈Java编程语言中接口类型的演变。<br />Java 中的接口最初设计为抽象类型，可用于类型的多重继承。在 Java 1.0 中，接口定义只能包含两种成员静态常量和抽象方法：
```java
public interface SomeInterface {
    int SOME_CONSTANT = 35; // variable declaration
    int abstractMethod(int x, int y); // method declaration
}
```
接口中的所有变量声明始终是 `public static` 和 `final` 的，并且需要赋值。除此之外，接口中的所有方法都是公共的和抽象的。<br />接口中只有抽象方法，没有方法实现，这使得它可以用于类型的多重继承，同时仍然避免了多重继承的菱形问题。<br />类可以继承接口的抽象方法或提供实现。当相同的方法签名出现在超类和它正在实现的接口中时，则该方法是从超类继承的，而不是从接口继承的。<br />Java 1.1 引入了内部类的概念，其中类可以是类的成员。从 Java 2 开始，有了静态嵌套类和接口，这些也可以在接口内部使用。因此，从 Java 2 开始，在接口中有四种成员：**静态常量**、**抽象方法**、**嵌套类**和**嵌套接口**：
```java
public interface SomeInterface {

    int SOME_CONSTANT = 35; // variable declaration
    int abstractMethod(int x, int y); // method declaration

  // nested class definition
    class NestedClass {
        // members of a class
    }

   // nested interface definition
    interface NestedInterface {
        // member of an interface
    }
}
```
在接口中定义的所有嵌套类和接口始终是公共的和静态的。<br />后来，Java 5 发布了许多新功能。它介绍了泛型以及新的枚举和注释类型。<br />**Java 5 中引入的泛型是关于类型参数的。**<br />有泛型类、泛型接口和泛型方法，即。可以使用类型参数定义的类、接口和方法。因此，从 Java 5 开始，可以在接口中有六种类型的成员，使用静态常量、抽象方法、嵌套类、嵌套接口、嵌套枚举和嵌套注释。
```java
// generic interface with one type parameter T
public interface SomeInterface<T> {

    int SOME_CONSTANT = 35; // variable declaration
    int abstractMethod(int x, int y); // method declaration

    T abstractMethodUsingGenericType(T[] array, int i); // method using type parameter

  // nested class definition
    class NestedClass {
        // members of a class
    }

     // nested interface definition
    interface NestedInterface {
        // member of an interface
    }

  // nested enum definition
    enum NestedEnum {
        OBJECT1,
        OBJECT2,
        ;
        // methods, variables and constructors
    }

  // nested annotation definition
    @interface NestedAnnotation {
        String attrib1();
    }
}
```
接口定义中的类型参数 T 只能用于抽象方法的返回类型和抽象方法的参数。它不能与静态成员一起使用。嵌套的枚举和注释始终是公共的和静态的。<br />**Java 的一个重要特性一直是它的向后兼容性**。尽管该语言多年来一直在发展，但仍非常注意支持遗留代码库。所有较新的 Java 版本始终能够编译和运行较旧的源代码——无需对代码进行任何更改。这种支持通常是有代价的。接口就是一个例子。接口的关键特性是它只能有抽象方法。为了实现向后兼容性，这种行为不能轻易更改，因此几乎不可能使用更新的方法来增强 API 中的现有接口。考虑自 Java 2 以来就存在的 List<E> 接口。最好为 List<E> 引入一个排序方法，为了解决这个问题，Java 8 在接口中添加了默认方法作为成员。这允许使用新方法增强接口，为新方法提供默认实现。<br />**Java 8 还允许接口包含静态方法。**因此，从 Java 8 开始，接口的成员可以是静态常量、抽象方法、默认方法、静态方法、嵌套类、嵌套接口、嵌套枚举和嵌套注解：
```java
// generic interface with one type parameter T
public interface SomeInterface<T> {

    int SOME_CONSTANT = 35; // variable declaration
    int abstractMethod(int x, int y); // method declaration

    T abstractMethodUsingGenericType(T[] array, int i); // method using type parameter

    default int defaultMethod(int x, int y) {
        // implementation of method
    }

    static void main(String[] args) {
        // any static method, including main can be included in interface
    }

     // nested class definition
    class NestedClass {
        // members of a class
    }

    // nested interface definition
    interface NestedInterface {
        // member of an interface
    }

  // nested enum definition
    enum NestedEnum {
        OBJECT1,
        OBJECT2,
        ;
        // methods, variables and constructors
    }

     // nested annotation definition
    @interface NestedAnnotation {
        String attrib1();
    }
}
```
接口中的默认方法和静态方法始终是公共的。现在，由于有默认方法，即实现，这意味着，在 Java 中，现在也有行为的多重继承，而不仅仅是类型。<br />现在遇到了 Java 中多重继承的钻石问题。由于可以通过默认方法实现行为，现在可以拥有重复的通用代码，可以在同一接口内的多个默认方法中复制。为了避免这种情况，通常会将方法的实现分解为更小的方法。而且，由于这些方法可能并不真正需要在接口之外可用，因此理想情况下它们应该是私有的。这现在可以通过 Java 9 来完成，Java 9 在接口中引入了私有方法。<br />从 Java 9 开始，接口中支持的成员是静态常量、抽象方法、默认方法、静态方法、私有方法、嵌套类、嵌套接口、嵌套枚举和嵌套注解：
```java
// generic interface with one type parameter T
public interface SomeInterface<T> {
	
	int SOME_CONSTANT = 35; // variable declaration
	int abstractMethod(int x, int y); // method declaration
	
	T abstractMethodUsingGenericType(T[] array, int i); // method using type parameter
	
	default int defaultMethod(int x, int y) {
		// implementation of method
		// can call the privateMethod and privateStaticMethod here
	}
	
	static void main(String[] args) {
		// any static method, including main can be included in interface
		// can call privateStatic method here
	}
	
	private int privateMethod(int x, int y) {
		// private method implementation
	}
	
	private static void privateStaticMethod(int x, int y) {
		// private method implementation
	}
	
	// nested class definition
	class NestedClass {
		// members of a class
	}
	
	// nested interface definition
	interface NestedInterface {
		// member of an interface
	}
	
	// nested enum definition
	enum NestedEnum {
		OBJECT1,
		OBJECT2,
		;
		// methods, variables and constructors
	}
	
	// nested annotation definition
	@interface NestedAnnotation {
		String attrib1();
	}
}
```
<a name="tCIQY"></a>
## 2、结论
有趣的是，接口的性质多年来是如何演变的，同时保持向后兼容性。在 Java 8 之前，接口的核心原则是它只能具有公共和抽象方法。但是，从 Java 8 开始，接口也可以有非抽象方法，而 Java 9 以后的接口也可以有私有方法。
