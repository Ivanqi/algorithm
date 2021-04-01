import numpy as np
import math

# 基于用户的过滤
'''
用户 - 物品矩阵
表示 N 个用户对M件物品的喜欢程度矩阵
i: 用户数量 j: 物品数量
'''
User = 4
Goods = 3

arr1 = np.array([0.11, 0.20, 0.0])      # 用户1对物品的喜爱程度
arr2 = np.array([0.81, 0.0, 0.0])       # 用户2对物品的喜爱程度
arr3 = np.array([0.0, 0.88, 0.74])      # 用户3对物品的喜爱程度
arr4 = np.array([0.0, 0.0, 0.42])       # 用户4对物品的喜爱程度

# '用户-物品'喜爱度矩阵
x = np.full([User, Goods], [arr1, arr2, arr3, arr4], dtype=float)
# 对'用户-物品'喜欢矩阵增加0，为了后面和us进行点乘
xten = np.full([User, User], [np.concatenate((arr1, [0])), np.concatenate((arr2, [0])), np.concatenate((arr3, [0])), np.concatenate((arr4, [0]))], dtype=float)
# x的转置矩阵，转置矩阵的意义得到其他用户对物品的喜爱，和原先的用户进行相似度计算
xT = x.transpose()

y = np.dot(x, xT)

# 用户相似矩阵
us = np.full([User, User], 0.0, dtype=float)

# 受用y来计算us，通过计算当前(i,j)的对角线，得到两两的相似度
for i in range(User):
    for j in range(User):
        us[i][j] = round(y[i][j] / (math.sqrt(y[i][i]) * math.sqrt(y[j][j])), 3)

print(us)
# 用户相似矩阵 乘以 原来的喜爱度矩阵。就可以得到是否当前用户喜欢的物品，其他用户也喜欢
usp = np.dot(us, xten)
usp = usp[...,0:-1]
print(usp)


usSum = np.sum(us, axis=1)
# 用于归一化
usr = np.full((User, Goods), [[usSum[0]], [usSum[1]], [usSum[2]], [usSum[3]]])
print(usr)

# 归一化后的喜爱矩阵。这样就可以通过x(已知的喜爱度剧组)预测到其他用户也喜欢的矩阵
p = usp / usr
print(p)
