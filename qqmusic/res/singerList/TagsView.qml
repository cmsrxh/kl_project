import QtQuick 2.0
import "../tool"

Item {
    Item {
        id: topEmpty
        width: parent.width
        height: 80
    }
    Column
    {
        anchors.top: topEmpty.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        spacing: 15
        TagFlow {
            id: index
            currentIndex: singerIndexModel.curIndex
            model: singerIndexModel
            onClicked: singerProc.qmlSingerList(1, choiceIndex)
        }
        Row {
            id: union
            spacing: 15
            TagFlow {
                id: area
                currentIndex: singerAreaModel.curIndex
                model: singerAreaModel
                onClicked: singerProc.qmlSingerList(2, choiceIndex)
            }
            TagFlow {
                id: sex
                currentIndex: singerSexModel.curIndex
                model: singerSexModel
                onClicked: singerProc.qmlSingerList(3, choiceIndex)
            }
        }

        TagFlow {
            id: genre
            currentIndex: singerGenreModel.curIndex
            model: singerGenreModel
            onClicked: singerProc.qmlSingerList(4, choiceIndex)
        }
    }
}
