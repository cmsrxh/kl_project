import QtQuick 2.0
import Hongjing.HMI.KL 1.0 as KL

//用于显示当前播放电台的节目列表
Rectangle
{
    id: popLab
    color: "#81212121"

    Item
    {
        width: parent.width / 2
        height: 80
        anchors.top: parent.top
        anchors.right: parent.right
        // color: "#373737"
        Rectangle //关闭按钮
        {

            width: 120
            height: 50
            anchors.bottom: parent.bottom
            anchors.bottomMargin:15
            anchors.left: parent.left
            //anchors.leftMargin: -20

            color: "black"
            radius: height / 2
            Rectangle
            {
                width: 20
                height: 50
                anchors.left: parent.left
                color: "black"
            }
            Image
            {
                width: Image.width
                height: Image.height
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/image/songListClose.png"
            }
            Text
            {
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideRight
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
                text: qsTr("关闭")
                color: "white"
                font.pixelSize: 20
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    popLab.visible = false
                }
            }
        }

    }

    Rectangle
    {
        id: list
        width: parent.width / 2
        height: parent.height
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        color: "#373737"

        ListView {
            id: view
            property bool isLoad: false
            anchors.fill: parent
            currentIndex: playList.playingIndex
            model: playList
            delegate: CurrentDelegate{}
            spacing: 2
            clip: true

            onDragEnded:
            {
                if(isLoad)
                {
                    isLoad = false
                    var isHaveNext = playList.playNeedNextPage();
                    if (!isHaveNext)
                    {
                        msgBox.boxType = 4
                        msgBox.msgContent = qsTr("已经加载到末尾了")
                        msgBox.running  = true
                        msgBox.interval = 1500
                    }
                }
            }
            onContentYChanged: {
                if (contentHeight < height || contentY <= contentHeight - height)
                {
                    upDrag.visible   = false
                } else
                {
                    upDrag.visible = true
                    upDrag.height  = -(contentHeight - height - contentY)
                    upDrag.y       = height - upDrag.height
                    if (upDrag.height > 60) isLoad = true;
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

            KlMsgTipBox {
                id: msgBox
                anchors.fill: parent
            }
        }
    }

    MouseArea
    {
        width: parent.width / 2
        height: parent.height
        anchors.right: parent.right
        onClicked:
        {
            popLab.visible = false
        }
    }
}

