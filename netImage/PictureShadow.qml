import QtQuick 2.6
import QtGraphicalEffects 1.0

Item {
    id : mybutton


    signal clicked();

    Rectangle {
        id : bgrect;
        y : 20
        x : 1
        color: "#999999";
        width: mybutton.width-4;
        height: mybutton.height-28
        radius: height/2
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


    PictureFrame {
        id : toprect
        width: mybutton.width;
        height: mybutton.height-5
        source: "http:\/\/y.gtimg.cn\/music\/photo_new\/T001R150x150M000002DYpxl3hW3EP.webp"

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled : true
            onPressed: {
                btnPress.start()
            }
            onCanceled: {
                if (btnPress.running)
                {
                    btnPress.stop()
                }
                btnRelease.start()
            }
            onReleased: {
                if (btnPress.running)
                {
                    btnPress.stop()
                }
                btnRelease.start()
            }
            onClicked: {
                mybutton.clicked();
            }
        }
    }

    NumberAnimation {
        id: btnPress
        target: toprect; property: "y"; to: toprect.x+2; duration: 100
    }
    NumberAnimation {
        id: btnRelease
        target: toprect; property: "y"; to: toprect.x-2; duration: 100
    }

//    SequentialAnimation {
//        id : animation
//        NumberAnimation { target: toprect; property: "y"; to: toprect.x+2; duration: 100 }
//        NumberAnimation { target: toprect; property: "y"; to: toprect.x-2; duration: 100 }
//    }

}
