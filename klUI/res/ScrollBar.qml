import QtQuick 2.2

Item {
    id: container

    property variant scrollArea
    property int orientation: Qt.Vertical
    property alias bgSrc: scrollBar.source

    opacity: 0

    function position()
    {
        var ny = 0;
        if (container.orientation == Qt.Vertical)
            ny = scrollArea.visibleArea.yPosition * container.height;
        else
            ny = scrollArea.visibleArea.xPosition * container.width;

        if (ny > 2)
            return ny;
        else
            return 2;
    }

    function size()
    {
        var nh;

        if (container.orientation == Qt.Vertical)
            nh = scrollArea.visibleArea.heightRatio * container.height;
        else
            nh = scrollArea.visibleArea.widthRatio * container.width;

        return nh;
    }

    Image {
        id: scrollBar
        //source: "qrc:/images/phone/record/scrollBar.png"

        x: container.orientation == Qt.Vertical ? 2 : position()
        y: container.orientation == Qt.Vertical ? position() : 2
        width: container.orientation == Qt.Vertical ? Image.width : size()
        height: container.orientation == Qt.Vertical ? size() : Image.height
    }

    states: State {
        name: "visible"
        when: container.orientation == Qt.Vertical ?
                  1 !== scrollArea.visibleArea.heightRatio :
                  1 !== scrollArea.visibleArea.widthRatio
        PropertyChanges { target: container; opacity: 1.0 }
    }

    function scrollPos(mouse){
        if(container.orientation == Qt.Vertical){
            if(mouse.y < 0 || mouse.y + scrollBar.height > container.height) return;
            scrollArea.contentY = mouse.y / container.height * scrollArea.contentHeight
        } else
        {
            if(mouse.x < 0 || mouse.x + scrollBar.width > container.width) return;
            scrollArea.contentX = mouse.x / container.width * scrollArea.contentWidth
        }
    }

    MouseArea {
        property int pressPos: 0
        property int prevPos: 0
        anchors.fill: parent
        onPressed: {
            if(container.orientation == Qt.Vertical)
            {
                pressPos = scrollBar.y;
                prevPos = mouse.y;
            }else
            {
                pressPos = scrollBar.x;
                pressPos = mouse.x;
            }
        }
        onClicked: {
            if(container.orientation == Qt.Vertical)
            {
                var pos;
                if(mouse.y < scrollBar.y)
                {
                    pos = scrollBar.y - 25;
                    if(pos < 0 ) pos = 0;
                    scrollArea.contentY = pos / container.height * scrollArea.contentHeight
                }else if( mouse.y > scrollBar.y + scrollBar.height )
                {
                    pos = scrollBar.y + 25;
                    if(pos > container.height - scrollBar.height) pos = container.height - scrollBar.height;
                    scrollArea.contentY = pos / container.height * scrollArea.contentHeight
                }
            } else
            {
                var pos;
                if(mouse.x < scrollBar.x)
                {
                    pos = scrollBar.x - 25;
                    if(pos < 0 ) pos = 0;
                    scrollArea.contentX = pos / container.width * scrollArea.contentWidth
                }else if( mouse.x > scrollBar.x + scrollBar.width )
                {
                    pos = scrollBar.x + 25;
                    if(pos > container.width - scrollBar.width) pos = container.width - scrollBar.width;
                    scrollArea.contentX = pos / container.width * scrollArea.contentWidth
                }
            }
        }
        onPositionChanged: {
            if(container.orientation == Qt.Vertical)
            {
                if(mouse.y > scrollBar.y
                        && mouse.y < scrollBar.height + scrollBar.y)
                {
                    if( prevPos > mouse.y )
                    {
                        pressPos -= prevPos - mouse.y;
                        if(pressPos < 0) pressPos = 0;
                    }else
                    {
                        pressPos += mouse.y - prevPos;

                        if(pressPos > container.height - scrollBar.height)
                        {
                            pressPos = container.height - scrollBar.height;
                        }
                    }

                    scrollArea.contentY = pressPos / container.height * scrollArea.contentHeight

                    prevPos = mouse.y
                }
            } else
            {
                if(mouse.x > scrollBar.x
                        && mouse.x < scrollBar.width + scrollBar.x)
                {
                    if( prevPos > mouse.x )
                    {
                        pressPos -= prevPos - mouse.x;
                        if(pressPos < 0) pressPos = 0;
                    }else
                    {
                        pressPos += mouse.x - prevPos;

                        if(pressPos > container.width - scrollBar.width)
                        {
                            pressPos = container.width - scrollBar.width;
                        }
                    }

                    scrollArea.contentX = pressPos / container.width * scrollArea.contentWidth

                    prevPos = mouse.x
                }
            }
        }
    }
}
