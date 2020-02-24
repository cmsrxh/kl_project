import QtQuick 2.0
import QtQuick.Controls 2.2

Item
{
    id: msgBox
    property int msgArg: 0
    property int boxType: 0
    property string msgContent: ""
    property string msgTitle: ""
    signal noClick()
    signal yesClick()
    signal failClick()

    Loader
    {
        anchors.fill: parent
        sourceComponent: {
            switch(boxType)
            {
                case 0: return nullEmpty;
                case 1: return generalBox;
                case 2: return msgBufferring;
                case 3: return twoBtnBox;
                case 4: return bottomTip;
                case 5: return failTip;
            }
        }
    }

    Component {
        id: nullEmpty
        Item{}
    }

    Component {
        id: generalBox
        Item{
            Rectangle
            {
                width: 400
                height: 250
                anchors.centerIn: parent
                radius: 5
                color: "#414141"
                Text
                {
                    width: parent.width
                    height: parent.height
                    font.pixelSize: 24
                    color: "white"
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                    wrapMode: Text.Wrap
                    text: msgContent
                }
            }
        }
    }

    Component {
        id: msgBufferring

        Item {
            id: bg

            Rectangle {
                id: box
                width: parent.width / 3
                height: parent.height / 3
                anchors.centerIn: parent
                radius: 5
                //visible: false
                color: "#414141"

                Image {
                    id: busy
                    anchors.bottom: text.top                    
                    anchors.horizontalCenter: parent.horizontalCenter

                    source: "qrc:/image/busy.png";

                    NumberAnimation on rotation {
                        running: busy.visible
                        from: 0; to: 360;
                        loops: Animation.Infinite;
                        duration: 1700
                    }
                }

                Text {
                    id: text
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: parent.height / 2

                    horizontalAlignment: Qt.AlignHCenter
                    //verticalAlignment: Qt.AlignVCenter
                    wrapMode: Text.Wrap

                    text: msgContent
                    color: "white"
                    font.pixelSize: 24
                }
            }
        }
    }

    Component {
        id: twoBtnBox
        Rectangle {
            color: "#81000000"
            MouseArea {
                anchors.fill: parent
                onPressed: mouse.accept = true
            }
            Rectangle {
                width: 500
                height: 300
                anchors.centerIn: parent
                radius: 5
                color: "#414141"
                Text {
                    id: btnTitle
                    width: parent.width * 0.8
                    height: parent.height * 0.2
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 28
                    color: "white"
                    verticalAlignment: Qt.AlignBottom
                    horizontalAlignment: Qt.AlignHCenter
                    text: msgTitle
                }

                Text {
                    id: btnText
                    width: parent.width * 0.8
                    height: parent.height * 0.3
                    anchors.top: btnTitle.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 24
                    color: "white"
                    verticalAlignment: Qt.AlignBottom
                    horizontalAlignment: Qt.AlignHCenter
                    wrapMode: Text.Wrap
                    text: msgContent
                }
                Button {
                    id: btnYes
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.margins: 50

                    onClicked: msgBox.noClick()
                    Text {
                        anchors.fill: parent
                        font.pixelSize: 20
                        color: "white"
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter
                        text: qsTr("确认")
                    }
                }
                Button {
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.margins: 50

                    onClicked: msgBox.noClick()

                    Text {
                        anchors.fill: parent
                        font.pixelSize: 20
                        color: "white"
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter
                        text: qsTr("取消")
                    }
                }
            }
        }
    }

    Component {
        id: bottomTip

        Item {
            Rectangle {
                id: box
                width: parent.width
                height: parent.height / 10
                anchors.bottom: parent.bottom

                // visible: false
                color: "#414141"

                Text {
                    id: text

                    anchors.fill: parent

                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Text.Wrap

                    text: msgContent
                    color: "white"
                    font.pixelSize: 24
                }
            }
        }
    }

    Component {
        id: failTip
        Item {
            Rectangle {
                id: box
                width: parent.width / 3
                height: parent.height / 3
                anchors.centerIn: parent
                radius: 5
                //visible: false
                color: "#414141"

                Text {
                    id: text

                    anchors.fill: parent

                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Text.Wrap

                    text: msgContent
                    color: "white"
                    font.pixelSize: 24
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: msgBox.failClick()
            }
        }
    }
}
