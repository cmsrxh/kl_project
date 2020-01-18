#ifndef KL_OBJECT_H
#define KL_OBJECT_H

#include "events/common_log.h"
#include "app_common.h"
#include "kl_common.h"
#include "net_util/net_url.h"
#include "net_util/load_item.h"

namespace kl
{
enum {
    KL_DATA_PRISER_OK,          // 分析数据正确
    KL_DATA_PRISER_EMPTY,       // 分析数据正确，但是得到的数据是空
    KL_DATA_PRISER_JSOC_ERROR,  // 不能正确解析json数据
};
class KLObject
{
public:
    KLObject(const ByteString &baseUrl, int methodType);
    virtual ~KLObject();

    void obtain();

    static char *genSign(NetUrl &mUrl);

    static void loadStatus (int status, void *data, void *arg);

    virtual NetUrl &genQueryUrl() = 0;

    virtual int  loadData (NetBuffer *data) = 0;
    virtual void loadErrorInfo(int /*type*/, const char */*str*/) {}

    void loadOver()
    {
        mLoad.loadOver();
    }

    static void jsonGenAudioItem(AudioItem &item, cJSON *result);
    static void jsonGenTypeRadio(TypeRadio &item, cJSON *result);

    void uiNotifyOver()
    {
        if (m_pUINotify) m_pUINotify->dataPrepare();
    }
    void uiNotifyErrorInfo(int type, const char *str)
    {
        if (m_pUINotify) m_pUINotify->errorInfo(type, str);
    }
    void setUINotify(UINotifyIface *pUINotify)
    {
        m_pUINotify = pUINotify;
    }

protected:
    NetUrl         mUrl;
    LoadItem       mLoad;
    UINotifyIface *m_pUINotify;

};

}

#endif // KL_OBJECT_H
