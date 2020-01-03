import QtQuick 2.0

Item {
    id: option

    Rectangle //标题Tab，如收藏
    {
        id: titleItem
        width: parent.width; height: 51
        property int curFocus : 0
        color: "#313131"
        Row
        {
            width: parent.width; height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter //水平
            spacing: 1
            Repeater
            {
                model: cltModel
                Item
                {
                    id: repItem
                    width: titleItem.width / cltModel.count
                    height: parent.height

                    Text
                    {
                        id: repText
                        anchors.fill: parent
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter
                        text: name
                        color: index == titleItem.curFocus ? "#EEA105" : "white"
                        font.pixelSize: 24
                    }
                    Rectangle
                    {
                        id: repBottom
                        width: parent.width
                        height: 5
                        anchors.bottom: parent.bottom
                        color: index == titleItem.curFocus ? "#EEA105" : "transparent"
                    }
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            titleItem.curFocus = index
                        }
                    }
                }
            }
        }
    }

    ListModel
    {
        id: cltModel
        ListElement
        {
            name: qsTr("收藏")
        }
        ListElement
        {
            name: qsTr("下载")
        }
        ListElement
        {
            name: qsTr("收听历史")
        }
        ListElement
        {
            name: qsTr("语音指导")
        }
        ListElement
        {
            name: qsTr("设置")
        }
    }

    Loader
    {
        id: collectLoader
        active: true
        width: option.width
        height: option.height - 51
        anchors.top: titleItem.bottom        
        sourceComponent: KlCollectView{ }
    }

}

