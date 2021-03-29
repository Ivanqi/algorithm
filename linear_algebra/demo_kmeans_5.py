from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.feature_extraction.text import CountVectorizer
import random
import numpy as np

'''
@description: 手动实现kmenas，以向量间的夹角余弦为相似度。根据上述tf-idf得到的7条文本向量tfidf_array，进行聚类算法
@param {type} vec_array - 向量组, n_clusters-聚类数目, epochs- 训练次数
@return: cluster_labels- 分类标签
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
            max_similarity[i] = computeCosine(vec_array[i], cluster_centers[0])
            
            for j in range(1, n_clusters):
                temp = computeCosine(vec_array[i], cluster_centers[j])
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