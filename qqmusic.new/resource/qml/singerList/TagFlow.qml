import QtQuick 2.0

Flow {
    id: flow
    spacing: 15
    property int currentIndex: -1

    property alias model: repeater.model
    signal clicked(var choiceIndex)
    Repeater {
        id: repeater
        property int cancelIndex: -1
        model: singerIndexModel
        anchors.fill: parent
        TagButton
        {
            isChoice: flow.currentIndex == index ? true : false
            isCancel: repeater.cancelIndex == index ? true : false
            text: ctgname
            onCanceled: {
                repeater.cancelIndex = index
            }
            onClicked: {
                repeater.cancelIndex = -1
                flow.clicked(index);
            }
        }
    }
}
