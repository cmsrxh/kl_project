/*************************************************************************
	> File Name:    common_func/common_func.h
	> Author:       陈猛
	> Mail:         chen-meng@hongjing-wh.com
	> Created Time: Sun 22 Mar 2020 09:41:55 AM CST
	> Description:  这种前端开发，通用的一些结构，和数据类型
 ************************************************************************/
#ifndef COMMON_FUNC_COMMON_FUNC_H
#define COMMON_FUNC_COMMON_FUNC_H


#include <util/byte_string.h>

// 通知UI，数据加载解析的结果
class UINotifyIface
{
public:
    enum ErrorInfoType
    {
        LOAD_EMPTY_DATA = 1,        // 分析数据正确，但是得到的数据是空
        LOAD_PRISER_JSOC_ERROR, // 不能正确解析json数据
        LOAD_SYS_API_FAILED,    // libcurl下载反馈的错误信息
    };
    virtual void dataPrepare() = 0;
    virtual void errorInfo(int /*type*/, const ByteString &) {}
};

class ImageStatus
{
public:
    virtual void dataPrepare() = 0;
    virtual void errorInfo(int , const char *) {}
    virtual void setLocalFile(ByteString const &) {}
};

#endif // COMMON_FUNC_COMMON_FUNC_H

