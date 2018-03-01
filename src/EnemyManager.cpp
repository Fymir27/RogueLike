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
    //-----------------------------------------------------------------------------------
    cout << "Loading in enemies..." << endl;
    xml_document doc;
    xml_parse_result res = doc.load(file);

    if(!res)
    {
        cout << "Parsing of " << filename << " failed:" << endl;
        cout << res << endl;
    }
    //-----------------------------------------------------------------------------------
    auto enemies = doc.child("enemies");

    for(auto enemy : enemies.children())
    {
        string class_name = enemy.attribute("class").value();

        if(class_name == "Template")
            continue;

        class_names_.push_back(class_name);

        cout << separator << endl;
        cout << "Enemy: " <<  class_name << endl;
        string texture = enemy.attribute("texture").value();
        //-----------------------------------------------------------------------------------
        auto stats_node = enemy.child("stats");
        Stats stats(stats_node);
        cout << stats << endl;
        //-----------------------------------------------------------------------------------
        auto attack_node = enemy.child("attack");
        string attack_verb = attack_node.child("verb").child_value();
        cout << "Attacking verb: " << attack_verb << endl;
        //-----------------------------------------------------------------------------------
        auto scaling_node = attack_node.child("scaling").children();
        float scaling[5];
        size_t i = 0;
        cout << "Attack scaling:" << endl;
        for(auto stat : scaling_node)
        {
            scaling[i++] = std::stof(stat.child_value());
            cout << scaling[i-1] << endl;
        }
        //-----------------------------------------------------------------------------------
        auto reward_node = enemy.child("reward");
        size_t exp_reward = std::stoul(reward_node.child("exp").child_value());
        //-----------------------------------------------------------------------------------
        auto behaviour_node = enemy.child("behaviour");
        MoveType move_type = static_cast<MoveType>(std::stoi(behaviour_node.child("move").child_value()));
        cout << "Move type: " << move_type << endl;
        //-----------------------------------------------------------------------------------

        enemy_classes_[class_name] = std::make_unique<Enemy>(class_name, texture, stats, exp_reward,
                                                             attack_verb, scaling, move_type);

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

vector<string> EnemyManager::getEnemyClasses()
{
    return class_names_;
}
