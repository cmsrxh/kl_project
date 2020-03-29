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
    property alias isshow: shadow.visible
    property alias color: toprect.color
    property alias text: mytext.text
    signal clicked();

    Rectangle {
        id : bgrect;
        y : 20
        x : 20
        width: mybutton.width-25;
        height: mybutton.height-25
        radius: height/7
    }

    DropShadow {
        id : shadow
        anchors.fill: bgrect
        horizontalOffset: 8

        verticalOffset: 8
        radius: 14.0
        samples: 17
        color: "#999999"
        source: bgrect
    }

    Rectangle{
        id : toprect
        color: "#5CB8FC"
        width: mybutton.width;
        height: mybutton.height-2
        radius: height/7

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled : true
            onPressed: {
                btnPress.start()
            }
            onCanceled: {
                if (btnPress.running)
                {
                    btnPress.stop()
                }
                btnRelease.start()
            }
            onReleased: {
                if (btnPress.running)
                {
                    btnPress.stop()
                }
                btnRelease.start()
            }            
        }
    }

    Text {
        id: mytext
        anchors.centerIn: toprect
        text: qsTr("text")
        color: "#ffffff"
        font.pixelSize : 28
        onContentWidthChanged: {
            mybutton.width  = mytext.contentWidth + 20
            mybutton.height = mytext.contentHeight + 18
        }
    }

    NumberAnimation {
        id: btnPress
        target: toprect; property: "y"; to: toprect.x+2; duration: 100
    }
    NumberAnimation {
        id: btnRelease
        target: toprect; property: "y"; to: toprect.x-2; duration: 100
        onStopped: {
            mybutton.clicked();
        }
    }
}
