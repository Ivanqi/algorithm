# 参考资料: https://zhuanlan.zhihu.com/p/103392455
def FMM_func(user_dict, sentence):
    '''
    正向最大匹配（FMM）
    :param user_dict: 词典
    :param sentence: 句子
    '''
    # 词典中中最长词长度
    max_len = max([len(item) for item in user_dict])
    start = 0

    while start != len(sentence):
        index = start + max_len
        if index > len(sentence):
            index = len(sentence)
        
        for i in range(max_len):
            if (sentence[start:index] in user_dict) or (len(sentence[start:index]) == 1):
                print(sentence[start:index], end='/')
                start = index
                break
            index += -1

user_dict = ['我们', '在', '在野', '生动', '野生', '动物园', '野生动物园', '物','园','玩']
sentence = '我们在野生动物园玩'
FMM_func(user_dict, sentence)