from numpy import *
from sklearn.preprocessing import scale
from sklearn.decomposition import PCA

# 原始数据，包含了3个样本和3个特征，每一行表示一个样本，每一列表示一维特征
x = mat([[1, 3, -7], [2, 5, -14], [-3, -7, 2]])

# 矩阵按列进行标准化
x_s = scale(x, with_mean=True, with_std=True, axis=0)
print("标准化后的矩阵: ", x_s)


# 挑选前2个主成分
pca = PCA(n_components=2)

# 进行PCA分析
pca.fit(x_s)

# 输出变换后的数据矩阵。注意,这里的三个值是表示3个样本，而特征从3维变成1维了
print("方差(特征值): ", pca.explained_variance_)
print("主成分(特征向量: )", pca.components_)
print("变换后的样本矩阵: ", pca.transform(x_s))
print("信息量: ", pca.explained_variance_ratio_)
