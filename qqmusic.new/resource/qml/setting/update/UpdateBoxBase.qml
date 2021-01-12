import QtQuick 2.0
import com.hongjing.layers 1.0 as LM
import "../../public/controls"

Item {
    id: root
    width: 1920
    height: 720

    property alias needClose : closeBtn.visible

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.6
    }

    property Component handle : null
    property Component background: Image {
        width: 864
        height: 648
        source: "qrc:/image/public/bg_864x648.png"
    }

    /*
    Rectangle {
        width: 864
        height: 648
        radius: 20

        color: "#616161"
    }*/

    Loader {
        id: backgroundId
        anchors.centerIn: parent
        sourceComponent: root.background
    }

    Loader {
        id: msgBox

        width: 864
        height: 648

        anchors.centerIn: parent
        sourceComponent: root.handle


        CustomBtn{
            id: closeBtn
            anchors.top: parent.top
            anchors.topMargin: 30

            anchors.right: parent.right
            anchors.rightMargin: 20

            width: 72; height: 72
            visible: false

            btnImgSrc: "qrc:/image/public/deleteBtn.png"
            btnImgSrc_hl: "qrc:/image/public/deleteBtn_hl.png"
            onBtnClicked: {
                LM.Api.btnUserClickedClose();
            }
        }
    } 
}
