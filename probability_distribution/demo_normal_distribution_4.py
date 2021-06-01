import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
# matplotlib inline

# 参考资料: https://blog.csdn.net/u010199356/article/details/87873596
# np.random.randn, 通过本函数可以返回一个或一组服从标准正态分布的随机样本值, 标准正态分布是以0为均数、以1为标准差的正态分布，记为N（0，1）
s = pd.DataFrame(np.random.randn(1000) + 10,columns = ['value'])
print(s.head())

# 创建随机数据
fig = plt.figure(figsize=(10, 6))
ax1 = fig.add_subplot(2, 1, 1)  # 创建子图1
ax1.scatter(s.index, s.values)
# 配置网格线
plt.grid()

# 绘制数据分布图
ax2 = fig.add_subplot(2, 1, 2)  # 创建子图2
# 制作直方图 
'''
bins: 要使用的直方图箱数. 如果给定整数，则将计算bins + 1 bins边缘并返回。如果bins是序列，则给出bin边缘，包括第一个bin的左边缘和最后一个bin的右边缘
ax: 绘制直方图的轴
'''
s.hist(bins=30, alpha = 0.5, ax = ax2)
# 绘制Series或DataFrame的图
'''
kind = 'kde': 核密度估计图
secondary_y: 如果是列表/元组，是否在辅助 y 轴上绘制，哪些列要在辅助 y 轴上绘制
ax: 当前图形的轴
'''
s.plot(kind = 'kde', secondary_y=True, ax = ax2)
plt.grid()
# 绘制直方图
# 呈现较明显的正态性
plt.show()