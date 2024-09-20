Java Spring 事务失效
<a name="HM1W6"></a>
## Spring事务不生效总览
简单来说，Spring事务会在几种特定的场景下失效，如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1640318709026-3e68f909-06b0-45b7-929a-927fb607174c.jpeg)
<a name="xf86I"></a>
## 数据库不支持事务
Spring事务生效的前提是所连接的数据库要支持事务，如果底层的数据库都不支持事务，则Spring的事务肯定会失效。例如，如果使用的数据库为MySQL，并且选用了MyISAM存储引擎，则Spring的事务就会失效。
<a name="NyAeY"></a>
## 事务方法未被Spring管理
如果事务方法所在的类没有加载到Spring IOC容器中，也就是说，事务方法所在的类没有被Spring管理，则Spring事务会失效，示例如下。
```java
public class ProductService {
    @Autowired
    private ProductDao productDao;

    @Transactional(propagation = Propagation.REQUIRES_NEW)
    public void updateProductStockCountById(Integer stockCount, Long id){
        productDao.updateProductStockCountById(stockCount, id);
    }
}
```
ProductService类上没有标注`@Service`注解，Product的实例没有加载到Spring IOC容器中，就会造成`updateProductStockCountById()`方法的事务在Spring中失效。
<a name="uvoVe"></a>
## 方法没有被`public`修饰
如果事务所在的方法没有被`public`修饰，此时Spring的事务会失效，例如，如下代码所示。
```java
@Service
public class ProductService {
    @Autowired
    private ProductDao productDao;

    @Transactional(propagation = Propagation.REQUIRES_NEW)
    private void updateProductStockCountById(Integer stockCount, Long id){
        productDao.updateProductStockCountById(stockCount, id);
    }
}
```
虽然ProductService上标注了`@Service`注解，同时`updateProductStockCountById()`方法上标注了`@Transactional(propagation = Propagation.REQUIRES_NEW)`注解。<br />但是，由于`updateProductStockCountById()`方法为内部的私有方法（使用`private`修饰），那么此时`updateProductStockCountById()`方法的事务在Spring中会失效。
<a name="GSxEX"></a>
## 同一类中方法调用
如果同一个类中的两个方法分别为A和B，方法A上没有添加事务注解，方法B上添加了`@Transactional`事务注解，方法A调用方法B，则方法B的事务会失效。例如，如下代码所示。
```java
@Service
public class OrderService {

    @Autowired
    private OrderDao orderDao;

    @Autowired
    private ProductDao productDao;

    public void submitOrder(){
        //生成订单
        Order order = new Order();
        long number = Math.abs(new Random().nextInt(500));
        order.setId(number);
        order.setOrderNo("order_" + number);
        orderDao.saveOrder(order);
        //减库存
        this.updateProductStockCountById(1, 1L);
    }

    @Transactional(propagation = Propagation.REQUIRES_NEW)
    public void updateProductStockCountById(Integer stockCount, Long id){
        productDao.updateProductStockCountById(stockCount, id);
    }
}
```
`submitOrder()`方法和`updateProductStockCountById()`方法都在OrderService类中，`submitOrder()`方法上没有标注事务注解，`updateProductStockCountById()`方法上标注了事务注解，`submitOrder()`方法调用了`updateProductStockCountById()`方法，此时，`updateProductStockCountById()`方法的事务在Spring中会失效。
<a name="LdqYO"></a>
## 未配置事务管理器
如果在项目中没有配置Spring的事务管理器，即使使用了Spring的事务管理功能，Spring的事务也不会生效。<br />例如，没有在项目的配置类中配置如下代码。
```java
@Bean
public PlatformTransactionManager transactionManager(DataSource dataSource) {
    return new DataSourceTransactionManager(dataSource);
}
```
此时，Spring的事务就会失效。
<a name="I2569"></a>
## 方法的事务传播类型不支持事务
如果内部方法的事务传播类型为不支持事务的传播类型，则内部方法的事务在Spring中会失效。<br />例如，如下代码所示。
```java
@Service
public class OrderService {
    @Autowired
    private OrderDao orderDao;
    @Autowired
    private ProductDao productDao;

    @Transactional(propagation = Propagation.REQUIRED)
    public void submitOrder(){
        //生成订单
        Order order = new Order();
        long number = Math.abs(new Random().nextInt(500));
        order.setId(number);
        order.setOrderNo("order_" + number);
        orderDao.saveOrder(order);
        //减库存
        this.updateProductStockCountById(1, 1L);
    }

    @Transactional(propagation = Propagation.NOT_SUPPORTED)
    public void updateProductStockCountById(Integer stockCount, Long id){
        productDao.updateProductStockCountById(stockCount, id);
    }
}
```
由于`updateProductStockCountById()`方法的事务传播类型为`NOT_SUPPORTED`，不支持事务，则`updateProductStockCountById()`方法的事务会在Spring中失效。
<a name="KbifP"></a>
## 不正确的捕获异常
不正确的捕获异常也会导致Spring的事务失效，示例如下。
```java
@Service
public class OrderService {
    @Autowired
    private OrderDao orderDao;
    @Autowired
    private ProductDao productDao;


    @Transactional(propagation = Propagation.REQUIRED)
    public void submitOrder(){
        //生成订单
        Order order = new Order();
        long number = Math.abs(new Random().nextInt(500));
        order.setId(number);
        order.setOrderNo("order_" + number);
        orderDao.saveOrder(order);
        //减库存
        this.updateProductStockCountById(1, 1L);
    }

    @Transactional(propagation = Propagation.REQUIRED)
    public void updateProductStockCountById(Integer stockCount, Long id){
        try{
            productDao.updateProductStockCountById(stockCount, id);
            int i = 1 / 0;
        }catch(Exception e){
            logger.error("扣减库存异常:", e.getMesaage());
        }
    }
}
```
`updateProductStockCountById()`方法中使用try-catch代码块捕获了异常，即使`updateProductStockCountById()`方法内部会抛出异常，但也会被catch代码块捕获到，此时`updateProductStockCountById()`方法的事务会提交而不会回滚，并且`submitOrder()`方法的事务会提交而不会回滚，这就造成了Spring事务的回滚失效问题。
<a name="frYlb"></a>
## 错误的标注异常类型
如果在`@Transactional`注解中标注了错误的异常类型，则Spring事务的回滚会失效，示例如下。
```java
@Transactional(propagation = Propagation.REQUIRED)
public void updateProductStockCountById(Integer stockCount, Long id){
    try{
        productDao.updateProductStockCountById(stockCount, id);
    }catch(Exception e){
        logger.error("扣减库存异常:", e.getMesaage());
        throw new Exception("扣减库存异常");
    }
}
```
在`updateProductStockCountById()`方法中捕获了异常，并且在异常中抛出了Exception类型的异常，此时，`updateProductStockCountById()`方法事务的回滚会失效。<br />为何会失效呢？这是因为Spring中对于默认回滚的事务异常类型为`RuntimeException`，上述代码抛出的是Exception异常。<br />默认情况下，Spring事务中无法捕获到Exception异常，所以此时`updateProductStockCountById()`方法事务的回滚会失效。<br />此时可以手动指定`updateProductStockCountById()`方法标注的事务异常类型，如下所示。
```java
@Transactional(propagation = Propagation.REQUIRED,rollbackFor = Exception.class)
```
这里，需要注意的是：Spring事务注解`@Transactional`中的`rollbackFor`属性可以指定 Throwable 异常类及其子类。
