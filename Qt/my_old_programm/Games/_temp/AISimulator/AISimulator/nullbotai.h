#ifndef NULLBOTAI_H
#define NULLBOTAI_H

#include "botai.h"

class NullBotAI : public BotAI {
public:
    void doStep( const AIModel& model, Bot* bot );
};

#endif // NULLBOTAI_H
