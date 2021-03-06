#ifndef LOAD_ITEM_H
#define LOAD_ITEM_H

#include <stdio.h>
#include "net_common.h"
#include "net_url.h"

/**
 * @brief The LoadItem class
 * @note 收到多少数据就通过回调，返回多少数据，适宜把数据保存在文件中。
 */
class CurlLoadItem;
class NetUrl;
class NetBuffer;
class LoadItem
{
public:
    /**
     * @param [in] 是否一次性把数据反回来
     * @details true 表示需要用到 allDataObtain
     *          false 表示需要用到 oneFrameObtain和oneFrameObtainOver
     */
    LoadItem();
    LoadItem(bool isAll);
    /**
     * @warning 此处加上‘virtual’kaola应用中的qq_ip_positioning直接继承这个父类，程序立马段错误，发生各种意想不到的越界错误
     *  只要去掉virtual 立马就没有错误了
     *  但是在其他测试程序中测试，却未发生错误。？？？？？？
     *  解决: 发现是头文件包含出现偏差"app_common.h", klUI当前目录中,没有这个文件,在klApi有这个文件,
     *       当编译查找这个头文件的时候,其实找到了netLoad目录下的头文件
     */
    virtual ~LoadItem();

    /*********************方式一********************/
    /**
     * @brief setLoad
     * @param url [in] 传递的网络地址
     * @param fstate [in] 传递的回调函数
     * @param ptr [in] 传递私有参数
     * @param args [in] 有时候发出的https GET/POST 消息不会收到信息，但是数据都是对的，这有时需要在http的协议头上，加上user-agent
     * @return 是否成功
     * @details 设置参数，并执行下载
     */
    bool setLoad(const NetUrl &url, OpCurlStatus fstate, void *ptr, CurlLoadArg *args = nullptr);

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
    static void loadStatus (int status, void *data, void *arg);

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
    virtual void allDataObtain(NetBuffer */*data*/);

    /**
     * @brief oneFrameObtain
     * @param data [out]
     * @param len [out]
     * @details 表示传递的是整个下载过程中的一帧/一小块的数据，当构造传递的参数是false
     * 与oneFrameObtainOver函数搭配使用，构成真个下载流程
     */
    virtual void oneFrameObtain(NetBuffer */*data*/);
    /**
     * @brief oneFrameObtainOver
     * @details 便是块数据下载完成，与oneFrameObtain函数搭配使用
     */
    virtual void oneFrameObtainOver();

    /**
     * @brief errorInfo
     * @param type [out] libcurl库的错误类型
     * @param str [out] 对应错误的解释
     */
    virtual void errorInfo(int /*type*/, const char */*str*/);
private:    
    CurlLoadItem  *m_pLoad;
    bool           mIsLoadAll;
};

#endif // LOAD_ITEM_H
