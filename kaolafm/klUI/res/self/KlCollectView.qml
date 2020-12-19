import QtQuick 2.0
import ".."
import Hongjing.HMI.KL 1.0 as KL

Item {
    id: collect
    Item {
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 30
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        ListView
        {
            id: cltView

            model: collectList
            delegate: collectDelegate
            currentIndex: collectList.collectId
            anchors.fill: parent
            clip: true;
            spacing: 0
        }

        ScrollBar {
            id: listScrollBar

            orientation:  Qt.Vertical
            height: cltView.height
            width: 13
            scrollArea: cltView
            anchors.right: cltView.right
            anchors.top: cltView.top
        }


        Component
        {
            id:collectDelegate
            Rectangle
            {
                width: ListView.view.width; height: 100
                color: (ListView.view.currentIndex === index) ? "#433A2D" : "#373737"
                ImageFrame {
                    id: comImage
                    width: parent.height - 10
                    height: parent.height - 10
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    anchors.verticalCenter: parent.verticalCenter
                    imageUri: pictureUrl
                }
                Column{
                    width: parent.width - 2 * parent.height
                    //height: parent.height
                    anchors.left: comImage.right
                    anchors.leftMargin: 30
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 10

                    Text
                    {
                        id: compTitle

                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        text: collectTitle
                        color: "white"
                        font.pixelSize: 24
                    }
                    Text
                    {
                        id: compText

                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        text: collectName
                        color: "red"
                        font.pixelSize: 18
                    }
                }

                ThreeStateBtn
                {
                    width: parent.height
                    height: parent.height
                    //anchors.right: comLine.left
                    anchors.right: parent.right
                    initSt: "qrc:/image/collect.png"
                    pressSt: "qrc:/image/collect_clk.png"
                    lastSt: "qrc:/image/collect_hl.png"
                    enable: isCollect

                    onClicked:
                    {
                        collectList.qmlCollectThirdLevelEnableClick(index, enable)
                    }
                }

                KlLine
                {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 2
                    fillColor: "gray"
                }
                MouseArea
                {
                    width: parent.width - parent.height //- parent.height
                    height: parent.height
                    onClicked:
                    {
                        collectList.qmlCollectThirdLevelClick(index);
                    }
                }
            }
        }

        Rectangle {
            anchors.fill: cltView
            border.color: "green"
            border.width: 2
            color: "transparent"
        }
    }
}

