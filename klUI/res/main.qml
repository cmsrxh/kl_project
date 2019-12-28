import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQml.Models 2.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.0
import Hongjing.HMI.KL 1.0 as KL

ApplicationWindow {
    id: application
    visible: true
    width: 800
    height: 600
    title: qsTr("Hello World")
    Material.theme: Material.Light
    Material.accent: Material.DeepOrange
    Material.primary: Material.Blue

    Item {
        id: ei
        clip: true
        anchors.fill: parent
        Rectangle {
            id: bg
            anchors.fill: parent
            // color: "red"
            LinearGradient
            {
                anchors.fill: parent
                start: Qt.point(0, parent.height)
                end: Qt.point(0, 0)
                gradient: Gradient{
                    GradientStop{position: 0.0; color: "#111111"}
                    GradientStop{position: 1.0; color: "#A5A5A9"}
                }
            }
        }
        MouseArea{
            anchors.fill: parent
            enabled: ei.visible
            //Eats mouse events
        }
        Loader{
            focus: true
            anchors.fill: parent
            source: stack.source
        }
    }

    footer: BottomLabel {
        id: showLabel
        width: application.width
        height: application.height / 4
    }
}
