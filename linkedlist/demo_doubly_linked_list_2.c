#include <stdio.h>
#include <stdlib.h>

int len;

// 定义双向链表的节点
typedef struct Node {
    int data;
    struct Node *prior;
    struct Node *next;
}Node;

// 初始化一个链表的节点
Node* create_node(void) {
    Node *p;
    p = (Node *)malloc(sizeof(Node));

    if (p == NULL) {
        printf("动态内存分配失败!\n");
        exit(0);
    }
    scanf("%d", &(p->data));
    p->prior = NULL;
    p->next = NULL;

    return (p);
}

// 建立含有N个结点的双向链表
Node* create_list(int n) {
    Node *p, *new1, *head;

    int i;
    if (n >= 1) {               // 结点的个数 >= 1的时候，先生成第一个结点
        new1 = create_node();
        head = new1;
        p = new1;
    }
    for (i = 2; i <= n; i++) {  // 生成第一个结点以后的结点，并建立双向链表的关系
        new1 = create_node();
        p->next = new1;
        new1->prior = p;
        p = new1;
    }

    len = n;
    if (n >= 1) {
        return (head);
    } else {
        return 0;
    }
}

// 链表长度
int len_list(int len) {
    return len;
}

// 定位到链表的任意位置
Node* pos_list(Node *head, int n) {
    int i = 1;
    Node *p;

    if (i <= n) {
        p = head;
        for (i = 2; i <= n; i++) {
            p = p->next;
        }
    }
    return p;
}

// 正向遍历一个链表
void out_front_list(Node *head) {
    if (head == NULL) {
        printf("输入链表信息有误，链表不存在!\n");
    } else {
        Node *p;
        p = head;
        while (p != NULL) {
            printf("%d", p->data);
            p = p->next;
        }
    }
}

// 反向遍历一个链表
void out_reverse_list(Node *head) {
    if (head == NULL) {
        printf("输入的链表信息有误，链表不存在\n");
    } else {
        int n;
        n = len_list(len);
        Node *p;
        p = pos_list(head, n);
        while (p != NULL) {
            printf("%d", p->data);
            p = p->prior;
        }
    }
}

// 在链表的头部插入结点
Node* start_insert_list(Node *head) {
    Node *p;
    p = create_node();
    p->next = head;
    head->prior = p;
    head = p;
    len++;
    return (p);
}

// 在链表的尾部插入结点
Node* end_insert_list(Node *head) {
    int n;
    n = len_list(len);

    Node *p, *new1;
    new1 = create_node();
    p = pos_list(head, n);
    p->next = new1;
    new1->prior = p;
    len++;
    return (head);
}

// 插入到任意位置之前
Node* insert_befor_list(Node *head) {
    int a, newlen;
    Node *pos, *p;
    printf("请输入要插入结点的位置:");
    scanf("%d", &a);
    printf("请输入要插入的结点的值:");
    newlen = len_list(len);

    if (a > newlen) {
        head = end_insert_list(head);
    } else {
        if (a <= 1) {
            head = start_insert_list(head);
        } else {
            pos = pos_list(head, a);
            p = create_node();
            pos->prior->next = p;
            p->prior = pos->prior;
            p->next = pos;
            pos->prior = p;
        }
    }
    len++;
    return (head);
}

// 插入到任意位置之后
Node* insert_after_list(Node *head) {
    int a,newlen;
    Node *pos,*p;
    printf("请输入要插入结点的位置：");
    scanf("%d",&a);
    printf("请输入要插入的结点的值："); 
    newlen = len_list(len);
    if(a >= newlen) {
        head = end_insert_list(head);
    } else {
        if(a < 1) {
            head = start_insert_list(head);
        } else {
            pos = pos_list(head,a);
            p = create_node();
            p->next = pos->next;
            pos->next->prior = p; 
            pos->next = p;
            p->prior = pos;
        }
    }
    len++;
    return (head);    
}

// 删除头结点
Node* delect_start_list(Node *head) {
    Node *pos;
    pos = head;
    head = head->next;
    head->prior = NULL;
    free(pos);
    len--; 
    return(head);
}

// 删除尾结点
Node* delect_end_list(Node *head)
{
    Node *p,*pos;
    int newlen;
    newlen = len_list(len);
    pos = pos_list(head,newlen);
    p = pos;
    p = p->prior;
    p->next = NULL;
    free(pos);
    len--;
    return (head); 
}

// 删除指定位置的节点
Node* delect_list(Node *head)
{
    int newlen,i;
    Node *pos;
    newlen = len_list(len);
    printf("请输入眼删除结点的位置：\n");
    scanf("%d",&i);
    if(i <= 1)
        head = delect_start_list(head);
    else if(i >=newlen)
        head = delect_end_list(head);
    else {
        pos =pos_list(head,i);
        pos->prior->next = pos->next;
        pos->next->prior = pos->prior;
        free(pos); 
    }
    len--; 
    return(head);
}

int main()
{
//函数的声明 
    Node* create_node(void);               //定义双向链表的节点 
    Node* create_list(int n);              //建立含有N个结点的双向链表
    int len_list(int len);                 //链表的长度 
    Node* pos_list(Node *head,int n);      //定位到链表的任意位置 
    Node* tail_list(Node *head);           //将指针定位在链表的尾部
    void out_front_list(Node *head);       //正向遍历一个链表
    void out_reverse_list(Node *head);     //反向遍历一个链表
    Node* start_insert_list(Node *head);   //在链表的头部插入结点
    Node* end_insert_list(Node *head);     //在链表的尾部插入结点
    Node* insert_befor_list(Node *head);   //插入到任意位置之前
    Node* insert_after_list(Node *head);   //插入到任意位置之后 
    Node* delect_start_list(Node *head);   //删除头结点
    Node* delect_end_list(Node *head);     //删除尾结点
    Node* delect_list(Node *head);         //删除指定位置的节点

    //int newlen;
    Node *head;
    
    printf("请输入要建立双向链表的长度：\n");
    scanf("%d",&len);
    printf("请为双向链表赋值：\n");
    head = create_list(len);
    printf("链表的长度为：%d\n",len = len_list(len));
    printf("正向遍历双向链表：\n");
    out_front_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    printf("\n反向遍历双向链表：\n");
    out_reverse_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    
    printf("\n请输入在链表头部插入结点的值：\n");
    head = start_insert_list(head);
    printf("链表的长度为：%d",len = len_list(len));
    printf("\n正向遍历双向链表：\n");
    out_front_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    printf("\n反向遍历双向链表：\n");
    out_reverse_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    
    printf("\n请输入在链表尾部插入结点的值：\n");
    head = end_insert_list(head);
    printf("链表的长度为：%d",len = len_list(len));
    printf("\n正向遍历双向链表：\n");
    out_front_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    printf("\n反向遍历双向链表：\n");
    out_reverse_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    
    printf("\n插入到任意位置之前：\n");
    head = insert_befor_list(head);
    printf("链表的长度为：%d",len = len_list(len));
    printf("\n正向遍历双向链表：\n");
    out_front_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    printf("\n反向遍历双向链表：\n");
    out_reverse_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    
    printf("\n插入到任意位置之后：\n");
    head = insert_after_list(head);
    printf("链表的长度为：%d",len = len_list(len));
    printf("\n正向遍历双向链表：\n");
    out_front_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    printf("\n反向遍历双向链表：\n");
    out_reverse_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    
    printf("\n删除头结点：\n");
    head = delect_start_list(head);
    printf("链表的长度为：%d",len = len_list(len));
    printf("\n正向遍历双向链表：\n");
    out_front_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    printf("\n反向遍历双向链表：\n");
    out_reverse_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    
    printf("\n删除尾结点：\n");
    head = delect_end_list(head);
    printf("链表的长度为：%d",len = len_list(len));
    printf("\n正向遍历双向链表：\n");
    out_front_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    printf("\n反向遍历双向链表：\n");
    out_reverse_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    
    printf("\n删除指定位置的结点：\n");
    head = delect_list(head);
    printf("链表的长度为：%d",len = len_list(len));
    printf("\n正向遍历双向链表：\n");
    out_front_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    printf("\n反向遍历双向链表：\n");
    out_reverse_list(head);
    printf("\n链表的长度为：%d",len = len_list(len));
    
    return 0;
}