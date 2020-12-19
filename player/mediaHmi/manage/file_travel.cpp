#include <events/common_log.h>
#include "file_travel.h"

#define CM_TO_LOWER(s) ( (s >= 'A' && s <= 'Z') ? s + 32 : s )
//#define CM_TO_LOWER(s) toLower(s)
#define  MUSIC_TYPE_FILE (void *)&mMusic
#define  VIDEO_TYPE_FILE (void *)&mVideo
#define  IMAGE_TYPE_FILE (void *)&mImage

FileTravel::FileTravel()
    : TravelDirent()
    , _media_strategy
{
{"mp3",  MUSIC_TYPE_FILE}, {"ogg",  MUSIC_TYPE_FILE}, {"flac", MUSIC_TYPE_FILE},
{"ape",  MUSIC_TYPE_FILE}, {"wav",  MUSIC_TYPE_FILE}, {"wma",  MUSIC_TYPE_FILE},
{"aac",  MUSIC_TYPE_FILE}, {"ac3",  MUSIC_TYPE_FILE}, {"amr",  MUSIC_TYPE_FILE},
{"mp2",  MUSIC_TYPE_FILE}, {"au",   MUSIC_TYPE_FILE}, {"m4a",  MUSIC_TYPE_FILE},

{"3g2",  VIDEO_TYPE_FILE}, {"3gp",  VIDEO_TYPE_FILE}, {"f4v",  VIDEO_TYPE_FILE},
{"trp",  VIDEO_TYPE_FILE}, {"ts",   VIDEO_TYPE_FILE}, {"flv",  VIDEO_TYPE_FILE},
{"avi",  VIDEO_TYPE_FILE}, {"ra",   VIDEO_TYPE_FILE}, {"rm",   VIDEO_TYPE_FILE},
{"rmvb", VIDEO_TYPE_FILE}, {"mpg",  VIDEO_TYPE_FILE}, {"divx", VIDEO_TYPE_FILE},
{"wmv",  VIDEO_TYPE_FILE}, {"mkv",  VIDEO_TYPE_FILE}, {"mpeg", VIDEO_TYPE_FILE},
{"asf",  VIDEO_TYPE_FILE}, {"webm", VIDEO_TYPE_FILE}, {"swf",  VIDEO_TYPE_FILE},
{"mp4",  VIDEO_TYPE_FILE}, {"mov",  VIDEO_TYPE_FILE}, {"vob",  VIDEO_TYPE_FILE},
{"adt",  VIDEO_TYPE_FILE},

{"jpg", IMAGE_TYPE_FILE}, {"bmp", IMAGE_TYPE_FILE}, {"jpeg", IMAGE_TYPE_FILE},
{"png", IMAGE_TYPE_FILE}, {"gif", IMAGE_TYPE_FILE}, {"ico",  IMAGE_TYPE_FILE},
{"tag", IMAGE_TYPE_FILE}
}
{
}

FileTravel::~FileTravel()
{
//    mMediaTree.clear();
}

void FileTravel::readDir(const ByteString &dirName)
{
    bool isExit = false;
//    mMediaTree.setRootName(dirName.string());
    DIR *tmpDir = opendir(dirName.string());
    if (tmpDir)
    {
        pRreadDir(tmpDir, dirName, isExit);
        closedir(tmpDir);
    }
}

void FileTravel::clear()
{
//    mMediaTree.clear();
    ListTable<ByteString>::iterator it = mMusic.begin();
    for (; it != mMusic.end(); ++it)
    {
        it->clear();
    }

    it = mVideo.begin();
    for (; it != mVideo.end(); ++it)
    {
        it->clear();
    }

    it = mImage.begin();
    for (; it != mImage.end(); ++it)
    {
        it->clear();
    }

    mMusic.clear();
    mVideo.clear();
    mImage.clear();
}

void FileTravel::pushFile(const ByteString &dir, const char *name)
{
    /* 从后向前遍历，找到第一个点号 */
    const char *nameSuffix = ::rindex(name, '.');

    if(nameSuffix)
    {
        /* 跳过点号 */
        ++nameSuffix;

        push(dir, name, nameSuffix);
    }
}

void FileTravel::push(const ByteString &dir, const char *name, const char *suffix)
{
    for(Strategy &stra : _media_strategy)
    {
        if (0 == fileSuffixCmp(suffix, stra.suffix))
        {
            ByteString filePath = ByteString::concatStrings(dir, "/", name, NULL);

            ListTable<ByteString> *lst = (ListTable<ByteString> *)stra.type;

            lst->push_back(filePath);

//            mMediaTree.pushDirPath(dir.string(), name);
        }
    }
}

int FileTravel::fileSuffixCmp(const char *file_suffix, const char *suffix)
{
    while(*file_suffix && *suffix &&
          (CM_TO_LOWER(*file_suffix) == *suffix))
    {
        ++file_suffix;
        ++suffix;
    }
    return (*file_suffix || *suffix) ? -1 : 0;
}
