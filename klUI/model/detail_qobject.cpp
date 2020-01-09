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
    if ((long)m_pUnion != ptr)
    {
        qWarning() << "Current is not need.";
        return;
    }

    m_pUnion->getDetail(mDetail);

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
