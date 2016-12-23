import { RendererBase } from "./RendererBase";
import { CanvasManager } from "./CanvasManager";
import { TimedGameLoop } from "./TimedGameLoop";

export class Scene extends RendererBase {
    protected canvas: CanvasManager;
    private loop = new TimedGameLoop();

    getCanvas() {
        return this.canvas;
    }

    render(time: number, frameTime: number) {
        this.canvas.clear();
        super.render(time, frameTime);
    }

    constructor(canvasElement: HTMLCanvasElement) {
        super(new CanvasManager(<HTMLCanvasElement>document.querySelector("canvas")));
        this.loop.onRender.connect(() => this.render(this.loop.totalRenderTime, this.loop.frameTime));
        this.loop.onUpdate.connect(() => this.update(this.loop.totalRenderTime, this.loop.frameTime));
    }
}