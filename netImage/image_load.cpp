#include <events/common_log.h>
#include "kl_url/kl_image.h"
#include "image_load.h"
#include <QPainter>

ImageFrame::ImageFrame()
{

}

void ImageFrame::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);

    painter->fillRect(QRect(0, 0, this->width(), this->height()), Qt::red);

}
