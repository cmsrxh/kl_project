#include "events/common_log.h"
#include "net_util/net_common.h"
#include "net_util/http_util.h"
#include "config_local_info.h"
#include "kl_res_suggestion_word.h"

/*
/v2/resource/suggestionword?word=������&limit=10&appid=cn5290&openid=cn52902019122410012497&deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&                                                                                                                                                                                                                                                                                                                                          1                                                       0.0000        2.38%         0.0100        90.802

{"result":["成真恋爱学","城市过客 周四","晨曦微露的声音世界","成为冠军销售员黄金能力","盛世谋臣","臣妾做不到","成全了自己的碧海蓝天","沈阳慕马大案纪实","陈丹青聊鲁迅","陈茂源星座","尘翳 又名莫残溪","成年人情感指南"],"debugInfos":{},"serverTime":1578131939644,"requestId":"cn5290157813193964493"}
*/
kl::SuggestionWord::SuggestionWord(const ByteString &suggestWord, int limit)
    : SaveObject<SuggestItem> ("http://open.kaolafm.com/v2/resource/suggestionword", NetUrl::NET_HTTP_METHOD_GET)
    , mLimit(ByteString::allocLong(limit))
{
    HttpUtil::urlEncoding(suggestWord, mSuggestWord);
}

kl::SuggestionWord::~SuggestionWord()
{
    mSuggestWord.clear();
    mLimit.clear();
}

NetUrl &kl::SuggestionWord::genQueryUrl()
{
    mUrl.appendChange("word", mSuggestWord);
    mUrl.appendChange("limit", mLimit);

    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_SuggestionWord);

    return mUrl;
}

void kl::SuggestionWord::profile()
{
    ListTable<SuggestItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "Suggest: %s", it->result.string());
    }
}

int kl::SuggestionWord::genResult(NetBuffer *data)
{
    int ret = KL_DATA_PRISER_OK;
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());
    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (result)
    {
        for (cJSON *item = result->child; NULL != item; item = item->next )
        {
            SuggestItem tmp;

            tmp.result = item->valuestring;
            item->valuestring = NULL;

            mNodes.push_back(tmp);
        }

        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load voice search list is empty.");
            ret = KL_DATA_PRISER_EMPTY;
        }/* else
        {
            profile();
        }*/
    }else
    {
        GEN_Printf(LOG_ERROR, "priser failed, size: %lu\n%s", data->size(), data->buffer());
        ret = KL_DATA_PRISER_JSOC_ERROR;
    }

    cJSON_Delete(root);
    return ret;
}
