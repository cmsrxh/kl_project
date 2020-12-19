import QtQuick 2.0
import HongJing.HMI.Media 1.0 as Media

Item
{
    id: msgBox
    property int msgArg: 0
    property int boxType: 0
    property string msgContent: ""
    property string msgTitle: ""
    MouseArea{
        anchors.fill: parent
        onPressed: {
            mouse.accepted = true;
        }
    }
    Loader
    {
        anchors.centerIn: parent
        sourceComponent: {
            switch(boxType)
            {
                case 0: return nullEmpty;
                case 1: return generalBox;
                case 2: return msgBufferring;
                case 3: return twoBtnBox;
            }
        }
    }

    Component {
        id: nullEmpty
        Item{}
    }

    Component {
        id: generalBox
        Image{
            source: "qrc:/images/multimedia/notice_bg.png"

            Text
            {
                anchors.fill: parent
                anchors.margins: 20
                font.pixelSize: 24
                color: "white"
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
                wrapMode: Text.Wrap
                text: msgContent
            }
        }
    }

    Component {
        id: msgBufferring

        Item
        {
            id: box
            width: 462
            height: 258
            Image
            {
                id: busy
                anchors.top: box.top
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter

                source: "qrc:/images/multimedia/busy.png";

                NumberAnimation on rotation {
                    running: true
                    from: 0; to: 360;
                    loops: Animation.Infinite;
                    duration: 1700
                }
            }

            Text
            {
                id: text

                anchors.left: box.left
                anchors.leftMargin: 40

                anchors.right: box.right
                anchors.rightMargin: 40

                anchors.top: busy.bottom
                anchors.bottom: parent.bottom

                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignTop

                wrapMode: Text.Wrap

                text: msgContent
                color: "white"
                font.pixelSize: 18
            }
        }

    }

    Component {
        id: twoBtnBox
        Rectangle
        {
            color: "#81000000"
            MouseArea
            {
                anchors.fill: parent
                onPressed: mouse.accept = true
            }
            Rectangle
            {
                width: 500
                height: 300
                anchors.centerIn: parent
                radius: 5
                color: "#414141"
                Text
                {
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

                Text
                {
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
                ButtonPress
                {
                    id: btnYes
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.margins: 50

                    onClicked: Media.Controller.qmlBtnYesPress(msgArg)
                    Text
                    {
                        anchors.fill: parent
                        font.pixelSize: 20
                        color: "white"
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter
                        text: qsTr("确认")
                    }
                }
                ButtonPress
                {
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.margins: 50

                    onClicked: Media.Controller.qmlBtnNoPress(msgArg)

                    Text
                    {
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

}
