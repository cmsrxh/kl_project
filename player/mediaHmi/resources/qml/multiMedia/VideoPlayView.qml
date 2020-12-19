import QtQuick 2.0
import HongJing.HMI.Media 1.0 as Media

Item {
    id: video

    property bool isShow: true

    Timer
    {
        id: hideTimer
        repeat: false
        running: video.isShow
        interval: 5000
        onTriggered:
        {
            video.isShow = false;
        }
    }

    Item
    {
        width: ctrlLab.width
        height: ctrlLab.height

        anchors.top: parent.top
        visible: video.isShow

        Rectangle
        {
            color: "black"
            opacity: 0.5
        }
        Text
        {
            anchors.fill: parent
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter

            text: Media.Controller.videoName
            color: "white"
            font.pixelSize: 24
        }
    }


    MediaSlider
    {
        id: slider
        width: 500
        height: 50
        anchors.bottom: ctrlLab.top
        anchors.horizontalCenter: parent.horizontalCenter
        visible: video.isShow

    }

    MediaCtrlLab
    {
        id: ctrlLab
        anchors.bottom: parent.bottom
        visible: video.isShow
        playModel: videoModel.playMode
    }

    MouseArea
    {
        anchors.fill: parent
        propagateComposedEvents: true
        onPressed:
        {
            mouse.accepted = false
            hideTimer.restart();
            if(!video.isShow)
            {
                video.isShow = true;
            }
            // console.log("Mouse Clicked.")
        }
    }
}
