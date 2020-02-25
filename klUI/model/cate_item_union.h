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
        CATE_ITEM_ALBUM = 1,
        CATE_ITEM_OPERATE,
        CATE_ITEM_TYPE_RADIO,
        CATE_ITEM_BDCAST,
    };
    CateItemUnion(int cid_type, CateItemModel *parent);
    virtual ~CateItemUnion();

    void loadCateItem(int cid_or_type = -1,
                      int bsorttype_or_classfyid = 1, int area_code = 0);

    void dataPrepare();

    void errorInfo(int, const char *);

    void onLoadOver(CateItemModel *model);

    bool isEmpty();
    /**
     * @brief loadNextPage
     * @return 返回是否还有下一页
     */
    bool loadNextPage();

    int  page();
    bool haveNext();
private:
    const int                           mCateItemType;
    UICategoryItemList                 *m_pCateItem;
    CateItemModel                      *m_pParentModel;

    void genCateItemByAlbumItem(ListTable<kl::AlbumItem> &nodes, VectorTable<MusicCateItemUnion *> &vec);
    void genCateItemByOperate(ListTable<kl::Operate> &nodes, VectorTable<MusicCateItemUnion *> &vec);
    void genCateItemByTypeRadio(ListTable<kl::TypeRadio> &nodes, VectorTable<MusicCateItemUnion *> &vec);
    void genCateItemByBDCast(ListTable<kl::BDCastItem> &nodes, VectorTable<MusicCateItemUnion *> &vec);
};

#endif // CATE_ITEM_UNION_H
