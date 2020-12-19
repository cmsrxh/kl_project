import QtQuick 2.0
import HongJing.HMI.Media 1.0 as Media
import "./Multimedia.js" as MediaScript

Image {
    id: ctrlLab
    property int playModel: 0
    source: "qrc:/images/multimedia/bgtop.png"


    ButtonPress {
        id: btnMenu
        width: 140; height: 80
        x:14; y:0

        btnImgUrl: pressed ? "qrc:/images/multimedia/music/btnMenu_hl.png"
                           : "qrc:/images/multimedia/music/btnMenu.png"

        onClicked: {
            Media.Controller.btnMenuClicked();
        }
    }

    ButtonPress {
        id:btnPrevious
        width: 140; height: 80
        x:225; y:0
        btnImgUrl: pressed ? "qrc:/images/multimedia/music/btnLast_hl.png"
                           : "qrc:/images/multimedia/music/btnLast.png"
        onClicked:
        {
            Media.Controller.btnPreviousClicked();
        }
    }

    ButtonPress {
        id:btnPlay
        width: 140; height: 80
        x:435; y:0
        btnImgUrl: pressed ? (Media.Controller.playStatus ? "qrc:/images/multimedia/music/btnPlay_hl.png"
                                                          : "qrc:/images/multimedia/music/btnPause_hl.png")
                           : (Media.Controller.playStatus? "qrc:/images/multimedia/music/btnPlay.png"
                                                         : "qrc:/images/multimedia/music/btnPause.png")
        onClicked: {
            Media.Controller.btnPlayPause();
        }
    }

    ButtonPress {
        id:btnNext
        width: 140; height: 80
        x:646; y:0

        btnImgUrl: pressed ? "qrc:/images/multimedia/music/btnNext_hl.png"
                           : "qrc:/images/multimedia/music/btnNext.png"
        onClicked: {
            Media.Controller.btnNextClicked();
        }

    }

    ButtonPress {
        id:btnModel
        width: 140; height: 80
        x:855; y:0
        btnImgUrl: pressed ? MediaScript.PlayModeList_hl[playModel]
                           : MediaScript.PlayModeList[playModel]
        onClicked: {            
            Media.Controller.btnModeClicked();
        }
    }    
}
