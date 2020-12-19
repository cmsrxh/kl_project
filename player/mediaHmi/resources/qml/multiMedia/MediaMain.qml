import QtQuick 2.0
import HongJing.HMI.Media 1.0 as Media
import "./Multimedia.js" as MediaScript

Item {
    width: 1024
    height: 600


    Loader
    {
        anchors.fill: parent
        source: MediaScript.MediaViewList[Media.Controller.viewIndex]
    }

//    MediaFileView
//    {
//        anchors.fill: parent
//    }

//    MusicPlayView
//    {
//        anchors.fill: parent
//    }

//    VideoPlayView
//    {
//        anchors.fill: parent
//    }

    Connections
    {
        target: Media.Controller
        onShowMsg:
        {
            //console.log("Show Message Box");
            msgBox.visible = true;
            msgBox.boxType = boxType;
            msgBox.msgArg = msgArgs;
            msgBox.msgTitle = title;
            msgBox.msgContent = content;
        }
        onHideMsg:
        {
            //console.log("Hide Msg Recv.")
            msgBox.visible = false;
        }
    }

    MediaMessageBox
    {
        id: msgBox
        z: 3
        anchors.fill: parent
        boxType: 0
        msgArg: 0
        visible: false
        msgContent: ""
        msgTitle: ""
    }

    Text {
        id: name
        color: "white"
        font.pixelSize: 48
        text: Media.Controller.cpuTemp
    }
}
