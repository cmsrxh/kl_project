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
        height: 40
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

    Loader {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom

        source: stack.albumSource
    }
}
