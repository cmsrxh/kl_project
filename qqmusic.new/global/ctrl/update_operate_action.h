#ifndef UPDATE_OPERATE_ACTION_H
#define UPDATE_OPERATE_ACTION_H

#include <QObject>
#include <QThread>

/**
 * @brief The UpdateOperateAction class
 * @note
 *
 * 升级操作过程和动作处理
 *  操作步骤:
 *      1. 插入U盘;
 *      2. 扫描u盘'根目录',检测最新版本(系统版本字符与升级包名字比较获取最新的那个升级包);
 *      3. 解压升级包到U盘;
 *      4. 根据解压文件中sign.md5签名文件, cd到'根目录',然后调用校验命令(md5)校验所有文件是否正确;
 *      5. 检查校验命 令结果,成功,就进入下一步升级操作;
 *      6. mcu升级;
 *      7. linux升级.
 *      8. 再次启动,照样会检测到U盘,然后删除解压的文件(根据校验文件中的内容)
 */
class UpdateOperateAction :  public QObject
{
    Q_OBJECT
public:
    UpdateOperateAction();


    //! 启动线程,在子线程中处理各种校验
    //! @note 用于U盘插入之后,在子线程中同步执行是否有新的文件
    void launchThread(std::string const &mp);

    //! 在U盘卸载的时候,尝试关闭UPan
    void quitThread();

    //! 升级包名字
    const std::string &getUpdateName(){
        return mUpdatePacketName;
    }

    //! 版本号码
    const std::string &getUpdateVersion() {
        return mUpdatePacketVersion;
    }

    //! 用于异步执行,不在U盘插入的时候就做检查
    bool checkNewUpdateFile()
    {
        return mMountPath.empty() ? false : findCorrectFile(mMountPath);
    }

    const std::string &getMountPath();
    void setMountPath(const std::string &mountPath);

    //! 在检查成功之后,启动线程进行校验
    void verifyStart();

    void showVersionInfo();

public Q_SLOTS:
    /**
     * @brief onUdiskMount
     * @note U盘挂载, 启动线程
     *  挂载目录, 升级文件存在目录,根据update前置和.zip后缀,
     *  适配所有的升级文件,找到最新的文件,里面需要带有文件签名信息,以便验证文件的完整性
     *  压缩文件解压之后生成在U盘根目录
     */
    void onStarted();

    /**
     * @brief onSignCheck
     * @note 校验开始
     */
    void onSignCheck();

private:    
    /**
     * @brief removeTempFile
     * @note 再次启动的时候检查上一次升级文件的临时文件,在车机 启动的时候就去删除它
     */
    void removeTempFile();

    //! 查找正确的升级文件, 只要指定的根目录下进行查找
    bool findCorrectFile(std::string const &dir);

    //! 在文件系统固定的位置找到版本信息字串,一般在/[etc|data|config]/version.md,或者直接写入程序代码中
    std::string readVersionString();

    //! 升级成功之后保存版本信息,防止在下一次的时候又检测 到相同版本文件,防止重复升级
    void recordVersionString();

    // 升级包文件压缩包名字(绝对路径)
    std::string mUpdateFile;

    // 解压文件生成目录
    std::string mUnZipOutDir;

    QThread mThread;

    std::string mMountPath;
    std::string mUpdateVersion;

    std::string mUpdatePacketName;
    std::string mUpdatePacketVersion;

};

#endif // UPDATE_OPERATE_ACTION_H
