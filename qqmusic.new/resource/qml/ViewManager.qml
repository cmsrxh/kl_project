import QtQuick 2.5
import page.item 1.0

PageItem {
    id: root

    Component.onCompleted: {
        console.log("completed.", width, height)
        qmlItemOver()
    }

    // 在C++代码中被调用,用于创建窗口
    function createItem(url) {
        // console.log("qml: ", url)
        var component = Qt.createComponent(url);

        if (component.status === Component.Ready)
        {
            var object = component.createObject(root);

            object.width  = root.width;
            object.height = root.height;

            setCurrentCreateResult(component, object)

            return true
        } else
        {
            setCurrentErrorString(component.errorString())
        }
        return false
    }

    function animateShow(animate) {
        var showItem = getNeedShowItem();
        internal.transition(showItem, animate)
    }

    QtObject
    {
        id: internal

        property int duration: 250
        property var fadeAnimation: ParallelAnimation
        {
            running: false

            // OpacityAnimator
            ScaleAnimator
            {
                id: fadeInAnimator
                from: 0
                to: 1
                duration: internal.duration
            }
            // OpacityAnimator
            ScaleAnimator
            {
                id: fadeOutAnimator
                from: 1
                to: 0
                duration: internal.duration
            }

            onStopped:
            {
                internal.transitionEnd( fadeInAnimator.target, fadeOutAnimator.target );
            }
        }

        function transition( enterItem, animate )
        {
            if (fadeAnimation.running) {
                fadeAnimation.stop();
                internal.transitionEnd( fadeInAnimator.target, fadeOutAnimator.target );
            }

            if ( animate === false || internal.duration === 0)
            {
                enterItem.opacity = 1;
                transitionEnd( enterItem, root.currentItem );
            }
            else // 默认使用动画效果
            {
                fadeOutAnimator.target = root.currentItem;
                root.currentItem.z = 0;

                enterItem.z = 1;
                enterItem.visible = true;

                fadeInAnimator.target = enterItem;
                fadeAnimation.start( );
            }
        }

        function transitionEnd( enterItem, exitItem )
        {
            // 直接进行转换
            if ( exitItem )
            {
                exitItem.visible = false;
            }

            enterItem.visible = true;
            root.currentItem = enterItem;
        }
    }
}

/*
Item {
    id: root

    anchors.fill: parent

    property string source: ""
    property var currentItem;
    //property var items: []

    function createItem(url) {
        if ( !ViewScript.items ) ViewScript.items = [ ];// 修复起始页面的时候items为undefined报的错误

        for ( var i in ViewScript.items )
        {
            console.log("items : " + i + " url: " + ViewScript.items[i].url);
            if ( ViewScript.items[i].url === url )
            {
                console.log("find exist item: " + url);
                return ViewScript.items[i];
            }
        }

        var component = Qt.createComponent(url);

        if (component.status === Component.Ready)
        {

            console.log("create object: " + url)

            var object = component.createObject(root);

            var item = {
                "url": url,
                "component": component,
                "object": object
            };

            if (object.width === 0) {
                object.width = root.width;
            }

            if (object.height === 0) {
                object.height = root.height;
            }

            ViewScript.items.push( item );

            return item;
        }else if ( component.status === Component.Error )
        {
            console.log("Qml Create Component Error", component.errorString( ) );
        } else
        {
            console.log("Qml Create Component failed", component.errorString( ) );
        }
    }

    function show(url) {
        if (source === "") {
            return;
        }

        var item = createItem(url);

        if (!item) {
            return;
        }

        if (currentItem) {
            currentItem.object.visible = false;
        }

        item.object.visible = true;

        currentItem = item;
    }

    Component.onCompleted: {
        console.log("on completed: " + source)
        show(source);
    }

    onSourceChanged: {
        console.log("on source changed: " + source)
        show(source);
    }

    Component.onDestruction:
    {
        for ( var i in ViewScript.items )
        {
            console.log("destroy item: " + ViewScript.items[i].url);

            ViewScript.items[i].component.destroy();
            ViewScript.items[i].object.destroy();
        }
    }
}
*/

/*
Item
{
    id: viewManager

    property var initialItem
    property Item currentItem
    property var mainItem: [ ]
    property var items: [ ]
    property alias bgImage: bgImg.source
    property bool needShowTop: false

    Image {
        id: bgImg
        anchors.fill: parent
    }


    QtObject
    {
        id: internal

        property int duration: 0
        property var fadeAnimation: ParallelAnimation
        {
            running: false

            OpacityAnimator
            {
                id: fadeInAnimator
                from: 0
                to: 1
                duration: internal.duration
            }
            OpacityAnimator
            {
                id: fadeOutAnimator
                from: 1
                to: 0
                duration: internal.duration
            }

            onStopped:
            {
                internal.transitionEnd( fadeInAnimator.target, fadeOutAnimator.target );
            }
        }

        function transition( enterItem, animate )
        {
            if ( viewManager.currentItem === enterItem ) return;// 没有任何操作

            if ( animate === false )
            {
                enterItem.opacity = 1;
                transitionEnd( enterItem, viewManager.currentItem );
            }
            else // 默认使用动画效果
            {
                fadeOutAnimator.target = viewManager.currentItem;
                viewManager.currentItem.opacity = 1;

                enterItem.opacity = 0;
                enterItem.visible = true;

                fadeInAnimator.target = enterItem;
                fadeAnimation.start( );
            }
        }

        function transitionEnd( enterItem, exitItem )
        {
            // 直接进行转换
            if ( exitItem )
            {
                exitItem.visible = false;
            }

            enterItem.visible = true;
            viewManager.currentItem = enterItem;
        }
    }

    function transition( itemURL, properties, otherArg )
    {
        // 判断是否在items中
        console.log("Create Start: ", itemURL);

        //说明有父窗口，并且不需要由当前管理器创建，是由父窗口创建
        if( otherArg.winType !==  otherArg.parentWin
                && !otherArg.needCreate)
        {
            judgeWindow( undefined , otherArg);
            return;
        }

        for ( var i in items )
        {
            if ( items[i].url === itemURL )
            {
                judgeWindow( items[i].object , otherArg);
                internal.transition( items[i].object, otherArg.animate );
                return;
            }
        }

        var component = Qt.createComponent( itemURL );

        // 异步存在加载不出来的情况,所以统一使用同步加载的方式
        if ( component.status === Component.Ready )
        {
            var item = component.createObject( viewManager, properties );
            var info = {
                "url": itemURL,
                "component": component,
                "object": item
            };

            judgeWindow( item , otherArg);

            if ( !items ) items = [ ];// 修复起始页面的时候items为undefined报的错误
            items.push( info );

            internal.transition( item, otherArg.animate );
        }
        else if ( component.status === Component.Error )
        {
            console.log("Qml Create Component Error", component.errorString( ) );
        } else
        {
            console.log("Qml Create Component failed", component.errorString( ) );
        }
    }

    function judgeWindow ( item, arg ) //判断窗口的父子结构，和继承关系
    {
        if( arg.winType === arg.parentWin ) //表示是主窗口
        {
            mainItem = [ ]
            mainItem.push( { "object": item,
                         "winType": arg.winType })
            console.log("Line 1: ", arg.parentWin, arg.winType, item)
        }else   //表示是子窗口
        {
            for ( var i in mainItem )
            {
                var find;
                console.log("Line 2: ", mainItem[i].winType, arg.parentWin, mainItem[i].object)
                if ( mainItem[i].winType === arg.parentWin ) //找到父窗口
                {
                    find = mainItem[i];
                    break;
                }
            }

            if( find )
            {
                console.log("Find Result: ", find.winType, arg.needCreate, arg.otherArg)

                if( arg.needCreate )
                {
                    mainItem.push( { "object": item,
                                     "winType": arg.winType} );
                    //下一部需要把item传给， find 即父窗口
                } else
                {
                    find.object.curIndex = arg.otherArg;
                    console.log("Set Index: ", find.object, find.object.curIndex, arg.otherArg)
                }
            } else
            {
                console.log("+++++++++++judgeWindow switch window invalid+++++++++++++")
            }
        }

    }

    onInitialItemChanged:
    {
        if ( typeof( initialItem ) == "string" )
        {
            viewManager.transition( initialItem, {
                                           "x": 0,
                                           "y": 0,
                                           "width": 600,
                                           "height": 1024
                                       } , {"animate": false});
        }
        else if ( typeof( initialItem ) == "object" )
        {
            internal.transition( initialItem, { "animate": false } );
        }
    }

    Component.onDestruction:
    {
        for ( var i in items )
        {
            items[i].component.destroy();
            items[i].object.destroy();
        }
    }
}
*/
