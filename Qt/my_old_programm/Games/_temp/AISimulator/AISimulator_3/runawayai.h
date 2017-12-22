#ifndef RUNAWAYAI_H
#define RUNAWAYAI_H

#include "botai.h"

class RunAwayAI : public BotAI {
public:
    void doStep( const AIModel& model, Bot* bot );
};

#endif // RUNAWAYAI_H
