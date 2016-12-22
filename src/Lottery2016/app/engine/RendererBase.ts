import { float3x2 } from "./float3x2";
import { PromiseEvent } from "./PromiseEvent";
import { CanvasManager } from "./CanvasManager";

export abstract class RendererBase {
    private _order = 0;
    onSetOrder = new PromiseEvent<number>();

    private renderers = Array<RendererBase>();

    get order() {
        return this._order;
    }
    set order(value: number) {
        this._order = value;
        this.onSetOrder.fire(this.order);
    }

    update(time: number) {
        for (let renderer of this.renderers) {
            renderer.update(time);
        }
    }

    render(time: number) {
        for (let renderer of this.renderers) {
            renderer.render(time);
        }
    }

    addRenderer(renderer: RendererBase) {
        this.renderers.push(renderer);
        this.sortRenderers();
        renderer.onSetOrder.connect(() => this.sortRenderers());
    }

    center() {
        return {
            x: this.canvas.width() / 2,
            y: this.canvas.height() / 2
        };
    }

    centerTransform() {
        let center = this.center();
        return float3x2.translation(center.x, center.y);
    }

    private sortRenderers() {
        this.renderers.sort((a, b) => b.order - a.order);
    }

    constructor(protected canvas: CanvasManager) {
    }
}