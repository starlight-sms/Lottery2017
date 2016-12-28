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
                                    this.canvas.stroke(person && person.Color || "black", 1 / scale);
                                }
                                break;
                            default:
                                console.warn(`Unknown shape type: ${shapeType}`);
                                break;
                        }
                    });
                
                if (person !== null) {
                    let text = this.getTextCanvas(person.Name.toString());
                    this.canvas.save();
                    this.canvas.setTransform(float3x2
                        .scale(scale / 95, scale / 95, text.width / 2, text.height / 2)
                        .rotation(angle, text.width / 2, text.height / 2)
                        .translation(
                            pos.x * scale - text.width / 2, 
                            pos.y * scale - text.height / 2)
                        );
                    this.canvas.ctx.drawImage(text,
                        0, 
                        0);
                    this.canvas.strokeRect(0, 0, text.width, text.height, "red");
                    
                    this.canvas.restore();
                }
                this.canvas.restore();
            });
    }

    textCache = <{ [key: string]: HTMLCanvasElement }>{
    };
    getTextCanvas(text: string) {
        let canvas = this.textCache[text];
        if (canvas === undefined) {
            canvas = document.createElement("canvas");

            canvas.width = 80;
            canvas.height = 14;
            let ctx = new CanvasManager(canvas);
            ctx.font = "12px 微软雅黑";
            ctx.drawText(text, ctx.width() / 2 - ctx.ctx.measureText(text).width / 2, 0, `black`);
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