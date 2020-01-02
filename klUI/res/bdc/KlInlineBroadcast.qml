import QtQuick 2.0

import Hongjing.HMI.KL 1.0 as KL

Rectangle {
    id: inline

    color: "#414141"

    KlInlineLeftView {
        id: leftView
        width: parent.width * 0.3;
        height: parent.height
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }
    Rectangle{
        width: 2
        height: parent.height
        anchors.right: leftView.right
    }

    Loader {
        id: channelLoader
        anchors.left: leftView.right
        width: inline.width * 0.7
        height: inline.height
        source: stack.bdcSource
    }
}

