#include <events/common_log.h>
#include <manage/application.h>
#include "media_hmi_ctrl.h"
#include "media_call_back.h"

void MediaCallBack::mediaNotify(int msg, int ext1, int ext2, const char *str)
{
    GEN_Printf(LOG_DEBUG, "mediaNotify: %d", msg);
    Q_EMIT mediaNotifySig(msg, ext1, ext2, str);
}

MediaCallBack::MediaCallBack()
{

}


//void SysDeviceCallBack::deviceAdd(dev::DeviceInfo *info)
//{
//    static char mountPath[256];

//    if ((dev::DEVICE_TYPE_UDISK == info->deviceType ||
//            dev::DEVICE_TYPE_SD == info->deviceType) &&
//            dev::DEVICE_ADD == info->action &&
//            dev::DEVICE_MOUNT_OK == info->mountState)
//    {
//        strcpy(mountPath, info->mountpath);

//        GEN_Printf(LOG_DEBUG, "Mount Path: %s", mountPath);
//        Application::instance()->postEvent(SIG_TRAVEL_MEDIA_FILE, (long)mountPath);
//    } else
//    {
//        GEN_Printf(LOG_DEBUG, "\n\t usb_index=%d"
//                              "\n\t part_index=%d"
//                              "\n\t action=%d"
//                              "\n\t mountState=%d"
//                              "\n\t deviceType=%d"
//                              "\n\t idVendor=%lu"
//                              "\n\t idProduct=%lu"
//                              "\n\t mountpath=%s",
//                   info->usb_index,  info->part_index,
//                   info->action,     info->mountState, info->deviceType,
//                   info->idVendor,   info->idProduct,  info->mountpath);
//    }
//}

//void SysDeviceCallBack::deviceRemove(int usb_index, int part_index, int deviceType)
//{
//    GEN_Printf(LOG_DEBUG, "usb_index=%d,part_index=%d,deviceType=%d", usb_index, part_index, deviceType);
//    if (dev::DEVICE_TYPE_UDISK == deviceType &&
//            dev::DEVICE_TYPE_SD == deviceType)
//    {
//        Application::instance()->postEvent(SIG_USB_DEVICE_REMOVE);
//    }
//}
