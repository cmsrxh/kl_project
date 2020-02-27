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
#define SIGN_VoiceSearchAll             "955d3a8e24056e71297c51bb5cf98381"
#define SIGN_SuggestionWord             "81230f5ae8db203704271b5c87cc6fbc"

namespace kl
{
// 基于KlObject派生类的名字，使用枚举表示，用各种异常处理
enum KlObjectName
{
    OBJECT_ACTIVE_MANAGE,
    OBJECT_INIT_MANAGE,
    OBJECT_ALBUM_DETAIL,
    OBJECT_ALBUM_LIST,
    OBJECT_BDC_AREA_LIST,
    OBJECT_BDC_ITEM_DETAIL,
    OBJECT_BDC_ITEM_LIST,
    OBJECT_BDC_ITEM_PROGRAM,
    OBJECT_CATEGORY_ALL,
    OBJECT_CATEGORY_BDC,
    OBJECT_CATEGORY_SUB_LIST,
    OBJECT_CHIP_AUDIO_DETAIL,
    OBJECT_CHIP_AUDIO_LIST,
    OBJECT_CHIP_RADIO_DETAIL,
    OBJECT_CHIP_RADIO_LIST,
    OBJECT_OPERATE_LIST,
    OBJECT_SUGGESTION_WORD,
    OBJECT_VOICE_SEARCH_ALL,
    OBJECT_TYPERADIO_LIST,

    LOCAL_OBJECT_LOAD,
    LOCAL_OBJECT_COLLECT,
    LOCAL_OBJECT_HISTROY,
    LOCAL_OBJECT_SEARCH,
};

class AreaItem
{
public:
    void clear()
    {
        id.clear();
        name.clear();
    }

    ByteString id;
    ByteString name;
};

class BDCastItem
{
public:
    void clear()
    {
        broadcastId.clear();
        status.clear();
        onLineNum.clear();
        likedNum.clear();
        isSubscribe.clear();
        name.clear();
        image.clear();
        currentProgramTitle.clear();
        playUrl.clear();
        freq.clear();
    }

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
    void clear()
    {
        mBroadcastId.clear();
        mName.clear();
        mImg.clear();
        mClassifyName.clear();
        mPlayUrl.clear();
        mClassifyId.clear();
        mAreaCode.clear();
    }

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
    void clear()
    {
        programId.clear();
        broadcastId.clear();
        nextProgramId.clear();
        preProgramId.clear();
        title.clear();
        backLiveUrl.clear();
        stime.clear();
        etime.clear();
        playUrl.clear();
        comperes.clear();
        beginTime.clear();
        endTime.clear();
        startTime.clear();
        finishTime.clear();
        status.clear();
        isSubscribe.clear();
        desc.clear();
        broadcastDesc.clear();
        broadcastName.clear();
        broadcastImg.clear();
    }

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
    void clear()
    {
        id.clear();
        type.clear();
        name.clear();
        img.clear();
    }

    ByteString id;
    ByteString type;
    ByteString name;
    ByteString img;
};

// CategorySublist
class CateSub
{
public:
    void clear()
    {
        cid.clear();
        name.clear();
        hasSub.clear();
        img.clear();
    }

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
    void clear()
    {
        rid.clear();
        rtype.clear();
        rname.clear();
        image.clear();
        rvalue.clear();
        listenNum.clear();
        desc.clear();
        optType.clear();
        isMusic.clear();
        isOpt.clear();
        index.clear();
    }

    ByteString rid;         // 资源 ID
    ByteString rtype;       // 资源类型 (0:专辑|3:电台)
    ByteString rname;       // 资源名称
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
    void clear()
    {
        id.clear();
        name.clear();
        img.clear();
        followedNum.clear();
        isOnline.clear();
        listenNum.clear();
        desc.clear();
        commentNum.clear();
        isSubscribe.clear();
        type.clear();
        host.clear(); // object
        ListTable<ByteString>::iterator it = keyWords.begin();
        for(; it != keyWords.end(); ++it)
        {
            it->clear();
        }
    }
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
    void clear()
    {
        id.clear();
        name.clear();
        img.clear();
        listenNum.clear();
        source.clear();
        sourceName.clear();
    }
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

    void clear()
    {
        id.clear();
        name.clear();
        img.clear();
        followedNum.clear();
        countNum.clear();
        isOnline.clear();
        desc.clear();
        listenNum.clear();
        sortType.clear();
        hasCopyright.clear();
        host.clear(); // object
        produce.clear();
        status.clear();
        updateDay.clear();
        copyrightLabel.clear();
        commentNum.clear();
        lastCheckDate.clear();
        type.clear();
        isSubscribe.clear();

        ListTable<ByteString>::iterator it = keyWords.begin();
        for (; it != keyWords.end(); ++it)
        {
            it->clear();
        }
    }
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
    void clear()
    {
        audioId.clear();
        audioName.clear();
        audioPic.clear();
        audioDes.clear();
        albumId.clear();
        albumName.clear();
        albumPic.clear();
        orderNum.clear();
        mp3PlayUrl32.clear();
        mp3PlayUrl64.clear();
        aacPlayUrl.clear();
        aacPlayUrl32.clear();
        aacPlayUrl64.clear();
        aacPlayUrl128.clear();
        aacPlayUrl320.clear();
        aacFileSize.clear();
        mp3FileSize32.clear();
        mp3FileSize64.clear();
        updateTime.clear();
        clockId.clear();
        duration.clear();
        originalDuration.clear();
        listenNum.clear();
        likedNum.clear();
        hasCopyright.clear();
        commentNum.clear();
        trailerStart.clear();
        trailerEnd.clear();
        categoryId.clear();
        source.clear();
        isListened.clear();
        isThirdParty.clear();
        hasNextPage.clear();
    }

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

// VoiceSearchAll
class SearchItem
{
public:
    void clear()
    {
        id.clear();
        name.clear();
        img.clear();
        type.clear();
        albumName.clear();
        source.clear();
        duration.clear();
        playUrl.clear();
        sourceName.clear();
        oldId.clear();
        callback.clear();
        listenNum.clear();
        isShowRed.clear();
        isRequest.clear();
        originalDuration.clear();
    }

    ByteString id;
    ByteString name;
    ByteString img;
    ByteString type; /* 0：专辑, 1：碎片, 3：智能电台, 11：传统电台 */
    ByteString albumName;
    ByteString source;
    ByteString duration;
    ByteString playUrl;
    ByteString sourceName;
    ByteString oldId;
    ByteString callback;
    ByteString listenNum;
    ByteString isShowRed;
    ByteString isRequest;
    ByteString originalDuration;

};

// SuggestionWord
class SuggestItem
{
public:
    SuggestItem() {}

    void clear()
    {
        result.clear();
    }

    ByteString result;
};

// RecordManage, CollectManage
class RecordItem
{
public:
    void clear()
    {
        id.clear();
        parentId.clear();
        name.clear();
        parentName.clear();
        image.clear();
        playUrl.clear();
        fileSize.clear();
    }
    void copy(RecordItem *item)
    {
        item->isLocal    = isLocal;
        item->type       = type;
        item->id         = ByteString::allocString(id);
        item->parentId   = ByteString::allocString(parentId);
        item->name       = ByteString::allocString(name);
        item->parentName = ByteString::allocString(parentName);
        item->image      = ByteString::allocString(image);
        item->playUrl    = ByteString::allocString(playUrl);
        item->fileSize   = ByteString::allocString(fileSize);
    }

    int        isLocal; // 表明当前媒体时候在本地
    int        type;    // PLAY_CHIP_TYPE*
    ByteString id;
    ByteString parentId;
    ByteString name;
    ByteString parentName;
    ByteString image;
    ByteString playUrl;
    ByteString fileSize;
};

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
}

class ImageStatus
{
public:
    virtual void dataPrepare() = 0;
    virtual void errorInfo(int , const char *) {}
};

#endif // KL_COMMON_H
