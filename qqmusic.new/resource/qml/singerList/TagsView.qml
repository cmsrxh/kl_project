import QtQuick 2.0
import com.hongjing.singerlist 1.0 as Singerlist

import "../public"

Item {
//    Item {
//        id: topEmpty
//        width: parent.width
//        height: 80
//    }
    Column
    {
        anchors.top: parent.top//topEmpty.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        spacing: 15
        TagFlow {
            id: index
            spacing: 12
            currentIndex: model.curIndex
            model: Singerlist.Api.singerIndexModel
            onClicked: Singerlist.Api.qmlSingerList(1, choiceIndex)
        }
        Row {
            id: union
            spacing: 15
            TagFlow {
                id: area
                currentIndex: model.curIndex
                model: Singerlist.Api.singerAreaModel
                onClicked: Singerlist.Api.qmlSingerList(2, choiceIndex)
            }
            TagFlow {
                id: sex
                currentIndex: model.curIndex
                model: Singerlist.Api.singerSexModel
                onClicked: Singerlist.Api.qmlSingerList(3, choiceIndex)
            }
        }

        TagFlow {
            id: genre
            currentIndex: model.curIndex
            model: Singerlist.Api.singerGenreModel
            onClicked: Singerlist.Api.qmlSingerList(4, choiceIndex)
        }
    }
}
