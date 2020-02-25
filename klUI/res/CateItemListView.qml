import QtQuick 2.0
import Hongjing.HMI.KL 1.0 as KL

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
            var isHaveNext = cateItemModel.qmlCtgNextPage();
            if (!isHaveNext)
            {
                msgBox.boxType = 4
                msgBox.msgContent = qsTr("已经加载到末尾了")
                msgBox.running  = true
                msgBox.interval = 1500
            }/* else
            {
                msgBox.boxType = 2
                msgBox.msgContent = qsTr("数据加载中……")
            }*/
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
            text: qsTr("上拉加载…")
            color: "white"
            font.pixelSize: 24
        }
    }

    Connections {
        target: KL.Controller
        onMsgTipCateItem: {
            msgBox.boxType    = boxType
            msgBox.msgContent = msgContent
        }
    }

    KlMsgTipBox {
        id: msgBox
        anchors.fill: parent
//        boxType: 0
//        msgContent: qsTr("数据加载中…")
        onFailClick: {
            KL.Controller.qmlReloadErrObject()
        }
    }
}
