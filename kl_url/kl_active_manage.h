/**
 * @file:    kl_active_manage.h
 * @author:  陈猛
 * @em:      chen-meng@hongjing-wh.com
 * @date:    2019年12月20日 星期五 14时43分37秒
 * @details: 激活管理
 ************************************************************************/
#ifndef KL_ACTIVE_MANAGE_H
#define KL_ACTIVE_MANAGE_H

#include "kl_object.h"

namespace kl
{

class ActiveManage : public KLObject
{
public:
    ActiveManage();

    NetUrl &genQueryUrl();

    void loadData (uint8_t *data, unsigned long size);
};

}

#endif // KL_ACTIVE_MANAGE_H

