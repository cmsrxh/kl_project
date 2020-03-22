#ifndef CATE_ITEM_UNION_H
#define CATE_ITEM_UNION_H

#include "kl_ui_data_union.h"

// 专辑: AlbumList
// 推荐: OperateList
// 智能电台:  TypeRadioList
// 传统电台:  BroadcastItemList
class CateItemModel;
class CateItemUnion : public UINotifyIface
{
public:
    enum {
        CATE_ITEM_ALBUM      = kl::OBJECT_ALBUM_LIST,
        CATE_ITEM_OPERATE    = kl::OBJECT_OPERATE_LIST,
        CATE_ITEM_TYPE_RADIO = kl::OBJECT_TYPERADIO_LIST,
        CATE_ITEM_BDCAST     = kl::OBJECT_BDC_ITEM_LIST,
    };

    CateItemUnion(int cid_type, CateItemModel *parent);
    virtual ~CateItemUnion();

    void loadCateItem(int cid_or_type = -1,
                      int bsorttype_or_classfyid = 1, int area_code = 0);

    void dataPrepare();

    void errorInfo(int, const ByteString &);

    void onLoadOver(CateItemModel *model);

    bool isEmpty();
    /**
     * @brief loadNextPage
     * @return 返回是否还有下一页
     */
    bool loadNextPage();

    int  page();
    bool haveNext();

    int getCateType()
    {
        return mCateItemType;
    }
private:
    const int                           mCateItemType;
    int                                 mLoadAction;
    UICategoryItemList                 *m_pCateItem;
    CateItemModel                      *m_pParentModel;

    void genCateItemByAlbumItem(ListTable<kl::AlbumItem> &nodes, VectorTable<MusicCateItemUnion *> &vec);
    void genCateItemByOperate(ListTable<kl::Operate> &nodes, VectorTable<MusicCateItemUnion *> &vec);
    void genCateItemByTypeRadio(ListTable<kl::TypeRadio> &nodes, VectorTable<MusicCateItemUnion *> &vec);
    void genCateItemByBDCast(ListTable<kl::BDCastItem> &nodes, VectorTable<MusicCateItemUnion *> &vec);
};

#endif // CATE_ITEM_UNION_H
