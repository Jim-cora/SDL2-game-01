#include "scene_main.h"


scene_main::scene_main() : game(game::getInstance())
{
    
}


scene_main::~scene_main()
{
}

void scene_main::update()
{
    keyboardControl();
    updateBullet(); //更新玩家子弹

    updateEnemy(); //生成敌人
    updateEnemyBullet(); //更新敌人子弹

}

void scene_main::render() 
{
    //场景 渲染玩家
    renderPlayer();
    //渲染子弹
    renderBullet();
    //渲染敌人
    renderEnemy();
    //渲染敌人子弹
    renderEnemyBullet();

}

void scene_main::handleEvent(SDL_Event *e)
{
}

void scene_main::keyboardControl()
{
    if(player.isDead) return;        //如果玩家死亡，则不处理键盘事件

    auto key_state = SDL_GetKeyboardState(NULL); //获取键盘状态
    //============================== 限制玩家位置 ===================================//
    if (player.position.x < 0){
        player.position.x = 0;
    }
    if (player.position.x > game.getWindowWidth() - player.width){
        player.position.x = static_cast<float>(game.getWindowWidth() - player.width);
    }
    if (player.position.y < 0){
        player.position.y = 0;
    }
    if (player.position.y > game.getWindowHeight() - player.height){
        player.position.y = static_cast<float>(game.getWindowHeight() - player.height);
    }

    //发射子弹
    if (key_state[SDL_SCANCODE_SPACE]){
        //发射子弹
        if (SDL_GetTicks() - projectile_player_template.lastCoolTime > projectile_player_template.coolDown){
            bulletGenerate();
            projectile_player_template.lastCoolTime = SDL_GetTicks();
        }
        
    }
    //=========================== 控制玩家位置上下左右 ================================//
    if (key_state[SDL_SCANCODE_W] || key_state[SDL_SCANCODE_UP]){
        player.position.y -= game.getDeltaTime() * player.speed;;
    }
    if (key_state[SDL_SCANCODE_S] || key_state[SDL_SCANCODE_DOWN]){
        player.position.y += game.getDeltaTime() * player.speed;;
    }
    if (key_state[SDL_SCANCODE_A] || key_state[SDL_SCANCODE_LEFT]){
        player.position.x -= game.getDeltaTime() * player.speed;;
    }
    if (key_state[SDL_SCANCODE_D] || key_state[SDL_SCANCODE_RIGHT]){
        player.position.x += game.getDeltaTime() * player.speed;;
    }
}

void scene_main::bulletGenerate()
{
    // Projectile_Player_Template bullet_p;
    auto bullet = new Projectile_Player_Template(projectile_player_template); //使用动态内存分配创建
    bullet->position.x = player.position.x + player.width / 2.0f - bullet->width / 2.0f; //子弹位置居中
    bullet->position.y = player.position.y;
    //放入容器
    projectile_player_list.push_back(bullet);
}

void scene_main::updateBullet()
{
    //用迭代器更新容器子弹
    int margin = 40;    //子弹消失的边界
    for (auto iter = projectile_player_list.begin(); iter != projectile_player_list.end();){
           auto bullet = *iter;
            bullet->position.y -= game.getDeltaTime() * bullet->speed; //子弹向上移动
            if (bullet->position.y + margin < 0){
                delete bullet;
                iter = projectile_player_list.erase(iter); //删除子弹
            }else{
                iter++; //手动更新迭代器
            }
    }
}

void scene_main::renderBullet()
{
    for (auto &bullet : projectile_player_list){
        SDL_Rect rect = {static_cast<int>(bullet->position.x), 
                        static_cast<int>(bullet->position.y),
                                        bullet->width,
                                        bullet->height};

        SDL_RenderCopy(game.getRenderer(), bullet->texture, NULL, &rect);
    }
}

void scene_main::renderPlayer()
{
    SDL_Rect rect = {static_cast<int>(player.position.x), 
                    static_cast<int>(player.position.y),
                                    player.width,
                                    player.height};

    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &rect);
    //渲染
    // SDL_RenderPresent(game.getRenderer());
}

void scene_main::enemyGenerate()
{
    auto enemy = new Enemy_Template(enemy_template);
    enemy->position.x = gen_random.getRand() * (game.getWindowWidth() - enemy->width);
    enemy->position.y = static_cast<float>(- enemy->height);
    enemy_list.push_back(enemy);

}

void scene_main::updateEnemy()
{
        //随机生成敌人
    auto current_time = SDL_GetTicks();
    float rate = (0.2f / 60.0f) * (player.level);

    if (gen_random.getRand() < rate){
        enemyGenerate();
    }
    //更新敌人
    for (auto iter = enemy_list.begin(); iter != enemy_list.end();){
        auto enemy = *iter;
        enemy->position.y += game.getDeltaTime() * enemy->speed;
        if (enemy->position.y > game.getWindowHeight()){
            delete enemy;
            iter = enemy_list.erase(iter);  
        }else{
            //发射子弹
            if (current_time - enemy->lastCoolTime > enemy->coolDown){
                enemyBulletGenerate(enemy);
                enemy->lastCoolTime = current_time;
            }

            ++iter;
        }
    }
}

void scene_main::renderEnemy()
{
    for (auto &enemy : enemy_list){
        SDL_Rect enemy_rect = {static_cast<int>(enemy->position.x), 
                        static_cast<int>(enemy->position.y),
                                        enemy->width,
                                        enemy->height};

        SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemy_rect);
    }

}

void scene_main::enemyBulletGenerate(Enemy_Template* enemy)
{
    auto bullet = new Projectile_Enemy_Template(projectile_enemy_template);
        //准备发射子弹
        bullet->position.x = enemy->position.x + enemy->width / 2.0f - bullet->width / 2.0f;
        bullet->position.y = enemy->position.y + enemy->height;
        bullet->direction = bulletDirection(enemy);
        projectile_enemy_list.push_back(bullet);

}

void scene_main::updateEnemyBullet()
{
    int margin = 30;
    for (auto iter = projectile_enemy_list.begin(); iter != projectile_enemy_list.end();){
        auto bullet = *iter;
        if (bullet->position.y > game.getWindowHeight() || 
            bullet->position.y < -(bullet->height) ||
            bullet->position.x > game.getWindowWidth() || bullet->position.x < -margin){
            delete bullet;
            iter = projectile_enemy_list.erase(iter);
        }else{
            //更新子弹
            bullet->position.x += bullet->direction.x * bullet->speed * game.getDeltaTime();
            bullet->position.y += bullet->direction.y * bullet->speed * game.getDeltaTime();
            iter++;
        }
    }
}

void scene_main::renderEnemyBullet()
{
    for (auto &bullet : projectile_enemy_list){
        SDL_Rect bullet_rect = {static_cast<int>(bullet->position.x), 
                        static_cast<int>(bullet->position.y),
                                        bullet->width,
                                        bullet->height};

        // SDL_RenderCopy(game.getRenderer(), bullet->texture, NULL, &bullet_rect);
        auto angle = atan2(bullet->direction.y, bullet->direction.x) * 180.0f / M_PI - 90.0f;
        SDL_RenderCopyEx(game.getRenderer(), bullet->texture, NULL, &bullet_rect, angle, NULL, SDL_FLIP_NONE);
    }
}

SDL_FPoint scene_main::bulletDirection(Enemy_Template *enemy)
{
    auto y = 0.0f;
    auto x = 0.0f;
    x = player.position.x + player.width / 2.0f - enemy->position.x - enemy->width / 2.0f;
    y = player.position.y + player.height / 2.0f - enemy->position.y - enemy->height / 2.0f;
    auto length = sqrt(x * x + y * y);
    if (length > 0.0001f){
        x /= length;
        y /= length;
    }
    else{
        x = 0.0f;
        y = 1.0f;
    }

    return {x, y};
}

void scene_main::init()
{
    //初始化player
    player.texture = IMG_LoadTexture(game::getInstance().getRenderer(), "game-packs\\other asserts\\pics\\Example\\05.png");
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    player.width /= 3;
    player.height /= 3;
    player.position.x = static_cast<float>(game::getInstance().getWindowWidth() / 2.0f - player.width / 2.0f);
    player.position.y = static_cast<float>(game::getInstance().getWindowHeight()/1.0 - player.height/1.0);

    //初始化 玩家子弹模板
    projectile_player_template.texture = IMG_LoadTexture(game.getRenderer(), "game-packs\\other asserts\\pics\\Bullets\\06.png");
            // 08.png 大 黄子弹
            // 03/06.png 小  基础子弹
            // 01.png 小 冰弹
            // 02.png 小 火弹
    SDL_QueryTexture(projectile_player_template.texture, 
                    NULL, 
                    NULL, 
                    &projectile_player_template.width,
                    &projectile_player_template.height);

    projectile_player_template.width /= 3;
    projectile_player_template.height /= 3;

    //初始化敌人模板
    enemy_template.texture = IMG_LoadTexture(game.getRenderer(), "game-packs\\other asserts\\ship-images\\1B.png");
    SDL_QueryTexture(enemy_template.texture,
                    NULL,
                    NULL,
                    &enemy_template.width,
                    &enemy_template.height);
    enemy_template.width /= 3;
    enemy_template.height /= 3;

    //初始化敌人子弹
    projectile_enemy_template.texture = IMG_LoadTexture(game.getRenderer(), "game-packs\\other asserts\\pics\\bullet-1.png");
    SDL_QueryTexture(projectile_enemy_template.texture,
                    NULL,
                    NULL,
                    &projectile_enemy_template.width,
                    &projectile_enemy_template.height);
    projectile_enemy_template.width /= 3;
    projectile_enemy_template.height /= 3;

}
void scene_main::clean()
{
    //清理player
    if (player.texture != nullptr){
        SDL_DestroyTexture(player.texture);
    }

    //清理玩家子弹模板
    if (projectile_player_template.texture != nullptr){
        SDL_DestroyTexture(projectile_player_template.texture);
    }

    //清理子弹
    for (auto &bullet : projectile_player_list){   
        if (bullet != nullptr){
            delete bullet;
        }
    }
    projectile_player_list.clear(); //清空子弹列表

    //清理敌人模板
    if (enemy_template.texture != nullptr){
        SDL_DestroyTexture(enemy_template.texture);
    }
    //清理敌人
    for (auto &enemy : enemy_list){
        if (enemy != nullptr){
            delete enemy;
        }
    }
    enemy_list.clear(); //清空敌人列表

    //清理敌人子弹
    if (projectile_enemy_template.texture != nullptr){
        SDL_DestroyTexture(projectile_enemy_template.texture);
    }
    for (auto &bullet : projectile_enemy_list){
        if (bullet != nullptr){
            delete bullet;
        }
    }
    projectile_enemy_list.clear(); //清空敌人子弹列表
}

