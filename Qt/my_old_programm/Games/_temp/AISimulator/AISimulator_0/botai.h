#ifndef BOTAI_H
#define BOTAI_H

class AIModel;
class Bot;

class BotAI {
public:
    virtual ~BotAI();

    virtual void doStep( const AIModel& model, Bot* bot ) = 0;
};

#endif // BOTAI_H
