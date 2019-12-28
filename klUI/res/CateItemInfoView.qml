import QtQuick 2.0
import QtQuick.Controls 2.0
import Hongjing.HMI.KL 1.0 as KL

Item {
    Column
    {
        id: topLab
        width: parent.width / 4
        height: parent.height
        spacing: 12
        Text {
            id: titleText
            width: parent.width - 20
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignLeft
            color: "white"
            elide: Text.ElideRight
            font.pixelSize: 20
            text: "TITLE"
        }

        ImageFrame
        {
            id: titleImg
            width: titleText.width
            height: titleText.width
            anchors.horizontalCenter: parent.horizontalCenter

            imageUri: ""//KaoLa.Controller.albumPicture
        }
        Row {
            id: hostLab
            width: titleText.width
            height: 40

            anchors.horizontalCenter: parent.horizontalCenter

            Text
            {
                id: anchorLab
                height: parent.height

                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
                text: qsTr("主播：")
                color: "white"
                font.pixelSize: 20
            }
            Text
            {
                id: anchorText
                height: parent.height

                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
                text: "Host Name"
                color: "white"
                elide: Text.ElideRight
                font.pixelSize: 20
            }
        }
        Flow
        {
            id: centreBar
            width: titleText.width
            anchors.horizontalCenter: parent.horizontalCenter
//            height: 233
            clip: true
            spacing: 8

            ListModel
            {
                id: keyWord
                ListElement {
                    name: "a"
                }
                ListElement {
                    name: "b"
                }
                ListElement {
                    name: "c"
                }
                ListElement {
                    name: "d"
                }
            }

            Repeater
            {
                model: keyWord
                anchors.fill: parent
                property int ypos: 5
                property int xpos: 5
                Rectangle
                {
                    width: featherArea.width + 30
                    height: 25
                    radius: height / 2
                    border.width: 1
                    border.color: "#F1C17D"
                    color: "transparent"
                    Text
                    {
                        id: featherArea
                        anchors.centerIn: parent
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter
                        text: name
                        color: "white"
                        font.pixelSize: 18
                    }
                }

            }
        }
    }

    AudioList
    {
        id: bottomBar
        anchors.left: topLab.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }

}

