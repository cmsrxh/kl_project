#ifndef IMAGE_LOAD_H
#define IMAGE_LOAD_H

// http://img.kaolafm.net/mz/images/201808/c65a54bc-86a0-4840-95b1-b0227587aca7/default.jpg
// http://img.kaolafm.net/mz/images/201906/eb01b107-d009-43be-a4bb-dbebc8496082/default.jpg
#include <QQuickPaintedItem>

class ImageFrame : public QQuickPaintedItem
{
    Q_OBJECT
public:
    ImageFrame();


    void paint(QPainter *painter);
};

#endif // IMAGE_LOAD_H
