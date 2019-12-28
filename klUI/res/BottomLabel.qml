import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import Hongjing.HMI.KL 1.0 as KL

Item {
    Connections {
        target: KL.Controller
        onPlayingInfo: {
            nameInfo.text = name;
            currDesc.text = desc;
        }
    }

    RowLayout{
        anchors.fill: parent
        spacing: 0

        Rectangle
        {  //底部Left显示栏
            id: bottomBarItem

            Layout.fillWidth: true
            Layout.fillHeight: true

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

            ColumnLayout {
                anchors.fill: parent
                Connections {
                    target: KL.Controller
                    onDurationChanged:
                    {
                        slider.to = duration;
                    }
                }

                Timer {
                    repeat: true
                    interval: 1000
                    running: (1 === KL.Controller.playState) ? true : false

                    onTriggered:
                    {
                        slider.value = KL.Controller.qmlGetCurrentPosition();
                    }
                }

                Slider //进度条
                {
                    id: slider
                    Layout.fillWidth: true
                    height: 30
                    anchors.horizontalCenter: parent.horizontalCenter
                    live: false
                    from: 0
                    value: 0
                    to: 100

                    onMoved: {
                        console.log("postion: ", position, value, stepSize)
                    }
                }

                RowLayout //三个播放控制按钮
                {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Button
                    {
                        visible: stack.isShowReturn
                        text: qsTr("返回")
                        onClicked: {
                            stack.pop();
                        }
                    }

                    Button
                    {
                        text: qsTr("上一曲")
                        onClicked: {
                            KL.Controller.qmlPlayPrev();
                        }
                    }

                    Button
                    {
                        id: btnPlay
                        text: {
                            switch(KL.Controller.playState)
                            {
                            case 1: return qsTr("播放")
                            case 2: return qsTr("暂停")
                            default: return qsTr("停止")
                            }
                        }
                        onClicked: {
                            KL.Controller.qmlPlayPause();
                        }
                    }
                    Button
                    {
                        text: qsTr("下一曲")
                        onClicked: {
                            KL.Controller.qmlPlayNext();
                        }
                    }

                }

            }
        }

        Rectangle { //Right           
            Layout.fillWidth: true
            Layout.fillHeight: true
            LinearGradient
            {
                anchors.fill: parent
                start: Qt.point(0, parent.height)
                end: Qt.point(0, 0)
                gradient: Gradient{
                    GradientStop{position: 0.0; color: "#111111"}
                    GradientStop{position: 1.0; color: "#C5A5A9"}
                }
            }

            Column {
                width: parent.width - 80
                height: parent.height
                anchors.left: parent.left
                anchors.leftMargin: 40
                spacing: 0
                Text
                {
                    id: nameInfo
                    width: parent.width
                    height: parent.height/3
                    verticalAlignment: Qt.AlignVCenter
                    color: "#AAAAAA"
                    font.pixelSize: 18
                    clip: true
                    elide: Text.ElideRight
                }
                Flickable {
                    id: flick
                    width: parent.width
                    height: parent.height *2/3
                    contentHeight: currDesc.height
                    clip: true

                    Text
                    {
                        id: currDesc
                        width: parent.width - 20;
                        verticalAlignment: Qt.AlignVCenter
                        color: "white"
                        font.pixelSize: 16
                        wrapMode: Text.Wrap
                        clip: true
                        //elide: Text.ElideRight
                    }
                }
            }
        }
    }
}
