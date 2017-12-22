!exists(obexagent.h):system(qdbusxml2cpp -c ObexAgentAdaptor -a obexagent.h:obexagent.cpp agent.xml)
!exists(obexclient.h):system(qdbusxml2cpp -p obexclient.h:obexclient.cpp client.xml)
!exists(obexsession.h):system(qdbusxml2cpp -p obexsession.h:obexsession.cpp -i types.h session.xml)
!exists(obextransfer.h):system(qdbusxml2cpp -p obextransfer.h:obextransfer.cpp transfer.xml)

SOURCES += \
    obex/obexagent.cpp \
    obex/obexclient.cpp \
    obex/obexsession.cpp \
    obex/obextransfer.cpp \
    obex/obexftp.cpp

HEADERS += \
    obex/obexagent.h \
    obex/obexclient.h \
    obex/obexsession.h \
    obex/obextransfer.h \
    obex/obexftp.h
