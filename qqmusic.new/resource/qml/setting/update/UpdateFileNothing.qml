import QtQuick 2.0
import com.hongjing.layers 1.0 as LM

UpdateBoxBase {
    handle: Text {
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter

        font.pixelSize: 32
        wrapMode: Text.WrapAnywhere
        color: "white"
        text: qsTr("未检测到升级文件!")
    }

    Timer {
        repeat: parent.visible
        running: parent.visible
        interval: 3000 // 3s
        onTriggered: {
            console.log("close dialog.")
            LM.Api.btnUserClickedClose()
        }
    }
}
