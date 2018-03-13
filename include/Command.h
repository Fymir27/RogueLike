//
// Created by Oliver on 13.03.2018.
//

#pragma once

#include "Common.h"
#include "Utils.h"

enum ECommandType
{
    ESC,
    CONFIRM,
    SPACE,
    DIRECTION,
    NUMBER,
    USEITEM,
    THROWITEM,
    DROPITEM,
    CASTSPELL
};

class Command
{
    public:

    Command(sf::Keyboard::Key const& key); // create new command from pressed key
    bool execute();                        // returns true when successfully executed

    inline bool         isValid() { return valid_; }
    inline ECommandType getType() { return type_; }

    private:

    static vector<unique_ptr<Command>> history_; // keeps track of all previous commands

    bool         valid_ = true;
    ECommandType type_ ;
    Direction    dir_;
    int          nr_;
    bool         self_ = false;
};



