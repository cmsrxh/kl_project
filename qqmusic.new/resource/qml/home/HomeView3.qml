import QtQuick 2.0

Item {
    Rectangle {
        width: 400
        height: 300

        anchors.centerIn: parent

        Text {
            anchors.centerIn: parent
            font.pixelSize: 32
            color: "green"

            text: qsTr("This is Home Page Three.")
        }
    }
}
