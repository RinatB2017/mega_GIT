#ifndef SMARTRANDOMAI_H
#define SMARTRANDOMAI_H

#include "botai.h"

class SmartRandomAI : public BotAI {
public:
    void doStep( const AIModel& model, Bot* bot );
};

#endif // SMARTRANDOMAI_H
