import QtQuick 2.0
import Hongjing.HMI.KL 1.0 as KL


Item {

    Text
    {
        y: 5
        anchors.left: parent.left
        anchors.leftMargin: 30

        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignRight
        text: qsTr("共") + chipList.itemCount + qsTr("期")
        color: "white"
        font.pixelSize: 20
    }

    Rectangle
    {
        id: sortTitle
        width: 80; height: 40
        anchors.right: parent.right
        anchors.rightMargin: 20
        radius: 20
        color: "#212121"
//        Image
//        {
//            id: sortImage
//            anchors.verticalCenter: parent.verticalCenter
//            anchors.left: parent.left
//            anchors.leftMargin: 15
//            width: Image.width
//            height: Image.height
//            source: ""
//        }
        Text
        {
            id: sortText
            anchors.fill: parent
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("返回")
            color: "white"
            font.pixelSize:24
        }

        MouseArea
        {
            anchors.fill: parent
            onClicked: stack.qmlPop();
            onPressed: sortTitle.color = "gray"
            onReleased: sortTitle.color = "#212121"
            onCanceled: sortTitle.color = "#212121"
        }
    }

    Rectangle
    {
        anchors.bottom: audioView.top
        width: parent.width
        height: 2
        color: "gray"
    }

    ListView {
        id: audioView
        clip: true
        spacing: 0
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.bottom: parent.bottom
        model: chipList
        currentIndex: chipList.playingIndex
//        boundsBehavior: Flickable.StopAtBounds
        onFlickEnded: {
            if(atYEnd) {
                chipList.needNextPage();
            }
        }
        delegate: audioDelegate
    }

    ScrollBar {
        id: listScrollBar

        orientation:  Qt.Vertical
        height: audioView.height
        width: 13
        scrollArea: audioView
        anchors.right: audioView.right
        anchors.top: audioView.top

        bgSrc: "qrc:/image/scrollBar.png"
    }

    Component {
        id: audioDelegate
        Item {
            id: chItem
            width: audioView.width - 20
            height: 51

            Image {
                id: imgPlaying
                visible: chipList.playingIndex === index
                width: Image.width
                height: Image.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                source: "qrc:/image/playing.png"
            }

            Text
            {
                id: chNameText
                width: parent.width - imgPlaying.width
                height: parent.height
                anchors.left: imgPlaying.right
                //anchors.leftMargin: 10
                elide: Text.ElideRight
                verticalAlignment: Qt.AlignVCenter
                text: chipItemName
                color: "white"
                font.pixelSize: 18
            }

            //KlLine
            Rectangle
            {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 2
                color: "gray"
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    chipList.chipItemClick(index)
                }
            }
        }

    }
}
