//
// Created by oliver on 01/03/18.
//

#ifndef ROGUELIKE_ENEMYMANAGER_H
#define ROGUELIKE_ENEMYMANAGER_H

#include "Common.h"
#include "Enemy.h"

//singleton
class EnemyManager
{
public:
    static EnemyManager* getInstance();
    static void destroy();
private:
    static EnemyManager* instance_;

public:
    shared_ptr<Enemy> createEnemy(string class_name);
    vector<string> getEnemyClasses();
private:
    EnemyManager();

    //class_name -> template entity
    map<string, unique_ptr<Enemy>> enemy_classes_;
    vector<string> class_names_;
};


#endif //ROGUELIKE_ENEMYMANAGER_H
