import QtQuick 2.6
import QtQuick.Window 2.2
import ImageFrame 1.0
import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    MyButton {
        width: 200
        height: 100
    }

    PictureShadow {
        width: 150; height: 150
        x: 200; y: 258
    }

    PictureFrame {
         id:musicCover
         width: 150; height: 150
         x: 10; y: 258

         source: "http:\/\/y.gtimg.cn\/music\/photo_new\/T001R150x150M000002DYpxl3hW3EP.webp"
     }

//    Rectangle {
//        id: textEdit
//        anchors.top: parent.top
//        anchors.topMargin: 20
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.margins: -10

//        width: 100
//        height: 80

//        color: "blue"
//        border.width: 1
//    }


//    ImageFrame {
//        id: image
//        anchors.left: textEdit.left
//        anchors.top: textEdit.bottom
//        anchors.topMargin: 40

//        width: 150
//        height: 150

////        source: "http://img.kaolafm.net/mz/images/201906/eb01b107-d009-43be-a4bb-dbebc8496082/default.jpg"
//        source: "http:\/\/y.gtimg.cn\/music\/photo_new\/T001R150x150M000002DYpxl3hW3EP.webp"

//        MouseArea {
//            anchors.fill: parent
//            onClicked: console.log("abcdefg")
//        }
//    }
}
