import QtQuick 2.7
import QtQuick.Controls 1.4

Item {
    id : cate
    property int currentGround: 0

    //ScrollView {
    Flickable {
//        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
//        verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff
//        frameVisible: true
        contentWidth: parent.width
        contentHeight: cateGroup.implicitHeight

        height: parent.height
        width: parent.width

        Column {
            id: cateGroup
            width: cate.width
            spacing: 10
            Repeater {
                id: repeater
                model: catePlayListProc.cateList
                Item {
                    id: filler

                    width: row.width
                    height: row.height + (currentGround == index ? loader.height : 0)

                    Item {
                        id: row
                        width: cateGroup.width
                        height: 60
                        // 行数据UI
                        Row {

                            x: 10
                            // 行图标
                            Item {
                                width: row.height * 0.5
                                height: row.height
                                Image {
                                    id: expander
                                    source: "qrc:/image/svg/Forward.svg"
                                    height: parent.height * 0.3
                                    width: parent.height * 0.3
                                    fillMode: Image.PreserveAspectFit
                                    opacity: mouse.containsMouse ? 1 : 0.7
                                    anchors.centerIn: parent
                                    rotation: currentGround == index ? 90 : 0
                                    Behavior on rotation {NumberAnimation { duration: 120}}
                                }
                            }
                            // 行文本
                            Text {
                                height: parent.height
                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                                text: catePlayListProc.cateList[index]
                                font.pixelSize: 28
                            }
                        }
                        MouseArea {
                            id: mouse
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                currentGround = index
                                // console.log("currentGround=", index)
                            }
                        }
                    }

                    Loader {
                        id: loader
                        anchors.top: row.bottom
                        anchors.topMargin: 5
                        property int groupIndex: index
                        property int groupCateIndex: catePlayListProc.cateSubs[index].curIndex
                        sourceComponent: currentGround == index ? secTree : null
                    }
                }
            }

        }
    }

    Component {
        id: secTree
        Column {
            spacing: 10
            x: 50
            Repeater {
                id: repeater
                model: catePlayListProc.cateSubs[groupIndex]
                Item {
                    width: textLab.implicitWidth + 10
                    height: 60
                    Text {
                        id: textLab
                        height: parent.height
                        anchors.centerIn: parent
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        text: ctgname
                        color: groupCateIndex === index ? "#5CB8FC" : "gray"
                        font.pixelSize: 28
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            catePlayListProc.qmlGroupCateIndex(groupIndex, index);
                        }
                    }
                }
            }
        }
    }

}
