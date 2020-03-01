#ifndef KL_SAVE_RECORD_H
#define KL_SAVE_RECORD_H

#include "kl_url/kl_common.h"
#include <util/byte_string.h>
#include "save_manage_object.h"

enum
{
    // for all op
    LOCAL_RECORD_OP_STATUS_DATA_LOAD_OVER,

    // for collect op
    // 四个枚举，用来操作考拉收藏当前信息
    LOCAL_RECORD_OP_STATUS_CHECK_IN_LIST,
    LOCAL_RECORD_OP_STATUS_CHECK_NOT_IN_LIST,
    LOCAL_RECORD_OP_STATUS_ADD_IN_LIST,
    LOCAL_RECORD_OP_STATUS_REMOVE_IN_LIST,

    // BDC Items List Collect Op
    LOCAL_RECORD_OP_STATUS_BDC_COLLECT_ADD_IN_LIST,
    LOCAL_RECORD_OP_STATUS_BDC_COLLECT_REMOVE_IN_LIST,

    // for history op
    // 当前项是新增项
    LOCAL_RECORD_OP_STATUS_HISTORY_APPEND,
    // 当前项已经在历史记录中，移动到最前面
    LOCAL_RECORD_OP_STATUS_HISTORY_CHANGE,
    // 清空所有项
    LOCAL_RECORD_OP_STATUS_HISTORY_CLEARALL,

};

typedef void (*LocalDataStatus)(int status, long pri);

namespace kl
{
class SaveRecord : public SaveFileObject<RecordItem>
{
public:
    SaveRecord();

    void genResult(const char *data, unsigned long size);

    bool genSaveString(char *& data, unsigned long &len);

    void profile();

    void checkCurrentItem(RecordItem *item);

    void opCurrentItem(RecordItem *item);

    void setCallBack(LocalDataStatus st)
    {
        mDataStatus = st;
    }

    void execStatus(int st, long ptr = 0)
    {
        if (mDataStatus)
        {
            mDataStatus(st, ptr);
        }
    }
private:
    LocalDataStatus      mDataStatus;
};
}


#endif // KL_SAVE_RECORD_H
