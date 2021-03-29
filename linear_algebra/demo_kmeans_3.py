from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.cluster import KMeans

'''
KMeans 的实现
    1. 把文档集合都转换成向量的形式
    2. 使用K均值算法对文档集合进行聚类
        1. 使用向量空间中的距离或者夹角余弦度量，计算两个向量的相似度
        2. 计算质心的向量。K均值里，质心是分组里成员的平均值。所以，需要求分组里所有文档向量的平均值
    3. 在每个分类中，选出和质心最接近的几篇文章作为代表。而其他的文章作为冗余的内容过滤掉
'''

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

# 进行聚类，在这里默认使用的是欧式距离
clusters = KMeans(n_clusters=3)
s = clusters.fit(tfidf_array)

# 输出所有质心点，可以看到质心点的向量是组内成员的平均值
print('所有质心点的向量')
print(clusters.cluster_centers_)
print('\n')

# 输出每个分组内的文档
dict = {}
for i in range(len(clusters.labels_)):
    label = clusters.labels_[i]
    if label not in dict.keys():
        dict[label] = []
        dict[label].append(corpus[i])
    else:
        dict[label].append(corpus[i])

print(dict)