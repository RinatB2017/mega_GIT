#ifndef SINGLEMEMRANDOMAI_H
#define SINGLEMEMRANDOMAI_H

#include "botai.h"

class SingleMemRandomAI : public BotAI {
public:
    void doStep( const AIModel& model, Bot* bot );
};

#endif // SINGLEMEMRANDOMAI_H
