#include <QDebug>
#include "update_operate_action.h"
#include "layer_manage_control.h"

enum MsgBoxContainer{
    MSG_BOX_CONTAINER_INVALID,
    // 兼容系统自身的以前的弹框逻辑,加载
    // "qrc:/qml/global/MessageBox.qml"
    MSG_BOX_CONTAINER_COMMON,

    MSG_BOX_CONTAINER_UPDATE_NOTHING,   // 没有升级文件
    MSG_BOX_CONTAINER_NEW_VERSION,
    MSG_BOX_CONTAINER_SIGN_CHECK,   // 签名校验
    MSG_BOX_CONTAINER_UPDATE_MCU,   // 升级MCU
    MSG_BOX_CONTAINER_FAIL_UPDATE_MCU,
    MSG_BOX_CONTAINER_FAIL_SIGN,
    MSG_BOX_CONTAINER_UDISK_TIP,
    MSG_BOX_CONTAINER_MCU_SUCCESS,
    MSG_BOX_CONTAINER_VERIFY_SUCCESS,
    MSG_BOX_CONTAINER_BUFFERRING,

    MSG_BOX_CONTAINER_MAX
};
class LayerManageControlPrivate
{
public:
    class Container {
    public:
        Container()
            : boxUrl(""), model(false)
        {}
        Container(QString const &url)
            : boxUrl(url), model(true), title("")
        {}

        Container(QString const &url, bool model, QString const &title)
            : boxUrl(url), model(model), title(title)
        {}

        Container(QString const &url, QString const &title)
            : boxUrl(url), model(true), title(title)
        {}

        QString boxUrl;
        bool    model;
        QString title;
    };
    LayerManageControlPrivate()
        : containerType(MSG_BOX_CONTAINER_NEW_VERSION )
        , container {
              Container(),
              Container("qrc:/qml/global/MessageBox.qml"),
              Container("qrc:/qml/global/update/UpdateFileNothing.qml"),
              Container("qrc:/qml/global/update/CheckNewVersion.qml"),
              Container("qrc:/qml/global/update/SliderMsgBox.qml", QObject::tr("升级包校验中...")),
              Container("qrc:/qml/global/update/SliderMsgBox.qml", QObject::tr("正在升级MCU请勿断电，请勿拔掉U盘！")),
              Container("qrc:/qml/global/update/FailedUpdateMcu.qml"),
              Container("qrc:/qml/global/update/FailedSign.qml"),
              Container("qrc:/qml/global/UdiskMessageBox.qml"),
              Container("qrc:/qml/global/update/McuUpdateSuccess.qml"),
              Container("qrc:/qml/global/update/VerifySuccess.qml"),
              Container("qrc:/qml/global/update/MsgBuffering.qml"),

        }
    {
    }

    QString &getCurrentTitle()
    {
        return container[containerType].title;
    }

    QString &getCurrentUrl()
    {
        return container[containerType].boxUrl;
    }
    bool getCurrentModel()
    {
        return container[containerType].model;
    }

    MsgBoxContainer getContainerType() const
    {
        return containerType;
    }

    void setContainerType(const MsgBoxContainer &value)
    {
        containerType = value;
    }

    MsgBoxContainer  containerType;
    Container        container[MSG_BOX_CONTAINER_MAX];
};

LayerManageControl::LayerManageControl(QObject *parent)
    : QObject(parent)
    , mStatusViewLoad(false)
    , mScreenOffLoad(false)
    , mClockLoad(false)
    , mUdiskLoad(false), mSysMessage(false)
    , mSliderValue(40)
    , m_pUpdateOp(new UpdateOperateAction)
    , d(new LayerManageControlPrivate)
{
//    connect(GlobalControls::getInstance(), SIGNAL(msgUdiskMounted(QString)), this, SLOT(onMsgUdiskMounted(QString)));
//    connect(GlobalControls::getInstance(), SIGNAL(msgUdiskUnMount()),        this, SLOT(onMsgUdiskUnMount()));

    connect(this, SIGNAL(sigVerify()), m_pUpdateOp, SLOT(onSignCheck()));
}

void LayerManageControl::viewJudge()
{
    // 关背光进入power off界面
    // AppConfig::instance()->getScreenOffState()
    // GlobalControls::getInstance()->initScreenState();

    // mPowerMode --> 待机时钟界面
    // AppConfig::instance()->getPowerOffState()
    // HomeController::getInstance()->initPowerMode();

    // U盘插拔的提示框
    // hasUsb
    // GlobalControls::getInstance()->setHasUsb();

    // UDiskBoot.Api.msgVisible
    // U盘激活
#if 0
    // 1. 检查系统UUID,决定系统是否起来
    if (!UDiskUpdateOperation::getInstance()->haveValidSN())
    {
#if 1
        UDiskBoot::getInstance()->setBootState(UDiskBoot::SN_Not_Detect);
        UDiskBoot::getInstance()->setMsgVisible(true);

        // 关闭主界面的初始化
        ActivityManager::getInstance()->setMainViewEnable(false);
#else
        ActivityManager::getInstance()->setMainViewEnable(false);
        setSysMessage(true);
#endif
    }
    // 2. 验证系统是否进入poweroff界面
    else if (AppConfig::instance()->getScreenOffState())
    {
        setScreenOffLoad(true);
    }
    // 3. 验证是否进入, 待机时钟界面
    else if (AppConfig::instance()->getPowerOffState())
    {
        // 可以加载时钟界面
        setClockLoad(true);
    }
#endif
    // 至于U盘是否插拔,先等设备管理服务,起来后通知UI就行了
    qDebug() << "UI Judge.";
}

void LayerManageControl::viewShowOver()
{
    mStatusViewLoad = true;
    mScreenOffLoad  = true;
    mClockLoad = true;
    mUdiskLoad = true;

    Q_EMIT statusViewLoadChanged();
    Q_EMIT screenOffLoadChanged();
    Q_EMIT clockLoadChanged();
    Q_EMIT udiskLoadChanged();
}

bool LayerManageControl::statusViewLoad() const
{
    return mStatusViewLoad;
}

void LayerManageControl::setStatusViewLoad(bool statusViewLoad)
{
    if (mStatusViewLoad != statusViewLoad)
    {
        mStatusViewLoad = statusViewLoad;
        Q_EMIT statusViewLoadChanged();
    }
}

bool LayerManageControl::screenOffLoad() const
{
    return mScreenOffLoad;
}

void LayerManageControl::setScreenOffLoad(bool screenOffLoad)
{
    if (mScreenOffLoad != screenOffLoad)
    {
        mScreenOffLoad = screenOffLoad;
        Q_EMIT screenOffLoadChanged();
    }
}

bool LayerManageControl::clockLoad() const
{
    return mClockLoad;
}

void LayerManageControl::setClockLoad(bool clockLoad)
{
    if (mClockLoad != clockLoad)
    {
        mClockLoad = clockLoad;
        Q_EMIT clockLoadChanged();
    }
}

bool LayerManageControl::udiskLoad() const
{
    return mUdiskLoad;
}

void LayerManageControl::setUdiskLoad(bool udiskLoad)
{
    if (mUdiskLoad != udiskLoad)
    {
        mUdiskLoad = udiskLoad;
        Q_EMIT udiskLoadChanged();
    }
}

void LayerManageControl::onMsgUdiskMounted(const QString &mountPath)
{
//    if (UDiskBoot::getInstance()->bootState() == UDiskBoot::SN_Not_Detect) //没有烧写uuid时，插入U盘才烧写
//    {

//    } else
//    {
//        m_pUpdateOp->launchThread(mountPath.toStdString());
//        GlobalControls::getInstance()->setHasUsb(true);
//    }
    qDebug() << mountPath;
    m_pUpdateOp->setMountPath(mountPath.toStdString());
}

void LayerManageControl::onMsgUdiskUnMount()
{
//    GlobalControls::getInstance()->setHasUsb(false);
    qDebug() << "onMsgUdiskUnMount.";
    m_pUpdateOp->setMountPath("");
}

void LayerManageControl::closeSysMsgBox()
{
    setSysMessage(false);

    d->setContainerType(MSG_BOX_CONTAINER_INVALID);
}

int LayerManageControl::sliderValue() const
{
    return mSliderValue;
}

void LayerManageControl::setSliderValue(int sliderValue)
{
    mSliderValue = sliderValue;
    Q_EMIT sliderValueChanged();
}

bool LayerManageControl::sysMessage() const
{
    return mSysMessage;
}

void LayerManageControl::setSysMessage(bool sysMessage)
{
    if (mSysMessage != sysMessage)
    {
        mSysMessage = sysMessage;
        Q_EMIT sysMessageChanged();
    }
}

QString LayerManageControl::msgBoxUrl()
{
    qDebug() << "msg url" << d->getCurrentUrl() << d->getCurrentModel();
    return d->getCurrentUrl();
}

bool LayerManageControl::msgBoxModel()
{
    return d->getCurrentModel();
}

QString LayerManageControl::getUpdateName()
{
    return QString::fromStdString(m_pUpdateOp->getUpdateName());
}

QString LayerManageControl::getUpdateVersion()
{
    return QString::fromStdString(m_pUpdateOp->getUpdateVersion());
}

void LayerManageControl::btnYesCheckNewVersion()
{
    qDebug () << "start check and unpackage file.";
    m_pUpdateOp->verifyStart();
    Q_EMIT sigVerify();

    setSliderValue(0);
    d->setContainerType(MSG_BOX_CONTAINER_SIGN_CHECK);

    Q_EMIT msgBoxUrlChanged();
}

void LayerManageControl::btnNoCheckNewVersion()
{
    qDebug () << "not update file.";

    closeSysMsgBox();
}

QString LayerManageControl::getSlidlerTitleName()
{
    return d->getCurrentTitle();
}

void LayerManageControl::btnReUpdateMcu()
{
    qDebug() << "re update mcu";
//    SettingsController::getInstance()->reqMcuUpdate();
}

QString LayerManageControl::getErrorReason()
{
    return mErrorInfo;
}

void LayerManageControl::btnUserClickedClose()
{
    closeSysMsgBox();
}

void LayerManageControl::btnUpdateApp()
{
    qDebug() << "update app.";
//    SettingsController::getInstance()->reqAppUpdate();
}

void LayerManageControl::btnUpdateMcu()
{
    qDebug() << "update MCU.";
//    SettingsController::getInstance()->reqMcuUpdate();

    d->setContainerType(MSG_BOX_CONTAINER_BUFFERRING);
    Q_EMIT msgBoxUrlChanged();
}

void LayerManageControl::appUpdate()
{
    qDebug() << "verify update file.";

    if (m_pUpdateOp->checkNewUpdateFile())
    {
        m_pUpdateOp->showVersionInfo();
        d->setContainerType(MSG_BOX_CONTAINER_NEW_VERSION);
    } else
    {
        d->setContainerType(MSG_BOX_CONTAINER_UPDATE_NOTHING);
    }
    Q_EMIT msgBoxUrlChanged();
    setSysMessage(true);
}

void LayerManageControl::mcuUpdateStart()
{
    d->setContainerType(MSG_BOX_CONTAINER_UPDATE_MCU);
    setSliderValue(0);
    Q_EMIT msgBoxUrlChanged();
}

void LayerManageControl::mcuUpdateSuccess()
{
//    closeSysMsgBox();
//    setSliderValue(0);
    d->setContainerType(MSG_BOX_CONTAINER_MCU_SUCCESS);
    Q_EMIT msgBoxUrlChanged();
}

void LayerManageControl::mcuUpdateFailed()
{
    mErrorInfo = tr("MCU升级文件出错");
    d->setContainerType(MSG_BOX_CONTAINER_FAIL_UPDATE_MCU);
    Q_EMIT msgBoxUrlChanged();
}

void LayerManageControl::mcuUpdateSliderValue(int value)
{
    setSliderValue(value);
}

void LayerManageControl::notifyVerifyFailed(const QString &errInfo)
{
    qDebug() << "verify failed.";
    mErrorInfo = errInfo;

    d->setContainerType(MSG_BOX_CONTAINER_FAIL_SIGN);
    Q_EMIT msgBoxUrlChanged();
}

void LayerManageControl::notifyVerifySuccess()
{
    qDebug() << "verify success.";
    d->setContainerType(MSG_BOX_CONTAINER_VERIFY_SUCCESS);
    Q_EMIT msgBoxUrlChanged();
}

