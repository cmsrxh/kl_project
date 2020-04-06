import QtQuick 2.0
import "../tool"

Item {

    GridView {
        id: playList
        anchors.fill: parent
        cellWidth: 250
        cellHeight: 300

        model: catePlayListModel
        // currentIndex: catePlayListModel.currenIndex
        clip: true

        header: Item {
            width: parent.width
            height: 80
        }
        footer: PageFooter {
            width: parent.width
            height: 120
        }
        delegate: Item {
            id: item
            width: 220
            height: 300

            FilletPictureButton
            {
                id: picture
                anchors.top: parent.top
                anchors.topMargin: 25
                anchors.horizontalCenter: parent.horizontalCenter
                width: 200
                height: 200
                source: imageUri
            }

            Text {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: picture.bottom
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                verticalAlignment: Qt.AlignTop
                horizontalAlignment: Qt.AlignHCenter
                maximumLineCount: 2
                text: cateItemName
                //clip: true
                color: "#5CB8FC"
                //elide: Text.ElideRight

                wrapMode:  Text.Wrap
                font.pixelSize: 22
            }

            MouseArea {
                anchors.fill: parent
                onClicked: catePlayListProc.qmlPlayListIndexClick(index)
            }
        }
    }

    ScrollBar {
        id: scrollbar

        orientation:  Qt.Vertical
        height: playList.height
        width: 13
        scrollArea: playList
        anchors.right: playList.right
        anchors.top: playList.top

        bgSrc: "qrc:/image/scrollBar.png"
    }
}
