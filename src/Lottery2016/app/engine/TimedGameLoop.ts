import { SimpleGameLoop } from "./SimpleGameLoop";
import { PromiseEvent } from "./PromiseEvent";

export class TimedGameLoop extends SimpleGameLoop {
    private _totalRenderTime = 0;
    private _lastFrameTime = new Date();
    onRunningSlow = new PromiseEvent<number>();
    frameTime = 0;

    get totalRenderTime() {
        return this._totalRenderTime;
    }

    get lastFrameTime() {
        return this._lastFrameTime;
    }

    update() {
        super.update();
        let thisFrameTime = new Date();
        let frameTime = (thisFrameTime.getTime() - this._lastFrameTime.getTime()) / 1000;
        this.frameTime = frameTime;
        this._lastFrameTime = thisFrameTime;
        this._totalRenderTime += frameTime;

        if (frameTime > 0.25) {
            this.onRunningSlow.fire(frameTime);
            return true;
        }
        return false;
    }

    roundPerSecond(round: number) {
        return round * this.totalRenderTime * Math.PI * 2;
    }

    anglePerSecond(angle: number) {
        return angle / 360 * this.totalRenderTime * Math.PI * 2;
    }
}