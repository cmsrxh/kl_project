import QtQuick 2.0
import HongJing.HMI.Media 1.0 as Media
import "./Multimedia.js" as MediaScript

Item {
//    property int currentType: 0
    Image
    {
        source: "qrc:/images/multimedia/fileTab_bg.png"
    }

    ButtonPress
    {
        id: btnBack
        x: 9; y:543
        btnImgUrl: pressed ? "qrc:/images/multimedia/btnBack_hl.png"
                           : "qrc:/images/multimedia/btnBack.png"
        onClicked:
        {
            Media.Controller.btnBackClicked()
        }
    }

    Row{
        id: mediaBtnTab
        x: 297
        y: 138
        width: 660
        height: 51
        spacing: 3
        Repeater
        {
            id: rep
            model: 3

            Item{
                width: 218; height: 52

                Text
                {
                    anchors.fill: parent
                    text: MediaScript.MediaTextList[index]
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: 24
                    color: "white"
                }

                ButtonPress
                {
                    anchors.fill: parent
                    btnImgUrl: Media.Controller.mediaType === index ? "qrc:/images/multimedia/btnTab_hl.png"
                                                                    : ""
                    onClicked:
                    {
                        Media.Controller.mediaType = index;
                        console.log("Current Tab Index: ", index)
                    }
                }
            }
        }
    }

    property var components: [musicComp, videoComp, imageComp]

    Loader
    {
        id: viewLoader
        x: 319; y: 192
        width: 620; height: 283

        sourceComponent: components[Media.Controller.mediaType]
    }
    Component
    {
        id: musicComp
        MediaFileListView
        {
            model: musicModel
            view_type: 0
            currentIndex: musicModel.currentIndex
            onItemClicked: {
                musicModel.mediaItemClicked(itemIndex);
            }
        }
    }
    Component
    {
        id: videoComp
        MediaFileListView
        {
            model: videoModel
            view_type: 1
            currentIndex: videoModel.currentIndex
            onItemClicked: {
                videoModel.mediaItemClicked(itemIndex);
            }
        }
    }
    Component
    {
        id: imageComp
        MediaFileListView
        {
            model: imageModel
            view_type: 2
            currentIndex: imageModel.currentIndex
            onItemClicked: {
                imageModel.mediaItemClicked(itemIndex);
            }
        }
    }

    ScrollBar {
        id: scrollBar
        width: 40
        height: 236
        anchors.verticalCenter: viewLoader.verticalCenter
        scrollArea: viewLoader.item
        anchors.right: viewLoader.right
//        anchors.rightMargin: 4
    }
}
