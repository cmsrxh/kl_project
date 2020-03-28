#ifndef CATEGORY_UNION_H
#define CATEGORY_UNION_H

#include "ui_data_union.h"

class CategoryModel;
class CategoryUnion : public UINotifyIface
{
public:
    enum {
        MAIN_CATE /*= kl::OBJECT_CATEGORY_ALL*/,
        SUB_CATE /* = kl::OBJECT_CATEGORY_SUB_LIST*/,
        BDC_CATE /* = kl::OBJECT_CATEGORY_BDC*/,
        BDC_AREA_CATE /*= kl::OBJECT_BDC_AREA_LIST*/,
    };
    CategoryUnion(int cate_type, CategoryModel *parent);
    virtual ~CategoryUnion();

    void loadCategory(int cate_type, int cid);

    void dataPrepare();

    void errorInfo(int, const ByteString &);

    void onLoadOver(VectorTable<FirstMenuUnion *> &vec);

private:
    const int                     mCateType;
    UICategory                   *m_pCate;
    CategoryModel                *m_pParentModel;
};

#endif // CATEGORY_UNION_H
