# 用线性回归模型预测房价
import numpy as np
import pandas as pd
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import MinMaxScaler

df = pd.read_csv("./data/boston_housing_data.csv")  # 读取Boston Housing中的train.csv

# 删除目标值是nan样本
nanindex = list(df[df['MEDV'].isna()].index)
for row in nanindex:
    df.drop(index=row, inplace=True)

df_features = df.drop(['MEDV'], axis=1) # Dataframe中除了最后一列，其余列都是特征，或者说是自变量

df_targets = df['MEDV'] # Dataframe最后一列是目标变量，或者说因变量

# 标准化处理后，再进行回归分析
standardScaler = StandardScaler() # 基于Z分数的标准化
standardScaler.fit(df)

df_standardized = standardScaler.transform(df) # 对原始数据进行标准化，包括特征值和目标变量
df_features_standardized = df_standardized[:, 0:-1] # 获取标准化之后特征值
df_targets_standardized = df_standardized[:, -1] # 获取标准化之后的目标值

# 线性代数
regression_standardized = LinearRegression().fit(df_features_standardized, df_targets_standardized)

# 计算预测误差

# 设置系统矩阵B
B = regression_standardized.coef_.reshape(len(regression_standardized.coef_), 1)

# 设置特征矩阵X
X = df_features_standardized

# 设置目标矩阵Y
Y = df_targets_standardized.reshape(len(df_targets_standardized), 1)

# 计算预测误差绝对值
E = np.dot(X, B) - Y

# 输出预测误差的平均值和方差
print('误差平均值=', np.mean(E))
print('误差的方差=', np.var(E))