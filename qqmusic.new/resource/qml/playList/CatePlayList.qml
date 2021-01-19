import QtQuick 2.0
import com.hongjing.catePlaylist 1.0 as CatePL
import "../public"

MainPageBase1 {

    Component.onCompleted: {
       CatePL.Api.qmlObtainCatePlayList()
   }

   handle: Rectangle {
       CategoryList {
           id: cate
           width: 200
           anchors.top: parent.top
           // anchors.topMargin: 80
           anchors.bottom: parent.bottom
       }

       PlayList {
           anchors.left: cate.right
           anchors.right: parent.right
           anchors.top: parent.top
           anchors.bottom: parent.bottom
       }
   }

   onNextPage: {
       console.log("catePlaylist, next page clicked")
   }

   onPrevPage: {
       console.log("catePlaylist, prev page clicked")
   }

   onUpToTop: {
       console.log("catePlaylist, up to top clicked")
   }
}
