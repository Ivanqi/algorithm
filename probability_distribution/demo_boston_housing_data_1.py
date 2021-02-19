import pandas as pd
from sklearn.linear_model import LinearRegression

'''
CRIM: 城镇人均犯罪率 ZN: 占地25,000平方英尺以上的住宅用地比例 INDUS: 每个城镇非零售业务英亩的比例
CHAS: 查尔斯河虚拟变量（如果束缚河，则为1；否则为0  NOX: 一氧化氮浓度（百万分之一）RM: 每个住宅的平均房间数
AGE: 1940年之前建造的自有住房的比例 DIS: 与五个波士顿就业中心的加权距离 RAD: 径向公路通达性指数
TAX: 每10,000美元的全值财产税率 PTRATIO: 各镇师生比例 B: 1000（Bk-0.63）^ 2，其中Bk是按城镇划分的黑人比例
LSTAT: 人口地位降低百分比 MEDV: 自有住房的中位价（以1000美元计）
'''
column_names = ['CRIM', 'ZN', 'INDUS', 'CHAS', 'NOX', 'RM', 'AGE', 'DIS', 'RAD', 'TAX', 'PTRATIO', 'B', 'LSTAT', 'MEDV']
df = pd.read_csv('./data/housing.csv', header=None, delimiter=r"\s+", names=column_names)


df_features = df.drop(['MEDV'], axis=1) # Dataframe中除了最后一列，其余列都是特征，或者说是自变量
df_targets = df['MEDV'] # Dataframe 最后一列是目标变量，或者说因变量


regression = LinearRegression().fit(df_features, df_targets)   # 使用特征和目标数据，拟合线性回归模型
print(regression.score(df_features, df_targets))    # 拟合程度的好坏
print(regression.coef_) # 各个特征所对应的系