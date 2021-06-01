import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
from scipy.stats import norm

# 参考资料: https://blog.csdn.net/bitcarmanlee/article/details/79153932
def demo1():
    mu, sigma = 0, 1
    sampleNo = 1000
    np.random.seed(0)

    '''
    从正态（高斯）分布中抽取随机样本
    用例: numpy.random.normal(loc=0.0, scale=1.0, size=None)
    参数:
        loc: 浮点型数据或者浮点型数据组成的数组, 分布的均值（中心）
        scale: 浮点型数据或者浮点型数据组成的数组, 分布的标准差（宽度）
        size: 整数或者整数组成的元组, 输出值的维度。如果给定的维度为(m, n, k)，那么就从分布中抽取m * n * k个样本
    '''
    s = np.random.normal(mu, sigma, sampleNo)

    plt.hist(s, bins=100, normed=True)
    plt.show()

def demo2():
    mu, sigma, num_bins = 0, 1, 50
    x = mu + sigma * np.random.randn(1000000)

    # 正态分布的数据
    n, bins, patches = plt.hist(x, num_bins, normed=True, facecolor='blue', alpha=0.5)

    # 拟合曲线
    y = norm.pdf(bins, mu, sigma)

    plt.plot(bins, y, 'r--')
    plt.xlabel('Expectation')
    plt.ylabel('Probability')
    plt.title('histogram of normal distribution: $\mu = 0$, $\sigma=1$')

    plt.subplots_adjust(left = 0.5)
    plt.show()

demo2()