import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import ".."

Item {
    Item {
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 30
        anchors.top: parent.top
        anchors.bottom: parent.bottom


        Item {
            id: topBarLab
            anchors.left: parent.left
            anchors.right: parent.right
            height: 60

//            LinearGradient
//            {
//                anchors.fill: parent
//                start: Qt.point(0, parent.height)
//                end: Qt.point(0, 0)
//                gradient: Gradient{
//                    GradientStop{position: 0.0; color: "#515151"}
//                    GradientStop{position: 1.0; color: "#A5A5A9"}
//                }
//            }

            Text
            {
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
                text: loadList.loadInfo
                color: "white"
                font.pixelSize: 20
            }
        }
        ListView
        {
            id: cltView
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: topBarLab.bottom
            anchors.bottom: parent.bottom

            model: loadList
            delegate: collectDelegate
            currentIndex: loadList.collectId

            clip: true;
            spacing: 0
        }

        ScrollBar {
            id: listScrollBar

            orientation:  Qt.Vertical
            height: cltView.height
            width: 13
            scrollArea: cltView
            anchors.right: cltView.right
            anchors.top: cltView.top
        }


        Component
        {
            id:collectDelegate
            Rectangle
            {
                width: cltView.width; height: 100
                color: (ListView.view.currentIndex === index) ? "#433A2D" : "#373737"
                ImageFrame
                {
                    id: comImage
                    width: parent.height - 10
                    height: parent.height - 10
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    anchors.verticalCenter: parent.verticalCenter
                    imageUri: pictureUrl
                }

                Column{
                    width: parent.width - 2 * parent.height
                    //height: parent.height
                    anchors.left: comImage.right
                    anchors.leftMargin: 30
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 10

                    Text
                    {
                        id: compTitle

                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        text: collectTitle
                        color: "white"
                        font.pixelSize: 24
                    }
                    Text
                    {
                        id: compText

                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        text: collectName
                        color: "red"
                        font.pixelSize: 18
                    }
                }

                KlLine
                {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 2
                    fillColor: "gray"
                }
                MouseArea
                {
                    width: parent.width - parent.height //- parent.height
                    height: parent.height
                    onClicked:
                    {
                        loadList.qmlDownloadThreadLevelClick(index);
                    }
                }
            }
        }

//        Rectangle {
//            anchors.fill: cltView
//            border.color: "green"
//            border.width: 4
//            color: "transparent"
//        }

    }
}
