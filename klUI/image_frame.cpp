#include <events/common_log.h>
#include "kl_url/kl_image.h"
#include "image_frame.h"
#include "image_cache_manage.h"
#include <QPainter>
#include <QDebug>

ImageFrameCpp::ImageFrameCpp()
{
    connect(this, SIGNAL(filePrepare()), this, SLOT(onFilePrepare()));
    connect(this, SIGNAL(widthChanged()), this, SLOT(correctUpdate()));
    connect(this, SIGNAL(heightChanged()), this, SLOT(correctUpdate()));
}

ImageFrameCpp::~ImageFrameCpp()
{
    ImageCacheManage::instance()->destructNotify(this);
}

void ImageFrameCpp::paint(QPainter *painter)
{
//    QRectF rectangle(0, 0, this->width(), this->height());
//    painter->setRenderHint(QPainter::Antialiasing);

//    painter->fillRect(QRectF(0, 0, this->width(), this->height()), Qt::gray);

    if (!mImage.isNull())
    {
        QRectF updateRect = imageScale(mImage.width(), mImage.height());
        QImage tmp        = mImage.scaled(updateRect.width(), updateRect.height());
        //qDebug() << updateRect;
        painter->drawImage(updateRect, tmp);
    }

//    QPen pen;
//    pen.setColor(QColor(0xff, 0x00, 0x00, 0xe1));
//    pen.setWidth(4);
//    pen.setJoinStyle(Qt::RoundJoin);
//    pen.setCapStyle(Qt::RoundCap);
//    painter->setPen(pen);

//    painter->drawRoundedRect(rectangle, 3.0, 4.0);
}

QString ImageFrameCpp::source() const
{
    return mSource;
}

void ImageFrameCpp::setSource(const QString &source)
{
    mSource = source;

    ImageCacheManage::instance()->loadImage(ByteString(source.toStdString().c_str(), source.size()), this);
}

void ImageFrameCpp::dataPrepare()
{
    Q_EMIT filePrepare();
}

void ImageFrameCpp::errorInfo(int , const char *)
{
    //qDebug() << "type = " << type << str;
}

void ImageFrameCpp::setLocalFile(const ByteString &name)
{
    mLocalFile = name;
}

QRectF ImageFrameCpp::imageScale(int w, int h)
{
    int screenW = this->width();
    int screenH = this->height();

    int scaleW = 0;
    int scaleH = 0;
    double scale = 0.0;   


    if (screenW * h > screenH * w) //高更突出
    {
        scaleW = screenW;
        scale = (double)screenW / w;
        scaleH = h * scale;;
    } else
    {
        scaleH = screenH;
        scale = (double)screenH / h;
        scaleW = w * scale;;
    }


    int x = (screenW - scaleW) / 2;
    int y = (screenH - scaleH) / 2;

//    qDebug() << x << y << scaleW << scaleH << "orgin" << w << h; // x, y, scaleW, scaleH;
//    qDebug() << screenW << screenH;

    return QRectF(x, y, scaleW, scaleH);
}

void ImageFrameCpp::onFilePrepare()
{
    //qDebug() << "image data prepare: " << mLocalFile.string();
    mImage.load(mLocalFile.string());
    correctUpdate();
}

void ImageFrameCpp::correctUpdate()
{
    if ((width() > 0 )
            && (height() > 0)
            && !mImage.isNull())
    {
        this->update();
    }
}
