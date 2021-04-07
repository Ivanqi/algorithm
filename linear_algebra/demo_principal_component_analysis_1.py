import numpy as np
import math

len = 3
xT = np.array([[1, 2, -3], [3, 5, -7], [-7, -14, 2]])

print("第一步: 计算每列平均值:")
avgXT1 = xT[0].sum() / len
print(avgXT1)
print('avgXT1: ', avgXT1)

avgXT2 = xT[1].sum() / len
print('avgXT2: ', avgXT2)

avgXT3 = xT[2].sum() / len
print('avgXT3: ', avgXT3)

print("第二步，计算每列方差:")
varXT1 = 0.0
for val in xT[0]:
    varXT1 = varXT1 + math.pow(val - avgXT1 ,2)

varXT1 = varXT1 / len
print('varXT1: ', varXT1)

varXT2 = 0.0
for val in xT[1]:
    varXT2 = varXT2 + math.pow(val - avgXT2 ,2)

varXT2 = varXT2 / len
print('varXT2: ', varXT2)

varXT3 = 0.0
for val in xT[2]:
    varXT3 = varXT3 + math.pow(val - avgXT3 ,2)

varXT3 = varXT3 / len
print('varXT3: ', varXT3)

print("第三步，计算标准化公式:")
print("xT[0]: ", end="\t")
for val in xT[0]:
    print((val - avgXT1) / math.sqrt(varXT1), end="\t")
print("\n")

print("xT[1]: ", end="\t")
for x in xT[1]:
    print((x - avgXT2) / math.sqrt(varXT2), end="\t")
print("\n")

print("xT[2]: ", end="\t")
for x in xT[2]:
    print((x - avgXT3) / math.sqrt(varXT3), end="\t")
print("\n")

print("第四步：计算协方差:")
list1 = []
for val in xT[0]:
    list1.append(val - avgXT1)
cov1 = np.array(list1)

list2 = []
for val in xT[1]:
    list2.append(val - avgXT2)
cov2 = np.array(list2)

list3 = []
for val in xT[2]:
    list3.append(val - avgXT3)
cov3 = np.array(list3)


print('[', np.dot(cov1.transpose(), cov1) / (len - 1), end="\t")
print(np.dot(cov2.transpose(), cov1) / (len - 1), end="\t")
print(np.dot(cov3.transpose(), cov1) / (len - 1),']')

print('[', np.dot(cov1.transpose(), cov2) / (len - 1), end="\t")
print(np.dot(cov2.transpose(), cov2) / (len - 1), end="\t")
print(np.dot(cov3.transpose(), cov2) / (len - 1), ']')

print('[', np.dot(cov1.transpose(), cov3) / (len - 1), end="\t")
print(np.dot(cov2.transpose(), cov3) / (len - 1), end="\t")
print(np.dot(cov3.transpose(), cov3) / (len - 1), ']')