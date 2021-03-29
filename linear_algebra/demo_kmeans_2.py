from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.feature_extraction.text import CountVectorizer

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
words = vectorizer.get_feature_names()

for i in range(len(tfidf_array)):
    print ("*********第", i + 1, "个文档中，所有词语的tf-idf*********")
    # 输出向量中每个维度的取值
    for j in range(len(words)):
        print(words[j], ' ', tfidf_array[i][j])
    print('\n')