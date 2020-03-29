import QtQuick 2.0

Item {
    id: ctrlLab
    property int index: -2
    Row {
        id: topbar
        anchors.fill: parent
        spacing: 12
        layoutDirection: Qt.RightToLeft

        Repeater {
            anchors.fill: parent
            model: listModel
            Item {
                id: lab
                y: 15
                width: 40
                height: 50

                Image {
                    id: img
                    x: 15
                    y: 5
                    width: 15
                    height: 15
                    smooth: true
                    fillMode: Image.PreserveAspectFit
                    source: imagePath
                }
                Rectangle {
                    anchors.fill: img
                    anchors.margins: -10
                    color: "transparent"
                    visible:  ctrlLab.index == index ? true : false
                    radius: 5
                    border.color: "#AA888888"
                    border.width: 3
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        ctrlLab.index = index
                        ctrlLab.parent.statusOperate(name)
                    }
                }
            }
        }
    }

    ListModel {
        id: listModel
        ListElement {
            name: "close"
            imagePath: "qrc:/image/svg/close.svg"
        }
        ListElement {
            name: "minimize"
            imagePath: "qrc:/image/svg/minimize.svg"
        }
        ListElement {
            name: "up"
            imagePath: "qrc:/image/svg/Up.svg" //alt="返回顶部
        }
        ListElement {
            name: "forward"
            imagePath: "qrc:/image/svg/Forward.svg" // alt="前进"
        }
        ListElement {
            name: "back"
            imagePath: "qrc:/image/svg/Back.svg" // alt="后退"
        }
    }
}
