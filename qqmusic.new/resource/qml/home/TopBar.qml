import QtQuick 2.0
import com.hongjing.home 1.0 as Home

Item {
    Rectangle {
        id: myStatus

        anchors.fill: parent

        color: "#DDFFFFFF"
        property alias index: left.index
        signal statusOperate(var op);

        LeftLabel {
            id: left
            width: parent.width > 300 ? 300 : parent.width / 2
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: 30
            clip: true
        }

        RightLabel {
            width: parent.width > 300 ? 300 : parent.width / 2
            height: parent.height
            anchors.right: parent.right
            anchors.rightMargin: 30
            clip: true
        }

        onStatusOperate: {
            Home.Api.qmlOperate(op)
        }
    }

//    MouseArea {
//        id: statusBarMouse
//        anchors.fill: parent

//        Component.onCompleted: {
//            Home.Api.qmlSetStatusBarMouseHandler(statusBarMouse)
//        }

//        onPressed: {
//            //console.log("Status Bar pressed.", mouseY)
//            Home.Api.statusBarState(true)
//        }
//        onReleased: {
//            //console.log("Status Bar release.", mouseY)
//            Home.Api.statusBarState(false)
//        }
//    }
}
