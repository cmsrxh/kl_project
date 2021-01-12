#include <QDebug>
#include "application/loadApi/qqmusic_category_playlist.h"
#include "category_playlist.h"
#include "cpl_cate_item_model.h"

CPLCateItemModel::CPLCateItemModel()
    : mCPL(new qqmusic::CategoryPlaylist())
{
    mCPL->setUINotify(this);

    roles[Qt::UserRole]   = "cateItemName";
    roles[Qt::UserRole+1] = "imageUri";
    roles[Qt::UserRole+2] = "collect";

    connect(this, SIGNAL(sigLoadOver()), this, SLOT(onLoadOver()));
    connect(this, SIGNAL(sigLoadFailed(int,QString)), this, SLOT(onLoadFailed(int,QString)));
}

CPLCateItemModel::~CPLCateItemModel()
{

}

void CPLCateItemModel::obtain()
{
    if (mList.empty())
    {
        if (mCPL->isEmpty())
        {
            bool ret = mCPL->obtain();

            qDebug() << "start load cate play list" << ret;
        } else
        {
            onLoadOver();
        }
    }
}

void CPLCateItemModel::loadByCateId(int id)
{
    mCPL->loadByCateId(id);
}

void CPLCateItemModel::loadPrevPage()
{
    mCPL->loadPrevPage();
}

void CPLCateItemModel::loadNextPage()
{
    mCPL->loadNextPage();
}

void CPLCateItemModel::loadPage(int index)
{
    mCPL->loadPage(index);
}

int CPLCateItemModel::getTotalPages()
{
    return mCPL->getTotalPages();
}

int CPLCateItemModel::getCurrentPage()
{
    return mCPL->getCurrentPage();
}

int CPLCateItemModel::getCurCateId()
{
    return mCPL->getCategoryId();
}

int CPLCateItemModel::rowCount(const QModelIndex &parent) const
{
    return mList.size();
}

QVariant CPLCateItemModel::data(const QModelIndex &index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= (int)mList.size())
    {
        return QVariant();
    }

    switch (role) {
    case Qt::UserRole:  // cateItemName
        return QStringFromByteString(mList[i].name);
    case Qt::UserRole + 1: // imageUri
        return QStringFromByteString(mList[i].img);
    case Qt::UserRole + 2: // collect
        return false;
    default:
        break;
    }

    return QVariant();
}

void CPLCateItemModel::onLoadOver()
{
    mList.clear();

    qDebug() << "Data Play List Load Over";
    ListTable<qqmusic::CatePlayList> &nodes = mCPL->nodes();
    ListTable<qqmusic::CatePlayList>::iterator it = nodes.begin();
    for (; it != nodes.end(); ++it)
    {
        ItemNode n;

        n.id   = it->dissid;
        n.name = it->dissname;
        n.img  = it->imgurl;

        mList.push_back(n);
    }

    Q_EMIT sigPlayListUpdate();

    reset();
}

void CPLCateItemModel::onLoadFailed(int type, const QString &errStr)
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
