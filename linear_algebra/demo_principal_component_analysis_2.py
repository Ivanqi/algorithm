from numpy import *
from numpy import linalg as LA
from sklearn.preprocessing import scale

# 原始数据，包含了3个样本和3个特征，每一行表示一个样本，每一列表示一维特征
x = mat([[1, 3, -7], [2, 5, -14], [-3, -7, 2]])

# 矩阵按列进行标准化
x_s = scale(x, with_mean=True, with_std=True, axis=0)
print("标准化后的矩阵: ", x_s)

# 计算协方差矩阵，注意这里需要先进行转置，因为这里的函数是看行与行之间的协方差
x_cov = cov(x_s.transpose())
# 输出协方差矩阵
print("协方差矩阵:\n", x_cov, "\n")

# 求协方差矩阵的特征值和特征向量
eigVals, eigVects = LA.eig(x_cov)
print("协方差矩阵的特征值: ", eigVals)
print("协方差的特征向量(主成分): \n", eigVects, "\n")

# 找到最大的特征值，及其对应的特征向量
max_eigVal = -1
max_eigVal_index = -1

for i in range(0, eigVals.size):
    if (eigVals[i] > max_eigVal):
        max_eigVal = eigVals[i]
        max_eigVal_index = i
    
    eigVect_with_max_eigVal = eigVects[:,max_eigVal_index]

# 输出最大的特征值及其对应的特征向量，也就是第一个主成分
print("最大的特征值: ", max_eigVal)
print("最大特征值所对应的特征向量: ", eigVect_with_max_eigVal)

# 输出变换后的数据矩阵。注意，这里的3个值表示3个样本，而特征从3维变成了1维
print("变换后的数据矩阵: ", x_s.dot(eigVect_with_max_eigVal), "\n")