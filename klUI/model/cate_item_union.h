#ifndef CATE_ITEM_UNION_H
#define CATE_ITEM_UNION_H

#include "kl_ui_data_union.h"

// 下面有专辑: AlbumList
// 推荐: OperateList
// 智能电台: TypeRadioList
class CateItemModel;
class CateItemUnion : public kl::UINotifyIface
{
public:
    enum {
        CATE_ITEM_ALBUM,
        CATE_ITEM_OPERATE,
        CATE_ITEM_TYPE_RADIO,
    };
    CateItemUnion(int cid, CateItemModel *parent);

    void loadCateItem(int cid = -1, bool sorttype = true);

    void dataPrepare();

    void errorInfo(int, const char *);

    void onLoadOver(CateItemModel *model);

    int  page();
    bool haveNext();
private:
    int                                 mCateItemType;
    UICategoryItemList                 *m_pCateItem;
    CateItemModel                      *m_pParentModel;

    void genCateItemByAlbumItem(ListTable<kl::AlbumItem> &nodes, VectorTable<MusicCateItemUnion *> &vec);
    void genCateItemByOperate(ListTable<kl::Operate> &nodes, VectorTable<MusicCateItemUnion *> &vec);
    void genCateItemByTypeRadio(ListTable<kl::TypeRadio> &nodes, VectorTable<MusicCateItemUnion *> &vec);
};

#endif // CATE_ITEM_UNION_H
