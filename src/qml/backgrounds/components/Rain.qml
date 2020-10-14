/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
import QtQuick 2.0
import QtQuick.Layouts 1.2
Canvas {
    id: mycanvas

    property var particles: []
    onPaint: {
        var ctx = getContext("2d");
        ctx.strokeStyle = 'rgba(255,255,255,0.5)';
        ctx.lineWidth = 3;
        ctx.lineCap = 'round';
        ctx.clearRect(0, 0, width, height);

        function draw() {
            for (var c = 0; c < particles.length; c++) {
                var p = particles[c];
                ctx.beginPath();
                ctx.moveTo(p.x, p.y);
                ctx.lineTo(p.x + p.l * p.xs, p.y + p.l * p.ys);
                ctx.stroke();
            }
            move();
        }

        function move() {
            for (var b = 0; b < particles.length; b++) {
                var p = particles[b];
                p.x += p.xs;
                p.y += p.ys;
                if (p.x > width || p.y > height) {
                    p.x = Math.random() * width;
                    p.y = -20;
                }
            }
        }
        draw();
    }
    Timer {
        interval: 30
        running: true
        repeat: true
        onTriggered: parent.requestPaint()
    }

    Component.onCompleted: {
        var init = [];
        var maxParts = 300;
        for (var a = 0; a < maxParts; a++) {
            init.push({
                          x: Math.random() * width,
                          y: Math.random() * height,
                          l: Math.random() * 5,
                          xs: -4 + Math.random() * 4 + 2,
                          ys: Math.random() * 10 + 10
                      })
        }

        particles = [];
        for (var b = 0; b < maxParts; b++) {
            particles[b] = init[b];
        }
    }
}
//Item {
//    property alias backGroundColor: cloud.backGroundColor
//    anchors.fill: parent
//    Cloudy {
//        id: cloud
//        anchors.fill: parent
//    }

//    RowLayout {
//        anchors.horizontalCenter: parent.horizontalCenter
//        spacing: parent.width / 4
//        Rectangle {
//            width: 2
//            height: 80
//            color: "white"
//            opacity: 0.8
//            y: - height
//        }
//        Rectangle {
//            width: 2
//            height: 50
//            color: "white"
//            opacity: 0.5
//            y: - height
//        }
//        Rectangle {
//            width: 3
//            height: 80
//            color: "white"
//            opacity: 0.7
//            y: - height
//        }
//        Rectangle {
//            width: 5
//            height: 80
//            color: "white"
//            opacity: 0.4
//            y: - height
//        }
//        y: 80
//        SequentialAnimation on y {
//            loops: Animation.Infinite
//            PropertyAnimation { to: 400; duration: 4000 }
//        }
//    }

//    RowLayout {
//        anchors.leftMargin: 50
//        spacing: parent.width / 4
//        Rectangle {
//            width: 2
//            height: 80
//            color: "white"
//            opacity: 0.8
//            y: - height
//        }
//        Rectangle {
//            width: 2
//            height: 50
//            color: "white"
//            opacity: 0.5
//            y: - height
//        }
//        Rectangle {
//            width: 3
//            height: 80
//            color: "white"
//            opacity: 0.7
//            y: - height
//        }
//        Rectangle {
//            width: 5
//            height: 80
//            color: "white"
//            opacity: 0.4
//            y: - height
//        }
//        y: 150
//        SequentialAnimation on y {
//            loops: Animation.Infinite
//            PropertyAnimation { to: 500; duration: 5000 }
//        }
//    }
//}
