import QtQuick 2.0
import "../"
import Hongjing.HMI.KL 1.0 as KL

Item {
    property bool isInArea: false
    property alias contentHight: view.contentHeight

    ListView {
        id: view
        anchors.fill: parent
        clip: true;
        model: bdcItem
        delegate: chDelegate
        currentIndex: bdcItem.currenBDCIndex
        spacing: 2
        boundsBehavior: Flickable.StopAtBounds
        cacheBuffer: 61
    }

    ScrollBar {
        id: listScrollBar

        orientation:  Qt.Vertical
        height: view.height
        width: 13
        scrollArea: view
        anchors.right: view.right
        anchors.top: view.top
    }

    Component {
        id: chDelegate
        Rectangle {
            id: chItem
            width: view.width; height: 61
            color: (bdcItem.currenBDCIndex === index) ?
                     "#433A2D" : "#545454"
            Rectangle {
                id: foreground
                anchors.fill: parent
                color: "transparent"
            }
            Row {
                spacing: 8
                width: parent.width * 4/5
                height: parent.height
                x: 10; y: 0

                state: bdcItem.currenBDCIndex === index ? "signSt" : ""
                Item {
                    id: playItem
                    width: 0
                    height: chItem.height
                    Image {
                        id: playImg
                        width: 0
                        height: Image.height
                        anchors.centerIn: parent
                        source: "qrc:/image/playing.png"
                    }
                }
                states: State {
                    name: "signSt"
                    PropertyChanges {target: playItem; width: chItem.height}
                    PropertyChanges {target: playImg; width: Image.width}
                }
                Text {
                    id: chNameText
                    width: parent.width - chItem.height; height: chItem.height
                    elide: Text.ElideRight
                    verticalAlignment: Qt.AlignVCenter
                    text: cateItemName
                    color: "white"
                    font.pixelSize: 18
                }
            }

            Item {
                width: parent.width /5
                height: parent.height
                anchors.right: parent.right
                Canvas {
                    id: verticalLine
                    width: 2; height: parent.height
                    onPaint: {
                        var cxt = verticalLine.getContext("2d")
                        cxt.lineWidth=verticalLine.width;
                        cxt.strokeStyle = "#818181"
                        cxt.lineCap="round";

                        cxt.moveTo(0, height/3);
                        cxt.lineTo(0, height *2/3);

                        cxt.stroke();
                    }
                }

                Item {
                    id: it
                    width: parent.height
                    height: parent.height
                    anchors.centerIn: parent

                    property bool enable: collect
                    property bool pressed: false

                    Image {
                        id: icon
                        width: Image.width
                        height: Image.height
                        anchors.centerIn: parent
                        source:  it.enable ? "qrc:/image/collect_hl.png"
                                           : "qrc:/image/collect.png"
                    }
                    Image {
                        id: icon2
                        width: Image.width
                        height: Image.height
                        anchors.centerIn: parent
                        source:  it.pressed ? "qrc:/image/collect_clk.png"
                                            : ""
                    }

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            it.pressed = true
                        }
                        onCanceled: it.pressed = false
                        onClicked: bdcItem.qmlClickBDCItemCollect(index, !it.enable)
                        onReleased: {
                            it.pressed = false
                        }
                    }
                }
            }

            MouseArea {
                width: parent.width * 4/5
                height: parent.height
                onClicked: {
                    bdcItem.qmlClickBDCItem(index, isInArea)
                }
                onPressed: {
                    foreground.color = "#433A2D"
                }
                onReleased: {
                    foreground.color = "transparent"
                }
                onCanceled: {
                    foreground.color = "transparent"
                }
            }
        }
    }

}

