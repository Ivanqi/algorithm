from numpy import *
from numpy import linalg as LA
from sklearn.preprocessing import scale

def SVD_Solve(X):
    # 计算U矩阵 = XX'的特征矩阵
    U = X.dot(X.T)
    U_feature, U_vector = LA.eig(U)

    # 计算V矩阵 = X'X的特征矩阵
    V = X.T.dot(X)
    V_feature, V_vector = LA.eig(V)

    # 计算西格玛对角矩阵 = U'XV
    XGM = (U_vector.T.dot(X)).dot(V_vector)

    return U_vector, XGM, V_vector.T

# 测试
X = mat([
    [1, 1, 1, 0, 0], 
    [2, 2, 2, 0, 0], 
    [1, 1, 1, 0, 0], 
    [5, 5, 5, 0, 0],
    [0, 0, 0, 2, 2],
    [0, 0, 0, 3, 3],
    [0, 0, 0, 1, 1]
])

U, XGM, V = SVD_Solve(X)
print("左奇异向量\n", "=" * 10, "\n", U)
print("奇异值矩阵\n", "=" * 10, "\n", XGM)
print("右奇异向量\n", "=" * 10, "\n", V)