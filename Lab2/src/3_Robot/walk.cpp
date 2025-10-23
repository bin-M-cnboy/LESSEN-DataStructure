#include "walk.h"

namespace DATA_STRUCTURE {

void walk::print_para() {
    std::cout << steps << std::endl;
    for (int i = 0; i < gold_num; i++) {
        std::cout << *(gold + 2 * i) << " " << *(gold + 2 * i + 1) << " " << *(gold_value + i) << std::endl;
    }
}

int walk::compute_distance(int i, int x, int y) {
    // 计算从(x, y)到第i个金币位置的最少步数
    // 金币位置: (gold[2*i], gold[2*i+1])
    int dx = abs(*(gold + 2 * i) - x);
    int dy = abs(*(gold + 2 * i + 1) - y);
    
    // 每次可以移动1或2格，计算最少步数
    // 优先使用移动2格，剩余用移动1格
    int steps_x = dx / 2 + dx % 2;  // x方向需要的步数
    int steps_y = dy / 2 + dy % 2;  // y方向需要的步数
    
    // 总步数是两个方向步数的最大值（可以同时移动x和y方向）
    return (steps_x > steps_y) ? steps_x : steps_y;
}

int walk::get_value() {
    int max_value = 0;
    
    // 初始状态：位置(0, 0)，剩余步数steps，已获得价值0
    element start;
    start.x = 0;
    start.y = 0;
    start.steps = steps;
    start.value = 0;
    start.get_gold.clear();
    
    q.push(start);
    
    // 使用unordered_set记录访问过的状态，避免重复
    // 状态用字符串表示: "x,y,steps,已获取金币集合"
    // 为简化，我们用"x,y,剩余步数"作为状态标识
    unordered_set<int> visited;
    
    while (!q.empty()) {
        element current = q.top();
        q.pop();
        
        // 更新最大价值
        if (current.value > max_value) {
            max_value = current.value;
        }
        
        // 如果没有剩余步数，跳过
        if (current.steps <= 0) {
            continue;
        }
        
        // 尝试移动到每个金币位置
        for (int i = 0; i < gold_num; i++) {
            // 如果已经获取过这个金币，跳过
            if (current.get_gold.find(i)) {
                continue;
            }
            
            int coin_x = *(gold + 2 * i);
            int coin_y = *(gold + 2 * i + 1);
            
            // 计算到达这个金币需要的步数
            int distance = compute_distance(i, current.x, current.y);
            
            // 如果步数足够
            if (distance <= current.steps) {
                // 创建新状态
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
                next.get_gold.insert(i);  // 添加当前金币
                
                // 将新状态加入队列
                q.push(next);
            }
        }
    }
    
    return max_value;
}

}