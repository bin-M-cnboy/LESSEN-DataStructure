#include "../include/graph.h"
#include<iostream>

void graph::init(int u[],int v[],int w[])
{
    // 遍历所有边，构建邻接表
    for(int i = 0; i < edge_num; i++) {
        // 创建新的边节点
        Edge* newEdge = new Edge();
        newEdge->adj = v[i];           // 设置邻接点
        newEdge->weight = w[i];        // 设置权重
        
        // 头插法：将新边插入到顶点 u[i] 的邻接表头部
        newEdge->next = vertex[u[i]].head;
        vertex[u[i]].head = newEdge;
    }
}

void graph::dfs(int s)
{
    // 标记当前节点为已访问
    vertex[s].visited = 1;
    
    // 输出当前访问的顶点编号
    std::cout << s << " ";
    
    // 遍历当前顶点的所有邻接点
    Edge* current = vertex[s].head;
    while(current != NULL) {
        int adjVertex = current->adj;
        
        // 如果邻接点未被访问，递归访问它
        if(vertex[adjVertex].visited == 0) {
            dfs(adjVertex);
        }
        
        // 移动到下一条边
        current = current->next;
    }
}