#ifndef EASYRANDOMAI_H
#define EASYRANDOMAI_H

#include "botai.h"

class EasyRandomAI : public BotAI {
public:
    void doStep( const AIModel& model, Bot* bot );
};

#endif // EASYRANDOMAI_H
