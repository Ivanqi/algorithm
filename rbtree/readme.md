#### 红黑树
- ![avatar](images/../../images/rbtree_1.png)
- 特征
  - 根节点是黑色的
  - 每个叶子节点都是黑色的空节点(NIL), 也就是说，叶子节点不存储数据
  - 任何相邻的节点都不能同时为红色，也就是说，红色节点是被黑色节点隔开的
  - 每个节点，从该节点到达其可达叶子节点的所有路径，都包含相同数目的黑色节点

#### 为什么说红黑树是“近似平衡”的？
- `平衡`的意思可以等价为性能不退化
- `近似平衡`就是等价为性能不会退化太厉害

#### 资料参考
- [红黑树(四)之 C++的实现](https://www.cnblogs.com/skywang12345/p/3624291.html)
- [五分钟搞定什么是红黑树](http://www.360doc.com/content/18/0904/19/25944647_783893127.shtml)
- [[Data Structure] 数据结构中各种树](https://www.cnblogs.com/maybe2030/p/4732377.html#_label4)
- [教你初步了解红黑树](https://blog.csdn.net/v_JULY_v/article/details/6105630)
- [经典算法研究系列：五、红黑树算法的实现与剖析](https://blog.csdn.net/v_JULY_v/article/details/6109153)
- [一步一图一代码，一定要让你真正彻底明白红黑树](https://blog.csdn.net/v_JULY_v/article/details/6124989)
- [清晰理解红黑树的演变---红黑的含义](https://www.cnblogs.com/tiancai/p/9072813.html)