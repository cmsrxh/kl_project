import QtQuick 2.0
import com.hongjing.layers 1.0 as LM
import "../../public/controls" as MyControls

UpdateBoxBase {
    handle: Item {
        id: root

        Text {
            anchors.top: parent.top
            anchors.topMargin: 60

            anchors.horizontalCenter: parent.horizontalCenter

            font.pixelSize: 32

            color: "white"
            text: LM.Api.getSlidlerTitleName()
        }

        Item {
            id: _private
            // 0 ~ 100
            property int value: LM.Api.sliderValue

            width: 438
            height: 20

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            Rectangle {
                width:  _private.width
                height: _private.height
                radius: 3
                color: "#bdbebf"

                /* available rectangle */
                Rectangle {
                    width:  _private.width * _private.value / 100
                    height: _private.height

                    radius: parent.radius
                    color: "#21be2b"
                }
            }
        }
    }
}
