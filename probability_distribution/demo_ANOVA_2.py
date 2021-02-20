import pandas as pd
import numpy as np

class VarianceAnaly:
    def __init__(self, sampledata):
        self.sampledata = sampledata
        total_sum = 0
        total_count = 0


        for col in list(self.sampledata.columns):
           total_sum += self.sampledata[col].sum()
           total_count += self.sampledata[col].count()

        # 求两个算法总平均值
        self.total_var = np.round(total_sum / total_count, 4)

    # 因变量方差(Total Sum of Squares)
    def sst(self):
        result = pd.DataFrame(index=self.sampledata.index, columns=self.sampledata.columns)
        total_sum = 0
        for col in list(result.columns):
            # SSt = (某天单个算法的转化率 - 全部天数的两个算法全部数据的平均值) ^ 2
            result[col] = (self.sampledata[col] - self.total_var) ** 2
            total_sum += result[col].sum()

        return np.round(total_sum, 6) 

    # 模型平方和(Sum of Squares for Model)
    def ssm(self):
        total_sum = 0
        for col in list(self.sampledata.columns):
            # SSm = (某个算法全部时间的平均值 - 全部天数的两个算法全部数据的平均值) ^ 2
            total_sum += ((self.sampledata[col].mean() - self.total_var) ** 2) * self.sampledata[col].count()
        return np.round(total_sum, 6) 
    
    # 误差平方和(Sum of Squaress for Error)
    def sse(self):
        total_sum = 0
        for col in list(self.sampledata.columns):
            # SSe = (某天单个算法的转化率 - 某个算法全部时间的平均值) ^ 2
            total_sum += sum((self.sampledata[col] - self.sampledata[col].mean()) ** 2)
        return np.round(total_sum, 6)

    # 验证F检验的正确性
    def ValidationOfANOVA(self):
        ret = (self.sst() == self.ssm() + self.sse())
        if (ret == True):
            print("SSt == SSm + SSe")
        else:
            print("SSt != SSm - SSe")
    
    def f_value(self):
        total_columns = len(self.sampledata.columns)
        total_sample_nums = sum(self.sampledata.count())

        # 方法1
        # f1 = self.ssm() * (total_sample_nums - total_columns) / (self.sse() * (total_columns - 1))
        # 方法2
        f2 = (self.ssm() / (total_columns - 1)) / (self.sse() / (total_sample_nums - total_columns))

        return np.round(f2, 6)
       
def test():
    df = pd.read_csv("./data/oneway.csv")   # 设置为自己的文件路径
    algo_a = df[df['algo'] == 'a']['ratio']
    algo_b = df[df['algo'] == 'b']['ratio']
    list_a = algo_a.values.tolist()
    list_b = algo_b.values.tolist()
    sampledata = pd.DataFrame({
        'algo_a': list_a,
        'algo_b': list_b
    })

    test_var = VarianceAnaly(sampledata)
    
    print('sst=', test_var.sst())
    print('ssm=', test_var.ssm())
    print('sse=', test_var.sse())
    test_var.ValidationOfANOVA()
    print('F=',test_var.f_value())


if __name__ == '__main__':
    test();