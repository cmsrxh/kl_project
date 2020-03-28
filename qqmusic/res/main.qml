import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.0
import QtQuick.Window 2.2
import Hongjing.HMI.KL 1.0 as KL

ApplicationWindow {
    id: application
    visible: true
    width: 800
    height: 600
    title: qsTr("KaoLa FM")
    Material.theme: Material.Light
    Material.accent: Material.DeepOrange
    Material.primary: Material.Blue
    Rectangle {
        id: bg
        anchors.fill: parent

        LinearGradient {
            anchors.fill: parent
            start: Qt.point(0, parent.height)
            end: Qt.point(0, 0)
            gradient: Gradient{
                GradientStop{position: 0.0; color: "#111111"}
                GradientStop{position: 1.0; color: "#C5A5A9"}
            }
        }
    }
    TabBar {
        id: topbar
        anchors.horizontalCenter: parent.horizontalCenter
        height: 40
        currentIndex: 0
        onCurrentIndexChanged: KL.Controller.qmlMainTabClick(currentIndex)

        TabButton {
            text: qsTr("set")
            width: 100
        }
        TabButton {
            text: qsTr("我的")
            width: 100
        }
    }
    Item {
        id: ei
        clip: true
        width: parent.width
        anchors.top: topbar.bottom
        anchors.bottom: showLabel.top

        Loader{
            focus: true
            anchors.fill: parent
            //source: "qrc:/self/KlDlgOptionView.qml"
            source: stack.source
        }
    }

//    BottomLabel {
//        id: showLabel
//        anchors.bottom: parent.bottom
//        width: application.width
//        height: application.height / 4

//        onPlayListShow: playListLab.visible = true
//    }

//    CurrentPlay{
//        id: playListLab
//        visible: false
//        anchors.fill: parent
//    }

    Connections {
        target: KL.Controller
        onMsgTipGlobal: {
            msgBox.boxType    = boxType
            msgBox.msgContent = msgContent
        }
    }

    KlMsgTipBox {
        id: msgBox
        anchors.fill: parent
        onFailClick: {
            KL.Controller.qmlReloadErrObject()
        }
    }
}
