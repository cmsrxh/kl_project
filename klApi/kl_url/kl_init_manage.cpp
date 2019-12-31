#include "events/common_log.h"
#include "kl_common.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "application.h"
#include "kl_init_manage.h"

/*
GET /v2/app/init?
appid=cn5290&
deviceid=4961c879191af84ef352e215642d569c&
os=linux&
packagename=com.edog.car.linuxwhhj&
sign=72e3649e3fd530f41995b8ebc7249050&
channel=linuxwhhj&
*/
/*
result:
{"result":{"carConfig":{"reportInterval":20},"openid":"cn52902019122310006506","carInfo":{"firstAppId":null,"marketType":"","firstAppIdName":"芜湖宏景（Linux SDK）","appIdType":"0","oem":"","carBrand":"","carType":"","developer":"3126269"}},"requestId":"cn52901577068486627650","serverTime":"1577068486634"}

*/
kl::InitManage::InitManage()
    : KLObject ("http://open.kaolafm.com/v2/app/init", NetUrl::NET_HTTP_METHOD_GET)
{
}

kl::InitManage::~InitManage()
{
}

void kl::InitManage::obtain()
{
    if (LocalConfig::instance()->openID().empty())
    {        
        if (mLoad.isLoading())
        {
            GEN_Printf(LOG_WARN, "is loading, need cancel loading.");
            mLoad.cancel();
        }

        mLoad.setLoad(genQueryUrl(), loadStatus, (void *)this);
    } else
    {
        GEN_Printf(LOG_INFO, "openid: %s is exist", LocalConfig::instance()->openID().string());
    }
}

NetUrl &kl::InitManage::genQueryUrl()
{    
    // mUrl.appendChange("sign", genSign(mUrl));
    mUrl.appendChange("sign", SIGN_InitManage);

    return mUrl;
}

void kl::InitManage::loadData(uint8_t *data, unsigned long size)
{
    // GEN_Printf(LOG_INFO, "app init data: \n%s", (char *)data);

    cJSON *root = cJSON_Parse((char *)data, size);

    cJSON *openid = json_items_proc(root, "result", "openid", NULL);
    if(openid != root)
    {
        LocalConfig::instance()->setOpenID(openid->valuestring);
        LocalConfig::instance()->save();
        Application::instance()->postKlEvent(SIG_HAVE_OPEN_ID);
        GEN_Printf(LOG_DEBUG, "openid: %s", openid->valuestring);
    } else
    {
        Application::instance()->postKlEvent(SIG_KL_INIT_ERROR, 1);
    }

    cJSON_Delete(root);
}

void kl::InitManage::loadErrorInfo(int type, const char *str)
{
    Application::instance()->postKlEvent(SIG_KL_INIT_ERROR, 1, type, str);
}

