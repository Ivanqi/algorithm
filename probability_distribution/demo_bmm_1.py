# 参考资料: https://zhuanlan.zhihu.com/p/103392455
def BMM_func(user_dict, sentence):
    '''
    反向最大匹配（BMM）
    :param user_dict:词典
    :param sentence:句子
    '''

    # 词典中最长词长度
    max_len = max([len(item) for item in user_dict])
    result = []
    start = len(sentence)

    while start != 0:
        index = start - max_len
        if index < 0:
            index = 0
        
        for i in range(max_len):
            if (sentence[index:start] in user_dict) or (len(sentence[start:index]) == 1):
                result.append(sentence[index:start])
                start = index
                break;
            index += 1
        
    for i in result[::-1]:
        print(i, end='/')
    

user_dict = ['我们', '在', '在野', '生动', '野生', '动物园', '野生动物园', '物','园','玩']
sentence = '我们在野生动物园玩'
BMM_func(user_dict, sentence)