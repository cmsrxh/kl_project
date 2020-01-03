#ifndef KL_UI_DATA_UNION_H
#define KL_UI_DATA_UNION_H

#include "kl_url/kl_common.h"
#include <util/vector_table.h>

struct MusicCateUnion
{
    ByteString cid;
    ByteString name;
    ByteString hasSub; // or == CateBCast.type
    ByteString img;
};

struct MusicCateItemUnion
{
    ByteString id;
    ByteString name;
    ByteString img;
    ByteString type; // 资源类型 (0:专辑|3:电台|11:传统广播)
    ByteString playUrl;
};

struct MusicChipItemUnion
{
    ByteString chipId;
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
    ListTable<ByteString> *keywords;
};

struct CollectNode
{
    int type;       // DetailUnion::DETAIL_TYPE_*
    ByteString id;
    ByteString name;
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
