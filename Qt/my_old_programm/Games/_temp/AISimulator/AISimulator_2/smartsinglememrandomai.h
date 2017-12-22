#ifndef SMARTSINGLEMEMRANDOMAI_H
#define SMARTSINGLEMEMRANDOMAI_H

#include "botai.h"

#include <QHash>
#include <QSet>

class SmartSingleMemRandomAI : public BotAI {
public:
    void doStep( const AIModel& model, Bot* bot );
    void reset();

private:
    QHash< int, QSet< int > > m_memory;

};

#endif // SMARTSINGLEMEMRANDOMAI_H
