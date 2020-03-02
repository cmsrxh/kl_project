import QtQuick 2.0
import ".."
import Hongjing.HMI.KL 1.0 as KaoLa

Item
{
    property alias contentHight: areaView.contentHeight

//    Component.onCompleted: {
//        if(bdcArea.bdcAreaIndex >= 0) {
//            areaView.contentY = bdcArea.radioAreaId * 71
//        }
//    }

    ListView {
        id: areaView
        anchors.fill: parent
        model: bdcArea
        delegate: areaDelegate
        boundsBehavior: Flickable.StopAtBounds
        currentIndex: bdcArea.bdcAreaIndex
        cacheBuffer: 61
        spacing: 0
        clip: true;
    }

    ScrollBar {
        id: listScrollBar
        orientation:  Qt.Vertical
        height: areaView.height
        width: 13
        scrollArea: areaView
        anchors.right: areaView.right
        anchors.top: areaView.top
    }

    Component {
        id: areaDelegate
        Item {
            id: areaItem

            width: areaView.width; height: textRect.height /*+ line.height*/ + childView.height

            Rectangle {
                id: textRect
                width: parent.width; height: 71
                color: areaItem.ListView.view.currentIndex === index ? "#545454" : "#414041"

                Text  {
                    id: chNameText
                    x: 0; y: 0
                    anchors.fill: parent
                    elide: Text.ElideRight
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                    text: ctgname
                    color: "white"
                    font.pixelSize: 18
                }
                KlLine {
                    id: line
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 4
                    fillColor: "gray"
                }
                MouseArea {
                    width: parent.width
                    height: parent.height
                    onClicked: {
                        bdcArea.qmlBDCAreaTabClick(index)
                    }
                }
            }

            Loader {
                id: childView
                active: areaItem.ListView.view.currentIndex === index
                width: areaItem.width;
                anchors.bottom: areaItem.bottom
                height: active == true ? (item.contentHight < 1 ? 100 : item.contentHight) : 0
                sourceComponent: KlInlineChannelView {
                    isInArea: true
                }

                KlLine {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 4
                    fillColor: "gray"
                }
            }
        }
    }

}


