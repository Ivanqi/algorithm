from numpy import *

# 加入截距进行计算
x = mat([[1, 0, 1], [1, 1, -1], [1, 2, 8]])
y = mat([[1.4],[-0.48],[13.2]])

print("\n系数矩阵B：\n", (x.transpose().dot(x)).I.dot(x.transpose()).dot(y))
