#ifndef KL_DATA_PROC_H
#define KL_DATA_PROC_H

#include "util/map_table.h"
#include "util/byte_string.h"

class CategoryUnion;
class CateItemUnion;
class ChipItemUnion;
class CategoryModel;
class CateItemModel;
class ChipItemModel;
class KLDataProc
{
public:
    enum
    {
        MEDIA_TYPE_BROADCAST,
        MEDIA_TYPE_ALBUMINFO,
    };
    struct SwitchPath
    {
    public:
        SwitchPath()
            : media_type(MEDIA_TYPE_ALBUMINFO)
          , cate_tab_index(0)
          , cate_item_index(-1)
          , chip_item_index(-1)
        {}
        void setCateTabIndex(int index)
        {
            cate_tab_index  = index;
            cate_item_index = -1;
            chip_item_index = -1;
        }
        void setCateItemIndex(int index)
        {
            cate_item_index = index;
            chip_item_index = -1;
        }
        void setChipItemIndex(int index)
        {
            chip_item_index = index;
        }
        int media_type;
        int cate_tab_index;
        int cate_item_index;
        int chip_item_index;
    };

    static KLDataProc *instance()
    {
        static KLDataProc i;
        return &i;
    }

    void init(CategoryModel *cate, CateItemModel *cateItem, ChipItemModel *chip);

    void cateTabClick(int index);
    void cateItemClick(int index);
    void chipItemChick(int index);

    void showPlayingInfo();
    void enterAlbumView();

    int  getCateTabIndex();
private:
    KLDataProc();

    CategoryUnion    *mMainCate;
    CategoryModel    *m_pCate;
    CateItemModel    *m_pCateItem;
    ChipItemModel    *m_pChipItem;

    SwitchPath        mSwitch;
    SwitchPath        mPlayPath;

    MapTable<int, CateItemUnion*>        mCidMap;
    MapTable<ByteString, ChipItemUnion*> mChipMap;

};

#endif // KL_DATA_PROC_H
