import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    Item {
        id: labPop
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 30
        anchors.top: parent.top
        anchors.bottom: parent.bottom


        Flow {
            id: flowlab
            width: parent.width
            height: 100

            spacing: 10

            Repeater {
                model: textModel
                Button {
                    id: btn
                    width: labPop.width / 5
                    text: tipText
                    font.pixelSize: 20

                    onClicked: {
                        console.log("setting click", index)
                    }
                }
            }
        }

        ListModel {
            id: textModel
            ListElement {
                tipText: qsTr("点击登录")
                image: ""
            }
            ListElement {
                tipText: qsTr("播放音质")
                image: ""
            }
            ListElement {
                tipText: qsTr("清除缓存")
                image: ""
            }
            ListElement {
                tipText: qsTr("检测更新")
                image: ""
            }
            ListElement {
                tipText: qsTr("下载手机端")
                image: ""
            }
            ListElement {
                tipText: qsTr("关于我们")
                image: ""
            }
            ListElement {
                tipText: qsTr("意见反馈")
                image: ""
            }

        }
        Rectangle {
            anchors.fill: flowlab
            border.color: "green"
            border.width: 2
            color: "transparent"
        }
    }
}
