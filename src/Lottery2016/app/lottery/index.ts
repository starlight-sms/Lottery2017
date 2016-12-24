import { RendererBase } from "../engine/RendererBase";
import { CanvasManager } from "../engine/CanvasManager";
import { LotteryController } from "./controller";
import { b2ShapeType } from "./Box2D/Collision/Shapes/b2Shape";
import { b2CircleShape } from "./Box2D/Collision/Shapes/b2CircleShape";
import { b2PolygonShape } from "./Box2D/Collision/Shapes/b2PolygonShape";
import { float3x2 } from "../engine/float3x2";
import { Person } from "./api";
import * as Rx from "rx";

export class LotteryRenderer extends RendererBase {
    controller = new LotteryController();

    constructor(protected canvas: CanvasManager) {
        super(canvas);
        this.controller.Reset(10);
    }

    update(time: number, frameTime: number) {
        this.controller.Update(frameTime);
    }

    render(time: number, frameTime: number) {
        this.controller.GetBodies()
            .subscribe(body => {
                let pos = body.GetPosition();
                let angle = body.GetAngle();

                this.canvas.save();
                let scale = 0.1 * Math.min(this.canvas.width(), this.canvas.height());
                this.canvas.setTransform(float3x2
                    .rotation(angle)
                    .translation(pos.x, pos.y)
                    .scale(scale, scale));
                this.canvas.lineWidth = 1 / scale;

                let person = <Person>body.GetUserData();

                GetBox2DAll(body.GetFixtureList())
                    .subscribe(fixture => {
                        let shapeType = fixture.GetShape().GetType();
                        switch (shapeType) {
                            case b2ShapeType.e_circleShape:
                                {
                                    let shape = <b2CircleShape>fixture.GetShape();
                                    this.canvas.drawCircle(
                                        0, 0,
                                        shape.m_radius,
                                        "red");
                                }
                                break;
                            case b2ShapeType.e_polygonShape:
                                {
                                    let shape = <b2PolygonShape>fixture.GetShape();

                                    this.canvas.beginPath();

                                    this.canvas.moveTo(shape.m_vertices[0].x, shape.m_vertices[1].y);
                                    for (let v = 1; v < shape.m_count; ++v) {
                                        this.canvas.lineTo(shape.m_vertices[v].x, shape.m_vertices[v].y);
                                    }
                                    this.canvas.closePath();
                                    this.canvas.stroke(person && person.Color || "black");
                                }
                                break;
                            default:
                                console.warn(`Unknown shape type: ${shapeType}`);
                                break;
                        }
                    });
                this.canvas.restore();
                
                if (person !== null) {
                    let text = this.getTextCanvas(person.Name.toString());
                    this.canvas.save();
                    this.canvas.setTransform(float3x2
                        .rotation(angle, pos.x * scale, pos.y * scale));
                    this.canvas.ctx.drawImage(text,
                        pos.x * scale - text.width / 3,
                        pos.y * scale - text.height / 2);
                    this.canvas.restore();
                }
            });
    }

    textCache = <{ [key: string]: HTMLCanvasElement }>{
    };
    getTextCanvas(text: string) {
        let canvas = this.textCache[text];
        if (canvas === undefined) {
            canvas = document.createElement("canvas");
            canvas.width = this.canvas.ctx.measureText(text).width;
            canvas.height = 14;

            let ctx = new CanvasManager(canvas);
            ctx.font = "12px Consolas";
            ctx.drawText(text, 0, 0, `black`);
            this.textCache[text] = canvas;
        }
        return canvas;
    }
}

function GetBox2DAll<T extends { GetNext(): T }>(t: T | null) {
    return Rx.Observable.create<T>(output => {
        while (t !== null) {
            output.onNext(t);
            t = t.GetNext();
        }
    });
}