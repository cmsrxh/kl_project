import QtQuick 2.0
import Hongjing.HMI.KL 1.0 as KL

Rectangle
{
    id: smartBCDelegate
    width: ListView.view.width; height: 51
    //color: (playList.playingIndex === index) ?
    color: (ListView.view.currentIndex === index) ?
             "#51EEA105" : "#414141"

    Rectangle
    {
        id: foreground
        anchors.fill: parent
        color: "transparent"
    }
    //对应PlayingInfo::Playing
    //state: (playList.playingIndex === index) ? "pySt" : ""
    state: (ListView.view.currentIndex === index) ? "pySt" : ""

    Item
    {
        id: playingIcon
        width:0; height: 48
        anchors.verticalCenter: parent.verticalCenter
        Image
        {
            id: playingImg
            width: 0
            height: Image.height
            anchors.centerIn: parent
            source: "qrc:/image/playing.png"
        }
    }
    states : State{
        name: "pySt"
        PropertyChanges { target: playingImg; width: Image.width }
        PropertyChanges { target: playingIcon; width: 48 }
    }

    Text
    {
        id: chTimeText
        width: parent.width - parent.height
        height: parent.height / 2
        anchors.left: playingIcon.right
        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        elide: Text.ElideRight
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignLeft
        text: chipItemName
        color: "white"
        font.pixelSize: 18
    }


    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            playList.playItemClick(index)
        }
        onPressed: {
            foreground.color = "#51EEA105"
        }
        onReleased: {
            foreground.color = "transparent"
        }
        onCanceled: {
            foreground.color = "transparent"
        }
    }

}

