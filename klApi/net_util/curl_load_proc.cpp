#include <curl/curl.h>
#include <events/common_log.h>
#include "curl_load_item.h"
#include "curl_load_proc.h"

CurlLoadProc::CurlLoadProc()
{    
}

CurlLoadProc::~CurlLoadProc()
{
    TRAVEL_LIST_AND_DEL(mRoot)
    {
        tmp->del();
        delete static_cast<CurlLoadItem *>(tmp);
    }
}

void CurlLoadProc::run()
{
    CURLMcode ret;
    long L = 0;
    int res, M = 0, U = -1;
    fd_set R, W, E;
    struct timeval T;

    while (1)
    {
        CurlGlobal::instance()->perform(&U);
//        GEN_Printf(LOG_DEBUG, "U: %d, L: %ld, M: %d", U, L, M);
        if (0 == U)
        {
            GEN_Printf(LOG_INFO, "Load exec over !!!");
            break;
        } else
        {
            FD_ZERO(&R);
            FD_ZERO(&W);
            FD_ZERO(&E);

            ret = CurlGlobal::instance()->fdset(&R, &W, &E, &M);
            if (CURLM_OK != ret)
            {
                GEN_Printf(LOG_ERROR, "curl_multi_fdset failed, %s", curl_multi_strerror(ret));
                break;
            }

            ret = CurlGlobal::instance()->timeout(&L);
            if (CURLM_OK != ret)
            {
                GEN_Printf(LOG_ERROR, "E: curl_multi_timeout, %s", curl_multi_strerror(ret));
                break;
            }

            if (L == -1)
            {
                L = 1000;
            }

            if (M == -1)
            {
                usleep((uint32_t)L * 1000);
            } else
            {
                T.tv_sec = L/1000;
                T.tv_usec = (L%1000)*1000;
                res = select(M + 1, &R, &W, &E, &T);
                if (res < 0)
                {
                    GEN_Printf(LOG_DEBUG, "E: select(%i,,,,%li): %i: %s", M + 1, L, errno, strerror(errno));
                    break;
                }
                cleanCurlMsg();
//                GEN_Printf(LOG_DEBUG, "%d, %d,  %s", res, errno, strerror(errno));
            }
        }
    }

    cleanCurlMsg();

    // 删除所有取消下载的项
    mMtx.lock();
    TRAVEL_LIST_AND_DEL(mRoot)
    {
        tmp->del();
        delete static_cast<CurlLoadItem *>(tmp);
    }
    mMtx.unlock();
}

void CurlLoadProc::addItem(CurlLoadItem *item)
{
    Autolock l(&mMtx);

    mRoot.add(item);
    CURLMcode ret = CurlGlobal::instance()->addHandle(item);
    if (CURLM_OK != ret)
    {
        GEN_Printf(LOG_ERROR, "curl_multi_add_handle failed, %s", curl_multi_strerror(ret));
        throw -1;
    }
}

void CurlLoadProc::removeItem(CurlLoadItem *item)
{
    Autolock l(&mMtx);
    TRAVEL_LIST_AND_DEL(mRoot)
    {
        if (tmp == item)
        {
            tmp->del();
            delete item;
            return;
        }
    }
}

void CurlLoadProc::cleanCurlMsg()
{
    CURLMsg *msg;
    int  Q;

    while ((msg = CurlGlobal::instance()->infoRead(&Q)))
    {
        if (CURLMSG_DONE == msg->msg)
        {
            CURL *e = msg->easy_handle;
            CurlLoadItem *cur_load = NULL;

            mMtx.lock();
            TRAVEL_LIST_AND_DEL(mRoot)
            {
                if (static_cast<CurlLoadItem *>(tmp)->check(e))
                {
                    cur_load = static_cast<CurlLoadItem *>(tmp);
                    tmp->del();
                    break;
                }
            }
            mMtx.unlock();

            if (cur_load)
            {
                if (msg->data.result == CURLE_OK)
                {
                    cur_load->tryCallBack();
                } else
                {
                    // GEN_Printf(LOG_WARN, "message result: %s", curl_easy_strerror(msg->data.result));
                    cur_load->errorState(msg->data.result, curl_easy_strerror(msg->data.result));
                }

                CurlGlobal::instance()->clean(e);
                // CurlLoadItem 析构函数中做释放cleanup
                delete cur_load;
            } else
            {
                CurlGlobal::instance()->clean(e);

                curl_easy_cleanup(e);
            }
        } else
        {
            GEN_Printf(LOG_ERROR, "CURLMsg failed, (%d)", msg->msg);
        }
    }
}


