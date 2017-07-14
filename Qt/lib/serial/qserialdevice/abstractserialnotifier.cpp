#include "abstractserialnotifier.h"
#include "nativeserialnotifier.h"


AbstractSerialNotifier *AbstractSerialNotifier::createSerialNotifier(NativeSerialEngine *parent)
{
    return new NativeSerialNotifier(parent);
}

void AbstractSerialNotifier::setDescriptor(Serial::DESCRIPTOR descriptor)
{
    if (this->descriptor != descriptor)
        this->descriptor = descriptor;
}
