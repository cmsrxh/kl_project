#include <QDebug>
#include <util/vector_table.h>
#include "application/loadApi/qqmusic_category.h"
#include "pl_category_model.h"

#define CATEGORY_NAME Qt::UserRole

PLCategoryModel::PLCategoryModel()
    : mCateLoad(new qqmusic::Category())
{
    //数据下载数据，通知到代理对象中
    mCateLoad->setUINotify(this);
    roles[CATEGORY_NAME] = "ctgname";

    connect(this, SIGNAL(sigLoadOver()), this, SLOT(onLoadOver()));
    connect(this, SIGNAL(sigLoadFailed(int,QString)), this, SLOT(onLoadFailed(int,QString)));
}

PLCategoryModel::~PLCategoryModel()
{
    qWarning() << "---cate list destruct.---";
}

void PLCategoryModel::obtain()
{
    if (mCateList.empty())
    {
        if (mCateLoad->isEmpty())
        {
            // 重上一次加载的配置中加载
            mCateLoad->loadNodesFile();

            if (mCateLoad->isEmpty())
            {
                mCateLoad->obtain();
            } else
            {
                onLoadOver();
            }
        }
    }
}

int PLCategoryModel::rowCount(const QModelIndex &parent) const
{
    return mCateList.size();
}

QVariant PLCategoryModel::data(const QModelIndex &index, int role) const
{
    if ( index.row() < 0 || index.row() >= (int)mCateList.size())
    {
        return QVariant();
    }

    if (CATEGORY_NAME == role)
    {
        return mCateList[index.row()];
    }

    return QVariant();
}

QObject * PLCategoryModel::getSubCate(int i)
{
    if (i >= 0 && i < mCateGroupItems.size())
    {
        return mCateGroupItems[i]; //dynamic_cast<PLCategorySubItemModel *> (mCateGroupItems[i])->getSubCateStrList();
    } else
    {
        return Q_NULLPTR; //QStringList();
    }
}

bool PLCategoryModel::getCateId(int groud_index, int index, int &cateId)
{
    if (groud_index < 0 || groud_index >= mCateGroupItems.size())
    {
        qWarning() << "Cate Group Item index is invalid";
        return false;
    }
    // qDebug() << "groud_index =" << groud_index << "size =" << mCateGroupItems.size();

    PLCategorySubItemModel *subModel = dynamic_cast<PLCategorySubItemModel *>(mCateGroupItems[groud_index]);

    //qDebug() << "subModel =" << (unsigned long)subModel;

    return subModel ? subModel->getCateId(index, cateId) : false;
}

void PLCategoryModel::updateIndex(int cateId, int oldCateId)
{
    // 更新标签ID
    int times = 2;

    ListTable<qqmusic::CateNode>::iterator it = mCateLoad->nodes().begin();
    for (int i = 0; it != mCateLoad->nodes().end(); ++it, i++)
    {
        ListTable<qqmusic::CateNode::Cate>::iterator its = it->items.begin();
        for (int j = 0; its != it->items.end(); ++its, j++)
        {
            if (its->categoryId == oldCateId)
            {
                dynamic_cast<PLCategorySubItemModel *>(mCateGroupItems[i])->setCurIndex(-1);
                if (--times == 0) return ;
            } else if (its->categoryId == cateId)
            {
                dynamic_cast<PLCategorySubItemModel *>(mCateGroupItems[i])->setCurIndex(j);
                if (--times == 0) return ;
            }
        }
    }
}

void PLCategoryModel::onLoadOver()
{
    qDebug() << "category load over.";

    mCateList.clear();
    ListTable<qqmusic::CateNode>::iterator it = mCateLoad->nodes().begin();
    for (int i = 0; it != mCateLoad->nodes().end(); ++it, i++)
    {
        // qDebug() << mCateGroupItems.size();

        if (mCateGroupItems.size() == i)
        {
            PLCategorySubItemModel *newNode = new PLCategorySubItemModel;
            mCateGroupItems.push_back(newNode);
        }

        PLCategorySubItemModel *tmpSub = dynamic_cast<PLCategorySubItemModel *>(mCateGroupItems[i]);
        ListTable<qqmusic::CateNode::Cate>::iterator its = it->items.begin();
        for (; its != it->items.end(); ++its)
        {
            PLCategorySubItemModel::ItemNode n;
            n.id   = its->categoryId;
            n.name = its->categoryName;
            tmpSub->pushNode(n);
        }

        // qDebug() << "i =" << i <<"subModel =" << (unsigned long)tmpSub;

        mCateList.push_back(QStringFromByteString(it->categoryGroupName));
    }

    Q_ASSERT(mCateGroupItems.size() == mCateList.size());

    if (!mCateGroupItems.isEmpty())
    {
        // 默认下载第一项标签数据
        dynamic_cast<PLCategorySubItemModel *>(mCateGroupItems[0])->setCurIndex(0);
    }

    beginResetModel();
    endResetModel();
}

void PLCategoryModel::onLoadFailed(int type, const QString &errStr)
{
    qWarning() << "Load failed," << type << errStr;

    switch (type)
    {
    case LOAD_EMPTY_DATA:        // 分析数据正确，但是得到的数据是空
        break;
    case LOAD_PRISER_JSOC_ERROR: // 不能正确解析json数据
        break;
    case LOAD_SYS_API_FAILED:    // libcurl下载反馈的错误信息
        break;
    default :
        SF_ASSERT(0);
        break;
    }
}

PLCategorySubItemModel::PLCategorySubItemModel()
    : mCurIndex(-1)
{
}

PLCategorySubItemModel::~PLCategorySubItemModel()
{
    qWarning() << "---sub cate list destruct.---";
}

void PLCategorySubItemModel::pushNode(const PLCategorySubItemModel::ItemNode &n)
{
    subCate.push_back(n);
    subCateStrList.push_back(QStringFromByteString(n.name));
}

int PLCategorySubItemModel::curIndex() const
{
    return mCurIndex;
}

void PLCategorySubItemModel::setCurIndex(int i)
{
    mCurIndex = i;
    Q_EMIT curIndexChanged();
}

bool PLCategorySubItemModel::getCateId(int index, int &cateId)
{
    if (index < 0 || index >= subCate.size())
    {
        qWarning() << "Cate Group Sub Item index is invalid";
        return false;
    }

    cateId = subCate[index].id;
    return true;
}

QStringList PLCategorySubItemModel::getSubCateStrList() const
{
    return subCateStrList;
}
