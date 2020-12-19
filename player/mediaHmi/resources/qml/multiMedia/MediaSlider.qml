import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import HongJing.HMI.Media 1.0 as Media

Item{
    id:slider

    property int maxValue: Media.Controller.qmlCurrentDur();
    property int currentValue: Media.Controller.qmlCurrentPos()
    property int valueBak: 0
    property int fontSize: 20
    property int textAlign: 20
    property alias sliderLen: slot.width

    Connections
    {
        target: Media.Controller
        onSliderValue:
        {
            slider.currentValue = current
            if(slider.maxValue != duration)
            {
                slider.maxValue = duration
            }
        }
    }

    Image{
        id:slot
//        width: 432
//        height: 7
        anchors.centerIn: parent
        source: "qrc:/images/multimedia/music/music_slider_bg.png"
    }

    BorderImage{
        id:slot_hl
        anchors.left: slot.left
        anchors.verticalCenter: slot.verticalCenter
        border.left: 10
        border.right: 1
        width: slider.currentValue * slot.width / slider.maxValue
        source: "qrc:/images/multimedia/music/music_slider_hl.png"
    }

    MouseArea
    {
        anchors.left: slot.left
        anchors.right: slot.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        onPressed: {
            slider.valueBak = mouseX*slider.maxValue/width;
            Media.Controller.qmlSeekingSwitch(); //设置filter和canSeek属性为false
        }
        onReleased: {
            Media.Controller.qmlPlaySeek(slider.valueBak);
        }
        onMouseXChanged: {
            //console.log("XChanged", mouseX, slider.currentValue, slider.valueBak, width)
            if(false === Media.Controller.canSeek) return;
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


    Text
    {
        id: leftTime
        width: parent.width * 0.2;
        height: parent.height
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignRight

        anchors.right: slot.left
        anchors.rightMargin: slider.textAlign

        text: Qt.formatTime(new Date(0, 0, 0, 0, parseInt(slider.currentValue/60), slider.currentValue%60)
                            , qsTr("hh:mm:ss"))
        color: "white"
        font.pixelSize: slider.fontSize
        //elide: Text.ElideLeft
    }

    Text
    {
        id: rightTime
        width: leftTime.width;
        height: parent.height
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignLeft

        anchors.left: slot.right
        anchors.leftMargin: slider.textAlign

        text: Qt.formatTime(new Date(0, 0, 0, 0, parseInt(slider.maxValue/60), slider.maxValue%60)
                            , qsTr("hh:mm:ss"))
        color: "white"
        font.pixelSize: slider.fontSize
        //elide: Text.ElideLeft
    }
}

