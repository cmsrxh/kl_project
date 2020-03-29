import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.0
import QtQuick.Window 2.2
import Hongjing.HMI.KL 1.0 as KL
import "./status"
import "./singerList"

Window {
    id: application
    visible: true
    width:  1500
    height: width * 0.618
    flags: Qt.FramelessWindowHint

    SingerList {
        z: 0
        anchors.fill: parent

    }

    StatusBar {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 80
        z: 1

        onIndexChanged: {
            KL.Controller.qmlMainTabClick(index)
        }

        onStatusOperate: {
            if (op === "close")
            {
                console.warn("Window Close !")
                application.close()
            } else if (op === "minimize")
            {
                console.warn("Window minimize !")
                application.visibility = Window.Minimized
            }
            console.log(op)
        }
    }


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
