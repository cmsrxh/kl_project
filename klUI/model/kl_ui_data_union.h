#ifndef KL_UI_DATA_UNION_H
#define KL_UI_DATA_UNION_H

#include "kl_url/kl_common.h"
#include <util/vector_table.h>

struct MusicCateUnion
{
    ByteString cid;
    ByteString name;
    ByteString hasSub;
    ByteString img;
};

struct MusicCateItemUnion
{
    ByteString id;
    ByteString name;
    ByteString img;
    ByteString type; // 资源类型 (0:专辑|3:电台)
};




typedef  void UICategory;
typedef  void UICategoryItemList;




#endif // KL_UI_DATA_UNION_H
