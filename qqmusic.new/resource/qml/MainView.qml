import QtQuick 2.0
import com.hongjing.pagesManager 1.0 as ActivityManager

import "home"

Item {
    /*背景*/
    Image {
        id: imgBg
        asynchronous: true

        visible: ActivityManager.Api.backgroundEnable
        //source: "qrc:/image/public/bg_1920x720.png"
    }

    /*加载功能界面*/
    ViewManager {
        id: content
        anchors.fill: parent
    }

    /*加载全局界面*/
    Global {
        id: global
        anchors.fill: parent
    }
}
