#ifndef scene_main_H
#define scene_main_H

#include "scene_base.h"
#include "game_structure.h"
#include "objects.h"
#include <list>
#include <map>

 class game;

 //枚举图片资源类型
enum class Bonus_TextureType {
    extra_life,
    extra_shield,
    freeze_time,
    extra_support,   //rocket help
};
 
class scene_main : public SceneBase {
public:
    scene_main();
    ~scene_main();
    void update() override;
    void render() override;
    void init() override;    
    void clean() override;
    void handleEvent(SDL_Event *e) override;

    void keyboardControl(); //键盘控制  
    void bulletGenerate(); //子弹发射
    void updateBullet();  //子弹刷新
    void renderBullet(); //渲染子弹
    void updatePlayer(); //刷新玩家
    void renderPlayer(); //渲染玩家

    
    void enemyGenerate(); //生成敌人
    void updateEnemy(); //刷新敌人
    void renderEnemy(); //渲染敌人
    void enemyBulletGenerate(Enemy_Template* enemy); //生成敌人子弹
    void updateEnemyBullet(); //刷新敌人子弹
    void renderEnemyBullet(); //渲染敌人子弹


    void updateExplosion(); //刷新爆炸
    void renderExplosion(); //渲染爆炸

    void ItemGenerate(Enemy_Template* enemy); //生成道具; //刷新道具
    void updateBonus(); //刷新奖励
    void renderBonus(); //渲染奖励
    //计算子弹方向
    SDL_FPoint bulletDirection(Enemy_Template* enemy); //计算子弹方向


    Player player;  //创建玩家，在此场景中

    Projectile_Player_Template projectile_player_template; //创建玩家子弹模板
    std::list<Projectile_Player_Template*> projectile_player_list;  //创建子弹容器list

    std::list<Enemy_Template*> enemy_list; //创建敌人容器list
    Enemy_Template enemy_template; //创建敌人模板
    
    Projectile_Enemy_Template projectile_enemy_template; //创建敌人子弹模板
    std::list<Projectile_Enemy_Template*> projectile_enemy_list; //创建敌人子弹容器list

    explosion_template explosion_base; //创建爆炸模板
    std::list<explosion_template*> explosion_list; //创建爆炸容器list

    //建立图片资源库map
    std::map<Bonus_TextureType, SDL_Texture*> bonus_texture_map; //奖励图片资源库
    Item bonus_item; //创建奖励
    std::list<Item*> bonus_list; //创建奖励容器list 



private:
    game& game; //单例模式引用，必须写在私有第一位
   

};

#endif // scene_main_H
