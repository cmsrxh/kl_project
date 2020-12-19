import QtQuick 2.0
import HongJing.HMI.Media 1.0 as Media

Item {
    id: image

    property bool isShow: true

    Timer
    {
        id: hideTimer
        repeat: false
        running: image.isShow
        interval: 5000
        onTriggered:
        {
            image.isShow = false;
        }
    }

    Item
    {
        width: ctrlLab.width
        height: ctrlLab.height

        anchors.top: parent.top
        visible: image.isShow

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

            text: Media.Controller.imageName
            color: "white"
            font.pixelSize: 24
        }
    }

    PictureCtrlLab
    {
        id: ctrlLab
        anchors.bottom: parent.bottom
        visible: image.isShow
        //playModel: imageModel.playMode
    }

    MouseArea
    {
        anchors.fill: parent
        propagateComposedEvents: true
        onPressed:
        {
            mouse.accepted = false
            hideTimer.restart();
            if(!image.isShow)
            {
                image.isShow = true;
            }
            console.log("Mouse Clicked.")
        }
    }
}
