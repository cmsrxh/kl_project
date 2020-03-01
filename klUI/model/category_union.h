#ifndef CATEGORY_UNION_H
#define CATEGORY_UNION_H

#include "kl_ui_data_union.h"

class CategoryModel;
class CategoryUnion : public kl::UINotifyIface
{
public:
    enum {
        MAIN_CATE = kl::OBJECT_CATEGORY_ALL,
        SUB_CATE  = kl::OBJECT_CATEGORY_SUB_LIST,
        BDC_CATE  = kl::OBJECT_CATEGORY_BDC,
        BDC_AREA_CATE = kl::OBJECT_BDC_AREA_LIST,
    };
    CategoryUnion(int cate_type, CategoryModel *parent);
    virtual ~CategoryUnion();

    void loadCategory(int cate_type, int cid);

    void dataPrepare();

    void errorInfo(int, const ByteString &);

    void onLoadOver(VectorTable<MusicCateUnion *> &vec);

private:
    const int                     mCateType;
    UICategory                   *m_pCate;
    CategoryModel                *m_pParentModel;

    void genCatesByCateMain(ListTable<kl::CateMain> &nodes, VectorTable<MusicCateUnion *> &vec);
    void genCatesByCateSub(ListTable<kl::CateSub> &nodes, VectorTable<MusicCateUnion *> &vec);
    void genCatesByBDCArea(ListTable<kl::AreaItem> &nodes, VectorTable<MusicCateUnion *> &vec);
    void genCatesByCateBDC(ListTable<kl::CateBCast> &nodes, VectorTable<MusicCateUnion *> &vec);

};

#endif // CATEGORY_UNION_H
