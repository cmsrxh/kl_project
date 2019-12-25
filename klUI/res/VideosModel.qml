import QtQuick 2.0
import QtGraphicalEffects 1.0

Item
{
    id: videoItem
    width: cateItemView.cellWidth
    height: cateItemView.cellHeight

    property bool playing: false
    signal clicked()

    Rectangle
    {
        id: btn

        width: parent.width * 0.85
        height: parent.height * 0.65

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10

        border.color: playing ? "#FFF1C17D" : "#61F1C17D"
        border.width: playing ? 9 : 5

        radius: 5
        color: "gray"

        Image
        {
            id: fgImg
            x: parent.border.width
            y: parent.border.width
            width: parent.width - parent.border.width * 2
            height: parent.height - parent.border.width * 2
            //sourceSize: Qt.size(parent.width - parent.border.width * 2, parent.height - parent.border.width * 2)
            source: imageUri
            asynchronous: true
            cache: true
            fillMode: Image.PreserveAspectCrop
            antialiasing: true

        }
//        LinearGradient
//        {
//            anchors.fill: fgImg
//            source: fgImg
//            start: Qt.point(0, fgImg.height)
//            end: Qt.point(0, 0)
//            gradient: Gradient{
//                GradientStop{position: 0.3; color: "#D0000000"}
//                GradientStop{position: 1.0; color: "#AAAAAAAA"}
//            }
//        }

        MouseArea {
            anchors.fill: parent
            onClicked: videoItem.clicked();
        }
    }

    Text
    {
        id: compText
        width: parent.width * 9/10
        height: 24
        anchors.top: btn.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        verticalAlignment: Qt.AlignVCenter
        //horizontalAlignment: Qt.AlignHCenter
        text: cateItemName
        color: "#B1C1C1C1"
        elide: Text.ElideRight
        font.pixelSize: 20
    }

}
