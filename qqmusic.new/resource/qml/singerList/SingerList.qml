import QtQuick 2.0
import "../public"

Item {
    Component.onCompleted: {
        singerProc.qmlObtainSingerList()
    }

    GridView {
        id: singerList
        anchors.fill: parent
        cellWidth: 300
        cellHeight: 340

        model: singerModel
        currentIndex: singerModel.currenIndex
        clip: true

        header: TagsView {
            width: parent.width
            height: 300
        }
        footer: PageSubFooter {
            width: parent.width
            height: contentHeight + 21
        }

        delegate: Item {
            id: item
            width: 300
            height: 340

            RoundPictureButton {
                anchors.top: parent.top
                anchors.topMargin: 25
                anchors.horizontalCenter: parent.horizontalCenter
                width: 250
                height: 250
                source: imageUri
                onClicked: singerProc.qmlSingerList(5, index)
            }
            Text {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
                text: cateItemName
                color: "#5CB8FC"
                elide: Text.ElideRight
                font.pixelSize: 32

                MouseArea {
                    anchors.fill: parent
                    onClicked: singerProc.qmlSingerList(5, index)
                }
            }
        }
    }

    ScrollBar {
        id: scrollbar

        orientation:  Qt.Vertical
        height: singerList.height
        width: 13
        scrollArea: singerList
        anchors.right: singerList.right
        anchors.top: singerList.top

        bgSrc: "qrc:/image/public/scrollBar.png"
    }
}
