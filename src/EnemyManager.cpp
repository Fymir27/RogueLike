//
// Created by oliver on 01/03/18.
//

#include <fstream>
#include "EnemyManager.h"

EnemyManager* EnemyManager::instance_ = nullptr;

EnemyManager::EnemyManager()
{
    const char* filename = "data/enemies.xml";
    std::ifstream file(filename);
    if(!file.is_open())
    {
        cout << "EnemyManager failed to open file " << filename << endl;
        return;
    }

    cout << "Loading in enemies..." << endl;
    xml_document doc;
    xml_parse_result res = doc.load(file);

    if(!res)
    {
        cout << "Parsing of " << filename << " failed:" << endl;
        cout << res << endl;
    }

    auto enemies = doc.child("enemies");

    for(auto enemy : enemies.children())
    {
        cout << separator << endl;
        string class_name = enemy.attribute("class").value();

        if(class_name.compare("Template") == 0)
            continue;

        cout << "Enemy: " <<  class_name << endl;
        string texture = enemy.attribute("texture").value();

        auto stats_node = enemy.child("stats");
        Stats stats;
        stats.str_ = std::stoi(stats_node.child("str").child_value());
        stats.end_ = std::stoi(stats_node.child("end").child_value());
        stats.dex_ = std::stoi(stats_node.child("dex").child_value());
        stats.int_ = std::stoi(stats_node.child("int").child_value());
        stats.wil_ = std::stoi(stats_node.child("wil").child_value());
        cout << stats << endl;

        auto attack_node = enemy.child("attack");
        string attack_verb = attack_node.child("verb").child_value();
        cout << "Attacking verb: " << attack_verb << endl;

        float scaling[5];

        auto reward_node = enemy.child("reward");
        size_t exp_reward = std::stoul(reward_node.child("exp").child_value());

        enemy_classes_[class_name] = std::make_unique<Enemy>(class_name, texture, stats, exp_reward, attack_verb);

        cout << separator << endl << endl;
    }
}

EnemyManager* EnemyManager::getInstance()
{
    if(instance_ == nullptr)
        instance_ = new EnemyManager();
    return instance_;
}

std::shared_ptr <Enemy> EnemyManager::createEnemy(string class_name)
{
    return std::make_shared<Enemy>(enemy_classes_[class_name].get());
}

void EnemyManager::destroy()
{
    delete instance_;
    instance_ = nullptr;
}
