Java8 Stream
<a name="lI3bh"></a>
### 一、前言
在日常开发中，必不可少的就是树形结构。有的时候可以使用Mybatis的ResultMap进行转化，有的时候需要进行Java进行手动转化为树结构。这里分享一个JDK8新特性Stream来实现List到树形结构的转化！
<a name="j5bpo"></a>
### 二、创建返回前端的实体类VO
```java
import lombok.Data;
import java.util.List;

/**
 * @date 2022/2/28 10:28
 */
@Data
public class NodeVO {
    private String id;
    private String name;
    private String pid;
    private List<NodeVO> children;

    public NodeVO(String id, String name, String pid) {
        this.id = id;
        this.name = name;
        this.pid = pid;
    }
}
```
<a name="VkfK7"></a>
### 三、List转树形方法
```java
public static List<NodeVO> streamToTree(List<NodeVO> treeList, String parentId) {
    List<NodeVO> list = treeList.stream()
            // 过滤父节点
            .filter(parent -> parent.getPid().equals(parentId))
            // 把父节点children递归赋值成为子节点
            .map(child -> {
                child.setChildren(streamToTree(treeList,child.getId()));
                return child;
            }).collect(Collectors.toList());
    return list;
}
```
<a name="nA2uu"></a>
### 四、测试
```java
public static void main(String[] args) {
	NodeVO NodeVO1 = new NodeVO("1","山东省","0");
	NodeVO NodeVO2 = new NodeVO("2","青岛市","1");
	NodeVO NodeVO3 = new NodeVO("3","市北区","2");
	NodeVO NodeVO4 = new NodeVO("4","济南市","1");
	NodeVO NodeVO5 = new NodeVO("5","浙江省","0");
	NodeVO NodeVO6 = new NodeVO("6","杭州市","5");
	NodeVO NodeVO7 = new NodeVO("7","西湖区","6");
	List<NodeVO> list = new ArrayList<>();
	list.add(NodeVO1);
	list.add(NodeVO2);
	list.add(NodeVO3);
	list.add(NodeVO4);
	list.add(NodeVO5);
	list.add(NodeVO6);
	list.add(NodeVO7);
	// 默认父节点id为0
	List<NodeVO> nodeVOList = streamToTree(list,"0");
	System.out.println(JSON.toJSONString(nodeVOList));
}
```
结果展示：
```json
[
	{
		"children": [
			{
				"children": [
					{
						"children": [],
						"id": "3",
						"name": "市北区",
						"pid": "2"
					}
				],
				"id": "2",
				"name": "青岛市",
				"pid": "1"
			},
			{
				"children": [],
				"id": "4",
				"name": "济南市",
				"pid": "1"
			}
		],
		"id": "1",
		"name": "山东省",
		"pid": "0"
	},
	{
		"children": [
			{
				"children": [
					{
						"children": [],
						"id": "7",
						"name": "西湖区",
						"pid": "6"
					}
				],
				"id": "6",
				"name": "杭州市",
				"pid": "5"
			}
		],
		"id": "5",
		"name": "浙江省",
		"pid": "0"
	}
]
```
