#ifndef ACCUMULATINGRANDOMAI_H
#define ACCUMULATINGRANDOMAI_H

#include "botai.h"

#include <QHash>

class AccumulatingRandomAI : public BotAI {
public:
    void doStep( const AIModel& model, Bot* bot );
    void reset();

private:
    QHash< int, std::vector< double > > m_memory;
};

#endif // ACCUMULATINGRANDOMAI_H
