#ifndef KL_COMMON_H
#define KL_COMMON_H

#include <util/byte_string.h>
#include <util/list_table.h>


#define SIGN_ActiveManage               "bae13cac5caacddfa3ca4c49cf2d0144"
#define SIGN_InitManage                 "bae13cac5caacddfa3ca4c49cf2d0144"
#define SIGN_BroadcastItemDetail        "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_BroadcastItemList          "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_CategoryBroadcast          "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_BroadcastAreaList          "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_CategorySublist            "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_BroadcastItemProgramlist   "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_CategoryAll                "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_OperateList                "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_TypeRadioList              "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_ChipAudioDetail            "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_ChipAudioList              "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_AlbumDetail                "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_AlbumList                  "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_ChipRadioList              "81230f5ae8db203704271b5c87cc6fbc"
#define SIGN_ChipRadioDetail            "81230f5ae8db203704271b5c87cc6fbc"

namespace kl
{
class AreaItem
{
public:
    AreaItem()
    {}


    ByteString id;
    ByteString name;
};

class BDCastItem
{
public:
    BDCastItem()
    {}

    ByteString broadcastId;
    ByteString status;
    ByteString onLineNum;
    ByteString likedNum;
    ByteString isSubscribe;
    ByteString name;
    ByteString image;
    ByteString currentProgramTitle;
    ByteString playUrl;
    ByteString freq;
};

class BDCastDetail
{
public:
    BDCastDetail() {}

    ByteString mBroadcastId;
    ByteString mName;
    ByteString mImg;
    ByteString mClassifyName;
    ByteString mPlayUrl;
    ByteString mClassifyId;
    ByteString mAreaCode;
};

class BDCastProgramItem
{
public:
    BDCastProgramItem()

    {}

    ByteString programId;
    ByteString broadcastId;
    ByteString nextProgramId;
    ByteString preProgramId;
    ByteString title;
    ByteString backLiveUrl;
    ByteString stime;
    ByteString etime;
    ByteString playUrl;
    ByteString comperes;
    ByteString beginTime;
    ByteString endTime;
    ByteString startTime;
    ByteString finishTime;
    ByteString status;
    ByteString isSubscribe;
    ByteString desc;
    ByteString broadcastDesc;
    ByteString broadcastName;
    ByteString broadcastImg;

};

// CategoryBroadcast
class CateBCast
{
public:
    CateBCast()
    {}

    ByteString id;
    ByteString type;
    ByteString name;
    ByteString img;
};

// CategorySublist
class CateSub
{
public:
    CateSub()
    {}

    ByteString cid;
    ByteString name;
    ByteString hasSub;
    ByteString img;
};

// CategoryAll
class CateMain : public CateSub
{
public:
    CateMain()
    {}
};

// OperateList
class Operate
{
public:
    Operate() {}

    ByteString rid;
    ByteString rtype;
    ByteString rname;
    ByteString image;
    ByteString rvalue;
    ByteString listenNum;
    ByteString desc;
    ByteString optType;
    ByteString isMusic;
    ByteString isOpt;
    ByteString index;
};

// TypeRadioList
class TypeRadio
{
public:
    TypeRadio()
    {}

    ByteString id;
    ByteString name;
    ByteString img;
    ByteString followedNum;
    ByteString isOnline;
    ByteString listenNum;
    ByteString desc;
    ByteString commentNum;
    ByteString isSubscribe;
    ByteString type;
    ByteString host; // object
    ListTable<ByteString> keyWords;
};

// AlbumList
class AlbumItem
{
public:
    ByteString id;
    ByteString name;
    ByteString img;
    ByteString listenNum;
    ByteString source;
    ByteString sourceName;
};

// AlbumDetail
class AlbDetail
{
public:
    AlbDetail() {}

    ByteString id;
    ByteString name;
    ByteString img;
    ByteString followedNum;
    ByteString countNum;
    ByteString isOnline;
    ByteString desc;
    ByteString listenNum;
    ByteString sortType;
    ByteString hasCopyright;
    ByteString host; // object
    ByteString produce;
    ByteString status;
    ByteString updateDay;
    ByteString copyrightLabel;
    ListTable<ByteString> keyWords;
    ByteString commentNum;
    ByteString lastCheckDate;
    ByteString type;
    ByteString isSubscribe;

};

// ChipAudioList
// ChipAudioDetail
class AudioItem
{
public:
    ByteString audioId;
    ByteString audioName;
    ByteString audioPic;
    ByteString audioDes;
    ByteString albumId;
    ByteString albumName;
    ByteString albumPic;
    ByteString orderNum;
    ByteString mp3PlayUrl32;
    ByteString mp3PlayUrl64;
    ByteString aacPlayUrl;
    ByteString aacPlayUrl32;
    ByteString aacPlayUrl64;
    ByteString aacPlayUrl128;
    ByteString aacPlayUrl320;
    ByteString aacFileSize;
    ByteString mp3FileSize32;
    ByteString mp3FileSize64;
    ByteString updateTime;
    ByteString clockId;
    ByteString duration;
    ByteString originalDuration;
    ByteString listenNum;
    ByteString likedNum;
    ByteString hasCopyright;
    ByteString commentNum;
    ByteString trailerStart;
    ByteString trailerEnd;
    ByteString categoryId;
    ByteString source;
    ByteString isListened;
    ByteString isThirdParty;
    ByteString hasNextPage;
};

// ChipRadioDetail
class RadioDetail : public TypeRadio
{
public:
    RadioDetail() {}

};

// ChipRadioList
class RadioItem : public AudioItem
{
public:
    RadioItem() {}
};

// 通知UI，数据加载解析的结果
class UINotifyIface
{
public:
    virtual void dataPrepare() = 0;
    virtual void errorInfo(int , const char *) {}
};
}

#endif // KL_COMMON_H
