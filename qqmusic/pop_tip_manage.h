#ifndef POP_TIP_MANAGE_H
#define POP_TIP_MANAGE_H

#include <util/byte_string.h>

class ATimer;
/**
 * @brief The PopTipManage class
 * @details 系统弹框处理
 * @warning 下载函数中的内容都是发送消息，所以都是线程安全的
 */
class PopTipManage
{
    PopTipManage(PopTipManage &);
    PopTipManage &operator=(PopTipManage &);
public:
    enum LoadAction
    {
        LOAD_MAIN_PAGE,  // 下载主页数据
        LOAD_NEXT_PAGE,  // 下载新数据，如：上一页、下一页、新的ID


        //old
        LOAD_ALBUM_NORMAL_SHOW,        // 正常加载数据，并显示出来，应用于专辑相关的界面
        LOAD_ALBUM_IN_PLAYVIEW,        // 正常加载的数据，显示在节目列表界面中，用于专辑当前正在播放的列表,发生在当前播放列表已经放完需要后台加载。
        LOAD_BDCPROGRAM_IN_PLAYVIEW,   // 显示广播节目，在节目列表界面上, 然后要自动播放最新的电台节目
        LOAD_BACK_PLAY_OP,             // 直接播放专辑等二级列表项
        LOAD_DETAIL_SHOW_IN_ALBUM_VIEW,     // 显示在在专辑信息界面中的
        LOAD_DETAIL_AUDIO_PLAYING           // 下载专辑碎片并进行播放
    };

    static PopTipManage *instance()
    {
        static PopTipManage i;
        return &i;
    }
    ~PopTipManage();

    /**
     * @brief klObjectObtainStart
     * @param state [in] 调用下载函数的返回状态
     * @param objectName [in] 对象名字
     * @param loadAction [in] LoadAction
     * @details 基于KlObject派生类开始调用obtain函数，获取kl数据
     * @note 在App事件循环子线程中执行
     */
    void klObjectObtainStart(bool state, int objectName, int loadAction);

    /**
     * @brief klObjectObtainOver
     * @param objectName
     * @details 基于KlObject派生类, 数据下载完成
     */
    void klObjectObtainOver(int objectName, int loadAction);

    /**
     * @brief klLoadDataExportEmpty
     * @param objectName [in] 对象名字
     * @param loadAction [in] LoadAction
     * @param err_str [in] error info
     * @details  kl下载的数据，是空列表
     * @note 在App事件循环子线程中执行
     * @warning 不以界面区分，应用各个页面有自己对应的信号接收
     */
    void klLoadDataExportEmpty(int objectName, int loadAction, const ByteString &err_str);

    /**
     * @brief klLoadDataPriserExcept
     * @param objectName [in] 对象名字
     * @param loadAction [in] LoadAction
     * @param err_str [in] error info
     * @details  kl下载的数据，json格式数据解析失败了（大多都是传递 了错误的参数，或者服务器那边出现问题）
     * @note 在App事件循环子线程中执行
     * @warning 不以界面区分，应用各个页面有自己对应的信号接收
     */
    void klLoadDataPriserExcept(int objectName, int loadAction, const ByteString &err_str);

    /**
     * @brief sysNetLoadApiExcept
     * @param objectName [in] 对象名字
     * @param loadAction [in] LoadAction
     * @param err_str [in] error info
     * @details 调用libcurl下载kl数据失败，一般时本地网络出现问题
     *          例如：下载超时、网络断开等等原因。
     * @note 在App事件循环子线程中执行
     * @warning 不以界面区分，应用各个页面有自己对应的信号接收
     */
    void sysNetLoadApiExcept(int objectName, int loadAction, const ByteString &err_str);

    /**
     * @brief closeMsgBox
     * @param objectName [in]
     * @details 加载本地数据时，关闭弹框
     */
    void closeMsgBox(int objectName);

    /**
     * @brief errReloadStart
     * @param objectName [in]
     * @details 表示错误的时候重新加载数据
     */
    void errReloadStart(bool state, int objectName)
    {
        klObjectObtainStart(state, objectName, mBackLoadAction);
    }
private:
    PopTipManage();
    static void msgTipTimer(ATimer *that, void *ptr);
    void showDelayMsgBox();


    int     mCurrenObjectName;
    int     mBackLoadAction;
    ATimer *m_pMsgPopDelayTimer;
};

#endif // POP_TIP_MANAGE_H
