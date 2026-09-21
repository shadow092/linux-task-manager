import QtQuick 2.15
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


    Rectangle{
        anchors{
            top:navBar.bottom; bottom: parent.bottom; right:parent.right; left:parent.left;
        }
    Rectangle{
        id: monitorBar
        width: parent.width *0.2
        anchors{
            top:parent.top; bottom: parent.bottom;  left:parent.left;
        }
        border.color: "grey"
        border.width: 2

        Column {

            anchors.fill: parent

            TabButton {
                id: cpuPageButton
                checked: true
                width: parent.width
                height: 100

                text: "CPU : " + client1.cpuUsage + "%"

                ButtonGroup.group: monitorGroup

                onClicked: {
                    monitorLayout.currentIndex = 0
                }

                background: Rectangle {
                    border.color: "gray"
                    border.width: 1

                    color: cpuPageButton.checked || cpuPageButton.hovered
                           ? "gray"
                           : "transparent"
                }

                contentItem: Text {
                    text: cpuPageButton.text
                    color: "black"
                    font.pixelSize: 15
                    font.bold: cpuPageButton.checked

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

            }

            TabButton {
                id: ramPageButton

                width: parent.width
                height: 100

                text: "RAM : " + client1.ramUsage + "%"

                ButtonGroup.group: monitorGroup

                onClicked: {
                    monitorLayout.currentIndex = 1
                }

                background: Rectangle {
                    border.color: "gray"
                    border.width: 1

                    color: ramPageButton.checked || ramPageButton.hovered
                           ? "gray"
                           : "transparent"
                }

                contentItem: Text {
                    text: ramPageButton.text
                    color: "black"
                    font.pixelSize: 15
                    font.bold: ramPageButton.checked

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

            }
        }}


        Rectangle{
            anchors{
                top:parent.top; bottom: parent.bottom; right:parent.right; left:monitorBar.right;
            }



        StackLayout{
                id:monitorLayout
                anchors.fill: parent

// ---------------------------------- cpu page ----------------------
                Rectangle{
                        anchors.fill: parent


                        MonitorChart{id:cpuChart
                        title: "CPU" }
                    }

// ------------------------ram page -----------------
                Rectangle{
                        anchors.fill: parent

                        MonitorChart{id:ramChart
                        title: "Ram" }

                }


        } // ------------ layout end -------------

        }

    }

