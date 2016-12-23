import { api } from "./api";
import * as _ from "lodash";

import { b2Vec2 } from "./Box2D/Common/b2Math";
import { b2World } from "./Box2D/Dynamics/b2World";
import { b2Body, b2BodyDef, b2BodyType } from "./Box2D/Dynamics/b2Body";
import { b2Fixture, b2FixtureDef } from "./Box2D/Dynamics/b2Fixture";
import { b2PolygonShape } from "./Box2D/Collision/Shapes/b2PolygonShape";

export class LotteryController {
    world: b2World;

    GetBodies() {

    }

    Tick() {

    }

    Reset(count: number) {
        // Define the gravity vector.
        const gravity: b2Vec2 = new b2Vec2(0, -10);

        // Construct a world object, which will hold and simulate the rigid bodies.
        const world: b2World = new b2World(gravity);

        // Define the ground body.
        const groundBodyDef: b2BodyDef = new b2BodyDef();
        groundBodyDef.position.Set(0, -10);

        // Call the body factory which allocates memory for the ground body
        // from a pool and creates the ground box shape (also from a pool).
        // The body is also added to the world.
        const groundBody: b2Body = world.CreateBody(groundBodyDef);

        // Define the ground box shape.
        const groundBox: b2PolygonShape = new b2PolygonShape();

        // The extents are the half-widths of the box.
        groundBox.SetAsBox(50, 10);

        // Add the ground fixture to the ground body.
        groundBody.CreateFixture(groundBox, 0);

        // Define the dynamic body. We set its position and call the body factory.
        const bodyDef: b2BodyDef = new b2BodyDef();
        bodyDef.type = b2BodyType.b2_dynamicBody;
        bodyDef.position.Set(0, 4);
        const body: b2Body = world.CreateBody(bodyDef);

        // Define another box shape for our dynamic body.
        const dynamicBox: b2PolygonShape = new b2PolygonShape();
        dynamicBox.SetAsBox(1, 1);

        // Define the dynamic body fixture.
        const fixtureDef: b2FixtureDef = new b2FixtureDef();
        fixtureDef.shape = dynamicBox;

        // Set the box density to be non-zero, so it will be dynamic.
        fixtureDef.density = 1;

        // Override the default friction.
        fixtureDef.friction = 0.3;

        // Add the shape to the body.
        const fixture: b2Fixture = body.CreateFixture(fixtureDef);

        // Prepare for simulation. Typically we use a time step of 1/60 of a
        // second (60Hz) and 10 iterations. This provides a high quality simulation
        // in most game scenarios.
        const timeStep: number = 1 / 60;
        const velocityIterations: number = 6;
        const positionIterations: number = 2;

        // This is our little game loop.
        for (let i: number = 0; i < 60; ++i) {
            // Instruct the world to perform a single step of simulation.
            // It is generally best to keep the time step and iterations fixed.
            world.Step(timeStep, velocityIterations, positionIterations);

            // Now print the position and angle of the body.
            const position: b2Vec2 = body.GetPosition();
            const angle: number = body.GetAngle();

            console.log(position.x.toFixed(2), position.y.toFixed(2), angle.toFixed(2));
        }
        this.world = world;
        //let names = _.chain(
        //    api.getUnluckyPersons())
        //    .sampleSize(count)
        //    .map(x => x.Name)
        //    .value();
    }
}