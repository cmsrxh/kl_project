#ifndef KL_UI_DATA_UNION_H
#define KL_UI_DATA_UNION_H

#include "kl_url/kl_common.h"
#include <util/vector_table.h>

enum
{
    PLAY_CHIP_TYPE_ALBUM     = kl::OBJECT_ALBUM_LIST,    // 专辑二级标签item
    PLAY_CHIP_TYPE_BROADCAST   = kl::OBJECT_BDC_ITEM_LIST ,    // 电台二级标签item
    PLAY_CHIP_TYPE_TYPE_RADIO  = kl::OBJECT_TYPERADIO_LIST ,    // 智能电台二级标签item

    PLAY_CHIP_TYPE_AUDIO_CHIP  = kl::OBJECT_CHIP_AUDIO_LIST ,    // 专辑音乐节目碎片三级标签item
    PLAY_CHIP_TYPE_RADIO_CHIP  = kl::OBJECT_CHIP_RADIO_LIST ,    // 智能电台节目碎片三级标签item
    PLAY_CHIP_TYPE_BDC_PROGRAM_CHIP = kl::OBJECT_BDC_ITEM_PROGRAM, // 电台节目碎片三级标签item

    PLAY_CHIP_TYPE_LOCAL_LOAD   = kl::LOCAL_OBJECT_LOAD,    // 下载的音频碎片（专辑音乐碎片）
    PLAY_CHIP_TYPE_COLLECT_RECORD = kl::LOCAL_OBJECT_COLLECT,   // 加载收藏
    PLAY_CHIP_TYPE_HISTROY_RECORD = kl::LOCAL_OBJECT_HISTROY,   // 加载历史记录
    PLAY_CHIP_TYPE_SEARCH_LOAD = kl::LOCAL_OBJECT_SEARCH,      // 加载搜索列表
};

struct MusicCateUnion
{
    ByteString cid;
    ByteString name;
    ByteString hasSub; // or == CateBCast.type
    ByteString img;
};

struct MusicCateItemUnion
{
    MusicCateItemUnion()
        : isCollect(false)
    {}

    bool isCollect;
    ByteString id;
    ByteString name;
    ByteString img;
    //ByteString type; // 资源类型 (0:专辑|3:电台|11:传统广播) 分别对应
    int type; // PLAY_CHIP_TYPE_ALBUM ....
    ByteString playUrl;
};

struct MusicChipItemUnion
{
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

class CollectNode : public kl::RecordItem
{
public:

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
