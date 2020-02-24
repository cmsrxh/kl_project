import QtQuick 2.0

GridView {
    property bool isLoad: false
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
    onDragEnded:
    {
        if(isLoad)
        {
            isLoad = false
            cateItemModel.qmlCtgNextPage();
        }
    }
    onContentYChanged: {
        if (contentY > contentHeight - height)
        {
            upDrag.visible = true
            upDrag.height  = -(contentHeight - height - contentY)
            upDrag.y       = height - upDrag.height
            if (upDrag.height > 60) isLoad = true;
        } else
        {
            upDrag.visible   = false
        }
    }
    Rectangle {
        id: upDrag
        color: "green"
        width: parent.width
        height: 0
        visible: false
        clip: true
        Text {
            anchors.centerIn: parent
            text: qsTr("上拉加载……")
        }
    }

    KlMsgTipBox {
        anchors.fill: parent
        boxType: 4
        msgContent: "sdnfsjkfn"
    }
}
