#ifndef IMAGE_PROVIDER_H
#define IMAGE_PROVIDER_H

#include <QQuickImageProvider>
#include <QImage>

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    void setImage(const QImage &image);

private:
    QImage mImage;
};

#endif // IMAGE_PROVIDER_H
