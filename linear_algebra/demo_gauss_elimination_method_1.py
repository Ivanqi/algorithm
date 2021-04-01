import numpy as np

B = np.array([[2, 1, 1], [4, 2, 1], [2, -1, 4]])
 
print("------ 消元过程 -------");
A = np.array([[2, 1, 1, 0], [4, 2, 1, 56], [2, -1, 4, 4]])
print("E1: 为了使用A(2, 1)和A(3, 1)为0")
E1 = np.array([[1, 0, 0], [-2, 1, 0], [-1, 0, 1]])
print(E1)

print("E1A = E1 * A")
E1A = np.dot(E1, A)
print(E1A)

print("-------------");

print("E2: E1A(2, 2) 为0， 所以E1A(2, 2) 和 E1A(3, 2)互换")
E2 = np.array([[1, 0, 0], [0, 0, 1], [0, 1, 0]])
print(E2)
print("E2E1A = E2 * E1A")
E2E1A = np.dot(E2, E1A)
print(E2E1A)

print("-------------");

print('消元矩阵(E3) = E2 * E1')
E3 = np.dot(E2, E1)
print(E3)

print("------ 回代过程 -------");

print('要使最后一列之外的部分变成一个单位矩阵，就能得到最终方程组解')
print("S1: 为对角线为1，最后一行取反")
S1 = np.array([[1, 0, 0], [0, 1, 0], [0, 0, -1]])
print(S1)
print("S1E3A: S1 * E2E1A")
S1E3A = np.dot(S1, E2E1A)
print(S1E3A)

print("-------------");

print("S2: 为了让第2行的-3消失")
S2 = np.array([[1, 0, 0], [0, 1, -3], [0, 0, 1]])
print(S2)
print("S2S1E3A = S2 * S1E3A")
S2S1E3A = np.dot(S2, S1E3A)
print(S2S1E3A)

print("-------------");

print("S3: 为了第2行的-2为1")
S3 = np.array([[1, 0, 0], [0, -0.5, 0], [0, 0, 1]])
print(S3)
print("S3S2S1E3A = S3 * S2S1E3A")
S3S2S1E3A = np.dot(S3, S2S1E3A)
print(S3S2S1E3A)

print("-------------");

print("S4: 为了让第一行的2,3列为0")
S4 = np.array([[1, -1, -1], [0, 1, 0], [0, 0, 1]])
print("S4S3S2S1E3A = S4 * S3S2S1E3A")
S4S3S2S1E3A = np.dot(S4, S3S2S1E3A)
print(S4S3S2S1E3A)
print("-------------");

print("S4: 为了让第一行的1列为1")
S5 = np.array([[0.5, 0, 0], [0, 1, 0], [0, 0, 1]])
print("S5S4S3S2S1E3A = S5 * S3S2SS4S3S2S1E3A1E3A")
S5S4S3S2S1E3A = np.dot(S5, S4S3S2S1E3A)
print("最后得到一个单位矩阵")
print(S5S4S3S2S1E3A)
print("-------------");

print("通过上面的回代过程，可以得到回代过程的消元矩阵 = S5 * S4 * S3 * S2 * S2 * S1")
S = np.dot(S2, S1)
S = np.dot(S3, S)
S = np.dot(S4, S)
S = np.dot(S5, S)
print(S)

print("回代过程得到的矩阵 乘以 消元过程得到的矩阵")
SE3 = np.dot(S, E3)
print(SE3)

print("最终结果，得到一个单位矩阵")
BSE3 = np.dot(B, SE3)
print(BSE3)