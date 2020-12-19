#include "image_provider.h"
#include <QDebug>

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{

}

//这个是自动调用的。路径"image://screen"，如需要使用id等参数，如"image://screen/id"等
QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    Q_UNUSED(size);
    Q_UNUSED(requestedSize)

    qDebug() << "id: " << id << "size: " << *size << "requestedSize: " << requestedSize;
    return mImage;
}

void ImageProvider::setImage(const QImage &image)
{
    mImage = image;
}
