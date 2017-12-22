/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Wapice Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef CONNECTION_P_H
#define CONNECTION_p_H

#include "requesthandler.h"

#include <atomic>
#include <mutex>
#include <qglobal.h>

namespace iot
{

class Connection;

class ConnectionPrivate
{
public:
    explicit ConnectionPrivate(Connection* parent);
    virtual ~ConnectionPrivate();

    static Connection* connectionInstance();

    RequestHandler* m_requestHandler;

protected:

    // C++11 features
    static std::atomic<Connection*> m_instance;
    static std::mutex m_mutex;

    Q_DECLARE_PUBLIC(Connection)
    Connection* const q_ptr;
};

} // namespace iot

#endif // CONNECTION_P_H
