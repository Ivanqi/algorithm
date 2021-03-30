import numpy as np

# 设置确定随机跳转概率alpha, 网页结点数
alpha = 0.9
N = 5

# 初始化随机跳转概率的矩阵
# 创建一个由常数填充的数组,第一个参数是数组的形状，第二个参数是数组中填充的常数
jump = np.full([2, 1], [[alpha], [1 - alpha]], dtype=float)
# print(jump)

# 邻接矩阵的构建
adj = np.full([N, N], [[0, 0, 1, 0, 0], [1, 0, 1, 0, 0], [1, 0, 0, 0, 0], [0, 0, 0, 0, 0], [0, 1, 0, 0, 1]], dtype=float)

# 对邻接矩阵进行归一化
row_sums = adj.sum(axis=1)      # 对每一行求和
row_sums[row_sums == 0] = 0.1   # 防止由于分母出现0而导致的Nan
adj = adj / row_sums[:, np.newaxis] # 除以每行之和和归一化

# 初始的PageRank，通常是设置所有值为1.0
pr = np.full([1, N], 1, dtype=float)


# 采样迭代方式，判断前后两次PageRank向量的差的平均值是否小于指定精度
pricision = 1e-9 # 设置计算精度
last_pr = None
i = 0

while True:
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
    
    if last_pr is not None:
        diff = np.average(np.absolute(pr - last_pr))
        if (diff <= pricision):
            break
    
    last_pr = pr.copy()
    i += 1

