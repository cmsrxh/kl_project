import QtQuick 2.0
import com.hongjing.home 1.0 as Home

Item {
    Rectangle {
        anchors.fill: parent
        color: "green"
    }

    MouseArea {
        id: statusBarMouse
        anchors.fill: parent

        Component.onCompleted: {
            Home.Api.qmlSetStatusBarMouseHandler(statusBarMouse)
        }

        onPressed: {
            //console.log("Status Bar pressed.", mouseY)
            Home.Api.statusBarState(true)
        }
        onReleased: {
            //console.log("Status Bar release.", mouseY)
            Home.Api.statusBarState(false)
        }
    }
}
