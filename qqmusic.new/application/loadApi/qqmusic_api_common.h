/*************************************************************************
	> File Name:    qqmusic_api_common.h
	> Author:       陈猛
	> Mail:         chen-meng@hongjing-wh.com
	> Created Time: Sun 22 Mar 2020 02:03:40 PM CST
	> Description:  用来定义每个api下载下来后，分析出来的数据结构
 ************************************************************************/
#ifndef QQMUSIC_API_COMMON_H
#define QQMUSIC_API_COMMON_H

#include "common_func/common_func.h"
#include "net_util/net_common.h"
#include <util/list_table.h>

namespace qqmusic {
enum QQMusicObjectName
{
    OBJECT_NAME_SINGER_LIST,
    OBJECT_NAME_CATEGORY,
    OBJECT_NAME_CATEGORY_PLAY_LIST,
};

//歌手列表中的项
class SingerNode
{
public:
    void clear()
    {
        country.clear();
        singer_id.clear();
        singer_mid.clear();
        singer_name.clear();
        singer_pic.clear();
    }

    ByteString country;  // 一直都是空数据
    ByteString singer_id;
    ByteString singer_mid;
    ByteString singer_name;
    ByteString singer_pic;
};
// 歌手数据参数标签, area genra index sex
class TagNode
{
public:
    void clear()
    {
        name.clear();
    }
    int id;
    ByteString name;
};

//标签
class CateNode
{
public:
    struct Cate
    {
        int categoryId;
        ByteString categoryName;
    };
    void clear()
    {
        categoryGroupName.clear();
        ListTable<Cate>::iterator it = items.begin();

        for (it = items.pop_front(); it != items.end(); it = items.pop_front())
        {
            it->categoryName.clear();
            items.remove(it);
        }
    }

    ByteString categoryGroupName;
    ListTable<Cate> items;
};

//标签ID，所对应的Playist
class CatePlayList
{
public:
    void clear()
    {
        dissid.clear();
        dissname.clear();
        imgurl.clear();
    }

    ByteString dissid;
    ByteString dissname;
    ByteString imgurl;
};


}


#endif // QQMUSIC_API_COMMON_H

