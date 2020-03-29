import QtQuick 2.6
import QtGraphicalEffects 1.0

/****************************************************************************
**自定义按钮功能实现
**主要实现按钮的按压效果，带有阴影效果
**
****************************************************************************/

Item {
    id : mybutton
    property bool isChoice: false
    property bool isCancel: false
    property alias color: toprect.color
    property alias text: mytext.text
    signal clicked();
    signal canceled();

    Rectangle {
        id : bgrect;
        y : 10
        x : 10
        width: mybutton.width-15
        height: mybutton.height-15
        radius: height/7
    }

    DropShadow {
        id : shadow
        anchors.fill: bgrect
        horizontalOffset: 8
        visible: isChoice ? true : false
        verticalOffset: 8
        radius: 10.0
        samples: 17
        color: "#999999"
        source: bgrect
    }

    Rectangle{
        id : toprect
        color: isChoice ? "#5CB8FC" : "white"
        width: mybutton.width;
        height: mybutton.height-2
        radius: height/7
        border.color: "#AA888888"
        border.width: isCancel ? 3 : 0
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled : true
            onCanceled: mybutton.canceled()
            onClicked: mybutton.clicked()
        }
    }

    Text {
        id: mytext
        anchors.centerIn: toprect
        color: isChoice ? "white" : "#88000000"
        font.pixelSize : 28
        onContentWidthChanged: {
            mybutton.width  = mytext.contentWidth + 20
            mybutton.height = mytext.contentHeight + 18
        }
    }
}
