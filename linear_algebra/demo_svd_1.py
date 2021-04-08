import numpy as np
from numpy import linalg as la

# 文档集合 文档和词条关系矩阵. 行表示文档，列表示词条
x = np.mat([
    [1, 1, 1, 0, 0], 
    [2, 2, 2, 0, 0], 
    [1, 1, 1, 0, 0], 
    [5, 5, 5, 0, 0],
    [0, 0, 0, 2, 2],
    [0, 0, 0, 3, 3],
    [0, 0, 0, 1, 1]
])

U, sigma, VT = la.svd(x)
print(U, "\n")
print(sigma, "\n")
print(VT, "\n")

S = np.zeros((7, 5))    # 奇异矩阵
for i in range(len(sigma)):
    S[i, i] = sigma[i]

print(" 与矩阵 x 一致？ \n", U.dot(S).dot(VT))
