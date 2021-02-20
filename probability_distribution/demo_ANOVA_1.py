import pandas as pd
from statsmodels.formula.api import ols
from statsmodels.stats.anova import anova_lm
import scipy.stats as ss

# 读取数据，d1对应于算法a，d2对应于算法b
df = pd.read_csv("./data/oneway.csv")   # 设置为自己的文件路径
d1 = df[df['algo'] == 'a']['ratio']
d2 = df[df['algo'] == 'b']['ratio']

# 检查两个水平的正态性
'''
ss.normaltest的原假设是数据符合正态分布
两次检验P值都是大于0.05的，所以原假设成立，这两者都符合正态分布
'''
print(ss.normaltest(d1))
print(ss.normaltest(d2))

print("\n")

# 检测两个水平的方差齐性
'''
ss.levene 分析了两者的方差齐性，同样P值都是远远大于0.05，因此符合方差齐的前提
'''
args = [d1, d2]
print(ss.levene(*args))

# F检验的第一种方法
print(ss.f_oneway(*args))

print("\n")

# F检验的第二种方法
model = ols('ratio ~ algo', df).fit()
anovat = anova_lm(model)
print(anovat)