#ifndef KL_UI_PROC_H
#define KL_UI_PROC_H

#include <QObject>

class CategoryAllModel;
class CateItemModel;
class QQmlContext;
class KLUIProc : public QObject
{
    Q_OBJECT
public:
    KLUIProc();

    void init(QQmlContext *);

private:
    CategoryAllModel    *m_pCate;
    CateItemModel       *m_pCateItem;
};

#endif // KL_UI_PROC_H
