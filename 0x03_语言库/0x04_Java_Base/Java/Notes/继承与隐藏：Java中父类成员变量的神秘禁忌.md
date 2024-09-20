Java
<a name="aPttd"></a>
## 摘要
在Java中，子类继承了父类的成员变量，但不能直接覆盖（或称为隐藏）父类的成员变量。本文将解释为什么Java设计了这样的规则，探讨与此相关的继承和多态的概念，并说明如何在子类中访问和使用父类的成员变量。
<a name="pwFoy"></a>
## 1、引言
Java作为一门面向对象的编程语言，支持继承和多态等特性，允许子类继承父类的属性和行为。然而，与成员方法不同，Java中的父类成员变量在子类中不能被覆盖。本文将探讨这个设计决策的原因，以及如何在子类中正确使用父类的成员变量。
<a name="dyVEv"></a>
## 2、成员变量的继承和隐藏
在Java中，继承是一种允许子类获取父类属性和方法的机制。通过使用关键字`extends`，子类可以继承父类的属性和方法，并且可以通过父类的引用来实现多态，即在运行时选择调用子类的方法。<br />当子类继承父类时，它会继承父类的成员变量。但是与方法不同，Java不允许子类直接覆盖（隐藏）父类的成员变量。子类可以声明与父类相同名称的成员变量，但它不会真正地覆盖父类的成员变量，而是在子类中创建一个新的成员变量，与父类的成员变量形成隐藏关系。<br />通过一个具体的例子来说明这一点：
```java
class Vehicle {
    int maxSpeed = 100;

    void displaySpeed() {
        System.out.println("Max speed of the vehicle: " + maxSpeed);
    }
}

class Car extends Vehicle {
    int maxSpeed = 200;

    void displaySpeed() {
        System.out.println("Max speed of the car: " + maxSpeed);
    }
}

public class Main {
    public static void main(String[] args) {
        Vehicle vehicle = new Vehicle();
        Vehicle carAsVehicle = new Car();
        Car car = new Car();

        vehicle.displaySpeed();        // 输出：Max speed of the vehicle: 100
        carAsVehicle.displaySpeed();   // 输出：Max speed of the vehicle: 100
        car.displaySpeed();            // 输出：Max speed of the car: 200
    }
}
```
在上面的例子中，定义了一个Vehicle类和一个Car类，其中Car类是Vehicle类的子类。两个类都有一个名为maxSpeed的成员变量，并且分别提供了一个名为displaySpeed的方法用于显示最大速度。<br />在Car类中，覆盖了displaySpeed方法，并在其中输出了maxSpeed成员变量的值。然而，可以注意到，尽管Car类中的maxSpeed和Vehicle类中的maxSpeed拥有相同的名称，但在运行时它们输出的值是不同的。这是因为在Car类中创建了一个新的成员变量，与父类中的maxSpeed成员变量形成了隐藏关系。<br />在main方法中，创建了一个Vehicle对象、一个Car对象，并使用Vehicle类的引用指向一个Car对象。当调用displaySpeed方法时，由于Java的动态绑定特性，会根据对象的实际类型来决定调用哪个类的方法。因此，`vehicle.displaySpeed()`和`carAsVehicle.displaySpeed()`输出的是Vehicle类的方法，而`car.displaySpeed()`输出的是Car类的方法。<br />这个例子展示了继承和隐藏的概念。尽管子类可以在声明中使用相同的名称来隐藏父类的成员变量，但实际上这并不是对父类成员变量的覆盖。如果需要访问父类的成员变量，可以使用super关键字来显式地引用父类的成员变量。

1. 多态与方法重写 多态是面向对象编程中的一个重要概念，它允许一个对象表现出多种形态。在Java中，多态通过方法重写来实现。当子类重写（覆盖）了父类的方法时，通过父类的引用调用该方法时，实际上会调用子类中的方法。这个过程称为动态绑定或运行时绑定。

继续使用上面的例子，来展示多态是如何工作的：
```java
class Vehicle {
    void makeSound() {
        System.out.println("Some generic sound");
    }
}

class Car extends Vehicle {
    void makeSound() {
        System.out.println("Car sound: Vroom Vroom!");
    }
}

class Motorcycle extends Vehicle {
    void makeSound() {
        System.out.println("Motorcycle sound: Vroom!");
    }
}

public class Main {
    public static void main(String[] args) {
        Vehicle vehicle = new Vehicle();
        Vehicle carAsVehicle = new Car();
        Vehicle motorcycleAsVehicle = new Motorcycle();

        vehicle.makeSound();           // 输出：Some generic sound
        carAsVehicle.makeSound();      // 输出：Car sound: Vroom Vroom!
        motorcycleAsVehicle.makeSound();// 输出：Motorcycle sound: Vroom!
    }
}
```
在上面的例子中，定义了一个Vehicle类和两个子类Car和Motorcycle，它们都重写了父类的makeSound方法。<br />在main方法中，创建了一个Vehicle对象、一个Car对象、一个Motorcycle对象，并使用Vehicle类的引用指向Car和Motorcycle对象。当调用makeSound方法时，由于多态的特性，会根据对象的实际类型来决定调用哪个类的方法。因此，`carAsVehicle.makeSound()`调用的是Car类的方法，`motorcycleAsVehicle.makeSound()`调用的是Motorcycle类的方法。<br />通过多态，可以在父类引用的层面上编写通用的代码，而在运行时根据实际对象的类型来调用适当的方法。这提高了代码的灵活性和可复用性，可以在不修改通用代码的情况下扩展和改变程序的行为。
<a name="P22tr"></a>
## 4、设计决策的原因
为什么Java不允许子类直接覆盖父类的成员变量呢？这涉及到Java语言的一些设计原则和语法约定。
<a name="b5xWc"></a>
### 4.1 保护继承的一致性
Java的设计者认为，直接覆盖父类的成员变量可能会导致继承关系的混乱和不一致性。子类通常被视为是父类的扩展，它们应该增加功能而不是完全改变继承的属性。如果允许子类直接覆盖父类的成员变量，可能会导致代码可读性降低、难以理解的bug以及维护困难等问题。
<a name="SxXyO"></a>
### 4.2 可通过方法实现灵活性
尽管不能直接覆盖父类的成员变量，子类仍然可以通过方法来访问和修改父类的成员变量。这种间接的方式可以实现灵活性，同时还能维护继承关系的一致性。通过在父类中提供合适的getter和setter方法，子类可以在需要时访问或修改父类的成员变量。
```java
class Parent {
    private int parentVariable;

    int getParentVariable() {
        return parentVariable;
    }

    void setParentVariable(int value) {
        parentVariable = value;
    }
}

class Child extends Parent {
    void doSomething() {
        int value = getParentVariable(); // 通过方法访问父类的成员变量
        // ...
    }
}
```
<a name="sr8Jk"></a>
## 小结
在Java中，父类的成员变量不能被子类直接覆盖。这是出于保护继承关系的一致性和灵活性的考虑。子类可以在自身中声明与父类相同名称的成员变量，但实际上这并不是覆盖，而是创建了一个新的成员变量，与父类的成员变量形成隐藏关系。通过提供适当的getter和setter方法，子类可以间接地访问和修改父类的成员变量，同时保持代码的清晰性和可维护性。<br />继承是面向对象编程的重要特性，正确理解和使用继承可以构建更加健壮和灵活的程序。在设计继承关系时，应该根据具体情况考虑继承的合理性和适用性，避免过度使用继承，以保持代码的可维护性和可扩展性。通过合理地使用继承和方法访问父类成员变量，可以构建出更具有复用性和可维护性的面向对象程序。
