import QtQuick 2.0

Image
{
    id: btn
    signal clicked()
    property bool pressed: false
    property alias btnImgUrl: btn.source

    MouseArea
    {
        anchors.fill: parent
        onPressed: {            
            btn.pressed = true
        }
        onCanceled: {
            btn.pressed = false
        }
        onReleased: {
            btn.pressed = false
        }
        onClicked: {
            btn.clicked();
        }
    }
}
