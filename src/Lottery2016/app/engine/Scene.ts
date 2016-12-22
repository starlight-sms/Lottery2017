import { RendererBase } from "./RendererBase";
import { CanvasManager } from "./CanvasManager";
import { TimedGameLoop } from "./TimedGameLoop";

export class Scene extends RendererBase {
    protected canvas: CanvasManager;
    private loop = new TimedGameLoop();

    getCanvas() {
        return this.canvas;
    }

    render(time: number) {
        this.canvas.clear();
        super.render(time);
    }

    constructor(canvasElement: HTMLCanvasElement) {
        super(new CanvasManager(<HTMLCanvasElement>document.querySelector("canvas")));
        this.loop.onRender.connect(time => this.render(time));
        this.loop.onUpdate.connect(time => this.update(time));
    }
}