#ifndef LAYERMANAGECONTROL_H
#define LAYERMANAGECONTROL_H

#include <QObject>

class UpdateOperateAction;
class LayerManageControlPrivate;
/**
 * @brief The LayerManageControl class
 * @note 用于管理系统系统UI层上面的系统弹框
 *  1. 背光  时钟  等
 *  2. 升级弹框处理逻辑
 *      appUpdate 启动整个升级流程操作
 *      查看挂载目录,扫描根目录
 *      根据/etc/version.md文件中记载的升级版本信息,找到U盘中同车型,版本比系统更新的zip压缩包文件
 *      使用unzip命令,解压,然后通过解压文件中sign.md5校验文件,校验整 个压缩包是否有错
 *      最后切换各自的升级目录中进行处理
 */
class LayerManageControl : public QObject
{
    Q_OBJECT

    explicit LayerManageControl(QObject *parent = nullptr);

    // 状态栏下拉列表是否加载,防止在初始化的时候,就去加载
    Q_PROPERTY(bool statusViewLoad READ statusViewLoad NOTIFY statusViewLoadChanged)

    // 背光图层是否加载
    Q_PROPERTY(bool screenOffLoad READ screenOffLoad NOTIFY screenOffLoadChanged)

    // 待机时钟界面是否加载
    Q_PROPERTY(bool clockLoad READ clockLoad NOTIFY clockLoadChanged)

    // U盘插拔的提示框是否加载
    Q_PROPERTY(bool udiskLoad READ udiskLoad NOTIFY udiskLoadChanged)

    // 系统消息弹框显示
    Q_PROPERTY(bool sysMessage READ sysMessage WRITE setSysMessage NOTIFY sysMessageChanged)
    Q_PROPERTY(QString msgBoxUrl READ msgBoxUrl NOTIFY msgBoxUrlChanged)
    Q_PROPERTY(bool msgBoxModel READ msgBoxModel NOTIFY msgBoxModelChanged)

    // 滑动条进度值
    Q_PROPERTY(int sliderValue READ sliderValue NOTIFY sliderValueChanged)
public:
    static LayerManageControl *instance()
    {
        static LayerManageControl *i = [] { return new LayerManageControl; } ();
        return i;
    }

    // 系统UI显示裁定, 表示启动的时候,需要提前设置需要显示的UI和显示层
    void viewJudge();

    // 表示UI已经显示完毕了, 可以把预加载属性至为true
    void viewShowOver();


    bool statusViewLoad() const;
    void setStatusViewLoad(bool statusViewLoad);

    bool screenOffLoad() const;
    void setScreenOffLoad(bool screenOffLoad);

    bool clockLoad() const;
    void setClockLoad(bool clockLoad);

    bool udiskLoad() const;
    void setUdiskLoad(bool udiskLoad);

    bool sysMessage() const;
    void setSysMessage(bool sysMessage);

    int  sliderValue() const;
    void setSliderValue(int sliderValue);

    // 当前弹框的qml 地址
    QString msgBoxUrl();
    // 是否模态显示,true 表示当前弹框不许下层接收触摸事件
    bool    msgBoxModel();

    // global/update/CheckNewVersion.qml 校验判断弹框
    // 升级包名字
    Q_INVOKABLE QString getUpdateName();
    // 版本号码
    Q_INVOKABLE QString getUpdateVersion();
    Q_INVOKABLE void btnYesCheckNewVersion();
    Q_INVOKABLE void btnNoCheckNewVersion();

    // 有滑动条窗口的消息弹框
    // 获取弹框的标题名字
    Q_INVOKABLE QString getSlidlerTitleName();

    // mcu升级失败弹框,重新升级
    // FailedUpdateMcu.qml
    Q_INVOKABLE void btnReUpdateMcu();
    // 错误原因 FailedUpdateMcu.qml/ FailedSign.qml
    Q_INVOKABLE QString getErrorReason();


    // 界面主动点击关闭系统弹框
    Q_INVOKABLE void btnUserClickedClose();

    // McuUpdateSuccess.qml/ VerifySuccess.qml
    //! 点击升级APP,新型烧录操作
    Q_INVOKABLE void btnUpdateApp();
    // VerifySuccess.qml
    Q_INVOKABLE void btnUpdateMcu();

    //! 升级app程序
    //! @note: call by SettingsController::reqZipUpdate()
    void appUpdate();

    //! MCU回调发送升级开始
    //! @note call by SettingsController::updateMcuUpdateData
    void mcuUpdateStart();
    void mcuUpdateSuccess();
    void mcuUpdateFailed();
    void mcuUpdateSliderValue(int value);

    //! 返回校验失败的通知
    //! @note call by UpdateOperateAction
    void notifyVerifyFailed(QString const &errInfo);
    void notifyVerifySuccess();

Q_SIGNALS:
    void statusViewLoadChanged();
    void screenOffLoadChanged();
    void clockLoadChanged();
    void udiskLoadChanged();
    void sysMessageChanged();
    void msgBoxUrlChanged();
    void msgBoxModelChanged();
    void sliderValueChanged();

    // 异步执行校验
    void sigVerify();

public Q_SLOTS:
    void onMsgUdiskMounted(QString const &mountPath);

    void onMsgUdiskUnMount();

private:
    void closeSysMsgBox();

    bool mStatusViewLoad;
    bool mScreenOffLoad;
    bool mClockLoad;
    bool mUdiskLoad;
    bool mSysMessage;
    int  mSliderValue;

    // 升级文件校验处理
    UpdateOperateAction       *m_pUpdateOp;
    LayerManageControlPrivate *d;

    QString mErrorInfo; // 错误信息
};

#endif // LAYERMANAGECONTROL_H
