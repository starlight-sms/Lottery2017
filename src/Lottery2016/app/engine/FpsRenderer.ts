import { RendererBase } from "./RendererBase";

const RefreshFpsInterval = 1;
class FpsContext {
    fps = 0;
    private lastRecordTime = 0;
    private framesSinceLastRecord = 0;

    update(currentTime: number) {
        let interval = currentTime - this.lastRecordTime;
        if (interval > RefreshFpsInterval) {
            this.fps = this.framesSinceLastRecord / interval;
            this.lastRecordTime = currentTime;
            this.framesSinceLastRecord = 1;
        } else {
            this.framesSinceLastRecord += 1;
        }
    }
}

export class FpsRenderer extends RendererBase {
    private ctx = new FpsContext();

    update(time: number, frameTime: number) {
        this.ctx.update(time);
    }

    render(time: number, frameTime: number) {
        this.canvas.drawText(`FPS: ${this.ctx.fps.toFixed(0)}`, 0, 0, "blue");
    }
}