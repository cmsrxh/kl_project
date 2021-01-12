#include <events/common_log.h>
#include "image_frame.h"
#include "common_func/image_cache_manage.h"
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
//    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (!mImage.isNull())
    {
        QRect updateRect = imageScale(mImage.width(), mImage.height());
        QImage tmp       = mImage.scaled(updateRect.width()
                                         , updateRect.height()
                                         , Qt::IgnoreAspectRatio
                                         , Qt::SmoothTransformation);
        painter->drawImage(updateRect, tmp);
    }
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

QRect ImageFrameCpp::imageScale(int w, int h)
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

    return QRect(x, y, scaleW, scaleH);
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
