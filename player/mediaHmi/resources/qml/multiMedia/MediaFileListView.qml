import QtQuick 2.7
import HongJing.HMI.Media 1.0 as Media
import "./Multimedia.js" as MediaScript

ListView
{
    id: view
    property int view_type: 0
    signal itemClicked(var itemIndex)
    clip: true
    spacing: 0
    orientation:  Qt.Vertical
    boundsBehavior: Flickable.StopAtBounds
    delegate: ButtonPress
    {
        width: 580
        height: 57

        btnImgUrl: pressed ? "qrc:/images/multimedia/item_hl.png"
                           : "qrc:/images/multimedia/item.png"

        Row
        {
            width: 540
            height: 55
            anchors.centerIn: parent
            spacing: 10

            Image
            {
                width: 55
                height: 55
                source: MediaScript.MediaPrefixList[view.view_type]
            }

            Text
            {
                id: textItem
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
                width: 420//460//580 - 120//55 - 55
                height: parent.height
                text: filename
                clip: true
                color: view.currentIndex === index ? "#F1C17D" : "white"
                font.pixelSize: 28
            }

            Image
            {
                width: 55
                height: 55
                source: view.currentIndex === index ? MediaScript.MediaSurfixList[view.view_type] : ""
            }
        }
        onClicked:
        {
            itemClicked(index);
        }
    }

}
