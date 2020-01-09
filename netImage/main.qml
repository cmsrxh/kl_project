import QtQuick 2.6
import QtQuick.Window 2.2
import ImageFrame 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log(qsTr('Clicked on background. Text: "' + textEdit.text + '"'))
        }
    }

    TextEdit {
        id: textEdit
        text: qsTr("Enter some text...")
        verticalAlignment: Text.AlignVCenter
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        Rectangle {
            anchors.fill: parent
            anchors.margins: -10
            color: "transparent"
            border.width: 1
        }
    }

    ImageFrame {
        id: image
        anchors.left: textEdit.left
        anchors.right: textEdit.right
        anchors.top: textEdit.bottom
        anchors.topMargin: 40
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 90

    }
}
