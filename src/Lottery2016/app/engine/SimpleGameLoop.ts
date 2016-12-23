import { PromiseEventVoid } from "./PromiseEvent";

export class SimpleGameLoop {
    public onRender = new PromiseEventVoid();
    public onUpdate = new PromiseEventVoid();
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
        this.onUpdate.fire();
        this.decideNextRender();
        return false;
    }

    render() {
        this.onRender.fire();
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