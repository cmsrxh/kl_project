#include <events/common_log.h>
#include "net_util/net_common.h"
#include "kl_save_record.h"

kl::SaveRecord::SaveRecord()
    : mDataStatus(nullptr)
{

}

void kl::SaveRecord::genResult(const char *data, unsigned long size)
{
    cJSON *root = cJSON_Parse((char *)data, size);
    cJSON *dataList = cJSON_GetObjectItem(root, "dataList");
    if (dataList)
    {
        for (cJSON *item = dataList->child; NULL != item; item = item->next )
        {
            RecordItem tmp;

            cJSON *obj =  cJSON_GetObjectItem(item, "type");
            if (obj)
            {
                tmp.type = atoi(obj->valuestring);
            }

            JSON_VALUETRING_SWAP_BYTESTRING(item, id, tmp.id);
            JSON_VALUETRING_SWAP_BYTESTRING(item, parentId, tmp.parentId);
            JSON_VALUETRING_SWAP_BYTESTRING(item, name, tmp.name);
            JSON_VALUETRING_SWAP_BYTESTRING(item, parentName, tmp.parentName);
            JSON_VALUETRING_SWAP_BYTESTRING(item, image, tmp.image);
            JSON_VALUETRING_SWAP_BYTESTRING(item, playUrl, tmp.playUrl);
            JSON_VALUETRING_SWAP_BYTESTRING(item, fileSize, tmp.fileSize);

            mNodes.push_back(tmp);
        }

        if (mNodes.empty())
        {
            GEN_Printf(LOG_WARN, "load broadcast item list is empty.");
        } else
        {
            profile();
        }
    }else
    {
        GEN_Printf(LOG_ERROR, "priser failed, size: %lu\n%s", size, data);
    }

    cJSON_Delete(root);
}

bool kl::SaveRecord::genSaveString(char *&data, unsigned long &len)
{
    GEN_Printf(LOG_DEBUG, "list count: %d", mNodes.size());
    cJSON *root = cJSON_CreateObject();
    if (root)
    {
        cJSON *dataList = cJSON_CreateArray();
        if (dataList)
        {
             cJSON_AddItemToObject(root, "dataList", dataList);
             ListTable<RecordItem>::iterator it = mNodes.begin();
             for (; it != mNodes.end(); ++it)
             {
                 GEN_Printf(LOG_DEBUG, "gen string: %s", it->id.string());
                 cJSON *item = cJSON_CreateObject();

                 cJSON_AddItemToArray(dataList, item);

                 cJSON_AddItemToObject(item, "type",      cJSON_CreateNumber(it->type));
                 cJSON_AddItemToObject(item, "id",        cJSON_CreateString(it->id.string()));
                 cJSON_AddItemToObject(item, "parentId",  cJSON_CreateString(it->parentId.string()));
                 cJSON_AddItemToObject(item, "name",      cJSON_CreateString(it->name.string()));
                 cJSON_AddItemToObject(item, "parentName",cJSON_CreateString(it->parentName.string()));
                 cJSON_AddItemToObject(item, "image",     cJSON_CreateString(it->image.string()));
                 cJSON_AddItemToObject(item, "playUrl",   cJSON_CreateString(it->playUrl.string()));
                 cJSON_AddItemToObject(item, "fileSize",  cJSON_CreateString(it->fileSize.string()));
             }
             data = cJSON_PrintUnformatted(root);
             len  = strlen(data);
        }
        cJSON_Delete(root);
    }
    GEN_Printf(LOG_DEBUG, "gen cjson %d, %s", len, data);

    return (data && len) ? true : false;
}

void kl::SaveRecord::profile()
{
    ListTable<RecordItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        GEN_Printf(LOG_DUMP, "type=%d, %s=%s", it->type, it->name.string(), it->id.string());
    }
}

/**
 * @brief kl::SaveRecord::checkCurrentItem
 * @param item [in]  item需要clear 和释放
 * @details 核对当前播放项，是否收藏了，异步遍历，防止收藏列表过多，遍历时拖慢主线程
 */
void kl::SaveRecord::checkCurrentItem(kl::RecordItem *item)
{
    ListTable<RecordItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        if (it->id == item->id
                && it->parentId == item->parentId)
        {
            break;
        }
    }

    if (it != mNodes.end())
    {
        execStatus(LOCAL_RECORD_OP_STATUS_CHECK_IN_LIST);
    } else
    {
        execStatus(LOCAL_RECORD_OP_STATUS_CHECK_NOT_IN_LIST);
    }

    item->clear();
    delete item;
}

/**
 * @brief kl::SaveRecord::opCurrentItem
 * @param item [in] item需要clear 和释放
 * @details 核对当前播放项，是否收藏了，如果当前项在收藏列表就取消收藏，反之添加, 并通知界面
 */
void kl::SaveRecord::opCurrentItem(kl::RecordItem *item)
{
    ListTable<RecordItem>::iterator it = mNodes.begin();

    for (; it != mNodes.end(); ++it)
    {
        if (it->id == item->id
                && it->parentId == item->parentId)
        {
            break;
        }
    }
    // 找到了需要删除， 同时通知UI
    if (it != mNodes.end())
    {

        it->clear();
        mNodes.remove(it);

        item->clear();

        execStatus(LOCAL_RECORD_OP_STATUS_REMOVE_IN_LIST);

        delete item;
    }
    // 未找到需要添加到列表中， 同时通知UI
    else
    {
        mNodes.push_front(*item);
        execStatus(LOCAL_RECORD_OP_STATUS_ADD_IN_LIST);
        delete item;
    }
}
