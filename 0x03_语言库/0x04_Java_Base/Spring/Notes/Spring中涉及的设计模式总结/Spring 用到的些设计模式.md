JavaSpring<br />关于设计模式，如果使用得当，将会使代码更加简洁，并且更具扩展性。这里主要讲解Spring中如何使用策略模式，工厂方法模式以及`Builder`模式。
<a name="C3oS8"></a>
## 1、策略模式
关于策略模式的使用方式，在Spring中其实比较简单，从本质上讲，策略模式就是一个接口下有多个实现类，而每种实现类会处理某一种情况。<br />以发奖励为例进行讲解，比如在抽奖系统中，有多种奖励方式可供选择，比如积分，虚拟币和现金等。在存储时，必然会使用一个类似于type的字段用于表征这几种发放奖励的，那么这里就可以使用多态的方式进行奖励的发放。比如抽象出一个PrizeSender的接口，其声明如下：
```java
public interface PrizeSender {

    /**
    * 用于判断当前实例是否支持当前奖励的发放
    */
    boolean support(SendPrizeRequest request);

    /**
    * 发放奖励
    */
    void sendPrize(SendPrizeRequest request);

}
```
该接口中主要有两个方法：`support()`和`sendPrize()`，其中`support()`方法主要用于判断各个子类是否支持当前类型数据的处理，而`sendPrize()`则主要是用于进行具体的业务处理的，比如这里奖励的发放。下面就是三种不同类型的奖励发放的具体代码：
```java
// 积分发放
@Component
public class PointSender implements PrizeSender {

    @Override
    public boolean support(SendPrizeRequest request) {
        return request.getPrizeType() == PrizeTypeEnum.POINT;
    }

    @Override
    public void sendPrize(SendPrizeRequest request) {
        System.out.println("发放积分");
    }
}
```
```java
// 虚拟币发放
@Component
public class VirtualCurrencySender implements PrizeSender {

    @Override
    public boolean support(SendPrizeRequest request) {
        return PrizeTypeEnum.VIRTUAL_CURRENCY == request.getPrizeType();
    }

    @Override
    public void sendPrize(SendPrizeRequest request) {
        System.out.println("发放虚拟币");
    }
}
```
```java
// 现金发放
@Component
public class CashSender implements PrizeSender {

    @Override
    public boolean support(SendPrizeRequest request) {
        return PrizeTypeEnum.CASH == request.getPrizeType();
    }

    @Override
    public void sendPrize(SendPrizeRequest request) {
        System.out.println("发放现金");
    }
}
```
这里可以看到，在每种子类型中，只需要在`support()`方法中通过`request`的某个参数来控制当前`request`是否是当前实例能够处理的类型，如果是，则外层的控制逻辑就会将`request`交给当前实例进行处理。关于这个类的设计，有几个点需要注意：

- 使用`@Component`注解对当前类进行标注，将其声明为Spring容器所管理的一个bean；
- 声明一个返回`boolean`值的类似于`support()`的方法，通过这个方法来控制当前实例是否为处理目标request的实例；
- 声明一个类似于`sendPrize()`的方法用于处理业务逻辑，当然根据各个业务的不同声明的方法名肯定是不同的，这里只是一个对统一的业务处理的抽象；
- 无论是`support()`方法还是`sendPrize()`方法，都需要传一个对象进行，而不是简简单单的基本类型的变量，这样做的好处是后续如果要在Request中新增字段，那么就不需要修改接口的定义和已经实现的各个子类的逻辑；
<a name="IlFpT"></a>
## 2、工厂方法模式
上面讲解了如何使用Spring来声明一个策略模式，那么如何为不同的业务逻辑来注入不同的bean呢，或者说外层的控制逻辑是什么样的，这里就可以使用工厂方法模式了。<br />所谓的工厂方法模式，就是定义一个工厂方法，通过传入的参数，返回某个实例，然后通过该实例来处理后续的业务逻辑。一般的，工厂方法的返回值类型是一个接口类型，而选择具体子类实例的逻辑则封装到了工厂方法中了。通过这种方式，来将外层调用逻辑与具体的子类的获取逻辑进行分离。如下图展示了工厂方法模式的一个示意图：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1670044200517-a0e1fe1b-66ad-4122-89e6-9c65941df9d0.jpeg#averageHue=%23fcfcfc&clientId=u8277ea7d-e926-4&from=paste&id=ud0e05de1&originHeight=628&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u850c6785-3f8e-41b0-a949-a5184ecd255&title=)<br />可以看到，工厂方法将具体实例的选择进行了封装，而客户端，也就是调用方只需要调用工厂的具体方法获取到具体的事例即可，而不需要管具体的实例实现是什么。<br />上面讲解了Spring中是如何使用策略模式声明处理逻辑的，而没有讲如何选择具体的策略，这里就可以使用工厂方法模式。<br />如下是声明的一个`PrizeSenderFactory`：
```java
@Component
public class PrizeSenderFactory {

    @Autowired
    private List<PrizeSender> prizeSenders;

    public PrizeSender getPrizeSender(SendPrizeRequest request) {
        for (PrizeSender prizeSender : prizeSenders) {
            if (prizeSender.support(request)) {
                return prizeSender;
            }
        }

        throw new UnsupportedOperationException("unsupported request: " + request);
    }
}
```
这里声明一个了一个工厂方法`getPrizeSender()`，其入参就是`SendPrizeRequest`，而返回值是某个实现了`PrizeSender`接口的实例，可以看到，通过这种方式，将具体的选择方式下移到了具体的子类中的，因为当前实现了`PrizeSender`的bean是否支持当前request的处理，是由具体的子类实现的。<br />在该工厂方法中，也没有任何与具体子类相关的逻辑，也就是说，该类实际上是可以动态检测新加入的子类实例的。这主要是通过Spring的自动注入来实现的，主要是因为这里注入的是一个List，也就是说，如果有新的`PrizeSender`的子类实例，只要其是Spring所管理的，那么都会被注入到这里来。下面就是编写的一段用于测试的代码来模拟调用方的调用：
```java
@Service
public class ApplicationService {

    @Autowired
    private PrizeSenderFactory prizeSenderFactory;

    public void mockedClient() {
        SendPrizeRequest request = new SendPrizeRequest();
        request.setPrizeType(PrizeTypeEnum.POINT);  // 这里的request一般是根据数据库或外部调用来生成的
        PrizeSender prizeSender = prizeSenderFactory.getPrizeSender(request);
        prizeSender.sendPrize(request);
    }
}
```
在客户端代码中，首先通过`PrizeSenderFactory`获取一个`PrizeSender`实例，然后通过其`sendPrize()`方法发放具体的奖励，通过这种方式，将具体的奖励发放逻辑与客户端调用进行了解耦。而且根据前面的讲解，可以知道，如果新增了一种奖励方式，只需要声明一个新的实现了`PrizeSender`的bean即可，而不需要对现有代码进行任何修改。
<a name="Ds5RH"></a>
## 3、`Builder`模式
关于`Builder`模式，使用过lombok的同学肯定会说builder模式非常的简单，只需要在某个bean上使用`@Builder`注解进行声明即可，lombok可以自动将其声明为一个`Builder`的bean。关于这种使用方式，本人不置可否，不过这里主要有两个点需要理解：<br />1、`Builder`模式就其名称而言，是一个构建者，更倾向于将其理解为通过一定的参数，通过一定的业务逻辑来最终生成某个对象。如果仅仅只是使用lombok的这种方式，其本质上也还是创建了一个简单的bean，这个与通过getter和setter方式构建一个bean是没有什么大的区别的；<br />2、在Spring框架中，使用设计模式最大的问题在于如果在各个模式bean中能够注入Spring的bean，如果能够注入，那么将大大的扩展其使用方式。就可以真的实现通过传入的简单的几个参数，然后结合Spring注入的bean进行一定的处理后，以构造出所需要的某个bean。显然，这是lombok所无法实现的；<br />关于Builder模式，可以以前面奖励发放的`SendPrizeRequest`的构造为例进行讲解。在构造request对象的时候，必然是通过前台传如的某些参数来经过一定的处理，最后生成一个request对象。那么就可以使用Builder模式来构建一个`SendPrizeRequest`。<br />这里假设根据前台调用，能够获取到prizeId和userId，那么就可以创建一个如下的`SendPrizeRequest`：
```java
public class SendPrizeRequest {

    private final PrizeTypeEnum prizeType;
    private final int amount;
    private final String userId;
    
    public SendPrizeRequest(PrizeTypeEnum prizeType, int amount, String userId) {
        this.prizeType = prizeType;
        this.amount = amount;
        this.userId = userId;
    }
    
    @Component
    @Scope("prototype")
    public static class Builder {

        @Autowired
        PrizeService prizeService;

        private int prizeId;
        private String userId;

        public Builder prizeId(int prizeId) {
            this.prizeId = prizeId;
            return this;
        }

        public Builder userId(String userId) {
            this.userId = userId;
            return this;
        }

        public SendPrizeRequest build() {
            Prize prize = prizeService.findById(prizeId);
            return new SendPrizeRequest(prize.getPrizeType(), prize.getAmount(), userId);
        }
    }

    public PrizeTypeEnum getPrizeType() {
        return prizeType;
    }
    
    public int getAmount() {
        return amount;
    }
    
    public String getUserId() {
        return userId;
    }
}
```
这里就是使用Spring维护一个`Builder`模式的示例，具体的 维护方式就是在Builder类上使用`@Component`和`@Scope`注解来标注该`Builder`类，这样就可以在`Builder`类中注入所需要的实例来进行一定的业务处理了。关于该模式，这里有几点需要说明：

- 在`Builder`类上必须使用`@Scope`注解来标注该实例为prototype类型，因为很明显，这里的`Builder`实例是有状态的，无法被多线程共享；
- 在`Builder.build()`方法中，可以通过传入的参数和注入的bean来进行一定的业务处理，从而得到构建一个`SendPrizeRequest`所需要的参数；
- `Builder`类必须使用`static`修饰，因为在Java中，如果内部类不用`static`修饰，那么该类的实例必须依赖于外部类的一个实例，而这里本质上是希望通过内部类实例来构建外部类实例，也就是说内部类实例存在的时候，外部类实例是还不存在的，因而这里必须使用static修饰；
- 根据标准的`Builder`模式的使用方式，外部类的各个参数都必须使用final修饰，然后只需要为其声明`getter`方法即可。

上面展示了如何使用Spring的方式来声明一个`Builder`模式的类，那么该如何进行使用呢，如下是一个使用示例：
```java
@Service
public class ApplicationService {

    @Autowired
    private PrizeSenderFactory prizeSenderFactory;

    @Autowired
    private ApplicationContext context;

    public void mockedClient() {
        SendPrizeRequest request = newPrizeSendRequestBuilder()
            .prizeId(1)
            .userId("u4352234")
            .build();

        PrizeSender prizeSender = prizeSenderFactory.getPrizeSender(request);
        prizeSender.sendPrize(request);
    }

    public Builder newPrizeSendRequestBuilder() {
        return context.getBean(Builder.class);
    }
}
```
上述代码中，主要要看一下`newPrizeSendRequestBuilder()`方法，在Spring中，如果一个类是多例类型，也即使用`@Scope("prototype")`进行了标注，那么每次获取该bean的时候就必须使用`ApplicationContext.getBean()`方法获取一个新的实例，至于具体的原因，可查阅相关文档。<br />这里就是通过一个单独的方法来创建一个Builder对象，然后通过流式来为其设置prizeId和userId等参数，最后通过`build()`方法构建得到了一个`SendPrizeRequest`实例，通过该实例来进行后续的奖励发放。
<a name="tS2vP"></a>
## 4、小结
这里主要通过一个奖励发放的示例来对Spring中如何使用工厂方法模式，策略模式和`Builder`模式的方式进行讲解，并且着重强调了实现各个模式时所需要注意的点。
