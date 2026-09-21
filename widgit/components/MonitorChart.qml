import QtQuick 2.15

Rectangle {
    property string title: "CPU"

    anchors.fill: parent

    color: "#202020"
    border.width: 1
    border.color: "grey"

    property int maxPoints: 15
    property int tickCounter: 0
    property var dataPoints: []

    Text {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        text: title + " Usage"
        color: "white"
        font.pixelSize: 16
        z: 1
    }

    Canvas {
        id: canvas

        anchors.fill: parent
        anchors.margins: 10

        onPaint: {
            var ctx = getContext("2d")

            ctx.clearRect(0, 0, width, height)

            ctx.fillStyle = "#202020"
            ctx.fillRect(0, 0, width, height)

            var left = 35
            var right = 10
            var top = 30
            var bottom = 25

            var graphWidth = width - left - right
            var graphHeight = height - top - bottom

            ctx.strokeStyle = "#303030"
            ctx.lineWidth = 1

            for (var y = 0; y <= 5; y++) {
                var gy = top + graphHeight * y / 5

                ctx.beginPath()
                ctx.moveTo(left, gy)
                ctx.lineTo(width - right, gy)
                ctx.stroke()
            }

            for (var x = 0; x < maxPoints; x++) {
                var gx = left + graphWidth * x / (maxPoints - 1)

                ctx.beginPath()
                ctx.moveTo(gx, top)
                ctx.lineTo(gx, height - bottom)
                ctx.stroke()
            }

            ctx.fillStyle = "#aaaaaa"
            ctx.font = "11px sans-serif"

            for (var label = 0; label <= 5; label++) {
                var value = 100 - label * 20
                var ly = top + graphHeight * label / 5

                ctx.fillText(value.toString(), 3, ly + 4)
            }


            if (dataPoints.length > 0) {

                ctx.strokeStyle = title === "CPU" ? "#00d9ff" : "#b47ef5"
                ctx.lineWidth = 2

                ctx.beginPath()

                for (var i = 0; i < dataPoints.length; i++) {

                    var px = left +
                             graphWidth *
                             i /
                             (maxPoints - 1)

                    var value2 = dataPoints[i]

                    // Keep value between 0 and 100
                    value2 = Math.max(0, Math.min(100, value2))

                    var py = top +
                             graphHeight *
                             (1 - value2 / 100)

                    if (i === 0)
                        ctx.moveTo(px, py)
                    else
                        ctx.lineTo(px, py)
                }

                ctx.stroke()

                ctx.fillStyle = title === "CPU" ? "#00d9ff" : "#b47ef5"

                for (var j = 0; j < dataPoints.length; j++) {

                    var pointX = left +
                                 graphWidth *
                                 j /
                                 (maxPoints - 1)

                    var pointValue = Math.max(
                        0,
                        Math.min(100, dataPoints[j])
                    )

                    var pointY = top +
                                 graphHeight *
                                 (1 - pointValue / 100)

                    ctx.beginPath()
                    ctx.arc(pointX, pointY, 3, 0, Math.PI * 2)
                    ctx.fill()
                }
            }
        }
    }

    Connections {
        target: client1

        function onCpuUsageChanged() {
            if (title === "CPU") {
                appendDataPoint(client1.cpuUsage)
            }
        }

        function onRamUsageChanged() {
            if (title === "Ram") {
                appendDataPoint(client1.ramUsage)
            }
        }
    }

    function appendDataPoint(value) {

        value = Number(value)

        if (isNaN(value))
            return

        dataPoints.push(value)

        if (dataPoints.length > maxPoints)
            dataPoints.shift()

        canvas.requestPaint()
    }
}
