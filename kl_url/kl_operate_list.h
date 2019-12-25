#ifndef KL_OPERATE_LIST_H
#define KL_OPERATE_LIST_H

#include "kl_save_object.h"

namespace kl
{
class OperateList : public SaveObject<Operate>
{
public:
    /**
     * @brief OperateList
     * @param pagenum
     * @param pagesize
     * @details 推荐
     */
    OperateList(int pagenum = 1, int pagesize = 20);
    ~OperateList();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(const char *data, unsigned long size);

private:
    ByteString mPageNum;
    ByteString mPageSize;


    int page;
    int pageSize;
    int count;
    int totalPages;
    int haveNext;
    int nextPage;
    int havePre;
    int prePage;
};
}

#endif // KL_OPERATE_LIST_H
