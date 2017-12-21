
DEPENDPATH	+= $$PWD
INCLUDEPATH 	= $$DEPENDPATH

# Input
HEADERS += \
    src/AddressBook.h \
    src/aes.h \
    src/base64.h \
    src/ClientContext.h \
    src/CryptoConst.h \
    src/Daemon.h \
    src/Destination.h \
    src/ElGamal.h \
    src/Garlic.h \
    src/hmac.h \
    src/HTTPProxy.h \
    src/HTTPServer.h \
    src/I2NPProtocol.h \
    src/I2PEndian.h \
    src/I2PTunnel.h \
    src/Identity.h \
    src/LeaseSet.h \
    src/LittleBigEndian.h \
    src/Log.h \
    src/NetDb.h \
    src/NTCPSession.h \
    src/Queue.h \
    src/Reseed.h \
    src/RouterContext.h \
    src/RouterInfo.h \
    src/SAM.h \
    src/Signature.h \
    src/SOCKS.h \
    src/SSU.h \
    src/SSUData.h \
    src/Streaming.h \
    src/Timestamp.h \
    src/TransitTunnel.h \
    src/Transports.h \
    src/Tunnel.h \
    src/TunnelBase.h \
    src/TunnelConfig.h \
    src/TunnelEndpoint.h \
    src/TunnelGateway.h \
    src/TunnelPool.h \
    src/UPnP.h \
    src/util.h \
    src/version.h

SOURCES += \
    src/AddressBook.cpp \
    src/aes.cpp \
    src/base64.cpp \
    src/ClientContext.cpp \
    src/CryptoConst.cpp \
    src/Daemon.cpp \
    src/DaemonLinux.cpp \
    src/DaemonWin32.cpp \
    src/Destination.cpp \
    src/Garlic.cpp \
    src/HTTPProxy.cpp \
    src/HTTPServer.cpp \
    src/I2NPProtocol.cpp \
    src/i2p.cpp \
    src/I2PEndian.cpp \
    src/I2PTunnel.cpp \
    src/Identity.cpp \
    src/LeaseSet.cpp \
    src/Log.cpp \
    src/NetDb.cpp \
    src/NTCPSession.cpp \
    src/Reseed.cpp \
    src/RouterContext.cpp \
    src/RouterInfo.cpp \
    src/SAM.cpp \
    src/SOCKS.cpp \
    src/SSU.cpp \
    src/SSUData.cpp \
    src/Streaming.cpp \
    src/TransitTunnel.cpp \
    src/Transports.cpp \
    src/Tunnel.cpp \
    src/TunnelEndpoint.cpp \
    src/TunnelGateway.cpp \
    src/TunnelPool.cpp \
    src/UPnP.cpp \
    src/util.cpp
