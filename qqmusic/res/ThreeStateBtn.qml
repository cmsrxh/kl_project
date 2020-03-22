import QtQuick 2.0

Item {
    id: it
    property url initSt: ""
    property url pressSt: ""
    property url lastSt: ""
    property bool enable: false
    property bool pressed: false
    signal clicked()
    Image
    {
        id: icon
        width: Image.width
        height: Image.height
        anchors.centerIn: parent
        source:  it.enable ? it.lastSt : it.initSt
    }
    Image
    {
        id: icon2
        width: Image.width
        height: Image.height
        anchors.centerIn: parent
        source:  it.pressed ? it.pressSt
                         : ""
    }

    MouseArea
    {
        anchors.fill: parent
        onPressed:
        {
            it.pressed = true
        }
        onCanceled: it.pressed = false
        onReleased:
        {
            it.pressed = false
            it.clicked()
        }
    }
}

