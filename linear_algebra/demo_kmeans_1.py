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

# 输出所有词条(所有维度的特征)
print('所有的词条(所有维度的特征)')
print(vectorizer.get_feature_names())
print('\n')

# 输出(文章ID, 词条ID)词频
print('(文章ID, 词条ID) 词频')
print(vectors)
print('\n')