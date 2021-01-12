import QtQuick 2.0
import com.hongjing.layers 1.0 as LM
import "../../public/controls"

UpdateBoxBase {
    needClose: true
    handle: Item {

        Item {
            id: root
            width: parent.width / 6
            height: parent.height / 4

            anchors.centerIn: parent

            Repeater{
                model: 5
                id: repeater
                Rectangle {
                    property real dx: root.width / 2 //圆心坐标
                    property real dy: root.height / 2
                    property real cx: radius * Math.sin(percent * 6.283185307179) + dx //各个圆点的实时坐标
                    property real cy: radius * Math.cos(percent * 6.283185307179) + dy
                    property real percent: 1

                    id: dot
                    width: root.width / repeater.count - 1
                    height: width
                    radius: root.width / 2
                    color: "white"
                    opacity: 0
                    x: cx
                    y: cy

                    SequentialAnimation on percent {

                        loops: Animation.Infinite
                        running: root.visible

                        PauseAnimation { duration: 200 * index }

                        ParallelAnimation {
                            NumberAnimation { target: dot; property: "opacity";from: 0;to: 1; duration: 200}
                            NumberAnimation { duration: 200; from: 1 + index * 0.05; to: 0.75}
                        }

                        NumberAnimation { duration: 60; from: 0.75; to: 0.7}
                        NumberAnimation { duration: 140; from: 0.7; to: 0.65}
                        NumberAnimation { duration: 160; from: 0.65; to: 0.6}

                        NumberAnimation { duration: 200; from: 0.6;  to: 0.55}
                        NumberAnimation { duration: 200; from: 0.55;  to: 0.5}

                        NumberAnimation { duration: 200; from: 0.5;  to: 0.45}
                        NumberAnimation { duration: 100; from: 0.45;  to: 0.4}
                        NumberAnimation { duration: 50; from: 0.4;  to: 0.35}
                        NumberAnimation { duration: 30; from: 0.35;  to: 0.3}
                        NumberAnimation { duration: 180; from: 0.3;  to: 0}
                        NumberAnimation { duration: 200; from: 1;    to: 0.75}

                        NumberAnimation { duration: 60; from: 0.75; to: 0.7}
                        NumberAnimation { duration: 140; from: 0.7; to: 0.65}
                        NumberAnimation { duration: 160; from: 0.65; to: 0.6}

                        NumberAnimation { duration: 200; from: 0.6;  to: 0.55}
                        NumberAnimation { duration: 200; from: 0.55;  to: 0.5}
                        NumberAnimation { duration: 200; from: 0.5;  to: 0.45}
                        NumberAnimation { duration: 100; from: 0.45;  to: 0.4}
                        NumberAnimation { duration: 50; from: 0.4;  to: 0.35}
                        NumberAnimation { duration: 30; from: 0.35;  to: 0.3}
                        NumberAnimation { duration: 180; from: 0.3;  to: 0}
                        NumberAnimation { target: dot; duration: 50; property: "opacity"; from: 1; to: 0}

                        PauseAnimation { duration: (repeater.count - index - 1) * 200}
                    }
                }
            }
        }
    }
}
