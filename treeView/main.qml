import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    CateTreeView {
        anchors.fill: parent
        id: tree
        model: modelTree2
        onSelectedItemChanged: console.log(item.text)
    }
    ListModel {
        id: modelTree2
        Component.onCompleted: {
            modelTree2.append([
                {title: "Node 1"},
                {title: "Node 2", items: [
                    {title: "Node 21", items:[
                      {title: "Node 211"},
                      {title: "Node 212"}
                      ]},
                    {title: "Node 22"}
                  ]
                },
                {title: "Node 3"}
            ]);
        }
    }
}
