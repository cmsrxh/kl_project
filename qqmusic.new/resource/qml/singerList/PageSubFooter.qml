import QtQuick 2.0
import com.hongjing.singerlist 1.0 as Singerlist

GridView {
    id: singerList
    width: parent.width

    cellWidth: 300
    cellHeight: 100

    model: Singerlist.Api.singerChildModel
    currentIndex: model.currentIndex
    clip: true

    footer: PageFooter {
        width: parent.width
        height: 70
    }

    delegate: Item {
        id: item
        width: 300
        height: 100

        Text {
            anchors.fill: parent
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
            text: cateItemName
            color: "#5CB8FC"
            elide: Text.ElideRight
            font.pixelSize: 32

            MouseArea {
                anchors.fill: parent
                onClicked: Singerlist.Api.qmlSingerList(6, index)
            }
        }
    }
}
