#ifndef KL_INIT_MANAGE_H
#define KL_INIT_MANAGE_H

#include "kl_object.h"

namespace kl
{

class InitManage : public KLObject
{
public:
    InitManage();
    ~InitManage();

    void obtain();

    NetUrl &genQueryUrl();

    int loadData(NetBuffer *data);
    void loadErrorInfo(int type, const char *str);
};

}

#endif // KL_INIT_MANAGE_H
