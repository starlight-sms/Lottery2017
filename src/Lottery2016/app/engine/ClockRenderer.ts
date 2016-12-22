import { RendererBase } from "./RendererBase";
import { float3x2 } from "./float3x2";

export class ClockRenderer extends RendererBase {
    radius() {
        return (Math.min(this.canvas.width(), this.canvas.height()) - 15) / 2;
    }

    fontScale() {
        return this.radius() / 300;
    }

    centerTransform() {
        return float3x2.translation(
            this.canvas.width() / 2,
            this.canvas.height() / 2);
    }

    second() {
        return new Date().getSeconds() + new Date().getMilliseconds() / 1000;
    }

    minute() {
        return new Date().getMinutes() + this.second() / 60;
    }

    hour() {
        return new Date().getHours() + this.minute() / 60;
    }

    pointers() {
        let second = {
            angle: this.second() * (360 / 60),
            length: this.radius() * 0.95,
            width: 3,
            color: "orange"
        };

        let minute = {
            angle: this.minute() * (360 / 60),
            length: this.radius() * 0.75,
            width: 6,
            color: "blue"
        };

        let hour = {
            angle: this.hour() * (360 / 12),
            length: this.radius() * 0.55,
            width: 9,
            color: "red"
        };

        return [second, minute, hour];
    }

    clockTime() {
        function padding2(v: number) {
            if (v < 10) return `0${v.toFixed(0)}`;
            else return `${v.toFixed(0)}`;
        }

        return `${padding2(this.hour())}:${padding2(this.minute())}:${padding2(this.second())}`;
    }

    render(time: number) {
        this.drawCircle();
        this.drawNumbers();
        this.drawCenterTime();
        this.drawPointers();
    }

    drawPointers() {
        for (let line of this.pointers()) {
            this.canvas.setTransform(float3x2
                .rotation((line.angle - 90) / 360 * Math.PI * 2)
                .multiply(this.centerTransform()));
            this.canvas.drawLine(0, 0, line.length, 0, line.color, line.width * this.fontScale());
        }
    }

    drawCircle() {
        this.canvas.setTransform(this.centerTransform());
        this.canvas.drawCircle(0, 0, this.radius(), "black", 2);
        this.canvas.fillCircle(0, 0, this.radius(), "gray");
    }

    drawCenterTime() {
        this.canvas.setTransform(float3x2
            .scale(this.fontScale(), this.fontScale())
            .multiply(this.centerTransform().translation(0, 20)));
        this.canvas.drawTextAtCenter(this.clockTime(), 0, 0, "white");
    }

    drawNumbers() {
        for (let i = 0; i < 12; ++i) {
            let hour = i + 1;
            this.canvas.setTransform(float3x2
                .scale(this.fontScale(), this.fontScale())
                .translation(0, -this.radius() * 0.9)
                .rotation(i * (360 / 12) / 360 * Math.PI * 2)
                .multiply(this.centerTransform()));

            this.canvas.drawTextAtCenter(`${hour}`, 0, 0, "white");
        }
        for (let i = 0; i < 60; ++i) {
            this.canvas.setTransform(float3x2
                .rotation(i * (360 / 60) / 360 * Math.PI * 2)
                .multiply(this.centerTransform()));
            if (i % 5 === 0) {
                this.canvas.drawLine(0, -this.radius() * 0.95, 0, -this.radius(), "white", 2);
            } else {
                this.canvas.drawLine(0, -this.radius() * 0.98, 0, -this.radius(), "white");
            }
        }
    }
}