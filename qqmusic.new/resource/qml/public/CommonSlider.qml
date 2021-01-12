import QtQuick 2.0

Item {
    id:slider

    property int maxValue: 1
    property int currentValue: 0

    property int fontSize: 20
    property int textAlign: 10
    property bool canSeek: true
    property int textLength: 80 //parent.width * 0.14

    property alias sliderLen: slot.width
    property alias imgbg: slot.source
    property alias imgfg: slot_hl.source
    property alias imgPoint: point.source

    signal seekValue(var value)

    Image {
        id:slot
        width: parent.width - textLength * 2.3 //Image.width
        height: Image.height
        anchors.centerIn: parent
    }

    BorderImage {
        id:slot_hl
        anchors.left: slot.left
        anchors.verticalCenter: slot.verticalCenter
        border.left: 10
        border.right: 1
        width: slider.currentValue * slot.width / slider.maxValue
    }
    Item {
        anchors.left: slot.left
        anchors.leftMargin: -5
        anchors.right: slot.right
        anchors.rightMargin: 5
        anchors.top: slot.top
        anchors.bottom: slot.bottom
        //clip: true

        Image {
            id: point
            x: slider.currentValue * parent.width / slider.maxValue
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        anchors.left: slot.left
        anchors.right: slot.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        propagateComposedEvents: true

        onMouseXChanged: {
            if(mouseX < 0) {
                slider.currentValue = 0;
            } else if(mouseX > width) {
                slider.currentValue = slider.maxValue;
            } else {
                slider.currentValue = mouseX*slider.maxValue/width;
            }

            slider.seekValue(slider.currentValue);
        }
    }


    Text
    {
        id: leftTime
        width: textLength
        height: parent.height
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignRight

        anchors.right: slot.left
        anchors.rightMargin: slider.textAlign

        text: Qt.formatTime(new Date(0, 0, 0, 0, parseInt(slider.currentValue/60), slider.currentValue%60)
                            , qsTr("hh:mm:ss"))
        color: "white"
        font.pixelSize: slider.fontSize
        //elide: Text.ElideLeft
    }

    Text
    {
        id: rightTime
        width: textLength;
        height: parent.height
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignLeft

        anchors.left: slot.right
        anchors.leftMargin: slider.textAlign

        text: Qt.formatTime(new Date(0, 0, 0, 0, parseInt(slider.maxValue/60), slider.maxValue%60)
                            , qsTr("hh:mm:ss"))
        color: "white"
        font.pixelSize: slider.fontSize
        //elide: Text.ElideLeft
    }
}
