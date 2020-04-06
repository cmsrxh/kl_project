import QtQuick 2.0
import QtGraphicalEffects 1.0
import "../tool"

Item {
    id : mybutton
    property int btnRadius: 10
    property alias source: toprect.source

    signal clicked();

    Rectangle {
        id : bgrect;
        y : 20
        x : 1
        color: "#999999";
        width: mybutton.width-4;
        height: mybutton.height-28
        radius: btnRadius
    }

    DropShadow {
        id : shadow
        anchors.fill: bgrect
        horizontalOffset: 9
        verticalOffset: 12
        radius: 12.0
        samples: 17
        color: "#999999"
        source: bgrect
    }

    RoundPicture {
        id : toprect
        width: mybutton.width;
        height: mybutton.height - 5
        radius: btnRadius
    }
}
