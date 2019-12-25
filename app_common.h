#ifndef _APP_COMMON_H
#define _APP_COMMON_H

#include "events/event_def.h"

enum {
    // SIG_USER_UNUSED = SIG_INNER_MAX,
    SIG_USER_UNUSED = 1000,     /* 前面999个信号枚举交给系统使用 */

};

#define NO_RETUEN __attribute__((noreturn))


#define APPID       "cn5290"
#define SECRETKEY   "d673b807770b49b4077cb3bfa0d6aedd"
#define STR_PATTERN "0123456789abcdef"

#define NET_LOCAL_CONFIG_FILE           "/tmp/kl.net.local.config.info"
#define NET_KL_BDC_AREA_LIST_FILE       "/tmp/kl.broadcast.area.list.info"
#define NET_KL_BDC_CATE_LIST_FILE       "/tmp/kl.broadcast.category.list.info"
#define NET_KL_CATE_ALL_LIST_FILE       "/tmp/kl.category.all.list.info"

#endif // _APP_COMMON_H
