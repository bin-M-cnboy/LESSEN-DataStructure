#include "walk.h"

namespace DATA_STRUCTURE {

void walk::print_para() {
    std::cout << steps << std::endl;
    for (int i = 0; i < gold_num; i++) {
        std::cout << *(gold + 2 * i) << " " << *(gold + 2 * i + 1) << " " << *(gold_value + i) << std::endl;
    }
}

int walk::compute_distance(int i, int x, int y) {
    // 金币位置 (gold[2*i], gold[2*i+1])
    int dx = abs(*(gold + 2 * i) - x);
    int dy = abs(*(gold + 2 * i + 1) - y);
    // 优先使用移动2格，剩余用移动1格
    int steps_x = dx / 2 + dx % 2;
    int steps_y = dy / 2 + dy % 2;
    
    return (steps_x > steps_y) ? steps_x : steps_y;
}

int walk::get_value() {
    int max_value = 0;
    
    element start;
    start.x = 0;
    start.y = 0;
    start.steps = steps;
    start.value = 0;
    start.get_gold.clear();
    q.push(start);
    
    // 使用unordered_set记录访问过的状态，避免重复
    unordered_set<int> visited;
    
    while (!q.empty()) {
        element current = q.top();
        q.pop();
        
        if (current.value > max_value) max_value = current.value;
        if (current.steps <= 0) continue;
        
        // BFS尝试移动到每个金币位置 
        for (int i = 0; i < gold_num; i++) {            
            if (current.get_gold.find(i)) continue; // 如果已经获取过这个金币，跳过
            
            int coin_x = *(gold + 2 * i);
            int coin_y = *(gold + 2 * i + 1);
            int distance = compute_distance(i, current.x, current.y);
            
            if (distance <= current.steps) {
                element next;
                next.x = coin_x;
                next.y = coin_y;
                next.steps = current.steps - distance;
                next.value = current.value + *(gold_value + i);
                
                next.get_gold.clear();
                for (int j = 0; j < gold_num; j++) {
                    if (current.get_gold.find(j)) next.get_gold.insert(j);
                }
                next.get_gold.insert(i);
                
                q.push(next);
            }
        }
    }
    
    return max_value;
}

}