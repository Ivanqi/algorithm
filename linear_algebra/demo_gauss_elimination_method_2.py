import numpy as np

B = np.array([[1, -2, 1, -4], [0, 1, -1, 1], [1, 3, 0, 1], [0, -7, 3, 1]])
A = np.array([[1, -2, 1, -4, 4], [0, 1, -1, 1, -3], [1, 3, 0, 1, 1], [0, -7, 3, 1, -3]])
print(A)

print("------- 消元过程 -------");
E1 = np.array([[1, 0, 0, 0], [0, 1, 0, 0], [-1, 0, 1, 0], [0, 0, 0, 1]])
E1A = np.dot(E1, A)
print(E1A)

E2 = np.array([[1, 0, 0, 0], [0, 1, 0, 0], [0, -5, 1, 0], [0, 7, 0, 1]])
E2E1A = np.dot(E2, E1A)
print(E2E1A)

E3 = np.array([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 1, 1]])
E3E2E1A = np.dot(E3, E2E1A)
print(E3E2E1A)

print("------- 消元矩阵 -------")

E = np.dot(E2, E1)
E = np.dot(E3, E)
# 验证消元矩阵E是否正确
# print(np.dot(E, A))
print(E)


print("------ 回代过程 -------");
S1 = np.array([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1/8]])
S1E3E2E1A = np.dot(S1, E3E2E1A)
print(S1E3E2E1A)

S2 = np.array([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1/4, 0], [0, 0, 0, 1]])
S2S1E3E2E1A = np.dot(S2, S1E3E2E1A)
print(S2S1E3E2E1A)

S3 = np.array([[1, 0, 0, 0], [0, 1, 1, -1], [0, 0, 1, 0], [0, 0, 0, 1]])
S3S2S1E3E2E1A = np.dot(S3, S2S1E3E2E1A)
print(S3S2S1E3E2E1A)

S4 = np.array([[[1, 2, -1, 4], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]])
S4S3S2S1E3E2E1A = np.dot(S4, S3S2S1E3E2E1A)
print("------ 方程的解 -------");
print(S4S3S2S1E3E2E1A)

print("------- 回代矩阵 -------")
S = np.dot(S2, S1)
S = np.dot(S3, S)
S = np.dot(S4, S)
print(S)

# 验证回代矩阵
# print(np.dot(S, E3E2E1A))

print("------- 消元-回代矩阵 -------")
SE = np.dot(S, E)
print(SE)
# 验证消元-回代矩阵
# print(np.dot(SE, B))
