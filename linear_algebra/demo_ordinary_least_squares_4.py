import pandas as pd
from sklearn.linear_model import LinearRegression

df = pd.read_csv("./data/test.csv")
df_features = df.drop(['y'], axis=1) # Dataframe中除了最后一列，其余列都是特征，或者说自变量
df_targets = df['y'] # Dataframe最后一列是目标变量，或者说是因变量

print(df_features)
print(df_targets)
regression = LinearRegression().fit(df_features, df_targets) # 使用特征和目标数据，拟合线性回归模型
print(regression.score(df_features, df_targets))    # 拟合程度的好坏
print(regression.intercept_)
print(regression.coef_) # 各个特征所对应的系数