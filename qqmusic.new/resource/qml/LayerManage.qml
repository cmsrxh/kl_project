import QtQuick 2.0
import com.hongjing.globalcontroller 1.0 as GC
import com.hongjing.layers 1.0 as LM
import com.hongjing.home 1.0 as Home

import "home"

/* 主页面之上的弹框,层级管理 */
Item {

    Connections {
        target: Home.Api
        onStatusBarState: {
            //console.log("isPress: ", isPressed, statusView.item)
            if (isPressed)
            {
                statusView.visible = true
                statusView.item.visible = true;
            } else
            {
                if (statusView.item && 0 === statusView.item.showHeight)
                {
                    statusView.visible = false;
                    statusView.item.visible = false;
                }
                //console.log("status down view visible, ", statusView.visible, LM.Api.statusViewLoad)
            }
        }
    }

    // 关背光进入power off界面
    Loader {
        anchors.fill: parent
        active: LM.Api.screenOffLoad
        asynchronous: true
        //visible: GC.Api.screenOffState
        source: "qrc:/qml/global/ScreenOff.qml"
    }

    // 待机时钟界面
    Loader {
        anchors.fill: parent
        active: LM.Api.clockLoad
        asynchronous: true
        //visible: Home.Api.powerMode
        source: "qrc:/qml/global/StandbyWindow.qml"
    }

    // 状态栏下拉列表
    Loader {
        id: statusView
        anchors.fill: parent
        active: LM.Api.statusViewLoad
        asynchronous: true
        visible: false
        source: "qrc:/qml/home/DropDownDock.qml"
        // 注意不要使用它(sourceComponent),它会在UI加载的时候构造 DropDownDock{}
    }

    // 系统消息弹框
    Loader {
        anchors.fill: parent
        active: LM.Api.sysMessage
        asynchronous: false
        visible: true
        source: "qrc:/qml/global/GlobalMsgLoader.qml"
    }

    // 蓝牙电话界面
    Loader {
        id: btCall
        anchors.fill: parent
        active: false
        source: "qrc:/qml/bluetooth/btCall/BtPhoneCall.qml"
    }

    // 倒车界面
    Loader {
        id: reverse
        anchors.fill: parent
        active: false
        source: "qrc:/qml/camera/CameraWindow.qml"
    }
}
