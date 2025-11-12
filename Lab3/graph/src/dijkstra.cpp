#include "../include/dijkstra.h"
#include <iostream>
#include <climits>

void dijkstra::ssp(int s)
{
    //TODO:实现以s为起始点的dijkstra算法求最短路径
    
    // 1. 初始化所有顶点
    for(int i = 0; i < g.vertex_num; i++) {
        vertex[i].sure = 0;           // 标记为未确定最短路径
        vertex[i].dist = INT_MAX;     // 距离初始化为无穷大
        vertex[i].path = -1;          // 前驱初始化为-1（无前驱）
    }
    
    // 2. 初始化源点
    vertex[s].dist = 0;               // 源点到自己的距离为0
    vertex[s].path = -1;              // 源点无前驱
    
    // 3. Dijkstra主循环：处理所有顶点
    for(int count = 0; count < g.vertex_num; count++) {
        // 3.1 找到未确定顶点中距离最小的顶点
        int minDist = INT_MAX;
        int u = -1;
        
        for(int i = 0; i < g.vertex_num; i++) {
            if(vertex[i].sure == 0 && vertex[i].dist < minDist) {
                minDist = vertex[i].dist;
                u = i;
            }
        }
        
        // 如果找不到可达的顶点，说明剩余顶点都不可达
        if(u == -1) break;
        
        // 3.2 标记该顶点的最短路径已确定
        vertex[u].sure = 1;
        
        // 3.3 更新u的所有邻接点的距离
        graph::Edge* edge = g.vertex[u].head;
        while(edge != NULL) {
            int v = edge->adj;
            int weight = edge->weight;
            
            // 如果通过u到达v的距离更短，则更新
            if(vertex[u].dist != INT_MAX && 
               vertex[u].dist + weight < vertex[v].dist) {
                vertex[v].dist = vertex[u].dist + weight;
                vertex[v].path = u;  // 记录前驱为u
            }
            
            edge = edge->next;
        }
    }
    
    // 4. 输出结果
    for(int i = 0; i < g.vertex_num; i++) {
        std::cout << "Vertex " << i << ": ";
        
        if(vertex[i].dist == INT_MAX) {
            std::cout << "unreachable" << std::endl;
        } else {
            std::cout << "distance = " << vertex[i].dist << ", path: ";
            print(i);
            std::cout << std::endl;
        }
    }
}

void dijkstra::print(int u)
{
    //TODO:用于输出最短路径的辅助函数，可以使用递归实现
    
    // 递归终止条件：到达源点（前驱为-1）
    if(vertex[u].path == -1) {
        std::cout << u;  // 输出源点
        return;
    }
    
    // 递归输出前驱路径
    print(vertex[u].path);
    
    // 输出当前节点
    std::cout << " -> " << u;
}