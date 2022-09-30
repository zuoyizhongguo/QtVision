import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtMultimedia

Window {
    width: 1000
    height: 600
    visible: true
    title: qsTr("QtQuickMultiCameras")

    Item {
        id: topItem
        anchors.fill: parent
        anchors.margins: 20
        anchors.bottomMargin: 100
        Rectangle{
            id: leftRectangle
            color: "gray"
            anchors.fill: parent
            anchors.rightMargin: parent.width*0.5+10
            Image {
                id: imageLeft
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                asynchronous: false
                cache: false
                Connections{                            //元素，信号与槽
                    property bool counter: false
                    target: hub                        //信号来源
                    function onCallQmlShowImg(){        //槽
                        counter=!counter
                        //imageLeft.source="image://live/"+"0#"+counter
                        imageLeft.source="image://live/"+counter
                    }
                }
            }
        }
        Rectangle{
            id: rightRectangle
            color:"gray"
            anchors.fill: parent
            anchors.leftMargin: parent.width*0.5+10
            Image {
                id: imageRight
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                asynchronous: false
                cache: false

                Connections{                            //元素，信号与槽
                    property bool counter: false
                    target: hub                        //信号来源
                    function onCallQmlShowImg2(){        //槽
                        counter=!counter
                        imageRight.source="image://live/"+"0#"+counter
                    }
                }                
            }
        }
    }
    Item {
        id: bottomItem
        width: parent.width
        anchors.top: topItem.bottom
        anchors.bottom: parent.bottom
        Row{
            anchors.centerIn: parent
            spacing: 20
            Button{
                text:qsTr("打开相机")
                width: 100
                height: 50
                font.bold: true
                font.pointSize: 12
                onClicked: {
                    hub.openCam()
                }
            }
            Button{
                text: qsTr("开始拉流")
                width: 100
                height: 50
                font.bold: true
                font.pointSize: 12
                onClicked: {
                    hub.startCam()
                }
            }
            Button{
                text:qsTr("停止拉流")
                width: 100
                height: 50
                font.bold: true
                font.pointSize: 12
                onClicked: {
                    hub.stopCam()
                }
            }
            Button{
                text:qsTr("关闭相机")
                width: 100
                height: 50
                font.bold: true
                font.pointSize: 12
                onClicked: {
                    hub.closeCam()
                }
            }
        }
    }
}
