from numpy import *

x = mat([[1, 3, -7], [2, 5, 4], [-3, -7, -2], [1, 4, -12]])
y = mat([[-7.5], [5.2], [-7.5], [-15]])

print("系数矩阵\n")
b = (x.transpose().dot(x)).I.dot(x.transpose()).dot(y)
print(b)
print("\n得到y的观察值矩阵\n")
y1 = x.dot(b)
print(y1)

