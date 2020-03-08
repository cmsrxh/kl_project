import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import Hongjing.HMI.KL 1.0 as KL

Item{
    id:slider

    property int maxValue: 1
    property int currentValue: 0
    property int valueBak: 0
    property int fontSize: 16
    property bool isPress: false

    Connections
    {
        target: KL.Controller
        onDurationChanged: {
            slider.maxValue = duration;
        }        
        onPositionChanged: {
            rightTime.text = durStr;
            leftTime.text  = curPos            
        }
        onCacheDataChanged: {
            cacheData.cacheValue = curPos
        }
    }

    Timer {
        repeat: true
        interval: 1000
        running: (1 === KL.Controller.playState) ? true : false

        onTriggered:
        {
            if (!slider.isPress)
            {
                slider.currentValue = KL.Controller.qmlGetCurrentPosition();
                if (slider.maxValue <= slider.currentValue)
                {
                    slider.maxValue = KL.Controller.qmlGetDuration();
                }
            }
        }
    }

    Item {
        anchors.left: leftTime.right
        anchors.leftMargin: 10
        anchors.right: rightTime.left
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter

        height: 5

        Image{
            id:slot
            width: parent.width
            height: parent.height
            source: "qrc:/image/slider_bg.png"
        }
        Rectangle {
            id: cacheData
            property int cacheValue: 0
            height: parent.height
            radius: parent.height / 2
            color: "gray"
            opacity: 0.6
            width: {
                // console.log(slider.currentValue, slider.maxValue, slider.maxValue - slider.currentValue, slot.width, slider.currentValue * slot.width / slider.maxValue)
                if (cacheValue <= slider.maxValue && slider.maxValue > 0 && slot.width > 0)
                {
                    return cacheValue * slot.width / slider.maxValue
                } else
                {
                    return 0;
                }
            }
        }

        BorderImage{
            id:slot_hl

            border.left: 1
            border.right: 1
            height: parent.height
            width: {
                // console.log(slider.currentValue, slider.maxValue, slider.maxValue - slider.currentValue, slot.width, slider.currentValue * slot.width / slider.maxValue)
                if (slider.currentValue <= slider.maxValue && slider.maxValue > 0 && slot.width > 0)
                {
                    return slider.currentValue * slot.width / slider.maxValue
                } else
                {
                    return 0;
                }
            }
            source: "qrc:/image/slider_hl.png"
        }

        MouseArea
        {
            anchors.left: slot.left
            anchors.right: slot.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            onPressed: {
                slider.valueBak = mouseX*slider.maxValue/width;
                slider.isPress = true;
            }
            onReleased: {
                slider.isPress = false;
                KL.Controller.qmlSeekTo(slider.valueBak, 0);
            }
            onMouseXChanged: {
                if(!KL.Controller.canSeek) return;
                if(mouseX < 0)
                {
                    slider.currentValue = 0;
                }
                else if(mouseX > width)
                {
                    slider.currentValue = slider.maxValue;
                }
                else
                {
                    slider.currentValue = mouseX*slider.maxValue/width;
                }
                slider.valueBak = slider.currentValue;
            }
        }

    }
    Text
    {
        id: leftTime

        height: parent.height
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignRight
        anchors.left: parent.left
        anchors.leftMargin: 10

        text: "00:00:00"
        color: "white"
        font.pixelSize: slider.fontSize
        //elide: Text.ElideLeft
    }

    Text
    {
        id: rightTime

        height: parent.height
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignLeft
        anchors.right: parent.right
        anchors.rightMargin: 10

        text: "00:00:00"
        color: "white"
        font.pixelSize: slider.fontSize
        //elide: Text.ElideLeft
    }
}

