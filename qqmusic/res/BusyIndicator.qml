import QtQuick 2.0

Image {
    id: container

    source: "qrc:/image/busy.png";

    NumberAnimation on rotation {
        running: container.visible
        from: 0; to: 360;
        loops: Animation.Infinite;
        duration: 1700
    }
}

