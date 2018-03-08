#include <fstream>
#include "Enemy.h"
#include "Room.h"
#include "Player.h"
#include "UI.h"

bool Enemy::step()
{
    advanceEffects();
    if(dead())
        return false;

	switch (move_type_)
	{
	case SLEEP:
		break;

	case STAY:
		checkSurroundings();
		break;

	case WAIT:
		checkSurroundings();
		break;

	case FOLLOW:
		move(current_room->getPathToPlayer(pos_));
		break;

	case RANDOM:
		moveRandomly();
		break;
	}
		
	return true;
}

Enemy::Enemy(string name, Position pos, Stats stats, MoveType move_type, string texture) : Character(name, pos, stats, texture), move_type_(move_type)
{
	//current_room->addEnemy(this);
}

bool Enemy::checkSurroundings()
{
	for(size_t i = 0; i < 4; i++)
	{
		if(current_room->getField(pos_ + DELTA_POS[i])->getFieldStatus() == OCCUPIED)
		{
			move(pos_ + DELTA_POS[i]);
			if(pos_ + DELTA_POS[i] == current_player->getPosition())
				move_type_ = FOLLOW;
			return true;
		}
	}		
}

void Enemy::moveRandomly()
{
	int counter = 0;
	int r = random_engine() % 4;
	do 
	{ 
		r = (r+1) % 4; 
		if(counter++ > 3) //trapped
		{
			move_type_ = SLEEP;
			break;
		}
	} while (!move(Position(pos_.x_ + DELTA_X[r], pos_.y_ + DELTA_Y[r])));
}

void Enemy::damage(unsigned int amount)
{
    Character::damage(amount);
    if(move_type_ == WAIT)
        move_type_ = FOLLOW;
}

Enemy::~Enemy()
{
	UI::displayText(name_ + " defeated. Gained " + std::to_string(exp_reward_) + " Exp.");
	current_player->grantExp(exp_reward_);
}

Enemy::Enemy(string name, string texture, Stats stats, size_t exp_reward, string attack_verb, float scaling[5], MoveType move_type) :
		Character(name, {0,0}, stats, texture), move_type_(move_type), exp_reward_(exp_reward), attack_verb_(attack_verb)
{
    std::copy(scaling, scaling + 5, scaling_);
}

void Enemy::attack(Character* target)
{
    auto damage = (unsigned)std::roundf(  stats_.str_ * scaling_[0] +
                                    stats_.end_ * scaling_[1] +
                                    stats_.dex_ * scaling_[2] +
                                    stats_.int_ * scaling_[3] +
                                    stats_.wil_ * scaling_[4] );

    UI::displayText(name_ + ' ' + attack_verb_ + ' ' + target->getName() + " for " + std::to_string(damage) + " damage.");
    target->damage(damage);
}

Enemy::Enemy(Enemy* orig) : Character(orig), move_type_(orig->move_type_),
                            exp_reward_(orig->exp_reward_), attack_verb_(orig->attack_verb_)
{
    std::copy(orig->scaling_, orig->scaling_ + 5, scaling_);
}

void Enemy::setSpawnConditions(list<Biomes::Condition*> conditions)
{
    cout << "Setting SpawnConditions" << endl;
    spawn_conditions_ = conditions;
}

list<Biomes::Condition*> Enemy::getSpawnConditions() const
{
    return spawn_conditions_;
}



template<>
Factory<Enemy>::Factory()
{
    const char* filename = "data/enemies.xml";
    std::ifstream file(filename);
    if(!file.is_open())
    {
        cout << "Factory<Enemy> failed to open file " << filename << endl;
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
        string name = enemy.attribute("name").value();

        if (name == "Template")
            continue;

        entity_names_.push_back(name);

        cout << separator << endl;
        cout << "Enemy: " << name << endl;
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
        for (auto stat : scaling_node)
        {
            scaling[i++] = std::stof(stat.child_value());
            cout << scaling[i - 1] << endl;
        }
        //-----------------------------------------------------------------------------------
        auto reward_node = enemy.child("reward");
        size_t exp_reward = std::stoul(reward_node.child("exp").child_value());
        //-----------------------------------------------------------------------------------
        auto behaviour_node = enemy.child("behaviour");
        MoveType move_type = static_cast<MoveType>(std::stoi(behaviour_node.child("move").child_value()));
        cout << "Move type: " << move_type << endl;
        //-----------------------------------------------------------------------------------
        auto condition_node = enemy.child("conditions");
        cout << "Spawn conditions:" << endl;
        list<Biomes::Condition*> conditions;
        for(auto condition : condition_node)
        {
            cout << condition.name() << " " << condition.attribute("equality").value() << " " << condition.child_value() << endl;
            if(string(condition.name()) == "temperature")
            {
                Biomes::Temperature attr = { std::stoi(condition.child_value()) };
                auto comp = getEnumFromAttribute<EComparison>(condition.attribute("comparison"));
                conditions.push_back(new Biomes::AttributeCondition<Biomes::Temperature>(attr, comp));
            }
            else if(string(condition.name()) == "humidity")
            {
                Biomes::Humidity attr = { std::stoi(condition.child_value()) };
                auto comp = getEnumFromAttribute<EComparison>(condition.attribute("comparison"));
                conditions.push_back(new Biomes::AttributeCondition<Biomes::Humidity>(attr, comp));
            }
            else if(string(condition.name()) == "floor")
            {
                Biomes::FloorType attr = { getEnumFromNode<Biomes::EFloorType>(condition) };
                auto comp = getEnumFromAttribute<EComparison>(condition.attribute("comparison"));
                conditions.push_back(new Biomes::AttributeCondition<Biomes::FloorType>(attr, comp));
            }
        }

        //-----------------------------------------------------------------------------------

        entity_templates_[name] = std::make_unique<Enemy>(name, texture, stats, exp_reward,
                                                                attack_verb, scaling, move_type);
        entity_templates_[name]->setSpawnConditions(conditions);
    }
}



