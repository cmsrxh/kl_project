import QtQuick 2.0
import com.hongjing.layers 1.0 as LM
import "../../public/controls"

UpdateBoxBase {
    handle: Item {
        Column {
            width: parent.width
            height: 420
            anchors.top: parent.top
            anchors.topMargin: 90

            spacing: 20

            Item {
                width: parent.width
                height: 80
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 32
                    color: "white"
                    text: qsTr("校验失败")
                }
            }

            Text {
                width: parent.height * 0.9
                height: 100
                anchors.horizontalCenter: parent.horizontalCenter

                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter

                font.pixelSize: 24
                wrapMode: Text.WrapAnywhere
                color: "white"
                text: LM.Api.getErrorReason()
            }
        }

        CustomBtn {
            width: 246
            height: 72

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter

            btnImgSrc: "qrc:/image/public/setings_btn.png"
            btnImgSrc_hl: "qrc:/image/public/setings_btn_hl.png"

            label.text: qsTr("关闭")
            label.font.pixelSize: 30

            label.topPadding: -12
            onBtnClicked: {
                LM.Api.btnUserClickedClose()
            }
        }
    }
}
