/**
 * @file:    kl_active_manage.h
 * @author:  陈猛
 * @em:      chen-meng@hongjing-wh.com
 * @date:    2019年12月20日 星期五 14时43分37秒
 * @details: 激活管理
 ************************************************************************/
#ifndef _KL_ACTIVE_MANAGE_H_
#define _KL_ACTIVE_MANAGE_H_

#include "util/byte_string.h"

namespace kl
{
class ActiveManage
{
public:
    ActiveManage();

    ByteString genQueryUrl();
private:
    ByteString  mBaseUrl;
};
}

#endif // _KL_ACTIVE_MANAGE_H_

