import { RendererBase } from "./RendererBase";
import { CanvasManager } from "./CanvasManager";

export class AlignToWindowUtil extends RendererBase {
    constructor(canvas: CanvasManager) {
        super(canvas);
        this.resize();
        window.addEventListener("resize", ev => this.resize());
    }

    resize() {
        this.canvas.resizeTo(window.innerWidth, window.innerHeight);
    }
}