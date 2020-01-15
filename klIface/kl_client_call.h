#ifndef KL_CLIENT_CALL_H
#define KL_CLIENT_CALL_H

#include "kl_client_result_common.h"

class ClientMsgPriserProc;
class ClientIfaceBase;
class KLClientCall
{
public:
    static KLClientCall *instance()
    {
        static KLClientCall i;
        return &i;
    }
    bool initClientResult(client::ClientResult *res);
    bool initClientIface(ClientMsgPriserProc *);

    /**
     * @brief searchKeyWord
     * @param str [in] 收搜关键字
     * @return 操作是否成功
     */
    bool searchKeyWord(const char *str);

    /**
     * @brief playSearchWord
     * @param str [in] 根据搜索结构列表中的名字进行处理
     * @details 播放搜索列表中的名字
     */
    bool playSearchWord(const char *str);

    /**
     * @brief playSearchIndex
     * @param index [in] 根据搜索结构列表中,所在的ID号位置
     * @details 播放搜索列表中对应的ID号
     */
    bool playSearchIndex(int index);

    /**
     * @brief playPause
     * @details 暂停
     */
    bool playPause();

    /**
     * @brief playPlaying
     * @details 播放
     */
    bool playPlaying();

    /**
     * @brief playNext
     * @details 播放下一首
     */
    bool playNext();

    /**
     * @brief playPrev
     * @details 播放上一首
     */
    bool playPrev();

    /**
     * @brief setIdentity
     * @param id [in] 表明当前时那个应用在搜索，否则多个应用同时调用搜索服务，
     *      会导致服务端不能识别身份，统一当成一个操作，
     *      这样就会导致，某个应用要得数据于收到的数据不符合.
     */
    void setIdentity(char id[32]);

    bool checkId(const char *id);
private:
    KLClientCall();
    KLClientCall(KLClientCall &);
    KLClientCall &operator=(KLClientCall &);

    bool commonOneString(int cmd, const char *str);
    bool commonNoArg(int cmd);

    ClientIfaceBase         *m_pHandler;
    client::ClientResult    *m_pResult;
    char                     mID[32];

    friend class MsgPriser;
};

#endif // KL_CLIENT_CALL_H
