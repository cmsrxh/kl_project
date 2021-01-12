import QtQuick 2.0

import com.hongjing.pagesManager 1.0 as ActivityManager

Item {
    width: 1500
    height: 1500 * 0.618

    Loader {
        anchors.fill: parent
        active: ActivityManager.Api.mainViewEnable
        source: "MainView.qml"
    }

    LayerManage {
        id: layers
        anchors.fill: parent
    }
}
