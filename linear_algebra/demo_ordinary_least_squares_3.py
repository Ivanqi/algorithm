from numpy import *

x = mat([[0, 1], [1, -1], [2, 8]])
y = mat([[1.4], [-0.48], [13.2]])

# 分别求出矩阵X', X'X, (X'X)的逆
# 注意, 这里的I表示逆矩阵而不是单位矩阵
print("X矩阵的转置X': \n", x.transpose())
print("\nX'点乘X: \n", x.transpose().dot(x))
print("\nX'X矩阵的逆\n", (x.transpose().dot(x)).I)

print("\nX'X矩阵的逆点乘X'", (x.transpose().dot(x)).I.dot(x.transpose()))
print("\n系数矩阵B:\n", (x.transpose().dot(x)).I.dot(x.transpose()).dot(y))