#include <events/common_log.h>
#include <util/run_shell_cmd.h>
#include <util/travel_dirent.h>
#include "layer_manage_control.h"
#include <QDir>
#include <QDebug>
#include <QRegExp>

#include "update_operate_action.h"

// 签名文件重新保存在系统的data目录中在下一次的时候, 用于删除这些临时文件
#define SIGN_FILE "sign.md5"
#define SIGN_DIR  "/tmp"  // not used

// 系统版本信息存储的位置和名字
#ifdef PLATFORM_PC
#define FILE_VERSION "/tmp/packet/version.md"
#else
#define FILE_VERSION "/etc/version.md"
#endif // PLATFORM_PC

UpdateOperateAction::UpdateOperateAction()
{
    this->moveToThread(&mThread);

//    connect(&mThread, SIGNAL(started()), this, SLOT(onStarted()));
//    connect(&mThread, SIGNAL(started()), this, SLOT(onStarted()));

#ifdef PLATFORM_PC
    mUpdatePacketName = "2020-1215-1143_310S_LV2_001_000_SA_20201215_0002_swdl.zip";
    mUpdatePacketVersion = "310S_LV2_001_000_SA_20201215_0002";

    mMountPath = "/media/hongjing-cm/9576-957C";
#endif

    mThread.start();
}

void UpdateOperateAction::launchThread(const std::string &mp)
{
    mMountPath = mp;

    if (!mThread.isRunning())
    {
        mThread.start();
    }
}

void UpdateOperateAction::quitThread()
{
    GEN_Printf(LOG_DEBUG, "--------------");
    try {
        if (mThread.isRunning())
        {
            mThread.quit();
        }
    } catch(...)
    {
        GEN_Printf(LOG_ERROR, "Qt Quit except");
    }
}

void UpdateOperateAction::onStarted()
{
    if (!findCorrectFile(mMountPath))
    {
        GEN_Printf(LOG_DEBUG, "Not Find correct update file");

        removeTempFile();

        mThread.terminate();
    } else
    {
        mUnZipOutDir = mMountPath;

//        Q_EMIT UpdateHmiCtrl::instance()->showMsg(3, 2, tr("是否升级"), "检测到新升级文件: \'" + QString(basename(mUpdateFile)) + "\'");
    }
}

const std::string &UpdateOperateAction::getMountPath()
{
    return mMountPath;
}

void UpdateOperateAction::setMountPath(const std::string &mountPath)
{
    mMountPath = mountPath;
}

void UpdateOperateAction::verifyStart()
{
    mUnZipOutDir = mMountPath;
}

void UpdateOperateAction::showVersionInfo()
{

}

//#define NEED_TEMP_DIR
void UpdateOperateAction::onSignCheck()
{
    RunShellCmd unzip("unzip");
    RunShellCmd verifySign("md5sum");
#ifdef NEED_TEMP_DIR
    RunShellCmd rmTmp("rm");
#endif // NEED_TEMP_DIR
    //RunShellCmd cpsign("cp");
    LayerManageControl::instance()->setSliderValue(0);
    // 设置校验文件
    verifySign.appendArgue("-c", SIGN_FILE);

    // 拷贝签名文件,在系统升级完成之后,删除升级之后的临时文件
    //cpsign.appendArgue(SIGN_FILE, SIGN_DIR);
#ifdef NEED_TEMP_DIR
    if (0 == ::access(mUnZipOutDir, F_OK))
    {
        strcat(mUnZipOutDir, "/temp");
    }
    if (::mkdir(mUnZipOutDir, 0775))
    {
        GEN_Printf(LOG_ERROR, "mkdir File: %s failed.", mUnZipOutDir);
        LayerManageControl::instance()->notifyVerifyFailed(tr("不能创建临时文件,可能目标目录是只读的文件系统!"));
        return;
    }

    rmTmp.appendArgue("-rf", mUnZipOutDir);
#endif // NEED_TEMP_DIR

    ::usleep(200000);
    LayerManageControl::instance()->setSliderValue(10);
    // -n 解压缩时不要覆盖原有的文件。
    // -o 不必先询问用户，unzip执行后覆盖原有文件。
    // -d<目录> 指定文件解压缩后所要存储的目录。
    unzip.appendArgue("-o", "-d", mUnZipOutDir.c_str(), mUpdateFile.c_str());

    ::usleep(200000);
    LayerManageControl::instance()->setSliderValue(30);
    int unzipSt = unzip.system();
    GEN_Printf(LOG_DEBUG, "unzip exec state: %d", unzipSt);
    if (unzipSt)
    {
        LayerManageControl::instance()->notifyVerifyFailed(tr("压缩包损坏,解压错误!"));
        return;
    }

    ::usleep(200000);
    LayerManageControl::instance()->setSliderValue(50);
    // 记录当前的执行路径
    char buff[256];
    ::getcwd(buff, 256);
    // GEN_Printf(LOG_DEBUG, "--%s--", buff);

    ::usleep(200000);
    LayerManageControl::instance()->setSliderValue(65);
    // 修改执行路径
    ::chdir(mUnZipOutDir.c_str());

    LayerManageControl::instance()->setSliderValue(80);
    int verifySt = verifySign.system();
    GEN_Printf(LOG_DEBUG, "sign verify state: %d", verifySt);

    // 开始拷贝
    //cpsign.system();

    // 恢复执行路径
    ::chdir(buff);
#ifdef NEED_TEMP_DIR
    // 删除临时目录文件
    int rmSt = rmTmp.system();
    GEN_Printf(LOG_DEBUG, "rm dir state: %d", rmSt);
#endif // NEED_TEMP_DIR

    ::usleep(200000);
    if (0 == verifySt) // sign successfully
    {
        //
        // this->recordVersionString();
        LayerManageControl::instance()->setSliderValue(100);
        LayerManageControl::instance()->notifyVerifySuccess();
    } else
    {
        LayerManageControl::instance()->notifyVerifyFailed(tr("升级文件损坏,签名验证错误!"));
    }
}

void UpdateOperateAction::removeTempFile()
{
    char tmp[256];
    char filepath[256];

    sprintf(filepath, "%s/%s" , mMountPath.c_str(), SIGN_FILE);
    FILE *file = fopen(filepath, "r");
    GEN_Printf(LOG_DEBUG, "remove prev tmp file: %p, %s", file, filepath);
    if (file)
    {
        RunShellCmd rm("rm");

        while (fgets(tmp, sizeof(tmp), file))
        {
            bool isDir = false;
            char *name = tmp + 32;
            while (*name == ' ') name++;

            char  *p = name;
            while (*p)
            {
                if (*p == '\r' || *p == '\n')
                {
                    *p = 0;
                } else if (*p == '/')
                {
                    *p = 0;
                    isDir = true;
                    break;
                }

                p++;
            }

            sprintf(filepath, "%s/%s", mMountPath.c_str(), name);

            // GEN_Printf(LOG_DEBUG, "remove: %s, %s", filepath, strerror(errno));

            if (0 == ::access(filepath, F_OK))
            {
                if (isDir)
                {
                    rm.appendArgue(filepath, "-rf");
                    rm.system();
                } else
                {
                    ::remove(filepath);
                }
            }
        }
        fclose(file);

        sprintf(filepath, "%s/%s" , mMountPath.c_str(), SIGN_FILE);
        ::remove(filepath);
    }
}

/*
日期+客户要求版本号+标识即可
2020-1215-1143_  310S_LV2_001_000_SA_20201215_0002  _swdl.zip
*/
class VersionNode
{
public:
    VersionNode()
    {}

    bool operator < (VersionNode &n)
    {
        return strcmp(hostPkgDate, n.hostPkgDate) < 0;
    }

    // 表示车型需要相等
    bool modelsEqual (VersionNode &n)
    {
        return strcmp(models, n.models) == 0;
    }

    char hostPkgDate[16];   // 自己主机打包的日期: 2020-1215-1143
    char models[16];        // 车型: 310S|730S
    char config[16];        // 车型配置: LV2
    char confwater[8];      // 配置流水: 001
    char seg[8];            // 细分系统: 000
    char supplier[16];      // 供应商: SA
    char usrPkgDate[16];    // 客户打包的日期: 20201215
    char waterId[8];        // 流水号: 0002
};

// VersionNode 类成员数组长度
static uint8_t segSize[8] = {16, 16, 16, 8, 8, 16, 16, 8};
static bool priserVersion(VersionNode &cur, const char *start, const char *end)
{
    char *pos = cur.hostPkgDate;
    int segId = 0; // < 8
    int size  = segSize[segId];
    char *q   = pos;

    for (const char *p = start; p < end; ++p)
    {
        // 2020-1215-1143_  310S_LV2_001_000_SA_20201215_0002
        if (*p == '_')
        {
            *q = '\0';
            pos += segSize[segId];
            q = pos;
            ++segId;
            size = segSize[segId];

            if (segId > 8)
            {
                GEN_Printf(LOG_DEBUG, "'%s' named invalid", start);
                return false;
            }
        } else if (*p != '\n' && *p != '\r')
        {
            if (UNLIKELY(size <= 0))
            {
                GEN_Printf(LOG_DEBUG, "'%s' named invalid", start);
                return false;
            }

            *q++ = *p;
            --size;
        }
    }
    *q = '\0';
    //GEN_Printf(LOG_DEBUG, "segId: %d", segId);
    return 7 == segId ? true : false;
}
static void printVersion(const char *tag, VersionNode &cur)
{
    GEN_Printf(LOG_DEBUG, "%s:\nhostPkgDate: \t%s, "
                          "\nmodels: \t\t%s, "
                          "\nconfig: \t\t%s, "
                          "\nconfwater: \t\t%s, "
                          "\nseg: \t\t\t%s, "
                          "\nsupplier: \t\t%s, "
                          "\nusrPkgDate: \t\t%s, "
                          "\nwaterId: \t\t%s", tag,
               cur.hostPkgDate, cur.models, cur.config,
               cur.confwater, cur.seg, cur.supplier,
               cur.usrPkgDate, cur.waterId);

}
bool UpdateOperateAction::findCorrectFile(std::string const &dirname)
{
    VersionNode sysVer;
    VersionNode maxVer;
    std::string version = readVersionString();

    if (version.empty())
    {
        GEN_Printf(LOG_ERROR, "Version Name is empty.");
        return false;
    }

    //GEN_Printf(LOG_DEBUG, "size: [%d], [%s]", version.size(), version.c_str());
    if (!priserVersion(sysVer, version.c_str(), version.c_str() + version.size()))
    {
        GEN_Printf(LOG_ERROR, "Sys Config Version in valid.");
        return false;
    }

    maxVer = sysVer;
    printVersion("System Version", sysVer);

    DIR *tmpDir = opendir(dirname.c_str());
    if (tmpDir)
    {
        struct dirent *dent;
        while ((dent = readdir(tmpDir)))
        {
            switch (dent->d_type)
            {
            case DT_REG:
            case DT_UNKNOWN:
            {
#define PREFIX "_swdl.zip"
                uint32_t size = strlen(dent->d_name);
                if (size <= sizeof(PREFIX)) continue;

                const char *p   = dent->d_name + size - sizeof(PREFIX) + 1;
                const char *zip = (0 == strcmp(p, PREFIX)) ? p : NULL;
                if (zip)
                {
                    GEN_Printf(LOG_DEBUG, "%s", dent->d_name);
                    VersionNode cur;

                    if (priserVersion(cur, dent->d_name, zip))
                    {
                        //printVersion("Scan Version", cur);
                        if (maxVer < cur && maxVer.modelsEqual(cur))
                        {
                            maxVer = cur;
                            mUpdateVersion = std::string(dent->d_name, (long)zip - (long)dent->d_name);
                            mUpdatePacketName = std::string(dent->d_name);
                        }
                    }
                }
                break;
            }
            default:
                break;
            }
        }

        closedir(tmpDir);
    }

    if (sysVer < maxVer)
    {
        printVersion("Newest Version", maxVer);

        GEN_Printf(LOG_DEBUG, "Find Newest version.");

        mUpdatePacketVersion = maxVer.hostPkgDate;

        mUpdateFile = dirname + "/" + mUpdatePacketName;
        return true;
    }

    return false;
}

std::string UpdateOperateAction::readVersionString()
{
    char buffer[128] = {0};
    int fd = ::open(FILE_VERSION, O_RDONLY);

    if (fd > 0)
    {
        ::read(fd, buffer, sizeof(buffer));
        ::close(fd);
    }

    return std::string(buffer);
}

void UpdateOperateAction::recordVersionString()
{
    int fd = ::open(FILE_VERSION, O_WRONLY | O_CREAT | O_TRUNC, 0664);

    if (fd > 0)
    {        
        ::write(fd, mUpdateVersion.c_str(), mUpdateVersion.size());
        ::close(fd);
    }
}


