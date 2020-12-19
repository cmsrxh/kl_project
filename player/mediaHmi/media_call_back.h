#ifndef MEDIA_CALL_BACK_H
#define MEDIA_CALL_BACK_H

#include <media_service_call_back.h>
//#include <sys_device_client_call_back.h>
#include <QObject>

//用于接收服务端反馈来的信息
class MediaCallBack: public QObject, public MediaServiceCallback
{
    Q_OBJECT
public:
    MediaCallBack();

    void mediaNotify(int msg, int ext1, int ext2, const char *str);

Q_SIGNALS:
    void mediaNotifySig(int msg, int ext1, int ext2, QString const &str);

private:

};

//class SysDeviceCallBack : public SysDeviceClientCallBack
//{
//public:
//    virtual void deviceAdd(dev::DeviceInfo *);

//    virtual void deviceRemove(int usb_index, int part_index, int deviceType);
//};

#endif // MEDIA_CALL_BACK_H
