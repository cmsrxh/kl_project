#include <events/common_log.h>
#include "model/data_proc.h"
#include "model/local_data_proc.h"
#include "model/chip_item_play_manage.h"
#include "ui_proc.h"
#include "events/app_timer.h"
#include "application.h"
#include "pop_tip_manage.h"

extern UIProc *gInstance;

PopTipManage::PopTipManage()
    : mCurrenObjectName(-1)
    , m_pMsgPopDelayTimer(new ATimer)
{
    m_pMsgPopDelayTimer->setHandler(PopTipManage::msgTipTimer, this);
    m_pMsgPopDelayTimer->setRepeat(false);
    m_pMsgPopDelayTimer->setInternal(0, 800);
}

PopTipManage::~PopTipManage()
{

}

void PopTipManage::klObjectObtainStart(bool state, int objectName, int loadAction)
{
    GEN_Printf(LOG_DEBUG, "klObjectObtainStart: mCurrenObjectName=%d, state=%d, objectName=%d, loadAction=%d", mCurrenObjectName, state, objectName, loadAction);
    if (state)
    {
        if (-1 != mCurrenObjectName)
        {
            closeMsgBox(mCurrenObjectName);
            mCurrenObjectName = -1;
        }
        bool needPop = false;

        if (needPop)
        {
            mCurrenObjectName = objectName;
            mBackLoadAction = loadAction;
            m_pMsgPopDelayTimer->restart(700);
        }
    } else
    {
        GEN_Printf(LOG_ERROR, "System Load start failed.");
    }
}

void PopTipManage::klObjectObtainOver(int objectName, int loadAction)
{
    GEN_Printf(LOG_DEBUG, "objectName=%d, loadAction=%d", objectName, loadAction);

    if (m_pMsgPopDelayTimer->isRunning())
    {
        GEN_Printf(LOG_DEBUG, "Timer is running.");
        m_pMsgPopDelayTimer->stop();
    } else
    {
        // 表示谁发起的弹框，就谁负责关闭
        if (mCurrenObjectName == objectName)
        {
            closeMsgBox(objectName);
            // 表示当前弹框已经关闭了
            mCurrenObjectName = -1;
        }
    }
}

void PopTipManage::klLoadDataExportEmpty(int objectName, int loadAction, const ByteString &)
{
    GEN_Printf(LOG_DEBUG, "objectName=%d, loadAction=%d", objectName, loadAction);

    if (m_pMsgPopDelayTimer->isRunning())
    {
        m_pMsgPopDelayTimer->stop();
    }

}

void PopTipManage::klLoadDataPriserExcept(int objectName, int loadAction, const ByteString &err_str)
{
    GEN_Printf(LOG_DEBUG, "klLoadDataPriserExcept: objectName=%d, loadAction=%d", objectName, loadAction);
    if (m_pMsgPopDelayTimer->isRunning())
    {
        m_pMsgPopDelayTimer->stop();
    }

}

void PopTipManage::sysNetLoadApiExcept(int objectName, int loadAction, const ByteString &err_str)
{
    GEN_Printf(LOG_DEBUG, "objectName=%d, loadAction=%d", objectName, loadAction);
    if (m_pMsgPopDelayTimer->isRunning())
    {
        m_pMsgPopDelayTimer->stop();
    }

    }

void PopTipManage::msgTipTimer(ATimer *that, void *ptr)
{
    that->setRunning(false);
    ((PopTipManage *)ptr)->showDelayMsgBox();
}

void PopTipManage::showDelayMsgBox()
{
    GEN_Printf(LOG_DEBUG, "Msg Box delay show, mCurrenObjectName=%d.", mCurrenObjectName);

    }

void PopTipManage::closeMsgBox(int objectName)
{
    if (m_pMsgPopDelayTimer->isRunning())
    {
        m_pMsgPopDelayTimer->stop();
    }

}

