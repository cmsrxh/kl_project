import QtQuick 2.0
//import com.hongjing.globalcontroller 1.0 as GC
import com.hongjing.layers 1.0 as LM
//import "../public/controls"

Item {

    MouseArea {
        anchors.fill: parent
        visible: LM.Api.msgBoxModel;
        onPressed: {
            console.log("Window is model.", LM.Api.msgBoxModel)
        }
    }

    Loader {
        anchors.fill: parent
        active: true
        source: LM.Api.msgBoxUrl;
    }

}
