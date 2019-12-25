#include "model/category_all_model.h"
#include "model/cate_item_model.h"
#include "kl_ui_proc.h"


KLUIProc::KLUIProc()
{

}

void KLUIProc::init(QQmlContext *ctx)
{
    m_pCate = new CategoryAllModel;
    m_pCateItem = new CateItemModel;

    m_pCate->setCateItemModel(m_pCateItem);

    ctx->setContextProperty("cateItemModel", m_pCateItem);
    ctx->setContextProperty("cateModel", m_pCate);
}
