import QtQuick 2.0

Item {
    Item {
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 30
        anchors.top: parent.top
        anchors.bottom: parent.bottom


        Item {
            id: topBarLab
            anchors.left: parent.left
            anchors.right: parent.right
            height: 60

            Text
            {
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
                text: qsTr("你可以这样问语音助手")
                color: "white"
                font.pixelSize: 20
            }
        }

        Column
        {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: topBarLab.bottom
            anchors.bottom: parent.bottom
            spacing: 10
            Repeater {
                model: textModel

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                    text: tipText
                    color: "white"
                    font.pixelSize: 20
                }
            }
        }

        ListModel {
            id: textModel
            ListElement {
                tipText: qsTr("我要听二货一箩筐")
            }
            ListElement {
                tipText: qsTr("我想听郭德纲的相声")
            }
            ListElement {
                tipText: qsTr("我要听脱口秀节目")
            }
            ListElement {
                tipText: qsTr("播放北京交通广播")
            }
            ListElement {
                tipText: qsTr("播放新闻电台")
            }
            ListElement {
                tipText: qsTr("上一首")
            }
            ListElement {
                tipText: qsTr("下一首")
            }
            ListElement {
                tipText: qsTr("继续播放")
            }
            ListElement {
                tipText: qsTr("暂停一下")
            }
        }
    }
}
