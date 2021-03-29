from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.feature_extraction.text import CountVectorizer
import random
import numpy as np

'''
@description: 手动实现kmenas，以向量间的夹角余弦为相似度。根据上述tf-idf得到的7条文本向量tfidf_array，进行聚类算法
@param {type} vec_array - 向量组, n_clusters-聚类数目, epochs- 训练次数
@return: cluster_labels- 分类标签

算法流程
    1. 从N个数据对西那个随机选k个对象作为质心
        这里每个群组的质心定义是，群组内所有成员对象的平均值
        因为是第一轮，所以第i个群组的质心就是第i个对象，而且这时候只有这一个组员

    2. 对剩余的对象，测量它和每个质心的相似度，并把它归到最近的质心所属的群组
        这里可以说距离，也可以说相似度，只是两者呈现反比关系
    
    3. 重新计算已经得到的各个群组的质心
        这里质心的计算关键，如果使用特征向量来表示数据对象，那么最基本的方法是取组内成员的特征向量
        将它们的平均值作为质心的向量表示
'''
def customKmeans(vec_array, n_clusters=3, epochs=50):
    # 初始化质心的位置
    cluster_centers = []
    cluster_centers_indx = []

    while (len(cluster_centers_indx) < n_clusters):
        indx = random.randint(0, len(vec_array) - 1)
        if indx not in cluster_centers_indx:
            cluster_centers_indx.append(indx)
            cluster_centers.append(vec_array[indx])

    # 初始化向量类别
    cluster_labels = [0] * len(vec_array)
    max_similarity = [-1] * len(vec_array)
    epoch = 0

    while (epoch < epochs):
        # 计算每个向量与质心的相似性，并将其归纳到最近的质心群中
        for i in range(0, len(vec_array)):
            # 得到vec_array[i] 和  cluster_centers[0] 的夹角余弦
            max_similarity[i] = computeCosine(vec_array[i], cluster_centers[0])
            
            for j in range(1, n_clusters):
                # 得到vec_array[i] 和  cluster_centers[j] 的夹角余弦
                temp = computeCosine(vec_array[i], cluster_centers[j])
                # 两个夹角余弦进行比较
                if (temp > max_similarity[i]):
                    max_similarity[i] = temp;
                    cluster_labels[i] = j

        # 更新质心位置
        for i in range(0, n_clusters):
            # 找到集群对应原向量的下标
            indxs = [indx for indx, cluster in enumerate(cluster_labels) if cluster == i]
            # 根据集群向量的平均值更新质点位置
            cluster_centers[i] = np.average([vec_array[indx] for indx in indxs], axis=0)
    
        # 当满足迭代次数或平均最大相似性时退出算法
        epoch += 1
        if (np.average(max_similarity) > 0.9):
            break
    
    return cluster_labels

'''
两个坐标的点乘 / 两个坐标各自与原点的欧氏距离的乘积再开平方
欧氏距离(x, y) = sqrt(((x1 - y1) ^ 2 + (x2 - y2) ^ 2))
L2​ 范数 ∣∣x∣∣2​ = 两个坐标各自与原点的欧氏距离的乘积再开平方
'''
def computeCosine(vec1, vec2):
    # 计算向量间余弦值
    # dot, 两个数组的点积，即元素对应相乘
    # np.linalg.norm, linalg=linear（线性）+algebra（代数）, norm则表示范数
    vecCosine = np.dot(vec1, vec2) / (np.linalg.norm(vec1) * np.linalg.norm(vec2))
    return vecCosine

# 模拟文档集合
corpus = [
    'I like great basketball game',
    'This video game is the best action game I have ever played',
    'I really really like basketball',
    'How about this movie? Is the plot great?',
    'Do you like RPG game?',
    'You can try this FPS game',
    'The movie is really great, so great! I enjoy the plot'
]

# 把文本中的词语转换为词典和相应的向量
vectorizer = CountVectorizer()
vectors = vectorizer.fit_transform(corpus)

# 构建tfidf的值
transformer = TfidfTransformer()
tfidf = transformer.fit_transform(vectors)


# 输出每个文档的向量
tfidf_array = tfidf.toarray()

# 应用customkmeans
labels = customKmeans(tfidf_array, n_clusters=3, epochs=1000)
print(labels)