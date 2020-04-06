#ifndef CATE_ITEM_UNION_H
#define CATE_ITEM_UNION_H

#include "ui_data_union.h"

// 专辑: AlbumList
// 推荐: OperateList
// 智能电台:  TypeRadioList
// 传统电台:  BroadcastItemList
class CateItemModel;
class CateItemUnion : public UINotifyIface
{
public:
    enum {
        CATE_ITEM_CATE_PLAY_LIST = qqmusic::OBJECT_NAME_CATEGORY_PLAY_LIST,

        CATE_ITEM_ALBUM      /*= kl::OBJECT_ALBUM_LIST*/,
        CATE_ITEM_OPERATE    /*= kl::OBJECT_OPERATE_LIST*/,
        CATE_ITEM_TYPE_RADIO /*= kl::OBJECT_TYPERADIO_LIST*/,
        CATE_ITEM_BDCAST     /*= kl::OBJECT_BDC_ITEM_LIST*/,
    };

    CateItemUnion(int cid_type, CateItemModel *parent);
    virtual ~CateItemUnion();

    /**
     * @brief loadCateItem
     * @param arg1 [in] 根据具体对象，对应相应的参数
     * @note 下载数据
     */
    void loadCateItem(int arg1 = -1);

    void dataPrepare();

    void errorInfo(int, const ByteString &);

    /**
     * @brief dataLoadOver
     * @param model [in] 数据model指针，表示数据需要展示在那个界面model中
     * @warning 这个函数必须在主线程中加载数据
     */
    void dataLoadOver(CateItemModel *model);

    bool isEmpty();

    int getCateType()
    {
        return mCateItemType;
    }

    //! 设置具体的现在项，以便数据处理
    void setProxyItem(UICategoryItemList *proxy)
    {
        m_pCateItem = proxy;
    }
    /**
     * @brief setLoadAction
     * @param action [in] 数值来源于：PopTipManage::LoadAction
     * @note 设置下载的动作，以便弹框正确处理
     */
    void setLoadAction(int action)
    {
        mLoadAction = action;
    }

private:
    const int                           mCateItemType;
    int                                 mLoadAction;
    UICategoryItemList                 *m_pCateItem;
    CateItemModel                      *m_pParentModel;

    void genCateItemByCatePlayList(ListTable<qqmusic::CatePlayList> &nodes, VectorTable<SecondMenuUnion *> &vec);

};

#endif // CATE_ITEM_UNION_H
