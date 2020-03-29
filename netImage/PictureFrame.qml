import QtQuick 2.0
import ImageFrame 1.0
import QtGraphicalEffects 1.0

Item {
    property alias source: img.source

    //         Image {
    //             id: img
    //             cache: false
    //             visible: false
    //             smooth: true
    //             anchors.fill: parent
    //             sourceSize: Qt.size(parent.width - 10, parent.height - 10)
    //             source: "http://img.kaolafm.net/mz/images/201906/eb01b107-d009-43be-a4bb-dbebc8496082/default.jpg"
    //         }
    ImageFrame {
        id: img
        visible: false
        anchors.fill: parent
    }

    Rectangle {
        id: imgMask
        visible: false
        smooth: true
        anchors.fill: parent
        radius: width / 2
    }

    //使用QML的覆盖属性，将图片显示成为maskSource的形状
    OpacityMask {
        id: opMask
        anchors.fill: img
        source: img
        maskSource: imgMask
    }
}
