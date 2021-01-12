#ifndef WIN_GLOBAL_COMMON_H_
#define WIN_GLOBAL_COMMON_H_


enum GlobalWinType {
    WIN_TYPE_INVALID = -1,

    WIN_TYPE_ONE,
    WIN_TYPE_TWO,
    WIN_TYPE_THREE,

    WIN_TYPE_MAX,
};

/**
 * @brief The WinSwitchInfo class
 * @note 用于窗口切换时调用
 */
class WinSwitchInfo
{
public:
    WinSwitchInfo()
        : type(WIN_TYPE_INVALID), arg(0)
    {}
    WinSwitchInfo(GlobalWinType type, long arg)
        : type(type), arg(arg)
    {}
    WinSwitchInfo(GlobalWinType type)
        : type(type), arg(0)
    {}

    GlobalWinType type;
    long          arg;
};


#endif // WIN_GLOBAL_COMMON_H_

