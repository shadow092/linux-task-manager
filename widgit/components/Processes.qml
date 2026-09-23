import QtQuick
import QtQuick.Controls
import Qt.labs.qmlmodels

Item {
    anchors.fill: parent

    property var selectedPid: null


    readonly property real idColumnWidth: width * 0.20
    readonly property real nameColumnWidth: width * 0.50
    readonly property real ramColumnWidth: width * 0.30

    Rectangle {
        id: tableHeader

        z: 10

        anchors {
            left: parent.left
            right: parent.right
            top: navbar.bottom
        }

        height: 40
        color: "#303030"

        Row {
            anchors.fill: parent
            spacing: 0

            Rectangle {
                width: idColumnWidth
                height: parent.height
                color: "transparent"

                border.width: 1
                border.color: "#555555"

                Text {
                    anchors.centerIn: parent
                    color: "white"
                    font.bold: true
                    text: "ID"
                }

            }

            Rectangle {
                width: nameColumnWidth
                height: parent.height
                color: "transparent"

                border.width: 1
                border.color: "#555555"

                Text {
                    anchors.centerIn: parent
                    color: "white"
                    font.bold: true
                    text: "Name"
                }
            }

            Rectangle {
                width: ramColumnWidth
                height: parent.height
                color: "transparent"

                border.width: 1
                border.color: "#555555"

                Text {
                    anchors.centerIn: parent
                    color: "white"
                    font.bold: true
                    text: "RAM"
                }
            }
        }
    }

    ScrollView {
        id: scrollView
        anchors {
            left: parent.left
            right: parent.right
            top: tableHeader.bottom
            bottom: tableFooter.top
        }

        clip: true

    Column{
        id:tableHolder
        width: scrollView.width


        Repeater{
        model: client1.processList


        delegate: Rectangle{
            width: parent.width
            height: 40
            color : modelData.pid === selectedPid ? "#3d6fa8" : "#303030"
            border.width: 1
            border.color: "#555555"

            Row{
            anchors.fill: parent

            Rectangle {
                width: idColumnWidth
                height: parent.height
                color: "transparent"

                border.width: 1
                border.color: "#555555"

                Text {
                    anchors.centerIn: parent
                    color: "white"
                    font.bold: true
                    text: index + 1
                }
            }

            Rectangle {
                width: nameColumnWidth
                height: parent.height
                color: "transparent"

                border.width: 1
                border.color: "#555555"

                Text {
                    anchors.centerIn: parent
                    color: "white"
                    font.bold: true
                    text: modelData.name
                }
            }

            Rectangle {
                width: ramColumnWidth
                height: parent.height
                color: "transparent"

                border.width: 1
                border.color: "#555555"

                Text {
                    anchors.centerIn: parent
                    color: "white"
                    font.bold: true
                    text: modelData.ramKb
                }
            }



        }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    selectedPid = modelData.pid
                }
            }
        }

        }/*-----------------Repeater end--------------------------*/
    }}

    Rectangle {
        id:tableFooter
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        height: 50
        color: "#303030"

        border.width: 1
        border.color: "#555555"

        Button {
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
            text: "Kill Process"

                background: Rectangle {
                    color: parent.pressed
                           ? "#1f4f7a"
                           : parent.hovered
                             ? "#3d6fa8"
                             : "#303030"

                    border.color: "#555555"
                    border.width: 1
                    radius: 5
                }

                onClicked: {
                        client1.killProcess(selectedPid)
                }

        }
    }
}
