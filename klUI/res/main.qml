import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQml.Models 2.3
import QtQuick.Controls.Material 2.0
import Hongjing.HMI.KL 1.0 as KL

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("Hello World")
    Material.theme: Material.Light
    Material.accent: Material.DeepOrange
    Material.primary: Material.Blue

    ColumnLayout{
        anchors.fill: parent

        GridView
        {
            id: cateItemView
            Layout.fillWidth:true
            Layout.fillHeight: true
            model: cateItemModel
            // currentIndex: YT.Controller.videoCtg
            clip: true
            //focus: true
            //boundsBehavior: Flickable.StopAtBounds
            //layoutDirection: Qt.LeftToRight
            //flow: GridView.FlowTopToBottom

            cellWidth: width /3
            cellHeight: width /4
            delegate: VideosModel {
                playing: false
                onClicked: cateModel.qmlClickCategory(index)
            }
            onFlickEnded:
            {
                if(atYEnd)
                {
                    cateModel.qmlCtgNextPage();
                }
            }
        }

    }

    header: TabBar {
        id: tabBar
        width: parent.width
        onCurrentIndexChanged: cateModel.qmlCateTabClick(currentIndex)
        Repeater {
            model: cateModel

            TabButton {
                text: ctgname
                width: Math.max(70, tabBar.width / 10)
            }
        }
    }
}
