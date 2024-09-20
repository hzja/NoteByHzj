Java Mybatis<br />resultMap 是 Mybatis 最强大的元素，它可以将查询到的复杂数据（比如查询到几个表中数据）映射到一个结果集当中。<br />**resultMap 包含的元素：**
```xml
<!--column不做限制，可以为任意表的字段，而property须为type定义的pojo属性-->
<resultMap id="唯一的标识"type="映射的pojo对象">
  <id column="表的主键字段，或者可以为查询语句中的别名字段" jdbcType="字段类型" property="映射pojo对象的主键属性" />
  <result column="表的一个字段（可以为任意表的一个字段）" jdbcType="字段类型" property="映射到pojo对象的一个属性（须为type定义的pojo对象中的一个属性）"/>
  <association property="pojo的一个对象属性" javaType="pojo关联的pojo对象">
    <id column="关联pojo对象对应表的主键字段" jdbcType="字段类型" property="关联pojo对象的主席属性"/>
    <result  column="任意表的字段" jdbcType="字段类型" property="关联pojo对象的属性"/>
  </association>
  <!-- 集合中的property须为oftype定义的pojo对象的属性-->
  <collection property="pojo的集合属性" ofType="集合中的pojo对象">
    <id column="集合中pojo对象对应的表的主键字段" jdbcType="字段类型" property="集合中pojo对象的主键属性" />
    <result column="可以为任意表的字段" jdbcType="字段类型" property="集合中的pojo对象的属性" />  
  </collection>
</resultMap>
```
如果 collection 标签是使用嵌套查询，格式如下：
```xml
<collection column="传递给嵌套查询语句的字段参数" property="pojo对象中集合属性" ofType="集合属性中的pojo对象"select="嵌套的查询语句" > 
</collection>
```
注意：`<collection>`标签中的`column`：要传递给 `select` 查询语句的参数，如果传递多个参数，格式为`column= "{参数名1=表字段1,参数名2=表字段2}"` ；
<a name="LV2eP"></a>
### 以下以实例介绍 resultMap 的用法：
<a name="sOFsr"></a>
#### 一、简单需求
一个商品的结果映射；

- 创建商品 pojo 对象
```java
public class TShopSku  {
  /**
   * 主键ID
   */
  private Long id;

  /**
   * 商品名
   */
  private String skuName;

  /**
   * 分类ID
   */
  private Long categoryId;

  /**
   * 主键ID
   * @return ID 
   */
  public Long getId() {
    returnid;
  }

  /**
   * 主键ID,
   * @param id 
   */
  public void setId(Long id) {
    this.id = id;
  }

  /**
   * 商品名
   * @return SKU_NAME 商品名
   */
  public String getSkuName() {
    return skuName;
  }

  /**
   * 商品名
   * @param skuName 商品名
   */
  public void setSkuName(String skuName) {
    this.skuName = skuName == null ? null : skuName.trim();
  }

  /**
   * 分类ID
   * @return CATEGORY_ID 分类ID
   */
  public Long getCategoryId() {
    return categoryId;
  }

  /**
   * 分类ID
   * @param categoryId 分类ID
   */
  public void setCategoryId(Long categoryId) {
    this.categoryId = categoryId;
  }

}
```
对应的 resultMap：
```xml
<resultMap id="BaseResultMap"type="com.shop.entity.TShopSku">
    <id column="ID" jdbcType="BIGINT" property="id" />
    <result column="SKU_NAME" jdbcType="VARCHAR" property="skuName" />
    <result column="CATEGORY_ID" jdbcType="BIGINT" property="categoryId" />
</resultMap> 
```
<a name="GFMM9"></a>
#### 二、商品pojo类添加属性集合
一个商品会有一些属性，现在需要将查询出的商品属性添加到商品对象中，首先需要在原商品 pojo 类的基础上中添加属性的集合：
```java
/**
 * 属性集合
 */
private List<TShopAttribute> attributes;

/**
 * 获得属性集合
 */
public List<TShopAttribute> getAttributes() {
  return attributes;
}

/**
 * 设置属性集合
 * @param attributes
 */
public void setAttributes(List<TShopAttribute> attributes) {
  this.attributes = attributes;
}
```
<a name="lE4Fp"></a>
### 将 Collection 标签添加到 resultMap 中
这里有两种方式：
<a name="v3wIP"></a>
#### 1、嵌套结果
对应的 resultMap
```xml
<resultMap id="BasePlusResultMap"type="com.shop.entity.TShopSku">
    <id column="ID" jdbcType="BIGINT" property="id" />
    <result column="SKU_NAME" jdbcType="VARCHAR" property="skuName" />
    <result column="CATEGORY_ID" jdbcType="BIGINT" property="categoryId" />
    <collection property="attributes" ofType="com.shop.entity.TShopAttribute" >
        <id column="CATEGORY_ID" jdbcType="BIGINT" property="id" />
        <result column="ATTRIBUTE_NAME" jdbcType="VARCHAR" property="attributeName" />
    </collection>
</resultMap>
```
查询语句
```xml
<select id="getById"  resultMap="basePlusResultMap">
    select
          s.ID, s.SKU_NAME, s.CATEGORY_ID, a.ID, a.ATTRIBUTE_NAME
    from
        t_shop_sku s, t_shop_attribute a 
    where
         s.ID = a.SKU_ID
    and
         s.ID = #{id,jdbcType =BIGINT};
</select>
```
<a name="lwH84"></a>
#### 2、关联的嵌套查询（在collection中添加`select`属性）
商品结果集映射 resultMap
```xml
<resultMap id="BasePlusResultMap"type="com.shop.entity.TShopSku">
    <id column="ID" jdbcType="BIGINT" property="id" />
    <result column="SKU_NAME" jdbcType="VARCHAR" property="skuName" />
    <result column="CATEGORY_ID" jdbcType="BIGINT" property="categoryId" />
    <collection column="{skuId=ID}" property="attributes" ofType="com.shop.entity.TShopAttribute"select="getAttribute" > 
    </collection>
</resultMap>
```
collection 的 select 会执行下面的查询属性语句：
```xml
<select id="getAttribute"  resultMap="AttributeResultMap">
    select
        a.ID, s.ATTRIBUTE_NAME
    from
        t_shop_attribute a
    where  
          a.ID = #{skuId,jdbcType =BIGINT};
</select>
```
属性结果集映射
```xml
<resultMap id="AttributeResultMap"type="com.shop.entity.TShopAttribute">
    <id column="ID" jdbcType="BIGINT" property="id" />
    <result column="ATTRIBUTE_NAME" jdbcType="VARCHAR" property="attributeName" />
</resultMap>
```
BasePlusResultMap 包含了属性查询语句的 Collection<br />所以通过下面的查询商品语句就可获得商品以及其包含的属性集合：
```xml
<select id="getById"  resultMap="BasePlusResultMap">
    select
           s.ID,s.SKU_NAME,s.CATEGORY_ID
    from
           t_shop_sku s
    where  
         s.ID = #{id,jdbcType =BIGINT};
</select>
```
