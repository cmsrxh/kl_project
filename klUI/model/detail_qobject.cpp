#include "detail_union.h"
#include "detail_qobject.h"
#include "kl_data_proc.h"
#include <QDebug>

DetailQobject *DetailQobject::priInstance = NULL;

DetailQobject::~DetailQobject()
{
    mDetail.keywords->clear();
}

void DetailQobject::getCurrent()
{
    m_pUnion->getDetail(mDetail);
}

void DetailQobject::onLoadOver(long ptr)
{
    DetailUnion *unionPtr = reinterpret_cast<DetailUnion *> (ptr);

    unionPtr->getDetail(mDetail);

    switch (unionPtr->getLoadAction())
    {
    case DetailUnion::LOAD_DETAIL_SHOW_IN_ALBUM_VIEW:
    {
        Q_ASSERT(PLAY_CHIP_TYPE_ALBUM == unionPtr->getChipType());
        if (unionPtr == m_pUnion)
        {
            QList<QString> keyWords;
            ListTable<ByteString>::iterator it = mDetail.keywords->begin();
            for (; it != mDetail.keywords->end(); ++it)
            {
                keyWords.push_back(QStringFromByteString(*it));
            }
            KLDataProc::instance()->enterAlbumView();

            mKeyWords.swap(keyWords);

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

        break;
    }
    default:
        break;
    }

}

QString DetailQobject::albumInfoName()
{
    return QStringFromByteString(mDetail.name);
}

QString DetailQobject::albumInfoImage()
{
    return QStringFromByteString(mDetail.image);
}

QString DetailQobject::albumInfoHostName()
{
    return QStringFromByteString(mDetail.hostName);
}

QStringList DetailQobject::keyWords()
{
    return mKeyWords;
}

DetailQobject::DetailQobject()
{
    connect(this, SIGNAL(dataLoadOver(long)), this, SLOT(onLoadOver(long)), Qt::QueuedConnection);
}
