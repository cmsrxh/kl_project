#ifndef DETAIL_QOBJECT_H
#define DETAIL_QOBJECT_H

#include "ui_data_union.h"
#include "util/map_table.h"
#include <QObject>

class DetailUnion;
class DetailQobject : public QObject
{    
    Q_OBJECT
    Q_PROPERTY(QStringList keyWords READ keyWords NOTIFY keyWordsChanged)

    Q_PROPERTY(QString albumInfoName READ albumInfoName NOTIFY albumInfoNameChanged)

    Q_PROPERTY(QString albumInfoImage READ albumInfoImage NOTIFY albumInfoImageChanged)

    Q_PROPERTY(QString albumInfoHostName READ albumInfoHostName NOTIFY albumInfoHostNameChanged)

public:
    ~DetailQobject();
    static DetailQobject *instance()
    {
        return priInstance ? priInstance
                           : priInstance = new DetailQobject;
    }

    void setDetailUnion(DetailUnion *pUnion)
    {
        m_pUnion = pUnion;
    }

    void getCurrent(DetailUnion *pUnion);

    void setDetailName(ByteString const &name)
    {
        mDetail.name = name;
    }

    QString albumInfoName()
    {
        return mName;
    }
    QString albumInfoImage()
    {
        return mImage;
    }
    QString albumInfoHostName()
    {
        return mHostName;
    }
    QStringList keyWords()
    {
        return mKeyWords;
    }

    void clearAlbumDetail();
public Q_SLOTS:
    void onLoadOver(long ptr); 

Q_SIGNALS:
    void dataLoadOver(long ptr);

    // NOT USED
    // void loadError(int type, long ptr, const QString &info);


    void keyWordsChanged();
    void albumInfoNameChanged();
    void albumInfoImageChanged();
    void albumInfoHostNameChanged();

     // 管理详情列表
public:
    void loadDetail(int type, ByteString const &id, int loadAction /*= DetailUnion::LOAD_DETAIL_SHOW_IN_ALBUM_VIEW*/);

private:
    DetailQobject();

    static DetailQobject *priInstance;

    DetailUnion *m_pUnion;
    DetailUnion *m_pPlayUnion;
    MusicDetail  mDetail;
    QStringList  mKeyWords;
    QString      mName;
    QString      mImage;
    QString      mHostName;

    MapTable<ByteString, DetailUnion *> mMap;
};

#endif // DETAIL_QOBJECT_H
