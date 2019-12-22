#ifndef LOAD_ITEM_H
#define LOAD_ITEM_H

#include <stdio.h>
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
    /**
     * @param [in] 是否一次性把数据反回来
     * @details true 表示需要用到 allDataObtain
     *          false 表示需要用到 oneFrameObtain和oneFrameObtainOver
     */
    LoadItem(bool = true);
    virtual ~LoadItem();

    /*********************方式一********************/
    /**
     * @brief setLoad
     * @param url [in] 传递的网络地址
     * @param fstate [in] 传递的回调函数
     * @param ptr [in] 传递私有参数
     * @return 是否成功
     * @details 设置参数，并执行下载
     */
    bool setLoad(const NetUrl &url, OpCurlStatus fstate, void *ptr);

    /**
     * @brief cancel
     * @return 是否成功
     * @details 取消下载过程
     */
    bool cancel();

    /**
     * @brief isLoading
     * @return
     * @details 判断是否正在下载
     */
    bool isLoading()
    {
        return !!m_pLoad;
    }

    /**
     * @brief loadOver
     * @details 表示下载完成了，不管对错，并释放了m_pLoad指向的指针
     */
    void loadOver()
    {
        m_pLoad = nullptr;
    }

    /*********************方式二********************/
    /**
     * @brief loadStatus
     * @param status [in] 下载方式
     * @param data [in] 数据部分，是否有效，需要看status的类型含义。
     * @param len [in] 长度，是否有效，需要看status的类型含义。
     * @param arg [in] 回调的私有数据
     * @details 下载功能的回调函数
     */
    static void loadStatus (int status, uint8_t *data, size_t len, void *arg);

    /**
     * @brief obtain
     * @param url [in] 传递进来的网络地址
     * @return 成功与否的标志
     * @details 开始下载
     */
    bool obtain(const NetUrl &url);

    /**
     * @brief allDataObtain
     * @param data [out]
     * @param len [out]
     * @details 表示一次性回调给的所有数据, 根据构造函数传递的参数是true
     */
    virtual void allDataObtain(uint8_t */*data*/, size_t /*len*/) {}

    /**
     * @brief oneFrameObtain
     * @param data [out]
     * @param len [out]
     * @details 表示传递的是整个下载过程中的一帧/一小块的数据，当构造传递的参数是false
     * 与oneFrameObtainOver函数搭配使用，构成真个下载流程
     */
    virtual void oneFrameObtain(uint8_t */*data*/, size_t /*len*/) {}
    /**
     * @brief oneFrameObtainOver
     * @details 便是块数据下载完成，与oneFrameObtain函数搭配使用
     */
    virtual void oneFrameObtainOver() {}

    /**
     * @brief errorInfo
     * @param type [out] libcurl库的错误类型
     * @param str [out] 对应错误的解释
     */
    virtual void errorInfo(int /*type*/, const char */*str*/) {}
private:    
    CurlLoadItem  *m_pLoad;
    bool           mIsLoadAll;
};

#endif // LOAD_ITEM_H
