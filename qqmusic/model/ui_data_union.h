#ifndef KL_UI_DATA_UNION_H
#define KL_UI_DATA_UNION_H

#include <util/byte_string.h>
#include <util/vector_table.h>
#include <util/list_table.h>
#include "common_func/common_func.h"

enum
{
    PLAY_CHIP_TYPE_ALBUM     /*= kl::OBJECT_ALBUM_LIST*/,           // 专辑二级标签item
    PLAY_CHIP_TYPE_BROADCAST   /*= kl::OBJECT_BDC_ITEM_LIST*/ ,     // 电台二级标签item
    PLAY_CHIP_TYPE_TYPE_RADIO /* = kl::OBJECT_TYPERADIO_LIST*/ ,    // 智能电台二级标签item

    PLAY_CHIP_TYPE_AUDIO_CHIP  /*= kl::OBJECT_CHIP_AUDIO_LIST*/ ,      // 专辑音乐节目碎片三级标签item
    PLAY_CHIP_TYPE_RADIO_CHIP  /*= kl::OBJECT_CHIP_RADIO_LIST*/ ,      // 智能电台节目碎片三级标签item
    PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP /*=kl::OBJECT_BDC_ITEM_PROGRAM*/, // 电台节目碎片三级标签item

    PLAY_CHIP_TYPE_LOCAL_LOAD    /*= kl::LOCAL_OBJECT_LOAD*/,        // 下载的音频碎片（专辑音乐碎片）
    PLAY_CHIP_TYPE_COLLECT_RECORD  /*= kl::LOCAL_OBJECT_COLLECT*/,   // 加载收藏
    PLAY_CHIP_TYPE_HISTROY_RECORD  /*= kl::LOCAL_OBJECT_HISTROY*/,   // 加载历史记录
    PLAY_CHIP_TYPE_SEARCH_LOAD  /*= kl::LOCAL_OBJECT_SEARCH*/,       // 加载搜索列表
    PLAY_CHIP_TYPE_PREV_PLAYING_RECORD,                         // 表示上一次程序退出时记录的播放信息
};

// 一级菜单集合，一般就只用有名字和ID就OK
struct FirstMenuUnion
{
    int id;
    ByteString name;
};

// 二级菜单集合，一般涉及到图片，和各种私有数据，需在此 结构体体现，有时一个变量（在不同的列表中）会有几种含义
struct SecondMenuUnion
{
    SecondMenuUnion()
        : isCollect(false)
    {}

    bool isCollect;
    int  type;       // PLAY_CHIP_TYPE_ALBUM ....

    ByteString id;
    ByteString name;
    ByteString img;    
    ByteString playUrl;
};

// 具体的播放项设定，已经是最小的碎片项，代表每个可以播放的数据项，其中 数据必然较为复杂
class PlayingChipnion
{
public:
    int type;
    int sub_type;
    ByteString parentId;
    ByteString chipId;

    ByteString parentName;
    ByteString name;

    ByteString image;
    ByteString playUrl;
    ByteString desc;
    ByteString startTime;
    ByteString finishTime;
   // ChipItemUnion *subChips; //播放列表项中子列表 pointer
};

struct MusicDetail
{
    ByteString id;
    ByteString name;
    ByteString desc;
    ByteString image;
    ByteString hostName;
    ByteString playUrl;
    ListTable<ByteString> *keywords;
};

class CollectNode
{
public:
    void clear()
    {
        id.clear();
        parentId.clear();
        name.clear();
        parentName.clear();
        image.clear();
        playUrl.clear();
        fileSize.clear();
    }
    void copy(CollectNode *item)
    {
        item->isLocal    = isLocal;
        item->type       = type;
        item->id         = ByteString::allocString(id);
        item->parentId   = ByteString::allocString(parentId);
        item->name       = ByteString::allocString(name);
        item->parentName = ByteString::allocString(parentName);
        item->image      = ByteString::allocString(image);
        item->playUrl    = ByteString::allocString(playUrl);
        item->fileSize   = ByteString::allocString(fileSize);
    }

    int        isLocal; // 表明当前媒体时候在本地
    int        type;    // PLAY_CHIP_TYPE*
    ByteString id;
    ByteString parentId;
    ByteString name;
    ByteString parentName;
    ByteString image;
    ByteString playUrl;
    ByteString fileSize;
};

typedef  void UICategory;
typedef  void UICategoryItemList;
typedef  void UIChipItemList;
typedef  void UIDetail;

#define QStringFromByteString(str) \
    QString::fromUtf8((str).string(), (str).size())

#define QStringFromCString(str) \
    QString::fromUtf8(str)


#endif // KL_UI_DATA_UNION_H
