import QtQuick 2.0

Canvas
{
    id: drawLine
    property color fillColor: "red"
    width: 30; height: 1
    //antialiasing: false  //去掉据曲

    onFillColorChanged:
    {
        drawLine.requestPaint();
    }

    onPaint:
    {       
        var ctx = drawLine.getContext("2d")
        //        ctx.antialiasing = false//去掉据曲
        //        ctx.translate(0.5, 0.5);//去掉据曲
        ctx.lineCap="round"; //设置lineCap属性即端点为半圆形

        ctx.moveTo(0, 0) //起点
        ctx.lineTo(drawLine.width, 0)  //线(到终点)
        ctx.lineWidth = drawLine.height;
        ctx.strokeStyle = fillColor;
        ctx.stroke() //描边，把线画出来
    }
}
