import QtQuick 2.0
import ImageFrame 1.0
import QtGraphicalEffects 1.0

Item {
    id: picture
    property alias source: img.source

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
