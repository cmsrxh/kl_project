#ifndef CATEGORY_UNION_H
#define CATEGORY_UNION_H

#include "kl_ui_data_union.h"

class CategoryModel;
class CategoryUnion : public kl::UINotifyIface
{
public:
    enum {
        MAIN_CATE = 0,
        SUB_CATE  = 1,
    };
    CategoryUnion(int cate_type, CategoryModel *parent);

    void loadCategory(int cate_type, int cid);

    void dataPrepare();

    void errorInfo(int, const char *);

    void onLoadOver(VectorTable<MusicCateUnion *> &vec);

private:
    const int                     mCateType;
    UICategory                   *m_pCate;
    CategoryModel                *m_pParentModel;

    void genCatesByCateMain(ListTable<kl::CateMain> &nodes, VectorTable<MusicCateUnion *> &vec);
    void genCatesByCateSub(ListTable<kl::CateSub> &nodes, VectorTable<MusicCateUnion *> &vec);
};

#endif // CATEGORY_UNION_H
