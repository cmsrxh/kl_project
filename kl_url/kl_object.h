#ifndef KL_OBJECT_H
#define KL_OBJECT_H

#include <stdio.h>
#include "app_common.h"
#include "net_util/net_url.h"
#include "net_util/load_item.h"

class LoadItem;
namespace kl
{

class KLObject
{
public:
    KLObject(const ByteString &baseUrl, int methodType);
    virtual ~KLObject();

    void obtain();

    static char *genSign(NetUrl &mUrl);

    static void loadStatus (int status, uint8_t *data, size_t len, void *arg);

    virtual NetUrl &genQueryUrl() = 0;

    virtual void loadData (uint8_t *data, unsigned long size) = 0;

    void loadOver()
    {
        mLoad.loadOver();
    }
protected:
    NetUrl    mUrl;
    LoadItem  mLoad;
};

}

#endif // KL_OBJECT_H
