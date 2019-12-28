import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQml.Models 2.3
import QtQuick.Controls.Material 2.0
import Hongjing.HMI.KL 1.0 as KL

Rectangle {
    id: cateView

    anchors.fill: parent

    TabBar {
        id: tabBar
        anchors.left: parent.left
        anchors.right: parent.right
        height: 60
        currentIndex: cateModel.qmlGetTabIndex();

        onCurrentIndexChanged: cateModel.qmlCateTabClick(currentIndex)
        Repeater {
            model: cateModel

            TabButton {
                text: ctgname
                width: Math.max(70, cateView.width / 10)
            }
        }
    }

    GridView {
        id: cateItemView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom

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
            onClicked: {
                cateItemModel.qmlClickCategory(index)
                // stack.push("CateItemInfoView.qml")
            }
        }
        onFlickEnded:
        {
            if(atYEnd)
            {
                cateItemModel.qmlCtgNextPage();
            }
        }
    }
}
