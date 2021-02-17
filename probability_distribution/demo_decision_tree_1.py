import pandas as pd
# 初始化10个武侠人物的属性
p = {
    '性别': ['男', '男', '男', '男', '男', '女', '女', '女', '女', '女'],
    '智商': ['高', '高', '高', '高', '中', '高', '高', '高', '高', '中'],
    '情商': ['高', '高', '中', '中', '高', '高', '中', '中', '中', '中'],
    '侠义': ['高', '中', '低', '中', '高', '低', '高', '高', '低', '中'],
    '个性': ['开朗', '拘谨', '开朗', '拘谨', '开朗', '开朗', '开朗', '拘谨', '开朗', '开朗']
}

name = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J']
knight = pd.DataFrame(data=p,index=name)

# 定义问题类型及增益大小
problem_type = {'性别': 1, '智商': 0.72, '情商': 0.97, '侠义': 1.58, '个性': 0.88}

# 计算测试问题的期望值
def get_exp_problems(knight, problem_type):

    result = {}
    sub_knight = [(knight, 0)]
    temp_sub_knight = []

    for pt in problem_type:
        # 用某一个问题类型对每一组侠客进行划分
        for sub in sub_knight:
            temp_sub = sub[0].groupby(by = [pt])

            # 该问题没有将该组侠客进行划分，放入中间结果，继续下一组划分
            if len(temp_sub) <= 1:
                temp_sub_knight.append((sub[0],sub[1]))
                continue
            
            # 对划分后的结果进行处理
            for label, member in temp_sub:
                list_member = list(member.index)
                if len(list_member) == 1:
                    result[list_member[0]] = sub[1] + 1
                else:
                    temp_sub_knight.append((member, sub[1] + 1))
        
        sub_knight.clear()
        sub_knight.extend(temp_sub_knight)
        temp_sub_knight.clear()

    # 计算问题数的期望值
    exp = 0
    for k in result:
        exp += result[k] / len(name)
    
    return exp

def main():
    problem = dict(sorted(problem_type.items(), key = lambda x : x[1], reverse= False))
    print(problem)
    print('选择信息增益小的问题对人物分类，问题数的期望值={}'.format(round(get_exp_problems(knight, problem),2)))

    print('\n')
    problem = dict(sorted(problem_type.items(), key = lambda x: x[1], reverse = True))
    print(problem)
    print('选择信息增益大的问题对人物分类，问题数的期望值={}'.format(round(get_exp_problems(knight, problem),2)))

if __name__ == '__main__':
    main()