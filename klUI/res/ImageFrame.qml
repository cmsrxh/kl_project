import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle
{
    id: btn

    property alias imageUri: fgImg.source

    border.color:  "#e1ff0000"
    border.width: 3

    Image
    {
        id: fgImg
        x: parent.border.width
        y: parent.border.width
        width: parent.width - parent.border.width * 2
        height: parent.height - parent.border.width * 2
        //sourceSize: Qt.size(parent.width - parent.border.width * 2, parent.height - parent.border.width * 2)
        asynchronous: true
        cache: true
        fillMode: Image.PreserveAspectCrop
        antialiasing: true
    }

    //    LinearGradient
    //    {
    //        anchors.fill: fgImg
    //        source: fgImg
    //        start: Qt.point(0, fgImg.height)
    //        end: Qt.point(0, 0)
    //        gradient: Gradient{
    //            GradientStop{position: 0.3; color: "#D0000000"}
    //            GradientStop{position: 1.0; color: "#AAAAAAAA"}
    //        }
    //    }
}
