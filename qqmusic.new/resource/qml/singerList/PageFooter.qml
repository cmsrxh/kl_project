import QtQuick 2.0
import com.hongjing.singerlist 1.0 as Singerlist

Item {
    id: pages
    property int currentIndex: -1

    Connections {
        target: Singerlist.Api
        onCurrentPageChanged: {
            listAll(Singerlist.Api.currentPage, Singerlist.Api.singerPages)
        }
    }

    Component.onCompleted: {
        listAll(Singerlist.Api.currentPage, Singerlist.Api.singerPages)
    }

    function listAll(cur, nums)
    {
        pageModel.clear();
        if (nums < 1)
        {
            return;
        } else if (nums < 2)
        {
            pageModel.append({pageIndex: "1"})
            return;
        }

        // console.log ("cur =", cur, "nums =", nums)
        pageModel.append({pageIndex: "Previous"})
        if (nums < 3)
        {
            pageModel.append({pageIndex: "1"})
            pageModel.append({pageIndex: "2"})
        } else if (cur < 2)
        {
            pages.currentIndex = 1
            pageModel.append({pageIndex: "1"})
            pageModel.append({pageIndex: "2"})
            pageModel.append({pageIndex: "..."})
            pageModel.append({pageIndex: Number(nums).toString()})
        } else if (cur >= nums)
        {
            pages.currentIndex = 4
            pageModel.append({pageIndex: "1"})
            pageModel.append({pageIndex: "..."})
            pageModel.append({pageIndex: Number(cur-1).toString()})
            pageModel.append({pageIndex: Number(cur).toString()})
        } else if (cur === 2)
        {
            pages.currentIndex = 2
            pageModel.append({pageIndex: "1"})
            pageModel.append({pageIndex: "2"})
            pageModel.append({pageIndex: "3"})
            pageModel.append({pageIndex: "..."})
            pageModel.append({pageIndex: Number(nums).toString()})
        } else if (cur === nums - 1)
        {
            pages.currentIndex = 4
            pageModel.append({pageIndex: "1"})
            pageModel.append({pageIndex: "..."})
            pageModel.append({pageIndex: Number(nums-2).toString()})
            pageModel.append({pageIndex: Number(nums-1).toString()})
            pageModel.append({pageIndex: Number(nums).toString()})
        } else
        {
            pages.currentIndex = 4
            pageModel.append({pageIndex: "1"})
            pageModel.append({pageIndex: "..."})
            pageModel.append({pageIndex: Number(cur-1).toString()})
            pageModel.append({pageIndex: Number(cur).toString()})
            pageModel.append({pageIndex: Number(cur+1).toString()})
            pageModel.append({pageIndex: "..."})
            pageModel.append({pageIndex: Number(nums).toString()})
        }
        pageModel.append({pageIndex: "Next"})
    }

    Flow {
        id: flow
        spacing: 5
        anchors.centerIn: parent

        Repeater {
            id: repeater
            model: pageModel
            anchors.fill: parent
            Rectangle
            {
                id: bgRect
                color: pages.currentIndex == index ? "#5CB8FC" : "white"
                radius: 5

                Text {
                    id: mytext
                    anchors.centerIn: parent
                    color: pages.currentIndex == index ? "white" : "#5CB8FC"
                    text: pageIndex
                    font.pixelSize : 28
                    onContentWidthChanged: {
                        bgRect.width  = mytext.contentWidth + 20
                        bgRect.height = mytext.contentHeight + 18
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var value = pageModel.get(index).pageIndex
                        var cur = Number(value).valueOf()

                        //console.log("value =", value, "cur =", cur);
                        if (value === "Previous") //上一页
                        {
                            cur = -2
                        } else if (value === "Next") // 下一页
                        {
                            cur = -1
                        } else if  (value === "..." || index == pages.currentIndex)
                        {
                            console.log("Invalid Clicked.", index)
                            return
                        }
                        Singerlist.Api.qmlSingerList(7, cur);
                    }
                }
            }
        }
    }

    ListModel {
        id: pageModel
    }
}