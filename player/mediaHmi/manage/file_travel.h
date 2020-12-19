/*************************************************************************
	> File Name:    travel_media_file.h
	> Author:       陈猛
	> Mail:         chen-meng@hongjing-wh.com
	> Created Time: 2019年09月26日 星期四 14时36分02秒
	> Description:  遍历目录下面的媒体文件
 ************************************************************************/
#ifndef _TRAVEL_MEDIA_FILE_H_
#define _TRAVEL_MEDIA_FILE_H_

#include <util/travel_dirent.h>
#include <util/list_table.h>
#include "dir_tree.h"

class DirTree;
class FileTravel : public TravelDirent
{
public:
    struct Strategy
    {
        const char *suffix;
        void       *type;
    };

    FileTravel();
    ~FileTravel();

    void readDir(const ByteString &dirName);

    void clear ();

    ListTable<ByteString> &music()
    {
        return mMusic;
    }

    ListTable<ByteString> &video()
    {
        return mVideo;
    }

    ListTable<ByteString> &image()
    {
        return mImage;
    }

protected:
    virtual void pushFile(const ByteString &dir, const char *name);

private:

    void push(const ByteString & dir, const char *name, const char *suffix);
    int  fileSuffixCmp(const char *file_suffix, const char *suffix);

    ListTable<ByteString> mMusic;
    ListTable<ByteString> mVideo;
    ListTable<ByteString> mImage;

    Strategy _media_strategy[41];
};

#endif // _TRAVEL_MEDIA_FILE_H_

