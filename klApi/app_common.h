#ifndef _APP_COMMON_H
#define _APP_COMMON_H

#include "events/event_def.h"

enum {
    SIG_USER_UNUSED = 1000,     /* 前面999个信号枚举交给系统使用 */

    SIG_SURFACE_CREATED,
    SIG_SURFACE_CHANGED,
    SIG_SURFACE_DESTROYED,

    SIG_MVP_NEED_RECREARE,
    SIG_SET_SOURCE_PREPARE,     /* 设置播放源信息完成 */

    SIG_ON_MPV_EVENT,

    SIG_STATE_EVENT_PROC,       // 状态机处理

    //考拉播放器信息
    SIG_HAVE_OPEN_ID,           // 表示设备已经，激活成功，并获得了一个openid，只有这样考拉才能使用
    SIG_KL_INIT_ERROR,          // kaola设备激活或者初始化失败处理信息

    SIG_SOCKET_CLIENT_MSG_EXIT, // 启动收数据线程，并连接播放服务端socket

    SIG_KL_CURRENT_IS_COLLECT,  // 核对当前播放项，是否收藏了，异步遍历，防止收藏列表过多，遍历时拖慢主线程
    SIG_KL_CURRENT_OP_COLLECT,  // 核对当前播放项，是否收藏了，如果当前项在收藏列表就取消收藏，反之添加, 并通知界面
    SIG_KL_BDC_OP_COLLECT,      // 当前电台列表项收藏与否
    SIG_KL_RECORD_CURRENT_PLAY, // 记录当前播放项，并把它放入历史记录
    SIG_KL_HISTORY_CLEAR_APP,   // 清空 历史记录 所有项
    SIG_KL_SEARCH_RESULT_PROC,  // kl搜索结果处理

    SIG_KL_OBJECT_OBTAIN_START, // (NOT USED)基于KlObject派生类开始调用obtain函数，获取kl数据
    SIG_KL_OBJECT_OBTAIN_OVER,  // (NOT USED)基于KlObject派生类, 数据下载完成

    SIG_KL_LOAD_DATA_EXCEPT,    // (NOT USED)下载kl数据异常处理等析
    SIG_SYS_NET_LOAD_API_EXCEPT,// (NOT USED)调用libcurl下载kl数据失败，一般时本地网络出现问题

    SIG_KL_RELOAD_ERR_OBJECT,   // 重新加载错误的对象
    SIG_KL_COLLECT_ERR_OBJECT,  // 收集kl加载错误的对象
};


#define NO_RETUEN __attribute__((noreturn))


#define APPID       "cn5290"
#define SECRETKEY   "d673b807770b49b4077cb3bfa0d6aedd"
#define STR_PATTERN "0123456789abcdef"

#define NET_LOCAL_CONFIG_FILE           "/tmp/kl.net.local.config.info"
#define NET_KL_BDC_AREA_LIST_FILE       "/tmp/kl.broadcast.area.list.info"
#define NET_KL_BDC_CATE_LIST_FILE       "/tmp/kl.broadcast.category.list.info"
#define NET_KL_CATE_ALL_LIST_FILE       "/tmp/kl.category.all.list.info"
#define LOCAL_KL_SAVE_COLLECT_FILE      "/tmp/kl.local.collect.info"
#define LOCAL_KL_SAVE_RECORD_FILE       "/tmp/kl.local.record.info"
#define LOCAL_KL_DOWNLOAD_FILE          "/tmp/kl.local.download.info"



#endif // _APP_COMMON_H
