#ifndef TESTREQUESTHANDLERPROVIDER_H
#define TESTREQUESTHANDLERPROVIDER_H

#include "requesthandlerprovider.h"

namespace iot
{

class TestRequestHandlerProvider : public RequestHandlerProvider
{
public:

    // because we know how tests are executed we dont have
    // to care about thread safety
    static void overrideRequestHandler(RequestHandler* handler) {
        m_instance = handler;
    }
};

} // namespace iot

#endif // TESTREQUESTHANDLERPROVIDER_H
