import QtQuick 2.0

Rectangle
{
    id: myStatus
    color: "#DDFFFFFF"
    property alias index: left.index
    signal statusOperate(var op);

    LeftLabel {
        id: left
        width: parent.width > 300 ? 300 : parent.width / 2
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: 30
        clip: true
    }

    RightLabel {
        width: parent.width > 300 ? 300 : parent.width / 2
        height: parent.height
        anchors.right: parent.right
        anchors.rightMargin: 30
        clip: true
    }
}
