#ifndef PATHFINDERAI_H
#define PATHFINDERAI_H

#include "botai.h"

class PathFinderAI : public BotAI {
public:
    void doStep( const AIModel& model, Bot* bot );
};

#endif // PATHFINDERAI_H
