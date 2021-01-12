#include "qqmusic_category_playlist.h"

/*
`http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_by_tag.fcg?picmid=1&rnd=${Math.random()}&g_tk=5381&jsonpCallback=getPlaylist&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&categoryId=${categoryId}&sortId=5&sin=${(page - 1) * 30}&ein=${page * 30 - 1}`

//OK
string[271]=http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_by_tag.fcg?picmid=1&rnd=0.9214369031553655&g_tk=5381&jsonpCallback=getPlaylist&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&categoryId=10000000&sortId=5&sin=0&ein=29
//ERROR
string[271]=http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_by_tag.fcg?picmid=1?rnd=0.9214369031553655&g_tk=5381&jsonpCallback=getPlaylist&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&categoryId=10000000&sortId=5&sin=0&ein=29

getPlaylist({"code":0,"subcode":0,"message":"","default":0,
"data":{"uin":0,"categoryId":10000000,"sortId":5,"sum":8861,"sin":0,"ein":29,
"list":[{"dissid":"7279198418","createtime":"2019-10-31","commit_time":"2019-10-31","dissname":"华语流行|简单的美好","imgurl":"http://qpic.y.qq.com/music_cover/MKjEtF7diatibd6B0iaeF5KgsnMIwVcdbu0NicU6iak88kCiceR64r6icUgsg/300?n=1&n=1","introduction":"","listennum":1156152,"score":0.0,"version":0,"creator":{"type":6,"qq":2783429033,"encrypt_uin":"owSFoivANKnioz**","name":"QQ音乐官方歌单","isVip":6,"avatarUrl":"","followflag":0}},
        {"dissid":"7100401222","createtime":"2019-07-18","commit_time":"2019-07-18","dissname":"粵声乐色","imgurl":"http://qpic.y.qq.com/music_cover/MKjEtF7diatibd6B0iaeF5KgsnMIwVcdbu09ZJAicI9P9vQc8MCLb7FVhg/300?n=1&n=1","introduction":"","listennum":16256989,"score":0.0,"version":0,"creator":{"type":6,"qq":2783429033,"encrypt_uin":"owSFoivANKnioz**","name":"QQ音乐官方歌单","isVip":6,"avatarUrl":"","followflag":0}},{"dissid":"6192858041","createtime":"2018-12-27","commit_time":"2018-12-27","dissname":"古典轻方式","imgurl":"http://qpic.y.qq.com/music_cover/7vlTTvwBiaibKJpyXffTHicMohuWJLDTv1ONom5kKPOKia4y4uSfO1smYw/300?n=1&n=1","introduction":"","listennum":7966527,"score":0.0,"version":0,"creator":{"type":6,"qq":2783429033,"encrypt_uin":"owSFoivANKnioz**","name":"QQ音乐官方歌单","isVip":6,"avatarUrl":"","followflag":0}},{"dissid":"7279288437","createtime":"2019-10-31","commit_time":"2019-10-31","dissname":"华语| 拾起回忆","imgurl":"http://qpic.y.qq.com/music_cover/7vlTTvwBiaibKJpyXffTHicMohuWJLDTv1ONWc8llzItJb4HZNvhwSvoA/300?n=1&n=1","introduction":"","listennum":895018,"score":0.0,"version":0,"creator":{"type":6,"qq"


*/

//默认一页30项
#define PAGE_SIZE 30
qqmusic::CategoryPlaylist::CategoryPlaylist(int categoryId)
    : ListObject<CatePlayList> ("http://c.y.qq.com/splcloud/fcgi-bin/fcg_get_diss_by_tag.fcg"
      , OBJECT_NAME_CATEGORY_PLAY_LIST, NetUrl::NET_HTTP_METHOD_GET)
    , mCategoryId(categoryId), mCurrentPage(1)
    , mTotals(0)
{
}

NetUrl &qqmusic::CategoryPlaylist::genQueryUrl()
{
    static char random[32] = "0.";
    static char categoryId[32];
    static char sin[32];
    static char ein[32];

    mUrl.appendChange("picmid", "1&g_tk=5381&jsonpCallback=getPlaylist&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&sortId=5");

    int res = sprintf(random + 2, "%d", rand());
    mUrl.appendChange("rnd", ByteString(random, res + 2));

    res = sprintf(categoryId, "%d", mCategoryId);
    mUrl.appendChange("categoryId", ByteString(categoryId, res));

    res = sprintf(sin, "%d", (mCurrentPage - 1) * PAGE_SIZE);
    mUrl.appendChange("sin", ByteString(sin, res));

    res = sprintf(ein, "%d", mCurrentPage * PAGE_SIZE - 1);
    mUrl.appendChange("ein", ByteString(ein, res));

    return mUrl;
}

int qqmusic::CategoryPlaylist::genResult(NetBuffer *data)
{
    int ret = KL_DATA_PRISER_OK;
    //GEN_Printf(LOG_DEBUG, "size: %lu\n%s", data->size(), (char *)data->buffer());
    cJSON *root = cJSON_Parse((char *)data->buffer() + 12, data->size() - 13);
    cJSON *obj;
    cJSON *dataItem = cJSON_GetObjectItem(root, "data");
    if (dataItem)
    {
        //清除上上次的数据， 并把旧数据备份起来，mNode用来接收存放新的数据
        //这样可以保证上层UI，即使在使用旧数据，也不会有访问已经释放的数据内存丢失
        mBackUp.swap(mNodes);
        // GEN_Printf(LOG_DEBUG, "mBackUp size = %d, Next Size= %d", mBackUp.size(), mNodes.size());
        clearData();

        // int categoryId = (obj = cJSON_GetObjectItem(dataItem, "categoryId")) ? atoi(obj->valuestring) : 0;
        mTotals = (obj = cJSON_GetObjectItem(dataItem, "sum")) ? atoi(obj->valuestring) : 0;
        cJSON *list = cJSON_GetObjectItem(dataItem, "list");
        for (cJSON *item = list->child; NULL != item; item = item->next )
        {
            CatePlayList tmp;
            JSON_VALUETRING_SWAP_BYTESTRING(item, dissid, tmp.dissid);
            JSON_VALUETRING_SWAP_BYTESTRING(item, dissname, tmp.dissname);
            JSON_VALUETRING_SWAP_BYTESTRING(item, imgurl, tmp.imgurl);

            mNodes.push_back(tmp);
        }
        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load CatePlayList is empty.");
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

void qqmusic::CategoryPlaylist::profile()
{
    GEN_Printf(LOG_DUMP, "category id: %d, totals: %d", mCategoryId, mTotals);
    ListTable<CatePlayList>::iterator it = mNodes.begin();
    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "[%s]=%s", it->dissid.string(), it->dissname.string());
    }
}

int qqmusic::CategoryPlaylist::getTotalPages()
{
    return mTotals / PAGE_SIZE;
}

int qqmusic::CategoryPlaylist::getCurrentPage()
{
    return mCurrentPage;
}

bool qqmusic::CategoryPlaylist::loadPage(int index)
{
    int total_pages = mTotals / PAGE_SIZE;

    if (index >= 1 || index <= total_pages)
    {
        mCurrentPage = index;

        return obtain();
    } else
    {
        GEN_Printf(LOG_DEBUG, "index[%d] invalid, total_page=%d", index, total_pages);
        return false;
    }
}

bool qqmusic::CategoryPlaylist::loadByCateId(int categoryId)
{
    mCurrentPage = 1;
    mCategoryId = categoryId;

    return obtain();
}
