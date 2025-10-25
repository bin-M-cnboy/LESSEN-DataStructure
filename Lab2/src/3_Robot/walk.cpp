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
    
    int steps_x = dx / 2 + dx % 2;
    int steps_y = dy / 2 + dy % 2;
    
    // x和y方向独立移动，总步数是两者之和
    return steps_x + steps_y;
}

int walk::get_value() {
    int max_value = 0;
    
    // 初始状态
    element start;
    start.x = 0;
    start.y = 0;
    start.steps = steps;
    start.value = 0;
    start.get_gold.clear();
    q.push(start);
    
    // BFS 遍历所有可能的状态
    while (!q.empty()) {
        element current = q.top();
        q.pop();
        
        // 更新最大价值
        if (current.value > max_value) {
            max_value = current.value;
        }
        
        // 如果步数用完，不再扩展
        if (current.steps <= 0) continue;
        
        // 尝试移动到每个未获取的金币位置
        for (int i = 0; i < gold_num; i++) {            
            // 如果已经获取过这个金币，跳过
            if (current.get_gold.find(i)) continue;
            
            int coin_x = *(gold + 2 * i);
            int coin_y = *(gold + 2 * i + 1);
            int distance = compute_distance(i, current.x, current.y);
            
            // 如果步数足够到达该金币
            if (distance <= current.steps) {
                element next;
                next.x = coin_x;
                next.y = coin_y;
                next.steps = current.steps - distance;
                next.value = current.value + *(gold_value + i);
                
                // 复制已获取的金币集合
                next.get_gold.clear();
                for (int j = 0; j < gold_num; j++) {
                    if (current.get_gold.find(j)) {
                        next.get_gold.insert(j);
                    }
                }
                next.get_gold.insert(i);
                
                q.push(next);
            }
        }
    }
    
    return max_value;
}

}