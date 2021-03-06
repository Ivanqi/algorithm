#### 什么是哈希算法
- 不管是“散列”还是“哈希”，这都是中文翻译差别，英文其实就是"Hash"
- 哈希算法定义
  - 将任意长度的二进制值串映射为固定长度的二进制值串，这个映射的规则就是哈希算法
  - 而原始数据映射之后得到的二进制值串就是哈希值
#### 如何设计哈希算法
- 从哈希值不能反向推导出原始数据(所以哈希算法也叫单向哈希算法)
- 对输入数据非常敏感，哪怕原始数据只修改了一个Bit，最后得到的哈希值也不打相同
- 散列冲突的概率要很小，对于不同的原始数据，哈希值相同的概率非常小
- 哈希算法的执行 效率要尽量高效，针对较长的文本，又能快速地计算出哈希值
#### 应用场景
- 安全加密
  - MD5(MD5 Message-Digest Algorithm， MD5消息摘要算法)
  - SHA(Secure Hash Algorithm, 安全散列算法)
  - DES(Data Encryption Standard, 数据加密标准)
  - AES(Advanced Encryption Standard, 高级加密标准)
  - 鸽巢原理(抽屉原理)
    - 如果有10个鸽巢，有11只鸽子，那肯定有1个鸽巢中的鸽子数量多于1个
    - 换句话说，肯定有2只鸽子在1个鸽巢里
- 唯一标识
  - 例子
    - 搜索一张几十KB 或 几MB的图片。如果把图片转化二进制，二进制会非常长
    - 我们可以从图片的二进制码串取100个字节，从中间取100字节，从最后再取100个字节，然后将300个字节放一起，通过哈希算法（如MD5）得到一个哈希字符串,用它作为图片唯一标识
- 数据校验
  - 例子   
    - BT下载原来是基于P2P协议的
    - 我们从多个机器上并行下载一个2GB的电影，这个电影文件可能会被分割成很多文件快(100快，每块20MB)。等文件快下载完后，再组装成一个完整的电影文件就可以了
    - 通过哈希算法。对100个文件快分别取哈希值，并且保存在种子文件中。当文件块下载完后，通过相同的哈希算法，对下载好的文件块逐一求哈希值，然后跟种子的哈希值对比。如果不同，说明这个文件快不完整或者被篡改了，需要重新下载
- 散列函数
- 负载均衡
  - 如何实现一个会话(session sticky) 的算法？
  - 方法一 - 映射表
    - 最直接的方法就是，维护一张映射关系表，这张表的内容是客户端IP地址或者会话ID与服务器编号的映射
    - 客户端发出的每次请求,都要先在映射表中查找应该路由到服务器编号，然后再请求编号对应的服务器
    - 缺点
      - 如何客户端很多，映射表可能会很大，比较浪费内存空间
    - 客户端下线，上线，服务器扩容，缩容都会导致映射失败，这样维护映射表的成本就会很高
  - 方法二 - 哈希算法
    - 可以通过哈希算法，对客户端IP地址或者会话ID计算哈希值
    - 将取得的哈希值与服务器列表的大小进行取模运算，最终得到的值就是应该被路由到的服务器编号
    - 这样，我们就可以把同一个IP过来的所有请求，都路由到同一个后端服务器上
- 数据分片
  - 如何统计“搜索关键词”出现的次数
    - 难点
      - 搜索日志过大，没办法放到一台机器的内存
      - 如何只用一台机器来处理这么巨大的数据，处理时间会很纯粹
    - 解决
      - 先把数据进行分片，然后采用多台机器处理的方法，来提高处理速度
      - 所有，使用n台机器并行处理，从搜索记录的日志文件中，依次读出每个搜索关键词，并且通过哈希函数计算哈希值，然后再跟n取模，最终得到的值，就是应该被分配到的机器编号
      - 这样，哈希值相同的搜索关键词就被分配到同一个机器上。也就说，同一个搜索关键词会被分配到同一个机器上。每个机器会分别计算关键词出现的次数，最后合并起来就是最终的结果
  - 如何快速判断图片是否在图库中？
    - 方法
      - 对数据分片，然后采用多机处理
      - 准备n台机器，让每台机器只维护某一部分的图片对应的散列表
      - 每次从图库中读取一个图片，计算唯一标识，然后与机器个数n求余取模，得到的值就对应要分配的机器编号，然后将这个图片的唯一标识和图片路径发往对应的机器构建散列表
    - 问题 - 计算1亿图片构建散列表需要多少台机器？
      - 构建散列表
        - 散列表中每个数据单元包含两个信息，哈希值和图片的路径
        - 假设通过MD5来计算哈希值，那长度就是128比特，也就是16字节
        - 文件路径长度上限是256字节，可以假设平均长度是128字节
        - 所以，散列表中每个数据单元就占用152字节(估算)
      - 机器数量
        - 假设一台机器的内存大小为2GB，散列表的转载因子为0.75,那一台机器可以给大约1000万(2GB*0.75/152)张图片构建散列表
        - 所有，要对1亿张图片构建索引，需要大约十几台机器。
    - 实际上，针对这种海量数据的处理问题，我们都可以采用多机分布式处理。借助这种分片思路，可以突破单机内存，CPU等资源的限制
- 分布式存储
  - 面对海量的数据，海量的用户。为了提高数据的读取，写入能力。一般都是采用分布式的方法。比如，分布式缓存。于是，我们就需要考虑如何把数据存储到多台机器上
  - 可以借助分片的思想，通过哈希算法对数据取哈希值，然后对机器个数取模，这个最终值就是存储存储的缓存机器编号
  - 问题
    - 随着数据增多，原来的10个机器已经无法承受，就需要扩容。如果扩容到11台。但是问题又来了？
    - 原来的数据通过与10取模来的，现在变成了要与11取模，肯定会造成冲突，如果不想冲突，就必须把所有重新取模，计算新的哈希值。这样可能会造成缓存雪崩
  - 方法 - 一致性Hash算法
    - 假设有k个机器，数据的哈希值的范围是[0, MAX]
    - 可以将整个范围划分为m个小区间(m 远大于 k),每个机器负责 m / k个小区间
    - 当有新机器加入的时候，我们就将某个小区间的数据，从原来的机器中搬移到新的机器中去
    - 这样，既不用全部重新哈希，搬移数据，也保持了各个机器上数据数量的均衡

#### 参考资料
- [白话解析：一致性哈希算法 consisten hashing](http://www.zsythink.net/archives/1182)
- [漫画：什么是一致性哈希？](https://mp.weixin.qq.com/s/yimfkNYF_tIJJqUIzV7TFA)
- [一致性哈希算法](https://github.com/julycoding/The-Art-Of-Programming-By-July/blob/master/ebook/zh/%E4%B8%80%E8%87%B4%E6%80%A7%E5%93%88%E5%B8%8C%E7%AE%97%E6%B3%95.md)
- [sha1 加密算法的 C++ 语言实现](https://blog.sbw.so/u/cpp-implements-class-for-sha1-encryption-algorithm.html)
- [SHA256算法原理详解](https://blog.csdn.net/u011583927/article/details/80905740)
- [MD5算法解析](https://www.cnblogs.com/chars/p/4983291.html)
- [加密算法原理分析(MD5、SHA-256)](https://juejin.im/post/5ce6b828f265da1bba58dd9e)
- [DES算法解析](https://www.cnblogs.com/chars/p/4984269.html)