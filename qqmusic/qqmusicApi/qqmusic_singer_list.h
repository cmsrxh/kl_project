#ifndef QQMUSIC_SINGER_LIST_H
#define QQMUSIC_SINGER_LIST_H

#include "qqmusic_list_object.h"

namespace  qqmusic
{
class SingerList : public ListObject<SingerNode>
{
public:
    /**
     * @brief SingerList
     * @param areaId [in] 地区
     * @param sex [in] 性别
     * @param genre [in] 类型，eg: 摇滚、电子
     * @param index [in] 字母标签 eg: A、B、
     * @param cur_page [in] 页码
     * @default 默认加载全部歌手列表
     */
    SingerList(int areaId = -100, int sex = -100, int genre = -100, int index = -100, int cur_page = 1);

    NetUrl &genQueryUrl();
};

}

#endif // QQMUSIC_SINGER_LIST_H
