#ifndef IMAGE_LOAD_H
#define IMAGE_LOAD_H

// http://img.kaolafm.net/mz/images/201808/c65a54bc-86a0-4840-95b1-b0227587aca7/default.jpg
// http://img.kaolafm.net/mz/images/201906/eb01b107-d009-43be-a4bb-dbebc8496082/default.jpg
#include <QQuickPaintedItem>
#include <QImage>
#include "kl_url/kl_common.h"

class ImageFrameCpp : public QQuickPaintedItem, public ImageStatus
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
public:
    ImageFrameCpp();
    ~ImageFrameCpp();

    void paint(QPainter *painter);

    QString source() const;
    void setSource(const QString &source);

    void dataPrepare();
    void errorInfo(int , const char *);
    QRectF imageScale(int width, int height);

public Q_SLOTS:
    void onFilePrepare();
    void correctUpdate();

Q_SIGNALS:
    void sourceChanged(const QString &source);

    void filePrepare();

private:

    QString     mSource;
    ByteString  mLocalFile;
    QImage      mImage;
    QRectF      mUpdateRect;
};

#endif // IMAGE_LOAD_H
