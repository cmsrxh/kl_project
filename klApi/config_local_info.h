#ifndef CONFIGLOCALINFO_H
#define CONFIGLOCALINFO_H

#include "app_common.h"
#include "events/common_log.h"
#include <util/config_setting.h>

class LocalConfig : public ConfigSetting
{
public:
    static LocalConfig *instance()
    {
        static LocalConfig i;
        return &i;
    }

    bool init()
    {
        loadFile(NET_LOCAL_CONFIG_FILE);

        mOpenID    = getValue("kaola", "openid");
        mAppID     = getValue("kaola", "appid");
        mSecretKey = getValue("kaola", "secretkey");
        mDeviceID  = getValue("kaola", "deviceid");

        if (mAppID.empty())
        {
            setAppID(APPID);
        }

        if (mSecretKey.empty())
        {
            setSecretKey(SECRETKEY);
        }

        if (mDeviceID.empty())
        {
            setDeviceID("69acf5ec77a42531d1748b7dfc6dcdb6");
        }

        return (mOpenID.empty()) ? false : true;
    }

    ByteString openID() const
    {
        return mOpenID;
    }

    void save()
    {
        saveFile(NET_LOCAL_CONFIG_FILE);
    }

    void setOpenID(const ByteString &openID)
    {
        setValue("kaola", "openid", openID);

        mOpenID = getValue("kaola", "openid");        
    }

    ByteString appID() const
    {
        return mAppID;
    }
    void setAppID(const ByteString &appID)
    {
        setValue("kaola", "appid", appID);

        mAppID = getValue("kaola", "appid");
    }

    ByteString secretKey() const
    {
        return mSecretKey;
    }

    void setSecretKey(const ByteString &secretKey)
    {
        setValue("kaola", "secretkey", secretKey);

        mSecretKey = getValue("kaola", "secretkey");
    }

    ByteString deviceID() const
    {
        return mDeviceID;
    }

    void setDeviceID(const ByteString &deviceID)
    {
        setValue("kaola", "deviceid", deviceID);

        mDeviceID = getValue("kaola", "deviceid");
    }
private:
    LocalConfig() {}

    ByteString mOpenID;
    ByteString mAppID;
    ByteString mSecretKey;
    ByteString mDeviceID;
};

#endif // CONFIGLOCALINFO_H



