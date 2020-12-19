import QtQuick 2.0
import HongJing.HMI.Media 1.0 as Media

Image {
    id: ctrlLab
    source: "qrc:/images/multimedia/bgtop.png"

    Row{
        id:rowBtn
        spacing: 6
        x: 4; y: 0

        ButtonPress {
            id:btnMenu
            width: 140; height: 80
            btnImgUrl: pressed ? "qrc:/images/multimedia/music/btnMenu_hl.png"
                               : "qrc:/images/multimedia/music/btnMenu.png"

            onClicked: {
                Media.Controller.btnMenuClicked();
            }
        }

        ButtonPress {
            id:btnWallpaper
            width: 140; height: 80
            btnImgUrl: pressed ? "qrc:/images/multimedia/pic/btnWallPaper_hl.png"
                               : "qrc:/images/multimedia/pic/btnWallPaper.png"

            onClicked:
            {
                Media.Controller.btnWallpaperClicked()
            }
        }

        ButtonPress {
            id:btnLast
            width: 140; height: 80
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
            btnImgUrl: pressed ? (Media.Controller.playStatus ? "qrc:/images/multimedia/music/btnPlay_hl.png"
                                                              : "qrc:/images/multimedia/music/btnPause_hl.png")
                               : (Media.Controller.playStatus? "qrc:/images/multimedia/music/btnPlay.png"
                                                             : "qrc:/images/multimedia/music/btnPause.png")
            onClicked:
            {
                Media.Controller.btnPlayPause();
            }
        }

        ButtonPress {
            id:btnNext
            width: 140; height: 80
            btnImgUrl: pressed ? "qrc:/images/multimedia/music/btnNext_hl.png"
                               : "qrc:/images/multimedia/music/btnNext.png"
            onClicked:
            {
                Media.Controller.btnNextClicked();
            }
        }

        ButtonPress {
            id:btnScale
            width: 140; height: 80
            btnImgUrl: pressed ? "qrc:/images/multimedia/pic/btnScale_hl.png"
                               : "qrc:/images/multimedia/pic/btnScale.png"

            onClicked:
            {
                Media.Controller.btnScaleClicked()
            }
        }

        ButtonPress {
            id:btnRotate
            width: 140; height: 80
            btnImgUrl: pressed ? "qrc:/images/multimedia/pic/btnRotate_hl.png"
                               : "qrc:/images/multimedia/pic/btnRotate.png"
            onClicked:
            {
                Media.Controller.btnRotateClicked();
            }
        }
    }
}
