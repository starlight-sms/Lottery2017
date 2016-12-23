import { RendererBase } from "../engine/RendererBase";
import { CanvasManager } from "../engine/CanvasManager";
import { LotteryController } from "./controller";

export class LotteryRenderer extends RendererBase {
    controller = new LotteryController();

    constructor(protected canvas: CanvasManager) {
        super(canvas);
        this.controller.Reset(10);
    }

    render(time: number, frameTime: number) {
        super.render(time, frameTime);
    }
}