import { PromiseEvent } from "./PromiseEvent";

export class SimpleGameLoop {
    public onRender = new PromiseEvent<number>();
    public onUpdate = new PromiseEvent<number>();
    private keepsRendering = true;

    get totalRenderTime() {
        return new Date().getTime();
    }

    start() {
        this.keepsRendering = true;
        this.decideNextRender();
    }

    stop() {
        this.keepsRendering = false;
    }

    update(): boolean {
        this.onUpdate.fire(this.totalRenderTime);
        this.decideNextRender();
        return false;
    }

    render() {
        this.onRender.fire(this.totalRenderTime);
    }

    decideNextRender() {
        if (this.keepsRendering) {
            requestAnimationFrame(() => {
                let skipRender = this.update();
                if (!skipRender) {
                    this.render();
                }
            });
        }
    }

    constructor() {
        this.start();
    }
}