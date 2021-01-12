import QtQuick 2.7
import com.hongjing.home 1.0 as Sound
import "../home"

Item {
    id: statusBar
    // objectName: "statusBar"
    // 表示当前状态栏是否需要显示出来,如果需要就告诉上层,鼠标 不要关闭这个窗口
    property alias showHeight: statusView.height
    property int animationDuration: 100

//    Component.onDestruction: {
//        console.log("status bar Destruction.")
//        Sound.Api.qmlSetStatusViewMouseHandler(null);
//    }

//    Component.onCompleted: {
//        console.log("status bar complete.")
//        Sound.Api.qmlSetStatusViewMouseHandler(mouseProc);
//    }
    onVisibleChanged: {
        if (visible)
        {
            Sound.Api.qmlSetStatusViewMouseHandler(mouseProc);
        } else
        {
            Sound.Api.qmlSetStatusViewMouseHandler(null);
        }
    }

    Rectangle {
        id: backLab
        objectName: "backLab"
        anchors.fill: parent
        color: "black"
        opacity: 0.0
    }

    ParallelAnimation {
        id: viewExit
        objectName: "viewExit"
        running: false

        OpacityAnimator { target: backLab; from: backLab.opacity; to: 0; duration: animationDuration }

        NumberAnimation { target: statusView; property: "height"; to: 0; duration: animationDuration }
        onStopped: {
            statusBar.visible = false
            console.log("status view close")
        }
    }

    ParallelAnimation {
        id: viewEnter
        objectName: "viewEnter"
        running: false

        OpacityAnimator { target: backLab; from: backLab.opacity; to: 0.83; duration: animationDuration }

        NumberAnimation { target: statusView; property: "height"; to: statusBar.height; duration: animationDuration }
    }

    MouseArea {
        id: mouseProc
        property int current: 0
        property int prevY: 0
        anchors.fill: parent
        onPressed: {
            current = mouseY
            //console.log("press: ", mouseY, prevY, statusView.height, statusBar.height)
        }
        onReleased: {
            var tmpY = parseInt(mouseY);
            //console.log("release: ", mouseY, tmpY, prevY, statusView.height, statusBar.height)
            if (statusView.height == statusBar.height || 0 == statusView.height) return;

            // 最后的动作是 向下滑动
            if (tmpY > prevY)
            {
                viewEnter.running = true
            } else if (tmpY < prevY) // 最后的动作是 向上滑动
            {
                viewExit.running = true
            } else
            {
                viewExit.running = true
            }
        }

        onMouseYChanged: {
            var tmpY = parseInt(mouseY);
            // 向下滑动
            if (tmpY > current)
            {
                var tmp = statusView.height + 5
                if (tmp > statusBar.height)
                {
                    tmp = statusBar.height
                }

                statusView.height = tmp;
            } else if (tmpY < current) // 向上滑动
            {
                var tmp1 = statusView.height - 5
                if (tmp1 < 0)
                {
                    tmp1 = 0
                }

                statusView.height = tmp1;
            }
            prevY   = current
            current = tmpY;

            //console.log("onMouseYChanged: ", mouseY, tmpY, prevY, statusView.height, current)
        }
    }

    DropDownBgCtrl {
        id: statusView

        width: parent.width
        height: 0
        clip: true

        onViewClose: {
            viewExit.running = true
        }
    }
}
