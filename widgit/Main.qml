import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

import MyApp 1.0

import "components"


Window {
    id: root

    width: 900
    height: 500
    visible: true
    title: "SysMon Widget"

    property int result: -1

    Client{
        id: client1
        Component.onCompleted:{
           result = connectToServer()
        }
    }

    Rectangle{
            visible:root.result < 0
            anchors.fill: parent

            Button {
            anchors.centerIn: parent
            text:  "Refresh"
            font.pixelSize: 20
            onClicked:{root.result = client1.connectToServer()}
            }

    }

Rectangle{
        visible:root.result >= 0
        anchors.fill: parent

        Navbar{id:navBar}

        StackLayout{
            id:navLayout
            anchors{
                top:navBar.bottom; bottom: parent.bottom; right:parent.right; left:parent.left;
            }

// -----------page1--------------

        Processes{id:processes}

// --------------page2-------------

        MonitorPage{id:monitorPage}

// --------------------end of layout -------------------
        }




        }



}
