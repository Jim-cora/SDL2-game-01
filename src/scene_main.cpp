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
    updatePlayer();
    updateBullet(); //更新玩家子弹

    updateEnemy(); //生成敌人
    updateEnemyBullet(); //更新敌人子弹

    updateExplosion(); //更新爆炸效果
    updateBonus(); //更新道具

}

void scene_main::render() 
{
    if(!player.isDead){
        //场景 渲染玩家
        renderPlayer();
    }
    //渲染子弹
    renderBullet();
    //渲染敌人
    renderEnemy();
    //渲染敌人子弹
    renderEnemyBullet();
    //渲染道具
    renderBonus();
    //渲染爆炸效果
    renderExplosion(); 

}

void scene_main::handleEvent(SDL_Event *)
{
}

void scene_main::keyboardControl()
{
    if(player.isDead) return;        //如果玩家死亡，则不处理键盘事件

    auto key_state = SDL_GetKeyboardState(NULL); //获取键盘状态

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
    if (player.isDead) return;        //如果玩家死亡，则不生成子弹
    // Projectile_Player_Template bullet_p;
    auto bullet = new Projectile_Player_Template(projectile_player_template); //使用动态内存分配创建
    bullet->position.x = player.position.x + player.width / 2.0f - bullet->width / 2.0f; //子弹位置居中
    bullet->position.y = player.position.y;
    //放入容器
    projectile_player_list.push_back(bullet);
    //播放音效
    Mix_PlayChannel(0, scene_sound_map[SoundType::Player_Shoot], 0);

}

void scene_main::updateBullet()
{
    //用迭代器更新容器子弹
    int margin = 40;    //子弹消失的边界
    for (auto iter = projectile_player_list.begin(); iter != projectile_player_list.end();){
           auto &bullet = *iter;
            bullet->position.y -= game.getDeltaTime() * bullet->speed; //子弹向上移动
            if (bullet->position.y + margin < 0){
                delete bullet;
                iter = projectile_player_list.erase(iter); //删除子弹
            }else{
                //遍历敌人
                bool hit_flag = false;
                for(auto &enemy : enemy_list){
                    SDL_Rect rect_bullet = {static_cast<int>(bullet->position.x),
                                            static_cast<int>(bullet->position.y),
                                            bullet->width,  
                                            bullet->height};
                    SDL_Rect rect_enemy = {static_cast<int>(enemy->position.x),
                                            static_cast<int>(enemy->position.y),
                                            enemy->width,
                                            enemy->height};
                    if (SDL_HasIntersection(&rect_bullet, &rect_enemy)){
                        enemy->health -= bullet->damage;
                        delete bullet;
                        iter = projectile_player_list.erase(iter); //删除子弹
                        hit_flag = true;
                        break; //跳出循环
                    }
                }
                if (!hit_flag) iter++; //手动更新迭代器
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

void scene_main::updatePlayer()
{
    if (player.isDead) return; //如果玩家死亡，则不渲染

    if(player.health <= 0){

        player.isDead = true;
        //TODO: 游戏结束
        //explode player
        auto explode = new explosion_template(explosion_base);
        explode->position.x = player.position.x + player.width / 2.0f - explode->frame_width * 2 / 2.0f;
        explode->position.y = player.position.y + player.height / 2.0f - explode->frame_height * 2 / 2.0f;
        explode->startTime = SDL_GetTicks();
        explode->current_frame = 0; //从第一帧开始播放
        explosion_list.push_back(explode);
        Mix_PlayChannel(-1, scene_sound_map[SoundType::Enemy_base_Die], 0);

        Mix_PlayChannel(1, scene_sound_map[SoundType::Player_Die], 0);
        return;
    }

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
    float rate = (0.1f / 30.0f) * (player.level);

    if (gen_random.getRand() < rate){
        enemyGenerate();
    }
    //更新敌人
    for (auto iter = enemy_list.begin(); iter != enemy_list.end();){
        auto &enemy = *iter;
        enemy->position.y += game.getDeltaTime() * enemy->speed;
        if (enemy->position.y > game.getWindowHeight()){
            delete enemy;
            iter = enemy_list.erase(iter);  
        }else{
            if (!player.isDead){
                //检测碰撞
                SDL_Rect enemy_rect = {static_cast<int>(enemy->position.x),
                                        static_cast<int>(enemy->position.y),
                                                enemy->width,
                                                enemy->height};
                SDL_Rect player_rect = {static_cast<int>(player.position.x),
                                        static_cast<int>(player.position.y),
                                                player.width,
                                                player.height}; 
                if (SDL_HasIntersection(&enemy_rect, &player_rect)){
                    player.health -= 1;
                    enemy->health = 0;
                }
            }
            if(enemy->health <= 0){
                //敌人爆炸
                auto explode = new explosion_template(explosion_base);
                explode->position.x = enemy->position.x + enemy->width / 2.0f - explode->frame_width * 2 / 2.0f;
                explode->position.y = enemy->position.y + enemy->height / 2.0f - explode->frame_height * 2 / 2.0f;
                explode->startTime = SDL_GetTicks();
                explode->current_frame = 0; //从第一帧开始播放
                explosion_list.push_back(explode); //加入爆炸列表
                if(gen_random.getRand() < enemy->dropRate) ItemGenerate(enemy); //生成道具 0.5的概率
                delete enemy;
                iter = enemy_list.erase(iter); 
                Mix_PlayChannel(-1, scene_sound_map[SoundType::Enemy_base_Die], 0); //播放音效
                continue;
            
            }
            //发射子弹
            if (current_time - enemy->lastCoolTime > enemy->coolDown && player.isDead == false){
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
        Mix_PlayChannel(-1, scene_sound_map[SoundType::Enemy_Shoot], 0);

}

void scene_main::updateEnemyBullet()
{
    int margin = 30;
    for (auto iter = projectile_enemy_list.begin(); iter != projectile_enemy_list.end();){
        auto &bullet = *iter;
        if (bullet->position.y > game.getWindowHeight() || 
            bullet->position.y < -(bullet->height) ||
            bullet->position.x > game.getWindowWidth() || bullet->position.x < -margin){
            delete bullet;
            iter = projectile_enemy_list.erase(iter);
        }else{
            //检测碰撞
            SDL_Rect bullet_rect = {static_cast<int>(bullet->position.x),
                                    static_cast<int>(bullet->position.y),
                                            bullet->width,
                                            bullet->height};    
            SDL_Rect player_rect = {static_cast<int>(player.position.x),
                                    static_cast<int>(player.position.y),
                                            player.width,
                                            player.height}; 
            if (SDL_HasIntersection(&bullet_rect, &player_rect) && player.isDead == false){
                player.health -= 1;
                delete bullet;
                iter = projectile_enemy_list.erase(iter);
                Mix_PlayChannel(-1, scene_sound_map[SoundType::Player_Hit], 0);
                continue;
            }
            //更新子弹
            bullet->position.x += bullet->direction.x * bullet->speed * game.getDeltaTime();
            bullet->position.y += bullet->direction.y * bullet->speed * game.getDeltaTime();
            ++iter; 
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

void scene_main::updateExplosion()
{
    for(auto iter = explosion_list.begin(); iter != explosion_list.end();){
        auto &explode = *iter;
        auto current_time = SDL_GetTicks();
        auto elapsed_time = (current_time - explode->startTime) / 1000.0f;
        explode->current_frame = static_cast<int>(elapsed_time * explode->fps);
        if(explode->current_frame >= explode->total_frames){
            //爆炸结束
            delete explode;
            iter = explosion_list.erase(iter);
            continue;
        }
        ++iter;
        std::cout << "current_frame: " << explode->current_frame << std::endl;
    }
}

void scene_main::renderExplosion()
{
    for (auto &explode : explosion_list){
        // 爆炸特效4*4序列帧
        SDL_Rect src_rect = {explode->frame_width * (explode->current_frame % (explode->width / explode->frame_width)), 
                            explode->frame_height * (explode->current_frame / (explode->width / explode->frame_width)),
                            explode->frame_width,
                            explode->frame_height}; //源矩形

        SDL_Rect dst_rect = {static_cast<int>(explode->position.x), 
                            static_cast<int>(explode->position.y),
                                        explode->frame_width * 2,
                                        explode->frame_height * 2}; //目标矩形
        SDL_RenderCopy(game.getRenderer(), explode->texture, &src_rect, &dst_rect);

    }

}

// 生成物品 目前主要是 bonus_item
void scene_main::ItemGenerate(Enemy_Template *enemy)
{
    auto item = new Item(bonus_item);
    // 随机生成概率
    int randType_num = static_cast<int>(gen_random.getRand() * 4); //[0, 4)
    auto rand_ang = (gen_random.getRand() * 2 * M_PI); //[0, 2pi]

    std::cout << "randType_num: " << randType_num << "\t rand_ang:"<< rand_ang << std::endl;
    //物品设置
    item->type = static_cast<Bonus_TextureType>(randType_num % item->total_cls); // 0-3
    // Bonus_TextureType type = static_cast<Bonus_TextureType>(randType_num % item->total_num); // 0-3
    item->texture = bonus_texture_map[item->type]; //随机物品类型
    SDL_SetTextureBlendMode(item->texture, SDL_BLENDMODE_ADD); //设置透明度
    SDL_QueryTexture(item->texture, NULL, NULL, &item->width, &item->height); //物品大小
    item->width /= 3;
    item->height /= 3;
    item->position.x = enemy->position.x + enemy->width / 2.0f - item->width / 2.0f;
    item->position.y = enemy->position.y + enemy->height / 2.0f - item->height / 2.0f;
    item->direction.x = static_cast<float>(cos(rand_ang));
    item->direction.y = static_cast<float>(sin(rand_ang)); //随机方向
    item->speed = 50;
    bonus_list.push_back(item); //物品加入容器
}

void scene_main::updateBonus()
{   
    for(auto iter = bonus_list.begin(); iter != bonus_list.end();){
        auto &bonus = *iter;
        //更新物品位置
        bonus->position.x += bonus->direction.x * bonus->speed * game.getDeltaTime();
        bonus->position.y += bonus->direction.y * bonus->speed * game.getDeltaTime();

        if(bonus->position.x > (game.getWindowWidth() - bonus->width) && bonus->bounceCount > 0){
            bonus->direction.x = -bonus->direction.x;
            bonus->bounceCount--;
        }else if(bonus->position.x < 0 && bonus->bounceCount > 0){
            bonus->direction.x = -bonus->direction.x;
            bonus->bounceCount--;
        }else if(bonus->position.y > (game.getWindowHeight() - bonus->height) && bonus->bounceCount > 0){
            
            bonus->direction.y = -bonus->direction.y;
            bonus->bounceCount--;
        }else if(bonus->position.y < 0 && bonus->bounceCount > 0){
            bonus->direction.y = -bonus->direction.y;
            bonus->bounceCount--;
        }else{
            if (bonus->position.y > game.getWindowHeight() || bonus->position.y < -bonus->height||
                bonus->position.x > game.getWindowWidth() || bonus->position.x < -bonus->width){
                delete bonus;
                iter = bonus_list.erase(iter);
                continue; 
            }
        }
        //检测碰撞
        SDL_Rect bonus_rect = {static_cast<int>(bonus->position.x),
                                static_cast<int>(bonus->position.y),
                                        bonus->width,
                                        bonus->height};    
        SDL_Rect player_rect = {static_cast<int>(player.position.x),
                                static_cast<int>(player.position.y),    
                                player.width,
                                player.height};
        if(SDL_HasIntersection(&bonus_rect, &player_rect)){
            //碰撞检测
            switch (bonus->type)
            {
                case Bonus_TextureType::extra_life:
                    if (player.health >= player.max_health)
                    {
                        player.health = player.max_health;
                    }else{
                        player.health++;
                    }
                    break;
                // case Bonus_TextureType::extra_shield:

                // case Bonus_TextureType::extra_support:

                // case Bonus_TextureType::freeze_time:     
            }
            delete bonus;
            iter = bonus_list.erase(iter);
            Mix_PlayChannel(-1, scene_sound_map[SoundType::Get_Bonus], 0);
            continue;
        }
        ++iter; 
    }
}

void scene_main::renderBonus()
{
    for(auto &bonus : bonus_list){
        SDL_Rect dst_rect = {static_cast<int>(bonus->position.x),
                            static_cast<int>(bonus->position.y),
                            bonus->width,
                            bonus->height};
        SDL_RenderCopy(game.getRenderer(), bonus->texture, NULL, &dst_rect);
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
    player.texture = IMG_LoadTexture(game.getRenderer(), "game-packs\\other-asserts\\pics\\Example\\05.png");
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    player.width /= 3;
    player.height /= 3;
    player.position.x = static_cast<float>(game.getWindowWidth() / 2.0f - player.width / 2.0f);
    player.position.y = static_cast<float>(game.getWindowHeight()/1.0 - player.height/1.0);

    //初始化 玩家子弹模板
    projectile_player_template.texture = IMG_LoadTexture(game.getRenderer(), "game-packs\\other-asserts\\pics\\Bullets\\06.png");
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
    enemy_template.texture = IMG_LoadTexture(game.getRenderer(), "game-packs\\other-asserts\\ship-images\\1B.png");
    SDL_QueryTexture(enemy_template.texture,
                    NULL,
                    NULL,
                    &enemy_template.width,
                    &enemy_template.height);
    enemy_template.width /= 3;
    enemy_template.height /= 3;

    //初始化敌人子弹
    projectile_enemy_template.texture = IMG_LoadTexture(game.getRenderer(), "game-packs\\other-asserts\\pics\\bullet-1.png");
    SDL_QueryTexture(projectile_enemy_template.texture,
                    NULL,
                    NULL,
                    &projectile_enemy_template.width,
                    &projectile_enemy_template.height);
    projectile_enemy_template.width /= 3;
    projectile_enemy_template.height /= 3;

    //初始化爆炸基础版 xpld1 4*4
    explosion_base.texture = IMG_LoadTexture(game.getRenderer(), "game-packs\\explosion\\xpld1.jpg");
    SDL_QueryTexture(explosion_base.texture,
                    NULL,
                    NULL,
                    &explosion_base.width,
                    &explosion_base.height
                    );
    explosion_base.frame_height = explosion_base.height / 4; //一帧高度
    explosion_base.frame_width = explosion_base.width / 4;
    explosion_base.total_frames = (explosion_base.height / explosion_base.frame_height) * \
                                    (explosion_base.width / explosion_base.frame_width); //总帧数
    // explosion_base.fps = 16; //帧率
    //混合模式.加强显示效果
    SDL_SetTextureBlendMode(explosion_base.texture, SDL_BLENDMODE_ADD);

    //================================初始化效果图片资源 =====================================
    bonus_texture_map[Bonus_TextureType::extra_life] = IMG_LoadTexture(game.getRenderer(), \
                                                        "game-packs/texture/SpaceShooterPack/PNG/bonus_life.png");
    if (bonus_texture_map[Bonus_TextureType::extra_life] == nullptr){
        std::cout << "Failed to load bonus texture! Error: " << SDL_GetError() << std::endl;
    }
    bonus_texture_map[Bonus_TextureType::extra_shield] = IMG_LoadTexture(game.getRenderer(), \
                                                    "game-packs/texture/SpaceShooterPack/PNG/bonus_shield.png");
    if (bonus_texture_map[Bonus_TextureType::extra_shield] == nullptr){
        std::cout << "Failed to load bonus texture! Error: " << SDL_GetError() << std::endl;
    }
    bonus_texture_map[Bonus_TextureType::extra_support] = IMG_LoadTexture(game.getRenderer(), \
                                                        "game-packs/texture/SpaceShooterPack/PNG/support.png");
    if (bonus_texture_map[Bonus_TextureType::extra_support] == nullptr){
        std::cout << "Failed to load bonus texture! Error: " << SDL_GetError() << std::endl;
    }
    bonus_texture_map[Bonus_TextureType::freeze_time] = IMG_LoadTexture(game.getRenderer(), \
                                                        "game-packs/texture/SpaceShooterPack/PNG/bonus_time.png");
    if (bonus_texture_map[Bonus_TextureType::freeze_time] == nullptr){
        std::cout << "Failed to load bonus texture! Error: " << SDL_GetError() << std::endl;
    }
    
    //=====================================初始化音效库=====================================
    scene_sound_map[SoundType::Player_Shoot] = Mix_LoadWAV("game-packs/音乐/hit.mp3");  //1，4，9
    if (scene_sound_map[SoundType::Player_Shoot] == nullptr){
        std::cout << "Failed to load sound effect! Error: " << Mix_GetError() << std::endl;
    }
    scene_sound_map[SoundType::Enemy_Shoot] = Mix_LoadWAV("game-packs/sounds/Pew__009.ogg");
    if (scene_sound_map[SoundType::Enemy_Shoot] == nullptr){
        std::cout << "Failed to load sound effect! Error: " << Mix_GetError() << std::endl;
    }
    scene_sound_map[SoundType::Player_Hit] = Mix_LoadWAV("game-packs/sounds/Pew__006.ogg");
    if (scene_sound_map[SoundType::Player_Hit] == nullptr){
        std::cout << "Failed to load sound effect! Error: " << Mix_GetError() << std::endl;
    }
    scene_sound_map[SoundType::Enemy_base_Die] = Mix_LoadWAV("game-packs/音乐/explosion.mp3");
    if (scene_sound_map[SoundType::Enemy_base_Die] == nullptr){
        std::cout << "Failed to load sound effect! Error: " << Mix_GetError() << std::endl;
    }
    scene_sound_map[SoundType::Player_Die] = Mix_LoadWAV("game-packs/音乐/Warp Jingle.wav");
    if (scene_sound_map[SoundType::Player_Die] == nullptr){
        std::cout << "Failed to load sound effect! Error: " << Mix_GetError() << std::endl;
    }
    scene_sound_map[SoundType::Get_Bonus] = Mix_LoadWAV("game-packs/攻击音效-hits/hit30.mp3.flac");
    if (scene_sound_map[SoundType::Get_Bonus] == nullptr){
        std::cout << "Failed to load sound effect! Error: " << Mix_GetError() << std::endl;
    }
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

    //清理爆炸
    if (explosion_base.texture != nullptr){
        SDL_DestroyTexture(explosion_base.texture);
    }
    for (auto &explosion : explosion_list){
        if (explosion != nullptr){
            delete explosion;
        }
    }
    explosion_list.clear(); //清空爆炸列表

    //清理效果图片资源
    for (auto &bonus_texture : bonus_texture_map){
        if (bonus_texture.second != nullptr){
            SDL_DestroyTexture(bonus_texture.second);
        }
    }

    bonus_texture_map.clear(); //清空效果图片资源列表

    //清理音效库
    for( auto &sound : scene_sound_map){
        if (sound.second != nullptr){
            Mix_FreeChunk(sound.second);
        }
    }
    scene_sound_map.clear(); //清空音效库


}

