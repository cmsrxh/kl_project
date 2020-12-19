#ifndef MEDIA_I_FACE_H
#define MEDIA_I_FACE_H

#include <media_iface_common.h>
#include <media_client_proxy.h>

//用于HMI主动调用，服务端的接口

class MediaHMIIFace : public MediaClientProxy
{
    MediaHMIIFace()
        : MediaClientProxy()
    {}
public:
   static MediaHMIIFace *instance()
   {
       static MediaHMIIFace i;
       return &i;
   }
   void playFile(int mediaType, int index);

   void autoPlayNext();
   void showPlayingInfo();

   void prev();
   void next();
   void modelSwitch(int mediaType);
   void pictureWallpaper();
   void pictureScaleModel(int model);
   void pictureRotateModel(int model);
};

#endif // MEDIA_I_FACE_H
