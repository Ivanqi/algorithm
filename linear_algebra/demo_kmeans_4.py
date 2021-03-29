# encoding=utf-8
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from Bio.Cluster import kcluster

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
tfidf = transformer.fit_transform(vectorizer.fit_transform(corpus))

# 输出每个文档的向量
tfidf_array = tfidf.toarray()
words = vectorizer.get_feature_names()

# 进行聚类，使用向量的夹角余弦作为相似读的度量
clusterid, error, nfound = kcluster(tfidf_array, nclusters=3, dict='u')
print(clusterid)