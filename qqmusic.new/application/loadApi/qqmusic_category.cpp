#include "qqmusic_category.h"

/*
"http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_tag_conf.fcg?g_tk=5381&notice=0&inCharset=utf8&outCharset=utf-8&platform=yqq&needNewCode=0"
'http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_tag_conf.fcg?g_tk=5381&notice=0&inCharset=utf8&outCharset=utf-8&platform=yqq&needNewCode=0'

MusicJsonCallback({"code":0,"subcode":0,"message":"","default":0,
"data":{"categories":
    [{"categoryGroupName":"热门",
        "items":[{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"},{"sortId":1,"sortName":"默认"}],
                 "categoryId":10000000,"categoryName":"全部","usable":0}],"usable":0},
     {"categoryGroupName":"语种",
        "items":[{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":165,"categoryName":"国语","usable":1},
                 {"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":167,"categoryName":"英语","usable":1},
                 {"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":168,"categoryName":"韩语","usable":1},
                 {"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":166,"categoryName":"粤语","usable":1},
                 {"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":169,"categoryName":"日语","usable":1},
                 {"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":170,"categoryName":"小语种","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":203,"categoryName":"闽南语","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":204,"categoryName":"法语","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":205,"categoryName":"拉丁语","usable":1}],"usable":1},{"categoryGroupName":"流派","items":[{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":6,"categoryName":"流行","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":15,"categoryName":"轻音乐","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":11,"categoryName":"摇滚","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":28,"categoryName":"民谣","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":8,"categoryName":"R&#38;B","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":153,"categoryName":"嘻哈","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":24,"categoryName":"电子","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":27,"categoryName":"古典","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":18,"categoryName":"乡村","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":22,"categoryName":"蓝调","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":21,"categoryName":"爵士","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":164,"categoryName":"新世纪","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":25,"categoryName":"拉丁","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":218,"categoryName":"后摇","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":219,"categoryName":"中国传统","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":220,"categoryName":"世界音乐","usable":1}],"usable":1},{"categoryGroupName":"主题","items":[{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":39,"categoryName":"ACG","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":136,"categoryName":"经典","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":146,"categoryName":"网络歌曲","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":133,"categoryName":"影视","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":141,"categoryName":"KTV热歌","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":131,"categoryName":"儿歌","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":145,"categoryName":"中国风","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":194,"categoryName":"古风","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":148,"categoryName":"情歌","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":196,"categoryName":"城市","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":197,"categoryName":"现场音乐","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":199,"categoryName":"背景音乐","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":200,"categoryName":"佛教音乐","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":201,"categoryName":"UP主","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":202,"categoryName":"乐器","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":226,"categoryName":"MC喊麦","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":14,"categoryName":"DJ","usable":1}],"usable":1},{"categoryGroupName":"心情","items":[{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":52,"categoryName":"伤感","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":122,"categoryName":"安静","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":117,"categoryName":"快乐","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":116,"categoryName":"治愈","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":125,"categoryName":"励志","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":59,"categoryName":"甜蜜","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":55,"categoryName":"寂寞","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":126,"categoryName":"宣泄","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":68,"categoryName":"思念","usable":1}],"usable":1},{"categoryGroupName":"场景","items":[{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":78,"categoryName":"睡前","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":102,"categoryName":"夜店","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":101,"categoryName":"学习","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":99,"categoryName":"运动","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":85,"categoryName":"开车","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":76,"categoryName":"约会","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":94,"categoryName":"工作","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":81,"categoryName":"旅行","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":103,"categoryName":"派对","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":222,"categoryName":"婚礼","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":223,"categoryName":"咖啡馆","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":224,"categoryName":"跳舞","usable":1},{"allsorts":[{"sortId":2,"sortName":"最新"},{"sortId":3,"sortName":"最热"},{"sortId":4,"sortName":"评分"}],"categoryId":16,"categoryName":"校园","usable":1}],"usable":1}]}})

*/
qqmusic::Category::Category()
    : ListObject<CateNode> ("http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_tag_conf.fcg?"
                            "g_tk=5381&notice=0&inCharset=utf8&outCharset=utf-8&platform=yqq&needNewCode=0"
                            , OBJECT_NAME_CATEGORY
                            , NetUrl::NET_HTTP_METHOD_GET)
{
    // test
    setSaveFile("/tmp/qqmusic.category.list");
}

NetUrl &qqmusic::Category::genQueryUrl()
{
    return mUrl;
}

int qqmusic::Category::genResult(NetBuffer *data)
{
    int ret = KL_DATA_PRISER_OK;
    //GEN_Printf(LOG_DEBUG, "size: %lu\n%s", data->size(), (char *)data->buffer());
    cJSON *root = cJSON_Parse((char *)data->buffer() + 18, data->size() - 19);
    cJSON *obj;
    cJSON *categories = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "data"), "categories");
    if (categories)
    {
        for (cJSON *item = categories->child; NULL != item; item = item->next )
        {
            CateNode tmp;

            JSON_VALUETRING_SWAP_BYTESTRING(item, categoryGroupName, tmp.categoryGroupName);
            cJSON *items = cJSON_GetObjectItem(item, "items");
            if (items)
            {
                for (cJSON *item = items->child; NULL != item; item = item->next )
                {
                    CateNode::Cate cate;
                    cate.categoryId = (obj = cJSON_GetObjectItem(item, "categoryId"))  ? atoi(obj->valuestring) : 0;
                    JSON_VALUETRING_SWAP_BYTESTRING(item, categoryName, cate.categoryName);
                    tmp.items.push_back(cate);
                }
            }
            // GEN_Printf(LOG_DEBUG, "items size=%d", tmp.items.size());
            mNodes.push_back(tmp);
        }
        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load Category is empty.");
            ret = KL_DATA_PRISER_EMPTY;
        }/* else
        {
            profile();
        }*/
    } else
    {
        GEN_Printf(LOG_ERROR, "priser failed, size: %lu\n%s", data->size(), data->buffer());
        ret = KL_DATA_PRISER_JSOC_ERROR;
    }

    cJSON_Delete(root);
    return ret;
}

void qqmusic::Category::profile()
{
    ListTable<CateNode>::iterator it = mNodes.begin();
    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "categoryGroupName: %s, items: %d", it->categoryGroupName.string(), it->items.size());
        ListTable<CateNode::Cate>::iterator itS = it->items.begin();
        for (; itS != it->items.end(); ++itS)
        {
            printf("\t\t[%d]=%s\n", itS->categoryId, itS->categoryName.string());
        }
        fflush(stdout);
    }
}
