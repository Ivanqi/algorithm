import numpy as np

'''
算法原理
    PageRank算法分2步
        给每个网页一个PR值(下面用PR值指代PageRank值)
        通过(投票)算法不断迭代，直到平稳分布
如果一个网页被很多其他网页链接到的话说明这个网页比较重要，也就是PageRank值会相对较高
如果一个PageRank值很高的网页链接到一个其他网页，那么被链接到的网页的PageRank值会相应地因此提高
'''

# 设置确定随机跳转概率alpha, 网页结点数
alpha = 0.9
N = 5

# 初始化随机跳转概率的矩阵
# 创建一个由常数填充的数组,第一个参数是数组的形状，第二个参数是数组中填充的常数
jump = np.full([2, 1], [[alpha], [1 - alpha]], dtype=float)
# print(jump)

# 邻接矩阵的构建
'''
邻接矩阵的行表示每个节点出边，列表示每个节点入边
做归一化是为了平均分配权重，矩阵的乘法恰好按照入边累加pr值
随机跳转还是线性关系，依然可以用矩阵处理，这里用到矩阵分块思想
'''
adj = np.full([N, N], [[0, 0, 1, 0, 0], [1, 0, 1, 0, 0], [1, 0, 0, 0, 0], [0, 0, 0, 0, 0], [0, 1, 0, 0, 1]], dtype=float)

# 对邻接矩阵进行归一化
row_sums = adj.sum(axis=1)      # 对每一行求和
row_sums[row_sums == 0] = 0.1   # 防止由于分母出现0而导致的Nan
adj = adj / row_sums[:, np.newaxis] # 除以每行之和和归一化

# 初始的PageRank，通常是设置所有值为1.0
pr = np.full([1, N], 1, dtype=float)

# PageRank算法本身是采样迭代方法进行的，当最终的取值趋于稳定后结束
for i in range(0, 20):
    # 进行点乘，计算Σ(PR(pj)/L(pj))
    pr = np.dot(pr, adj)
    
    # 转置保存Σ(PR(pj)/L(pj))结果的矩阵，并增加长度N的列向量，其中每个元素的值为1/N，便于下一步的点乘
    pr_jump = np.full([N, 2], [[0, 1 / N]])
    pr_jump[:, :-1] = pr.transpose()

    # 进行点乘，计算α(Σ(PR(pj)/L(pj))) + (1-α)/N)
    pr = np.dot(pr_jump, jump)

    # 归一化PageRank得分
    pr = pr.transpose()
    pr = pr / pr.sum()

    print("round", i + 1, pr)

