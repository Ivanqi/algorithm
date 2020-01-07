#### BF 算法
- BF 算法中的BF 是Brute Force的缩写，中文叫做暴力匹配法，也叫朴素匹配算法
- ![avatar](images/../../images/string_matching_1.png)
- 作为最简单，最暴力的字符串匹配算法，BF算法的思想可以用一句话来概括，那么就是`在主串中，检查起始位置分别是0, 1, 2 ... n - m 且长度为m的n - m + 1个子串，看有没有跟模式串匹配` 
- 时间复杂度
  - O(n * m)

#### 参考资料
- [BF算法（串模式匹配算法）C语言详解](http://data.biancheng.net/view/179.html)
- [字符串匹配之RK算法——学习笔记](https://blog.csdn.net/jjwwwww/article/details/81676479)
- [字符串的模式匹配：RK算法](https://blog.csdn.net/xlxxcc/article/details/64125265)