import pandas as pd
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import MinMaxScaler

'''
CRIM: 城镇人均犯罪率 ZN: 占地25,000平方英尺以上的住宅用地比例 INDUS: 每个城镇非零售业务英亩的比例
CHAS: 查尔斯河虚拟变量（如果束缚河，则为1；否则为0  NOX: 一氧化氮浓度（百万分之一）RM: 每个住宅的平均房间数
AGE: 1940年之前建造的自有住房的比例 DIS: 与五个波士顿就业中心的加权距离 RAD: 径向公路通达性指数
TAX: 每10,000美元的全值财产税率 PTRATIO: 各镇师生比例 B: 1000（Bk-0.63）^ 2，其中Bk是按城镇划分的黑人比例
LSTAT: 人口地位降低百分比 MEDV: 自有住房的中位价（以1000美元计）
'''
column_names = ['CRIM', 'ZN', 'INDUS', 'CHAS', 'NOX', 'RM', 'AGE', 'DIS', 'RAD', 'TAX', 'PTRATIO', 'B', 'LSTAT', 'MEDV']
df = pd.read_csv('./data/housing.csv', header=None, delimiter=r"\s+", names=column_names)


standardScaler = StandardScaler()   # 基于Z分数的标准化

standardScaler.fit(df)
df_standardized = standardScaler.transform(df)  #  对原始数据进行标准化，包括特征值和目标变量

df_features_standardized = df_standardized[:, 0:-1] # 获取标准化之后的特征值
df_targets_standardized = df_standardized[:, -1] # 获取标准化之后的特征值

# 再次线性回归
regression_standardized = LinearRegression().fit(df_features_standardized, df_targets_standardized)
print(regression_standardized.score(df_features_standardized, df_targets_standardized))
print(regression_standardized.coef_)