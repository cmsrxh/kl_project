#include <net_util/http_util.h>
#include "qqmusic_singer_list.h"

/*
//歌手列表
http://u.y.qq.com/cgi-bin/musicu.fcg?g_tk=5381&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&data=%7B%22comm%22%3A%7B%22ct%22%3A24%2C%22cv%22%3A10000%7D%2C%22singerList%22%3A%7B%22module%22%3A%22Music.SingerListServer%22%2C%22method%22%3A%22get_singer_list%22%2C%22param%22%3A%7B%22area%22%3A-100%2C%22sex%22%3A-100%2C%22genre%22%3A-100%2C%22index%22%3A-100%2C%22sin%22%3A0%2C%22cur_page%22%3A1%7D%7D%7D");
http://u.y.qq.com/cgi-bin/musicu.fcg?
g_tk=5381
&loginUin=0
&hostUin=0
&format=jsonp
&inCharset=utf8
&outCharset=utf-8
&notice=0
&platform=yqq
&needNewCode=0
&data={"comm":{"ct":24,"cv":10000},
 "singerList":{"module":"Music.SingerListServer","method":"get_singer_list",
      "param":{"area":-100,"sex":-100,"genre":-100,"index":-100,"sin":0,"cur_page":1}}}
*/

qqmusic::SingerTags qqmusic::SingerList::mTags;
qqmusic::SingerList::SingerList(int areaId, int sex, int genre, int index)
    : ListObject<SingerNode> ("http://u.y.qq.com/cgi-bin/musicu.fcg", OBJECT_NAME_SINGER_LIST, NetUrl::NET_HTTP_METHOD_GET)
    , mAreaId(areaId), mSex(sex), mGenre(genre)
    , mIndex(index), mSin(0), mCurPage(1)
    , mTotals(0)
{
    // test
    setSaveFile("/tmp/qqmusic.singer.list");
    mUrl.append("g_tk", "5381");
    mUrl.append("loginUin", "0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&needNewCode=0");
    mUrl.append("platform", "yqq");
}

NetUrl &qqmusic::SingerList::genQueryUrl()
{
    int ret = snprintf(mDataBuffer, sizeof(mDataBuffer) - 1,
                       "{\"comm\":{\"ct\":24,\"cv\":10000},"
                       "\"singerList\":{\"module\":\"Music.SingerListServer\",\"method\":\"get_singer_list\","
                       "\"param\":{\"area\":%d,\"sex\":%d,\"genre\":%d,\"index\":%d,\"sin\":%d,\"cur_page\":%d}}}"
                       , mAreaId, mSex, mGenre, mIndex, mSin, mCurPage);
    if (ret <= 0 || ret >= (int)(sizeof(mDataBuffer) - 1))
    {
        GEN_Printf(LOG_ERROR, "General signer list data param failed !");
        SF_ASSERT(0);
    }
    mDataString.clear();

    HttpUtil::urlEncoding(ByteString(mDataBuffer, ret), mDataString);
    mUrl.appendChange("data", mDataString);
    return mUrl;
}

int qqmusic::SingerList::genResult(NetBuffer *data)
{
    int ret = KL_DATA_PRISER_OK;
    //GEN_Printf(LOG_DEBUG, "size: %lu\n%s", data->size(), (char *)data->buffer());
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());
    cJSON *obj;
    cJSON *singerList = cJSON_GetObjectItem(root, "singerList");
    if (singerList)
    {
        clearData();

        cJSON *data = cJSON_GetObjectItem(singerList, "data");

        mAreaId = (obj = cJSON_GetObjectItem(data, "area"))  ? atoi(obj->valuestring) : 0;
        mGenre  = (obj = cJSON_GetObjectItem(data, "genre")) ? atoi(obj->valuestring) : 0;
        mIndex  = (obj = cJSON_GetObjectItem(data, "index")) ? atoi(obj->valuestring) : 0;
        mSex    = (obj = cJSON_GetObjectItem(data, "sex"))   ? atoi(obj->valuestring) : 0;
        mTotals = (obj = cJSON_GetObjectItem(data, "total")) ? atoi(obj->valuestring) : 0;

        cJSON *singerlist = cJSON_GetObjectItem(data, "singerlist");
        if (singerlist)
        {
            for (cJSON *item = singerlist->child; NULL != item; item = item->next )
            {
                SingerNode tmp;

                JSON_VALUETRING_SWAP_BYTESTRING(item, country, tmp.country);
                JSON_VALUETRING_SWAP_BYTESTRING(item, singer_id, tmp.singer_id);
                JSON_VALUETRING_SWAP_BYTESTRING(item, singer_mid, tmp.singer_mid);
                JSON_VALUETRING_SWAP_BYTESTRING(item, singer_name, tmp.singer_name);
                JSON_VALUETRING_SWAP_BYTESTRING(item, singer_pic, tmp.singer_pic);

                mNodes.push_back(tmp);
            }
        } else
        {
            GEN_Printf(LOG_WARN, "'singerlist' Json Item priser failed !");
        }

        cJSON *tags = cJSON_GetObjectItem(data, "tags");
        if (tags)
        {
            for (int i = 0; i < ARRAY_SIZE((mTags.tags)); ++i)
            {
                if (!mTags.tags[i].tag.empty())
                {
                    GEN_Printf(LOG_DEBUG, "Current[%s] data is exist !!!", mTags.tags[i].name);
                    continue;
                }
                cJSON *tag = cJSON_GetObjectItem(tags, mTags.tags[i].name);
                if (tag)
                {
                    for (cJSON *item = tag->child; NULL != item; item = item->next )
                    {
                        TagNode tmp;

                        tmp.id = (obj = cJSON_GetObjectItem(item, "id")) ? atoi(obj->valuestring) : 0;
                        JSON_VALUETRING_SWAP_BYTESTRING(item, name, tmp.name);

                        mTags.tags[i].tag.push_back(tmp);
                    }
                } else
                {
                    GEN_Printf(LOG_WARN, "SingerList Tags: %s is not exist !!!", mTags.tags[i].name);
                }
            }
        } else
        {
            GEN_Printf(LOG_WARN, "'tags' Json Item priser failed !");
        }

        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load AlbumList is empty.");
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

void qqmusic::SingerList::profile()
{
    GEN_Printf(LOG_DUMP, "area=%d", mAreaId);
    GEN_Printf(LOG_DUMP, "sex=%d", mSex);
    GEN_Printf(LOG_DUMP, "genre=%d", mGenre);
    GEN_Printf(LOG_DUMP, "index=%d", mIndex);
    GEN_Printf(LOG_DUMP, "cur_page=%d", mCurPage);
    GEN_Printf(LOG_DUMP, "totals=%d", mTotals);

    ListTable<SingerNode>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "%s=%s", it->singer_name.string(), it->singer_id.string());
    }

    for (int i = 0; i < ARRAY_SIZE((mTags.tags)); ++i)
    {
        GEN_Printf(LOG_DUMP, "Tag: %s", mTags.tags[i].name);
        ListTable<TagNode>::iterator it = mTags.tags[i].tag.begin();

        for (; it != mTags.tags[i].tag.end(); ++it)
        {
             GEN_Printf(LOG_DUMP, "%s=%d", it->name.string(), it->id);
        }
    }
}

#define DEFAULT_PAGE 80
int qqmusic::SingerList::getTotalPages()
{
    return mTotals / DEFAULT_PAGE;
}

bool qqmusic::SingerList::loadNextPage(int page_index)
{
    int total_page = getTotalPages();
    int load_page = (-1 == page_index) ? 1 + mCurPage
                                       : ((-2 == page_index) ? mCurPage - 1
                                                             : page_index);
    // 默认一页的大小
    load_page = (load_page < 0) ? 0
                                : (load_page > total_page) ? total_page
                                                           : load_page ;

    if (mCurPage == load_page)
    {
        GEN_Printf(LOG_WARN, "Current Page=%d equal Load page", mCurPage);
        return false;
    }

    int sin_page = load_page < 1 ? 0 : load_page - 1;
    int sin_start= sin_page * DEFAULT_PAGE;

    if (sin_start >= mTotals)
    {
        GEN_Printf(LOG_WARN, "Page start[%d] out of range total pages[%d]", sin_start, mTotals);
        return false;
    }

    mCurPage = load_page;
    mSin = sin_start;
    // GEN_Printf(LOG_DEBUG, "page_index: %d, mSin: %d, mCurPage: %d, mTotals: %d", page_index, mSin, mCurPage, mTotals);

    return obtain();
#undef DEFAULT_PAGE
}

bool qqmusic::SingerList::loadBySex(int sex)
{
    mSex = sex;
    mSin = 0;
    mCurPage = 1;
    mTotals = 0;
    return obtain();
}

bool qqmusic::SingerList::loadByGenre(int genre)
{
    mGenre = genre;
    mSin = 0;
    mCurPage = 1;
    mTotals = 0;
    return obtain();
}

bool qqmusic::SingerList::loadByIndex(int index)
{
    mIndex = index;
    mSin = 0;
    mCurPage = 1;
    mTotals = 0;
    return obtain();
}

bool qqmusic::SingerList::loadByArea(int area)
{
    mAreaId = area;
    mSin = 0;
    mCurPage = 1;
    mTotals = 0;
    return obtain();
}

void qqmusic::SingerList::ensureTagsIndex()
{
    int NeedCheckId[4] =
    {
        mAreaId, mGenre, mIndex, mSex
    };

//    NeedCheckId[SingerTags::AREA_TAG]  = mAreaId;
//    NeedCheckId[SingerTags::GENRE_TAG] = mGenre;
//    NeedCheckId[SingerTags::INDEX_TAG] = mIndex;
//    NeedCheckId[SingerTags::SEX_TAG]   = mSex;

    for (int i = 0; i < ARRAY_SIZE((mTags.tags)); ++i)
    {
        ListTable<TagNode>::iterator it = mTags.tags[i].tag.begin();
        for (int j = 0; it != mTags.tags[i].tag.end(); ++it, ++j)
        {
            if (it->id == NeedCheckId[i])
            {
                mTags.tags[i].curIndex = j;
                break;
            }
        }
    }
}
