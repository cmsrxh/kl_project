import QtQuick 2.0
import com.hongjing.globalcontroller 1.0 as GC
import com.hongjing.home 1.0 as Home

Item {
    /*顶部状态栏*/
    TopBar {
        id: topBar
        x: 0; y: 0; z: 2
        width: parent.width
        height: 72

        visible: Home.Api.topBarVisible
    }

    /*左侧停靠栏*/
//    DockBar{
//        id: dockBarId
//        x: 0; y:0
//        width: 60
//        height: parent.height

//        visible: Home.Api.dockBarVisible
//    }

}
