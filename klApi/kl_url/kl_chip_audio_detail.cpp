#include "events/common_log.h"
#include "net_util/net_common.h"
#include "config_local_info.h"
#include "kl_chip_audio_detail.h"
/*
http://open.kaolafm.com/v2/audio/detail?id=1000013097452&appid=cn5290&openid=cn52902019122410012497&deviceid=69acf5ec77a42531d1748b7dfc6dcdb6&os=linux&packagename=com.edog.car.linuxwhhj&sign=81230f5ae8db203704271b5c87cc6fbc&channel=linuxwhhj&
{"result":{"audioId":1000013097452,"audioName":"刘惜君-菩提偈（电视剧《独孤天下》片尾曲）","audioPic":"http://img.kaolafm.net/mz/images/201803/6352e836-dc75-4dc0-ac43-e84f7c1e9265/default.jpg","audioDes":"《菩提偈》（电视剧《独孤天下》片尾曲）\r\n\r\n词：张赢\r\n曲：罗锟\r\n演唱：刘惜君\r\n\r\n\r\n监制：宋鹏飞\r\n制作人：罗锟\r\n编曲：张楚弦\r\n吉他/贝斯：张楚弦\r\n和音：赵贝尔\r\n弦乐：国际首席爱乐乐团\r\n混音/母带：姚海毅\r\n音乐出品发行：北京听见时代娱乐传媒有限公司\r\n\r\n\r\n问世间 转过多少流年\r\n才会有一次擦肩\r\n明镜水月菩提树下又见\r\n缘 刻眷指尖\r\n\r\n燎岁月 惹尘埃动情念\r\n可会熬不过时间\r\n\r\n百转千遍愿\r\n最好是成全\r\n含笑一睹你容颜\r\n天地多辽远\r\n而你在眼前\r\n怎拂去眷恋\r\n\r\n————repeat————\r\n\r\n问世间 转过多少流年\r\n才会有一次擦肩\r\n明镜水月菩提树下又见\r\n缘 刻眷指尖\r\n\r\n燎岁月 惹尘埃动情念\r\n可会熬不过时间\r\n\r\n百转千遍愿\r\n最好是成全\r\n含笑一睹你容颜\r\n天地多辽远\r\n而你在眼前\r\n怎拂去眷恋\r\n\r\n百转千遍劫\r\n最难是成全\r\n等一世我亦甘愿\r\n一生多遥远\r\n但求一瞬间\r\n刻骨绕心间\r\n\r\n————repeat————\r\n\r\n百转千遍夜\r\n你陪我 默念\r\n来生可否能再见\r\n一诺永生缘\r\n偈言颂离别\r\n愿此情不灭\r\n\r\n愿此情不灭","albumId":1100000048156,"albumName":"考拉华语新歌推荐","albumPic":"http://img.kaolafm.net/mz/images/201506/cdd592cc-017f-4e2f-828c-729d94d75e8a/default.jpg","orderNum":569,
"mp3PlayUrl32":"http://image.kaolafm.net/mz/mp3_32/201803/4d12f15e-da11-4d16-9b50-be19ebd886be.mp3",
"mp3PlayUrl64":"http://image.kaolafm.net/mz/mp3_64/201803/4d12f15e-da11-4d16-9b50-be19ebd886be.mp3",
"aacPlayUrl":"http://image.kaolafm.net/mz/aac_32/201803/4d12f15e-da11-4d16-9b50-be19ebd886be.aac",
"aacPlayUrl32":"http://image.kaolafm.net/mz/aac_32/201803/4d12f15e-da11-4d16-9b50-be19ebd886be.aac",
"aacPlayUrl64":"http://image.kaolafm.net/mz/aac_64/201803/4d12f15e-da11-4d16-9b50-be19ebd886be.aac",
"aacPlayUrl128":"http://image.kaolafm.net/mz/aac_128/201803/4d12f15e-da11-4d16-9b50-be19ebd886be.aac",
"aacPlayUrl320":"","host":[{"name":"尹小山","des":"","img":""}],"aacFileSize":2059344,"mp3FileSize32":979312,"mp3FileSize64":1959125,"updateTime":1520212923000,"clockId":"","duration":244920,"originalDuration":244924,"listenNum":201130,"likedNum":9,"hasCopyright":1,"commentNum":2,"trailerStart":0,"trailerEnd":0,"categoryId":141,"source":2,"isListened":0,"icon":null,"isThirdParty":0,"callBack":"","hasNextPage":1,"singer":"","contentType":0,"contentTypeName":"","mainTitleName":"","subheadName":"","launchType":null,"launchName":null},"serverTime":"1577189658579","requestId":"cn52901577189658569424"}

*/
kl::ChipAudioDetail::ChipAudioDetail(const ByteString &aid)
    : ItemObject<AudioItem>("http://open.kaolafm.com/v2/audio/detail", NetUrl::NET_HTTP_METHOD_GET)
    , mAudioId(ByteString::allocString(aid))
{
}

kl::ChipAudioDetail::~ChipAudioDetail()
{
    mAudioId.clear();
}

NetUrl &kl::ChipAudioDetail::genQueryUrl()
{
    mUrl.appendChange("id", mAudioId);

    mUrl.appendChange("openid", LocalConfig::instance()->openID());
    mUrl.appendChange("sign", SIGN_ChipAudioDetail);

    return mUrl;
}

void kl::ChipAudioDetail::profile()
{
    GEN_Printf(LOG_DUMP, "audioId: %s", mItem.audioId.string());
    GEN_Printf(LOG_DUMP, "audioName: %s", mItem.audioName.string());
    GEN_Printf(LOG_DUMP, "audioPic: %s", mItem.audioPic.string());
    GEN_Printf(LOG_DUMP, "audioDes: %s", mItem.audioDes.string());
    GEN_Printf(LOG_DUMP, "albumId: %s", mItem.albumId.string());
    GEN_Printf(LOG_DUMP, "albumName: %s", mItem.albumName.string());
    GEN_Printf(LOG_DUMP, "albumPic: %s", mItem.albumPic.string());
    GEN_Printf(LOG_DUMP, "orderNum: %s", mItem.orderNum.string());
    GEN_Printf(LOG_DUMP, "mp3PlayUrl32: %s", mItem.mp3PlayUrl32.string());
    GEN_Printf(LOG_DUMP, "mp3PlayUrl64: %s", mItem.mp3PlayUrl64.string());
    GEN_Printf(LOG_DUMP, "aacPlayUrl: %s", mItem.aacPlayUrl.string());
    GEN_Printf(LOG_DUMP, "aacPlayUrl32: %s", mItem.aacPlayUrl32.string());
    GEN_Printf(LOG_DUMP, "aacPlayUrl64: %s", mItem.aacPlayUrl64.string());
    GEN_Printf(LOG_DUMP, "aacPlayUrl128: %s", mItem.aacPlayUrl128.string());
    GEN_Printf(LOG_DUMP, "aacPlayUrl320: %s", mItem.aacPlayUrl320.string());
    GEN_Printf(LOG_DUMP, "aacFileSize: %s", mItem.aacFileSize.string());
    GEN_Printf(LOG_DUMP, "mp3FileSize32: %s", mItem.mp3FileSize32.string());
    GEN_Printf(LOG_DUMP, "mp3FileSize64: %s", mItem.mp3FileSize64.string());
    GEN_Printf(LOG_DUMP, "updateTime: %s", mItem.updateTime.string());
    GEN_Printf(LOG_DUMP, "clockId: %s", mItem.clockId.string());
    GEN_Printf(LOG_DUMP, "duration: %s", mItem.duration.string());
    GEN_Printf(LOG_DUMP, "originalDuration: %s", mItem.originalDuration.string());
    GEN_Printf(LOG_DUMP, "listenNum: %s", mItem.listenNum.string());
    GEN_Printf(LOG_DUMP, "likedNum: %s", mItem.likedNum.string());
    GEN_Printf(LOG_DUMP, "hasCopyright: %s", mItem.hasCopyright.string());
    GEN_Printf(LOG_DUMP, "commentNum: %s", mItem.commentNum.string());
    GEN_Printf(LOG_DUMP, "trailerStart: %s", mItem.trailerStart.string());
    GEN_Printf(LOG_DUMP, "trailerEnd: %s", mItem.trailerEnd.string());
    GEN_Printf(LOG_DUMP, "categoryId: %s", mItem.categoryId.string());
    GEN_Printf(LOG_DUMP, "source: %s", mItem.source.string());
    GEN_Printf(LOG_DUMP, "isListened: %s", mItem.isListened.string());
    GEN_Printf(LOG_DUMP, "isThirdParty: %s", mItem.isThirdParty.string());
    GEN_Printf(LOG_DUMP, "hasNextPage: %s", mItem.hasNextPage.string());
}

void kl::ChipAudioDetail::genResult(NetBuffer *data)
{
    cJSON *root = cJSON_Parse((char *)data->buffer(), data->size());
    cJSON *result = cJSON_GetObjectItem(root, "result");

    if (result)
    {
        jsonGenAudioItem(mItem, result);
        // profile();
    } else
    {
        GEN_Printf(LOG_ERROR, "Priser result failed.");
    }

    cJSON_Delete(root);
}
