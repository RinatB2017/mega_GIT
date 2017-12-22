#include "nullbotai.h"

#include "bot.h"

void NullBotAI::doStep( const AIModel&, Bot* bot ) {
    bot->stopMoving();
}
