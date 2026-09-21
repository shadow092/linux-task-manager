import QtQuick 2.15
import QtQuick
import QtQuick.Controls

Item {
width: parent.width
height: parent.height * 0.1
property var currentPage: ["p", "s"]

TabBar {
    id: tabBar
    width: parent.width
    height: parent.height
    background: Rectangle {
        color: "#1F2937"
    }

    TabButton {
        text: "Process"
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        contentItem: Text {
            text: parent.text
            color: parent.checked ? "#F9FAFB" : "#9CA3AF"
            font.pixelSize: 15
            font.bold: parent.checked
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            color: parent.checked ? "#425672" :
                   parent.hovered ? "#374151" :
                   "transparent"
        }
    }

    TabButton {
        text: "Performance"
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        contentItem: Text {
            text: parent.text
            color: parent.checked ? "#F9FAFB" : "#9CA3AF"
            font.pixelSize: 15
            font.bold: parent.checked
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            color: parent.checked ? "#425672" :
                   parent.hovered ? "#374151" :
                   "transparent"
        }
    }

    onCurrentIndexChanged: {
        navLayout.currentIndex = currentIndex
        client1.setCurrentPage(currentPage[currentIndex])
    }
}



}
