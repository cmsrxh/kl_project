import QtQuick 2.0

Item {
    property alias text: leftText.text
    property alias pixelSize: leftText.font.pixelSize
    property alias textColor: leftText.color
    property int speed: 15 //滚动速度，数字越大越慢，为0时不滚动

    id: textLab
    clip: true

    Text
    {
        id: leftText
        anchors.verticalCenter: parent.verticalCenter

        verticalAlignment: Text.AlignVCenter

        color: "#AAAAAA"
        font.pixelSize: 28

        SequentialAnimation on x {
            id: animation
            loops: Animation.Infinite   //循环执行动画，直到stop
            running: leftText.contentWidth > textLab.width  //文本实际宽度超出范围时开始滚动
            PropertyAnimation {
                easing.type: Easing.Linear
                from: 0
                to: -leftText.contentWidth
                duration: leftText.contentWidth * speed

                //duration与文本长度绑定，若duration变化，即文本变化，应该重新设置动画
                onDurationChanged: {
//                    console.log("onDurationChanged: text = ", leftText.text, "cw = ", leftText.contentWidth, "width = ", leftText.width)
                    if (leftText.contentWidth > textLab.width)
                    {
                        animation.restart()
                    }
                    else
                    {
                        animation.complete()
                    }
                }
            }
            PropertyAnimation {
                easing.type: Easing.Linear
                from: textLab.width
                to: 0
                duration: textLab.width * speed
            }
        }
    }
}
