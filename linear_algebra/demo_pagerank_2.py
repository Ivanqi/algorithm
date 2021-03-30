import networkx as nx
import matplotlib.pyplot as plt

# 创建有向图
G = nx.DiGraph()

# 添加带权重有向边
G.add_weighted_edges_from([(1, 3, 1), (2, 1, 1), (2, 3, 1), (3, 1, 1), (5, 2, 1)])

# 添加孤立节点
G.add_node(4)

# 计算pagerank值
pagerank_list = nx.pagerank(G, alpha=0.85)
print("pagerank 值是：", pagerank_list)

nx.draw(G, with_labels=True, font_weight='bold')
plt.show()