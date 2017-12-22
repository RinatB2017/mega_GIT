#ifndef SMARTPATHFINDERAI_H
#define SMARTPATHFINDERAI_H

#include "botai.h"

class SmartPathFinderAI : public BotAI {
public:
    void doStep( const AIModel& model, Bot* bot );
};

#endif // SMARTPATHFINDERAI_H
