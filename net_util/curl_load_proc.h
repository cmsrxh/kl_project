#ifndef CURL_LOAD_PROC_H
#define CURL_LOAD_PROC_H

#include <util/list_node.h>
#include <events/daemon_thread.h>
#include <events/sf_mutex.h>

class CurlLoadItem;
class CurlLoadProc : public DaemonThread
{
    CurlLoadProc();
public:
    static CurlLoadProc* instance()
    {
        static CurlLoadProc i;
        return &i;
    }
    ~CurlLoadProc();

    void addItem(CurlLoadItem *item);
    void removeItem(CurlLoadItem *item);

protected:
    void run();
    void cleanCurlMsg();
private:
    ListNode mRoot;
    SFMutex  mMtx;
};

#endif // CURL_LOAD_PROC_H
