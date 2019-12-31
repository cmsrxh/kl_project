#ifndef DETAIL_QOBJECT_H
#define DETAIL_QOBJECT_H

#include "kl_ui_data_union.h"
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
        return priInstance ? priInstance : priInstance = new DetailQobject;
    }

    void setDetailUnion(DetailUnion *pUnion)
    {
        m_pUnion = pUnion;
    }

    void getCurrent();

    void setDetailName(ByteString const &name)
    {
        mDetail.name = name;
    }

public Q_SLOTS:
    void onLoadOver(long ptr);

    QString albumInfoName();
    QString albumInfoImage();
    QString albumInfoHostName();
    QStringList keyWords();

Q_SIGNALS:
    void dataLoadOver(long ptr);

    void loadError(int type, const QString &info);

    //
    void keyWordsChanged();
    void albumInfoNameChanged();
    void albumInfoImageChanged();
    void albumInfoHostNameChanged();
private:
    DetailQobject();

    static DetailQobject *priInstance;

    DetailUnion *m_pUnion;
    MusicDetail  mDetail;
    QStringList  mKeyWords;
};

#endif // DETAIL_QOBJECT_H
