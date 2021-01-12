import QtQuick 2.0

Rectangle {
    id: btn

    signal clicked()
    property bool pressed: false
    property alias text: btnTitle.text
    property alias pixelSize: btnTitle.font.pixelSize

    radius: 5
    color: pressed ? "#818181" : "#414141"

    Text {
        id: btnTitle
        anchors.fill: parent
        font.pixelSize: 20
        color: "white"
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter
    }

    MouseArea
    {
        anchors.fill: parent
        onPressed: {
            btn.pressed = true
        }
        onCanceled: {
            btn.pressed = false
        }
        onReleased: {
            btn.pressed = false
        }
        onClicked: {
            btn.clicked();
        }
    }
}
