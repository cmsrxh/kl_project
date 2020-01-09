#ifndef MUSIC_LOAD_MANAGE_H
#define MUSIC_LOAD_MANAGE_H

#include <util/list_table.h>

namespace kl
{
class MusicLoad;
}

class MusicLoadManage
{
public:
    MusicLoadManage();



private:
    ListTable<kl::MusicLoad *> mList;
};

#endif // MUSIC_LOAD_MANAGE_H
