#include "detail_union.h"
#include "detail_qobject.h"
#include "kl_data_proc.h"
#include <QDebug>

DetailQobject *DetailQobject::priInstance = NULL;

DetailQobject::~DetailQobject()
{
    mDetail.keywords->clear();
}

void DetailQobject::getCurrent(DetailUnion *pUnion)
{
    pUnion->getDetail(mDetail);
}

void DetailQobject::onLoadOver(long ptr)
{
    DetailUnion *unionPtr = reinterpret_cast<DetailUnion *> (ptr);

    unionPtr->getDetail(mDetail);
    qDebug() << "detail info load over, loadaction=" << unionPtr->getLoadAction() << ptr;

    switch (unionPtr->getLoadAction())
    {
    case DetailUnion::LOAD_DETAIL_SHOW_IN_ALBUM_VIEW:
    {
        // Q_ASSERT(PLAY_CHIP_TYPE_ALBUM == unionPtr->getChipType());
        if (unionPtr == m_pUnion)
        {
            QList<QString> keyWords;
            ListTable<ByteString>::iterator it = mDetail.keywords->begin();
            for (; it != mDetail.keywords->end(); ++it)
            {
                keyWords.push_back(QStringFromByteString(*it));
            }

            mName     = QStringFromByteString(mDetail.name);
            mImage    = QStringFromByteString(mDetail.image);
            mHostName = QStringFromByteString(mDetail.hostName);

            mKeyWords.swap(keyWords);

            KLDataProc::instance()->enterAlbumView();
            Q_EMIT albumInfoNameChanged();
            Q_EMIT albumInfoImageChanged();
            Q_EMIT albumInfoHostNameChanged();
            Q_EMIT keyWordsChanged();
        }
        break;
    }
    case DetailUnion::LOAD_DETAIL_AUDIO_PLAYING:
    {
        Q_ASSERT(PLAY_CHIP_TYPE_AUDIO_CHIP == unionPtr->getChipType());
        m_pPlayUnion = unionPtr;
        KLDataProc::instance()->audioDetailLoadOver(mDetail);
        break;
    }
    default:
        break;
    }

}

void DetailQobject::onLoadError(int /*type*/, long ptr, const QString &info)
{
    DetailUnion *unionPtr = reinterpret_cast<DetailUnion *> (ptr);
    qDebug() << "detail load error" << type << info;
    switch (unionPtr->getLoadAction())
    {
    case DetailUnion::LOAD_DETAIL_SHOW_IN_ALBUM_VIEW:
    {
        break;
    }
    case DetailUnion::LOAD_DETAIL_AUDIO_PLAYING:
    {
        break;
    }
    default:
        break;
    }
}

void DetailQobject::loadDetail(int type, const ByteString &id, int loadAction)
{
    ByteString detailId = ByteString::allocString(id);
    DetailUnion *&pUnion = mMap[detailId];
    if (pUnion)
    {
        if (loadAction == DetailUnion::LOAD_DETAIL_SHOW_IN_ALBUM_VIEW)
        {
            m_pUnion = pUnion;
        }
        onLoadOver((long)pUnion);
    } else
    {
        pUnion = new DetailUnion(type);
        qDebug() << "loaddetail action=" << loadAction << (long)pUnion;
        pUnion->loadDetail(detailId, loadAction);
        if (loadAction == DetailUnion::LOAD_DETAIL_SHOW_IN_ALBUM_VIEW)
        {
            m_pUnion = pUnion;
        }
        clearAlbumDetail();
    }
}

void DetailQobject::clearAlbumDetail()
{
    mName.clear();
    mImage.clear();
    mHostName.clear();
    mKeyWords.clear();
}

DetailQobject::DetailQobject()
{
    connect(this, SIGNAL(dataLoadOver(long)), this, SLOT(onLoadOver(long)));
}
