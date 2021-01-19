import QtQuick 2.0
import com.hongjing.singerlist 1.0 as Singerlist
import "../public"

MainPageBase1 {
    Component.onCompleted: {
        Singerlist.Api.qmlObtainSingerList()
    }
    handle: Rectangle {

        GridView {
            id: singerList
            anchors.fill: parent
            cellWidth: 300
            cellHeight: 340

            model: Singerlist.Api.singerModel
            currentIndex: model.currentIndex
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
                    onClicked: Singerlist.Api.qmlSingerList(5, index)
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
                        onClicked: Singerlist.Api.qmlSingerList(5, index)
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

    onNextPage: {
        console.log("singerlist, next page clicked")
    }

    onPrevPage: {
        console.log("singerlist, prev page clicked")
    }

    onUpToTop: {
        console.log("singerlist, up to top clicked")
    }
}
