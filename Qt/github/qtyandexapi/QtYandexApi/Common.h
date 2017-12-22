#ifndef COMMON_H
#define COMMON_H

template<typename MetaType> class MetaTypeRegister
{
public:
    MetaTypeRegister(const char* metaTypeName) {
        qRegisterMetaType<MetaType>(metaTypeName);
    }
};


#endif // COMMON_H
