import QtQuick 2.0

GridView {
    id: cateItemView

    model: cateItemModel
    currentIndex: cateItemModel.currenIndex
    clip: true
    //focus: true
    //boundsBehavior: Flickable.StopAtBounds
    //layoutDirection: Qt.LeftToRight
    //flow: GridView.FlowTopToBottom

    cellWidth: width /3
    cellHeight: width /4
    delegate: VideosModel {
        playing: false
        onClicked: {
            cateItemModel.qmlClickCategory(index)
            // stack.push("CateItemInfoView.qml")
        }
    }
    onFlickEnded:
    {
        if(atYEnd)
        {
            cateItemModel.qmlCtgNextPage();
        }
    }
}
