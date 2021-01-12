import QtQuick 2.0
import com.hongjing.layers 1.0 as LM

Item {
    id: root
    width: 1024
    height: 600

    property alias needClose : closeBtn.visible

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.6
    }

    property Component handle : null
    property Component background: Rectangle {
        width: 600
        height: 400
        radius: 20

        color: "#41313131"
    }

        /*Image {
        width: 864
        height: 648
        source: "qrc:/image/public/bg_864x648.png"
    }*/

    Loader {
        id: backgroundId
        anchors.centerIn: parent
        sourceComponent: root.background
    }

    Loader {
        id: msgBox

        width: 600
        height: 400

        anchors.centerIn: parent
        sourceComponent: root.handle


        ButtonPress{
            id: closeBtn
            anchors.top: parent.top
            anchors.topMargin: 30

            anchors.right: parent.right
            anchors.rightMargin: 20

            width: 72; height: 72
            visible: false

            btnImgUrl: pressed ? "qrc:/image/public/deleteBtn_hl.png"
                               : "qrc:/image/public/deleteBtn.png"

            onClicked: {
                LM.Api.btnUserClickedClose();
            }
        }
    } 
}
