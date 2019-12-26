import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import Hongjing.HMI.KL 1.0 as KL

Item {

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
            color: "blue"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
