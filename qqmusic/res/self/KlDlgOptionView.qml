import QtQuick 2.0
import Hongjing.HMI.KL 1.0 as KL

Item {
    id: option
    property int curFocus : KL.Controller.qmlGetSelfTabIndex()
    property var views: [   "KlCollectView.qml",
                            "KlLoadView.qml",
                            "KlHistoryRecord.qml",
                            "KlVioceGuide.qml",
                            "KlSettingView.qml"]

    Item //标题Tab，如收藏
    {
        id: titleItem
        width: parent.width < 800 ? parent.width : 800
        anchors.left: parent.left
        anchors.leftMargin: 30
        height: 51
        // color: "#313131"
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
                        color: index == curFocus ? "#EEA105" : "white"
                        font.pixelSize: 20
                    }
                    Rectangle
                    {
                        id: repBottom
                        width: parent.width
                        height: 3
                        anchors.bottom: parent.bottom
                        color: index == curFocus ? "#EEA105" : "transparent"
                    }
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            curFocus = index
                            KL.Controller.qmlSelfTabClick(index)
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
        source: views[curFocus]
//        sourceComponent: KlCollectView{}
//        sourceComponent: KlLoadView{}
//        sourceComponent: KlHistoryRecord{}
//        sourceComponent: KlVioceGuide{}
//        sourceComponent: KlSettingView{}

    }

}

