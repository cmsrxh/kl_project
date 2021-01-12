import QtQuick 2.0

// 下拉面板在MouseArea控件up面的控件
Item {    
    id: bgPanel
    signal viewClose()

    Rectangle {
        width: 400
        height: 340

        anchors.centerIn: parent

        color: "red"
    }
}
