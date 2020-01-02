import QtQuick 2.0

import Hongjing.HMI.KL 1.0 as KL

Item {    
    ListView
    {
        id: view
        anchors.fill: parent
        model: bdcTab
        clip: true
        spacing: 0
        //orientation: ListView.Horizontal
        delegate: pageDelegate
    }

    Component
    {
        id: pageDelegate
        Item
        {
            width: view.width; height: 61
            Item
            {
                id: leftItem
                width: parent.width/4; height: 61
                anchors.left: parent.left
                KlLine
                {
                    y: 10
                    width: parent.width
                    fillColor: bdcTab.bdcTabIndex === index ? "#F1C17D" : "white"
                }
                Canvas
                {
                    id: drawLine
                    y: 10
                    width: parent.width/2; height: parent.height
                    antialiasing: false  //去掉据曲

                    onPaint:
                    {
                        var ctx = drawLine.getContext("2d")
                        ctx.lineWidth = 1;
                        ctx.strokeStyle = "gray";

                        ctx.moveTo(0, leftItem.height * 1 / 5) //起点
                        ctx.lineTo(drawLine.width, leftItem.height * 1 / 5)  //线(到终点)

                        ctx.moveTo(0, leftItem.height * 2 / 5) //起点
                        ctx.lineTo(drawLine.width, leftItem.height * 2 / 5)  //线(到终点)

                        ctx.moveTo(0, leftItem.height * 3 / 5) //起点
                        ctx.lineTo(drawLine.width, leftItem.height * 3 / 5)  //线(到终点)

                        ctx.moveTo(0, leftItem.height * 4 / 5) //起点
                        ctx.lineTo(drawLine.width, leftItem.height * 4 / 5)  //线(到终点)
//                        ctx.moveTo(0, 50) //起点
//                        ctx.lineTo(drawLine.width, 50)  //线(到终点)
                        ctx.stroke() //描边，把线画出来
                    }
                }
            }
            Text
            {
                id: textItem
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
                anchors.left: leftItem.right
                anchors.leftMargin: 10
                text: ctgname
                clip: true
                color: bdcTab.bdcTabIndex === index ? "#F1C17D" : "white"
                font.pixelSize: 20
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    bdcTab.qmlBDCCateTabClick(index);
                }
            }
        }
    }    
}

