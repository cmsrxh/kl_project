import QtQuick 2.0
import QtGraphicalEffects 1.0
import HongJing.HMI.Media 1.0 as Media

Item {
    Connections
    {
        target: Media.Controller
        onFlushPicture:
        {
            console.log("Image Update")
            //img.update();
            img.source= ""//更新图片
            img.source= "image://screen/0" //image:必须
        }
    }

    Image
    {
        source: "qrc:/images/multimedia/music/music_bg.png"
    }

    Item {
        id:musicCover
        width: 254; height: 259
        x: 310; y: 158

        Image {
            id: img
            cache: false
            visible: false
            smooth: true
            anchors.fill: parent
            sourceSize: Qt.size(parent.width - 10, parent.height - 10)
            source: "image://media/0"
        }

        Image {
            id: imgMask
            visible: false
            smooth: true
            anchors.fill: parent
            sourceSize: Qt.size(parent.width, parent.height)
            source: "qrc:/images/multimedia/music/musicCover.png"
        }

        //使用QML的覆盖属性，将图片显示成为maskSource的形状
        OpacityMask {
            id: opMask
            anchors.fill: img
            source: img
            maskSource: imgMask
        }
    }

    ScrollFont{
        id:musicName
        x:672
        y:238
        width: 214
        height: 35
        textColor: "white"
        text: Media.Controller.musicName
        pixelSize: 28
    }

    Text{
        id:musicArtist
        x:672; y:296
        width: 200; height: 26
        color: "white"
        text: Media.Controller.musicAuther
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        font.pixelSize: 20
    }

    Text {
        id: musicAlbumText
        x:672; y:329
        width: 200; height: 26
        text: Media.Controller.musicAlbum
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        color: "white"
        font.pixelSize: 20
    }

    Item {
        id: musicIndex
        x: 858; y:207
        width: 100; height: 30
        Text {
            id: musicIndexText
            color: "white"
            text: Media.Controller.musicNumList
            font.pixelSize: 20
        }
    }


    MediaSlider
    {
        id: slider
        width: 500
        height: 50
        fontSize: 16
        textAlign: 15

        anchors.bottom: ctrlLab.top
        anchors.bottomMargin: 15
        anchors.right: parent.right
        anchors.rightMargin: 135
    }

    MediaCtrlLab
    {
        id: ctrlLab
        anchors.bottom: parent.bottom
        playModel: musicModel.playMode
//        onPlayModelChanged:
//        {
//            console.log("playModel: ", playModel, musicModel.playMode);
//        }
    }


}
