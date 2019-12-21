#ifndef LOAD_ITEM_H
#define LOAD_ITEM_H

#include "app_common.h"
#include "net_url.h"

/**
 * @brief The LoadItem class
 * @note 收到多少数据就通过回调，返回多少数据，适宜把数据保存在文件中。
 */
class CurlLoadItem;
class LoadItem
{
public:
    /** 是否一次性把数据反回来 */
    LoadItem(bool = true);

    bool setLoad(const NetUrl &url, OpCurlStatus fstate, void *ptr);

    bool cancel();
    bool isLoading()
    {
        return !!m_pLoad;
    }
    void loadOver()
    {
        m_pLoad = nullptr;
    }

private:    
    CurlLoadItem  *m_pLoad;
    bool           mIsLoadAll;
};

#endif // LOAD_ITEM_H
