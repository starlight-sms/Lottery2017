import { api, Person } from "./api";
import * as rx from "rx";
import { b2Vec2 } from "./Box2D/Common/b2Math";
import { b2World } from "./Box2D/Dynamics/b2World";
import { b2Body, b2BodyDef, b2BodyType } from "./Box2D/Dynamics/b2Body";
import { b2Fixture, b2FixtureDef } from "./Box2D/Dynamics/b2Fixture";
import { b2PolygonShape } from "./Box2D/Collision/Shapes/b2PolygonShape";
import { b2CircleShape } from "./Box2D/Collision/Shapes/b2CircleShape";

import * as KeyBoard from "../engine/KeyBoard";

export class LotteryController {
    world = new b2World(new b2Vec2(0, -100));

    GetBodies() {
        return rx.Observable.create<b2Body>(output => {
            let body = this.world.GetBodyList();
            while (body !== null) {
                output.onNext(body);
                body = body.GetNext();
            }
        });
    }

    GetPersonBodies() {
        return this.GetBodies()
            .filter(x => x.GetUserData() !== null);
    }

    GetBorderBodies() {
        return this.GetBodies()
            .filter(x => x.GetUserData() === null);
    }

    Update(frameTime: number) {
        const velocityIterations: number = 6;
        const positionIterations: number = 2;

        if (!KeyBoard.isKeyDown("p")) {
            this.world.Step(frameTime, velocityIterations, positionIterations);
        }        

        if (KeyBoard.isKeyDown(" ")) {
            this.GetBodies()
                .filter(x => x.GetUserData() === null)
                .subscribe(v => {
                    v.SetType(b2BodyType.b2_unknown);
                    
                    //this.world.DestroyBody(v);
                });
        }
    }

    Reset(count: number) {
        this.world.SetAllowSleeping(false);
        setTimeout(() => this.world.SetGravity(new b2Vec2(0, 0), true), 2000);
        let CreateBorder = (x: number, y: number, w: number, h: number) => {
            const groundBodyDef = new b2BodyDef();
            groundBodyDef.type = b2BodyType.b2_kinematicBody;
            groundBodyDef.position.Set(x, y);

            let groundBody = this.world.CreateBody(groundBodyDef);
            let groundBox = new b2PolygonShape();
            groundBox.SetAsBox(w, h);
            groundBody.CreateFixture(groundBox, 0);
        }
        CreateBorder(50, 25, 25, 0.2);
        CreateBorder(25, 50, 0.2, 25);
        CreateBorder(75, 50, 0.2, 25);
        CreateBorder(50, 75, 25, 0.2);

        let CreateShapeAtRandom = (dto: Person) => {
            // Define the dynamic body. We set its position and call the body factory.
            let bodyDef = new b2BodyDef();
            bodyDef.type = b2BodyType.b2_dynamicBody;
            bodyDef.position.Set(Math.random() * 40 + 30, Math.random() * 40 + 30);
            let body = this.world.CreateBody(bodyDef);
            body.SetUserData(dto);
            body.ApplyForce(
                new b2Vec2(Math.random() * 100, Math.random() * 100),
                new b2Vec2(1, 1), true);

            // Define another box shape for our dynamic body.
            if (Math.random() > 0.5) {
                let dynamicBox = new b2CircleShape(1.5)
                let fixtureDef = new b2FixtureDef();
                fixtureDef.shape = dynamicBox;
                fixtureDef.density = 5;
                fixtureDef.friction = 0;
                fixtureDef.restitution = 1;
                let fixture = body.CreateFixture(fixtureDef);
            } else {
                let dynamicBox = new b2PolygonShape();
                dynamicBox.SetAsBox(2, 1);
                let fixtureDef = new b2FixtureDef();
                fixtureDef.shape = dynamicBox;
                fixtureDef.density = 5;
                fixtureDef.friction = 0;
                fixtureDef.restitution = 1;
                let fixture = body.CreateFixture(fixtureDef);
            }
        }

        api.getUnluckyPersons().forEach(v => {
            console.log(v);
            CreateShapeAtRandom(v);
        });
    }
}