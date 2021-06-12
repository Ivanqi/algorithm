def load_dict():
    '''
    加载字典
    :return: 返回字典 => "词: 频率"
    '''

    dic_file = open("./data/WordFrequency.txt", 'r', encoding='utf-8')
    freq_dict = {}

    for l in dic_file:
        freq_dict[l.split(',')[0]] = l.split(',')[2].strip()
    dic_file.close()
    return freq_dict

def find_word_in_dict(s):
    '''
    在字典中查找候选词
    :param s: 输入句子
    :return: 返回字典 => "词: 词频 | 候选左邻词1 / 候选左邻词2"
    '''
    freq_dict = load_dict()
    result = {}

    for index in range(0, len(s)):  # 遍历所有字
        for wordLen in range(0, len(s) - index): # 遍历该字的所有可能词
            seg_word = s[index:index + wordLen + 1]
            if seg_word in freq_dict.keys():
                # 找到候选词，找其左邻词
                left_words = ''
                for word_len in range(index, 0, -1):    # 在之前的候选词列表里找左邻词(最大词长开始)
                    for k in result.keys():
                        if s[index - word_len:index] == k.split('-')[1]:
                            left_words += str(index - word_len) + '-' + s[index - word_len:index] + '/'
                
                # 返回候选词及其语料库词频和候选左邻词
                result[str(index) + '-' + seg_word] = freq_dict[seg_word] + '|' + left_words

    return result

def cl_probability(words_dict):
    '''
    计算累加概率并选择最佳左邻词
    :param words_dict: "词:词频 | 候选左邻词1 / 候选左邻词2"
    :return: 返回新字典 ==> "词: 累计概率 | 最佳左邻词"
    '''
    for k, v in words_dict.items():
        freq = v.split('|')[0][:-1]
        left_words = v.split('|')[1]
        if left_words == '':
            continue
        else:
            left_word = left_words.split('/')
            max_left_p = 0.0
            which_word = ''
            for num in range(0, len(left_word) - 1):
                curr_left_word_p = float(words_dict[left_word[num]].split('|')[0][:-1])
                if curr_left_word_p > max_left_p:   # 比较当前左邻的累计概率
                    max_left_p = curr_left_word_p
                    which_word = left_word[num]
            
            curr_max_p = float(max_left_p) * float(freq)
            # 用最大累计概率替换原来的词频，用最佳左邻词替换候选左邻词
            words_dict[k] = v.replace(freq, str(curr_max_p)).replace(left_words, which_word)

    return words_dict


def seg(sentence):
    '''
    接收输入，调用函数并输出
    :param sentence
    :return: 分词后的句子
    '''

    words_dict = find_word_in_dict(sentence)
    best_words_dict = cl_probability(words_dict)
    seg_line = ''

    keys = list(best_words_dict.keys())
    key = keys[-1]

    while key != '':
        seg_line = key.split('-')[1] + '/' + seg_line
        key = best_words_dict[key].split('|')[1]
    return seg_line


if __name__ == '__main__':
    print("概率最大化分词-请输入待切分的句子")
    intput_str = input()
    print(seg(intput_str))