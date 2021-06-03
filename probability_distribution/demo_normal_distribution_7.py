import numpy as np
from scipy.special import erfinv

'''
函数原型: numpy.random.uniform(low,high,size)
功能:
    从一个均匀分布[low,high)中随机采样，注意定义域是左闭右开，即包含low，不包含high
参数介绍:
    low: 采样下界，float类型，默认值为0
    high: 采样上界，float类型，默认值为1
    size: 输出样本数目，为int或元组(tuple)类型，例如，size=(m,n,k), 则输出m*n*k个样本，缺省时输出1个值
返回值: 
    ndarray类型，其形状和参数size中描述一致
'''

# Box-Muller方法
# 用Box-Muller方法，随机抽出两个从[0,1]均匀分布的数字u和v,通过正态分布的公式获得最后的值
def boxmullersampling(mu = 0, sigma = 1, size = 1):
    u = np.random.uniform(size=size)
    v = np.random.uniform(size=size)
    z = np.sqrt(-2 * np.log(u)) * np.cos(2 * np.pi * v)

    return mu + z * sigma

# 使用反函数，先随机抽出一个从[0,1 ]均匀分布的数字u, 然后利用inverse CDF公式计算，得到最后的值
def inverfsampling(mu = 0, sigma = 1, size = 1):
    z = np.sqrt(2) * erfinv(2 * np.random.uniform(size=size) - 1)
    return mu + z * sigma



sample1 = []
for i in range(0, 10):
    sample1.append(boxmullersampling())

print(sample1)


sample2 = []
for i in range(0, 10):
    sample2.append(inverfsampling())

print(sample2)