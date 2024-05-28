/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
import QtQuick
import QtQuick.Layouts

Canvas {
    id: mycanvas

    property var particles: []

    renderStrategy: Canvas.Threaded
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
                ctx.lineTo(p.x, p.y + p.l * p.ys);
                ctx.stroke();
            }
            move();
        }
        function move() {
            for (var b = 0; b < particles.length; b++) {
                var p = particles[b];
                p.y += p.ys;
                if (p.x < 1 || p.x > width || p.y > height) {
                    p.x = Math.random() * width;
                    p.y = -20;
                }
            }
        }
        draw();
    }
    Timer {
        id: animationTimer
        interval: 16
        running: true
        repeat: true
        onTriggered: parent.requestPaint()
    }

    Component.onCompleted: {
        var init = [];
        var maxParts = 80;
        for (var a = 0; a < maxParts; a++) {
            init.push({
                x: Math.random() * width,
                y: Math.random() * height,
                l: 2 + Math.random() * 4,
                ys: Math.random() * 10 + 10
            });
        }
        particles = [];
        for (var b = 0; b < maxParts; b++) {
            particles[b] = init[b];
        }
    }
}
