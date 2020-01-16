#ifndef KL_LOCAL_DATA_PROC_H
#define KL_LOCAL_DATA_PROC_H

#include <QObject>
#include "kl_ui_data_union.h"

class CollectNode;
class CollectModel;
class LocalDataProc : public QObject
{
    Q_OBJECT

    LocalDataProc();
    LocalDataProc(LocalDataProc &);
    LocalDataProc &operator=(LocalDataProc &);
public:
    ~LocalDataProc();
    static LocalDataProc* instance()
    {
        static LocalDataProc i;
        return &i;
    }

    void initLocal(CollectModel *, CollectModel *, CollectModel *);

    /**
     * @brief getLocalPlayIndex
     * @param ptr [in] 那个实例在调用，需要获取ID号
     * @return
     * @details 根据当前传递进来的实例获取播放index
     */
    int getLocalPlayIndex(ListTable<kl::RecordItem>::vector &vec);

    /**
     * @brief checkCurIsCollect
     * @param info
     * @details 确定当前播放项是否收藏
     */
    void checkCurIsCollect(CollectNode *info);

    /**
     * @brief opCurCollect
     * @param info
     * @details 如果当前项在收藏列表就取消收藏，反之添加，并通知界面
     */
    void opCurCollect(CollectNode *info);

    /**
     * @brief bdcTypeRadioCollect
     * @param bdcItemIndeex
     * @param info
     * @details 当前电台列表项, 收藏与否的点击
     */
    void bdcTypeRadioCollect(int bdcItemIndeex, CollectNode *info);

    /**
     * @brief checkBDCItemIsCollect
     * @param type
     * @param id
     * @return
     * @details 确定当前电台列表项是否在收藏列表
     */
    bool checkBDCItemIsCollect(int type, ByteString const& id);

    /**
     * @brief recordCurrentPlayItem
     * @param info
     * @details 记录当前播放的数据信息
     */
    void recordCurrentPlayItem(CollectNode *info);

    /**
     * @brief collectItemPlay
     * @param index [in] 列表标签项ID
     * @details 点击收藏项开始播放
     */
    void collectItemPlay(int index);

    /**
     * @brief collectItemEnable
     * @param index [in] 列表标签项ID
     * @details 在收藏列表中，删除或删除收藏项
     */
    void collectItemEnable(int index);

    /**
     * @brief downLoadItemPlay
     * @param index [in] 列表标签项ID
     * @details 点击下载项播放
     */
    void downLoadItemPlay(int index);

    /**
     * @brief historyItemPlay
     * @param index [in] 列表标签项ID
     * @details 点击历史记录播放
     */
    void historyItemPlay(int index);

    /**
     * @brief historyClearAll
     * @details 清空所有历史记录
     */
    void historyClearAll();

    void showCollectIndex();
    void showDownloadIndex();
    void showHistoryIndex();

public Q_SLOTS:
    void onCollect(int st, long ptr);
    void onDownload(int st, long ptr);
    void onRecord(int st, long ptr);

Q_SIGNALS:
    void collectStatus(int st, long ptr);
    void downloadStatus(int st, long ptr);
    void historyStatus(int st, long ptr);

private:
    //collect
    CollectModel      *m_pCollect;
    CollectModel      *m_pDownload;
    CollectModel      *m_pHistory;
};

#endif // KL_LOCAL_DATA_PROC_H
