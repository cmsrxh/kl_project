import QtQuick 2.0
import ".."
import Hongjing.HMI.KL 1.0 as KL

Item {
    id: collect

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
            width: cltView.width; height: 100
            color: (collectList.collectId === index) ? "#433A2D" : "#373737"

            Text
            {
                id: compText
                width: parent.width - 2 * parent.height
                height: parent.height
                anchors.left: parent.left
                anchors.leftMargin: 30
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
                text: collectName
                color: "white"
                font.pixelSize: 24
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
}

