import numpy as np
# 矩阵X
X = np.array([[1, 3, -7]
            ,[2, 5, 4]
            ,[-3, -7, -2]
            ,[1, 4, -12]
             ]
            )
# X的转置X'
TRANS_X = X.T

# 目标值向量Y
Y = np.array([[-7.5, 5.2, -7.5, -15]]).T

# X'X
B = TRANS_X.dot(X)

# (X'X)^-1
B = np.matrix(B).I

# [(X'X)^-1]X'
B = B.dot(TRANS_X)

# [(XX')^-1]X'Y
B = B.dot(Y)

print('系数矩阵：')
print(B)

print('验证：')
print(X.dot(B))