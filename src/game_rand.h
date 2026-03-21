#ifndef GAME_RAND_H
#define GAME_RAND_H

#include <random>

// 随机数生成器
class RandomGenerator {
public:
    RandomGenerator() {
        std::random_device rd;
        gen.seed(rd());
        dis = std::uniform_real_distribution<float>(0.0f, 1.0f);
    }
    
    float getRand() {
        return dis(gen);
    }

private:
    std::mt19937 gen;  // 修正：mt19937 不是 mt1937
    std::uniform_real_distribution<float> dis;
};

// 定义一个全局随机数生成器实例
static RandomGenerator gen_random;  // 使用 static 避免多次定义

#endif  // GAME_RAND_H