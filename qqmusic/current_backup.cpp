#include "events/common_log.h"
#include "model/data_proc.h"
#include "model/chip_item_union.h"
#include "net_util/net_common.h"
#include "ui_proc.h"
#include "application.h"
#include "current_backup.h"

#define LOCAL_KL_CURRENT_INFO_FILE "/tmp/kl.local.current.info"
#define CURRENT_MAGIC "ABCDEF"

extern UIProc *gInstance;
CurrentBackup::CurrentBackup()
    : mCurPosition(0)
    , m_pPrevRecChip(new ChipItemUnion(PLAY_CHIP_TYPE_PREV_PLAYING_RECORD))
{
    mRecPlay.type = -1;
}

CurrentBackup::~CurrentBackup()
{
    delete m_pPrevRecChip;
}

void CurrentBackup::saveCurrent()
{
    int fd = open(LOCAL_KL_CURRENT_INFO_FILE, O_WRONLY | O_TRUNC | O_CREAT, 0664);
    if (fd > 0)
    {
        char *data;
        int len;

        DataProc::instance()->getViewSwitchInfo(data, len);
        write(fd, data, len);
        write(fd, CURRENT_MAGIC, sizeof(CURRENT_MAGIC)-1);
        data = genCurrentInfo();
        if (data)
        {
            write(fd, data, strlen(data));
            delete [] data;
        }
        close(fd);
    } else
    {
        GEN_Printf(LOG_ERROR, LOCAL_KL_CURRENT_INFO_FILE " write failed, %s", strerror(errno));
    }
}

void CurrentBackup::recoveryCurrent()
{
    struct stat st;

    if (stat(LOCAL_KL_CURRENT_INFO_FILE, &st) == 0
            && st.st_size > (int)sizeof(DataProc::SwitchPath))
    {
        int fd = open(LOCAL_KL_CURRENT_INFO_FILE, O_RDONLY);
        if (fd > 0)
        {
            char *data = new char[st.st_size + 1];
            char *p;

            int ret = read(fd, data, st.st_size);
            if(ret > 0)
            {
                data[ret] = '\0';
                DataProc::instance()->setViewSwitchInfo(data);
                p = data + sizeof(DataProc::SwitchPath);
                if (0 == memcmp(p, CURRENT_MAGIC, sizeof(CURRENT_MAGIC)-1))
                {
                    p += sizeof(CURRENT_MAGIC)-1;
                    priserCurrentInfo(p, st.st_size - sizeof(DataProc::SwitchPath) - sizeof(CURRENT_MAGIC) + 1);
                }
            } else
            {
                GEN_Printf(LOG_ERROR, "read failed, %s", strerror(errno));
            }
            close(fd);
            delete [] data;
        } else
        {
            GEN_Printf(LOG_ERROR, "open failed, %s", strerror(errno));
        }
    } else
    {
        GEN_Printf(LOG_ERROR, LOCAL_KL_CURRENT_INFO_FILE " open failed, %s", strerror(errno));
    }
}

bool CurrentBackup::empty()
{
    return mRecPlay.id.empty();
}

void CurrentBackup::pushRecPlayInto(VectorTable<MusicChipItemUnion *> &vec)
{
    MusicChipItemUnion *tmp = new MusicChipItemUnion;

    tmp->type       = PLAY_CHIP_TYPE_PREV_PLAYING_RECORD;
    tmp->sub_type   = mRecPlay.type;
    tmp->parentId   = mRecPlay.parentId;
    tmp->chipId     = mRecPlay.id;
    tmp->parentName = mRecPlay.parentName;
    tmp->name       = (!mRecPlay.name.empty()) ? mRecPlay.name : mRecPlay.parentName;
    tmp->image      = mRecPlay.image;
    tmp->playUrl    = mRecPlay.playUrl;

    vec.push_back(tmp);
}

void CurrentBackup::playPrevRecordInfo()
{
    if (!mRecPlay.id.empty()
            && mRecPlay.type > 0)
    {
        GEN_Printf(LOG_INFO, "Start play prev record info.");
        DataProc::instance()->localItemPlay(PLAY_CHIP_TYPE_PREV_PLAYING_RECORD, 0, m_pPrevRecChip);
    }
}

char* CurrentBackup::genCurrentInfo()
{
    const CollectNode *curPlay = DataProc::instance()->getPlayInfoIfPlaying();
    char *data = nullptr;
    cJSON *root = cJSON_CreateObject();
    if (curPlay && root)
    {
        cJSON_AddItemToObject(root, "type",      cJSON_CreateNumber(curPlay->type));
        cJSON_AddItemToObject(root, "id",        cJSON_CreateString(curPlay->id.string()));
        cJSON_AddItemToObject(root, "parentId",  cJSON_CreateString(curPlay->parentId.string()));
        cJSON_AddItemToObject(root, "name",      cJSON_CreateString(curPlay->name.string()));
        cJSON_AddItemToObject(root, "parentName",cJSON_CreateString(curPlay->parentName.string()));
        cJSON_AddItemToObject(root, "image",     cJSON_CreateString(curPlay->image.string()));
        cJSON_AddItemToObject(root, "playUrl",   cJSON_CreateString(curPlay->playUrl.string()));
        cJSON_AddItemToObject(root, "curPos",    cJSON_CreateNumber(gInstance->getCurrentPlayingPosition()));
    }

    data = cJSON_PrintUnformatted(root);

    cJSON_Delete(root);

    return data;
}

void CurrentBackup::priserCurrentInfo(char *data, int len)
{
    GEN_Printf(LOG_DEBUG, "priser current info.");
    if (len < 0)
    {
        GEN_Printf(LOG_ERROR, "priser current error=%d, data: %s", len, data);
        return ;
    }

    cJSON *root = cJSON_Parse(data, len);
    if (root)
    {
        cJSON *obj =  cJSON_GetObjectItem(root, "type");
        if (obj)
        {
            int type = atoi(obj->valuestring);
            switch (type)
            {
            case PLAY_CHIP_TYPE_ALBUM:
            case PLAY_CHIP_TYPE_AUDIO_CHIP:
            case PLAY_CHIP_TYPE_BROADCAST:
            case PLAY_CHIP_TYPE_TYPE_RADIO:
                mRecPlay.type = type;
                break;
            case PLAY_CHIP_TYPE_RADIO_CHIP:
                mRecPlay.type = PLAY_CHIP_TYPE_TYPE_RADIO;
                break;
            case PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP:
                mRecPlay.type = PLAY_CHIP_TYPE_BROADCAST;
                break;
            default:
                GEN_Printf(LOG_WARN, "Read a invalid type: %d", type);
                break;
            }
        }
        JSON_VALUETRING_SWAP_BYTESTRING(root, id, mRecPlay.id);
        JSON_VALUETRING_SWAP_BYTESTRING(root, parentId, mRecPlay.parentId);
        JSON_VALUETRING_SWAP_BYTESTRING(root, name, mRecPlay.name);
        JSON_VALUETRING_SWAP_BYTESTRING(root, parentName, mRecPlay.parentName);
        JSON_VALUETRING_SWAP_BYTESTRING(root, image, mRecPlay.image);
        JSON_VALUETRING_SWAP_BYTESTRING(root, playUrl, mRecPlay.playUrl);

        obj =  cJSON_GetObjectItem(root, "curPos");
        if (obj)
        {
            mCurPosition = atoi(obj->valuestring);
        }
        cJSON_Delete(root);
    } else
    {
        GEN_Printf(LOG_ERROR, "current json priser error.");
    }
    GEN_Printf(LOG_DEBUG, "mCurPosition=%d, type=%d, id=%s", mCurPosition, mRecPlay.type, mRecPlay.id.string());
}
