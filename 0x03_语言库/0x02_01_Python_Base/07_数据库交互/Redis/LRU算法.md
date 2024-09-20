# LRU算法

> https://leetcode.cn/problems/lru-cache/

当缓存数据达到预设的上限后，就会优先淘汰近期最少使用的缓存对象，原理是使用双端队列存放元素，哈希表判断数据是否存在



## 思路

- 维护一个双端队列用于存放元素，哈希表判断元素是否存在，越接近队列表尾部的元素表示越少被使用到
- 插入一个元素时，如果元素已存在则将其移动到队列头部，并更新哈希表Key所对应的Value值，如果不存在，则：

- - 如果缓存容量已达到最大值，则将队列尾部节点删除掉，将新的数据放入队列头部
  - 如果缓存容量未达到最大值，则直接将新的数据放入队列头部

- 查询一个数据时，我们可以使用哈希表来记录每个Key所对应的位置，时间复杂度为O(1)。

~~~ python
# 自建双端列表、哈希表
class LRUCache:
    def __init__(self, capacity):
        self.dicts = {}
        self.queue = []
        # capacity是总大容量
        self.capacity = capacity

    def get(self, key):
        if key in self.dicts:
            # 将其移动到双向队列的头部，最后返回该节点的值
            self.queue.remove(key)
            self.queue.append(key)
            return self.dicts[key]
        else:
            return -1

    def put(self, key, value):
        # 判断再次插入缓存会不会满
        if len(self.dicts) < self.capacity:
            if key in self.dicts:
                # 将其移动到双向队列的头部
                self.dicts[key] = value
                self.queue.remove(key)
                self.queue.append(key)
            else:
                # 直接放到头部
                self.dicts[key] = value
                self.queue.append(key)
        # 如果满了就可能就需要干掉尾部的一些元素
        else:
            if key in self.dicts:
                # 将其移动到双向队列的头部
                self.dicts[key] = value
                self.queue.remove(key)
                self.queue.append(key)
            else:
                # 缓存满了就干掉尾部的一些元素，再放到头部
                self.dicts.pop(self.queue[0])
                self.queue.remove(self.queue[0])
                self.dicts[key] = value
                self.queue.append(key)
~~~



