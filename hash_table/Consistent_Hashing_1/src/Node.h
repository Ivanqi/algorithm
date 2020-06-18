#ifndef Node_H
#define Node_H
#include <string>

// 实体节点
class Node
{
    private:
        std::string nodeName;   // 节点名, 也是服务器名
        int vNodeNum;           // 虚拟节点

    public:
        Node(std::string nodeName, int vNodeNum): nodeName(nodeName), vNodeNum(vNodeNum)
        {}

        ~Node() {}

        int GetVNodeNum() 
        {
            return vNodeNum;
        }

        std::string GetNodeName()
        {
            return nodeName;
        }
};

#endif