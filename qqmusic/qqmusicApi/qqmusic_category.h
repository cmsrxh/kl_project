#ifndef QQMUSIC_CATEGORY_H
#define QQMUSIC_CATEGORY_H

#include "qqmusic_list_object.h"

namespace  qqmusic
{
class Category : public ListObject<CateNode>
{
public:
    Category();

    //! 生成请求字串
    NetUrl &genQueryUrl();

    //! 分析数据
    int genResult(NetBuffer *data);

    //! 打印当前页信息
    void profile();
};
}

#endif // QQMUSIC_CATEGORY_H
