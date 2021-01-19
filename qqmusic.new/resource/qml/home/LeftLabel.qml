import QtQuick 2.0
import QtQuick.Controls 2.0
import com.hongjing.home 1.0 as Home

import "../public"

Item {
    id: ctrlLab
    property int index: Home.Api.leftTabIndex

    Row {
        id: topbar
        anchors.fill: parent
        spacing: 15

        Repeater {
            anchors.fill: parent
            model: listModel
            Item {
                id: lab
                y: 15
                width: 40
                height: 50
                Image {
                    x: 5
                    y: 3
                    width: 35
                    height: 35
                    //anchors.centerIn: parent
                    smooth: true
                    fillMode: Image.PreserveAspectFit
                    source: imagePath
                }

                LineCtrl {
                    width: parent.height
                    height: ctrlLab.index == index ? 6 : 0
                    anchors.bottom: parent.bottom
                    fillColor: "blue"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: Home.Api.qmlMainTabClick(index)
                }
            }
        }
    }

    //{ name: 'Setting',    title: '设置' },
    //{ name: 'Favorite',   title: '收藏' },
    //{ name: 'Search',     title: '搜索' },
    //{ name: 'SingerList', title: '歌手列表' },
    //{ name: 'PlayList',   title: '歌单推荐' }
    ListModel {
        id: listModel
        ListElement {
            imagePath: "qrc:/image/svg/Setting.svg"
        }
        ListElement {
            imagePath: "qrc:/image/svg/Favorite.svg"
        }
        ListElement {
            imagePath: "qrc:/image/svg/Search.svg"
        }
        ListElement {
            imagePath: "qrc:/image/svg/SingerList.svg"
        }
        ListElement {
            imagePath: "qrc:/image/svg/PlayList.svg"
        }
    }
}
