#### B+树特点
- 每个节点中子节点的个数不超过m，也不能小于m/2
- 根节点的子节点个数可以不超过m/2
- m叉树只存储索引，并不真正存储数据，这个类似于跳表
- 通过链表将叶子节点串联在一起，这样可以方便查找
- 一般情况，根节点会被存储在内存中，其他节点存储在磁盘中

#### B树和B+树的区别
- B+树中的节点不存储数据，只是索引，而B树中的节点存储数据
- B树的叶子节点并不需要链表来串联

#### 参考资料
- [B+树怎么在磁盘存储？](https://www.zhihu.com/question/269033066)
- [B+树总结](https://www.jianshu.com/p/71700a464e97)
- [B树和B+树的插入、删除图文详解](https://www.cnblogs.com/nullzx/p/8729425.html)
- [B+树在磁盘存储中的应用](https://www.cnblogs.com/nullzx/p/8978177.html)
- [Mysql索引机制(B+Tree)](https://www.cnblogs.com/wuzhenzhao/p/10341114.html)