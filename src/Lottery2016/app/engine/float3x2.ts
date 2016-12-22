import * as $ from "jquery";

export class float3x2 {
    constructor(
        public m11: number,
        public m12: number,
        public m21: number,
        public m22: number,
        public m31: number,
        public m32: number) {
    }

    get() {
        return [
            this.m11, this.m12,
            this.m21, this.m22,
            this.m31, this.m32
        ];
    }

    clone() {
        return new float3x2(
            this.m11, this.m12,
            this.m21, this.m22,
            this.m31, this.m32
        );
    }

    assign(value: float3x2) {
        this.m11 = value.m11;
        this.m12 = value.m12;
        this.m21 = value.m21;
        this.m22 = value.m22;
        this.m31 = value.m31;
        this.m32 = value.m32;
    }

    animate(to: float3x2, options?: JQueryAnimationOptions) {
        var baseOptions: JQueryAnimationOptions = {
            queue: false
        };
        var targetOption = $.extend({}, baseOptions, options);

        return $(this).animate({
            m11: to.m11,
            m12: to.m12,
            m21: to.m21,
            m22: to.m22,
            m31: to.m31,
            m32: to.m32
        }, targetOption).promise();
    }

    static _2pi = Math.PI * 2;

    static _pi = Math.PI;

    static _piD2 = Math.PI / 2;

    static identity() {
        return new float3x2(
            1, 0,
            0, 1,
            0, 0);
    }

    translation(x: number, y: number) {
        return this.multiply(float3x2.translation(x, y));
    }

    scale(xScale: number, yScale: number, centerX = 0, centerY = 0) {
        return this.multiply(float3x2.scale(xScale, yScale, centerX, centerY));
    }

    skew(radiansX: number, radiansY: number, centerX = 0, centerY = 0) {
        return this.multiply(float3x2.skew(radiansX, radiansY, centerX, centerY));
    }

    rotation(radians: number, centerX = 0, centerY = 0) {
        return this.multiply(float3x2.rotation(radians, centerX, centerY));
    }

    add(value2: float3x2) {
        return new float3x2(
            this.m11 + value2.m11, this.m12 + value2.m12,
            this.m21 + value2.m21, this.m22 + value2.m22,
            this.m31 + value2.m31, this.m32 + value2.m32);
    }

    minus(value2: float3x2) {
        return new float3x2(
            this.m11 - value2.m11, this.m12 - value2.m12,
            this.m21 - value2.m21, this.m22 - value2.m22,
            this.m31 - value2.m31, this.m32 - value2.m32);
    }

    multiply(value2: float3x2) {
        return new float3x2(
            // First row
            this.m11 * value2.m11 + this.m12 * value2.m21,
            this.m11 * value2.m12 + this.m12 * value2.m22,

            // Second row
            this.m21 * value2.m11 + this.m22 * value2.m21,
            this.m21 * value2.m12 + this.m22 * value2.m22,

            // Third row
            this.m31 * value2.m11 + this.m32 * value2.m21 + value2.m31,
            this.m31 * value2.m12 + this.m32 * value2.m22 + value2.m32
        );
    }

    multiply_scalar(value2: number) {
        return new float3x2(
            this.m11 * value2, this.m12 * value2,
            this.m21 * value2, this.m22 * value2,
            this.m31 * value2, this.m32 * value2);
    }

    negative() {
        return new float3x2(
            -this.m11, -this.m12,
            -this.m21, -this.m22,
            -this.m31, -this.m32);
    }

    equals(value2: float3x2) {
        return this.m11 === value2.m11 && this.m22 === value2.m22 && // Check diagonal element first for early out.
            this.m12 === value2.m12 && this.m21 === value2.m21 &&
            this.m31 === value2.m31 && this.m32 === value2.m32;
    }

    not_equals(value2: float3x2) {
        return this.m11 !== value2.m11 || this.m12 !== value2.m12 ||
            this.m21 !== value2.m21 || this.m22 !== value2.m22 ||
            this.m31 !== value2.m31 || this.m32 !== value2.m32;
    }

    is_identity() {
        return this.m11 === 1 && this.m22 === 1 && // Check diagonal element first for early out.
            this.m12 === 0 && this.m21 === 0 &&
            this.m31 === 0 && this.m32 === 0;
    }

    determinant() {
        return (this.m11 * this.m22) - (this.m21 * this.m12);
    }

    invert() {
        var det = this.determinant();

        var floatEpsilon = 1.192092896e-07;

        if (Math.abs(det) < floatEpsilon) {
            return null;
        }

        var invDet = 1.0 / det;

        return new float3x2(
            // First row
            this.m22 * invDet,
            -this.m12 * invDet,

            // Second row
            -this.m21 * invDet,
            this.m11 * invDet,

            // Third row
            (this.m21 * this.m32 - this.m31 * this.m22) * invDet,
            (this.m31 * this.m12 - this.m11 * this.m32) * invDet
        );
    }

    static translation(x: number, y: number) {
        return new float3x2(
            1, 0,
            0, 1,
            x, y);
    }

    static scale(xScale: number, yScale: number, centerX = 0, centerY = 0) {
        var tx = centerX * (1 - xScale);
        var ty = centerY * (1 - yScale);

        return new float3x2(
            xScale, 0,
            0, yScale,
            tx, ty);
    }

    static skew(radiansX: number, radiansY: number, centerX = 0, centerY = 0) {
        var xTan = Math.tan(radiansX);
        var yTan = Math.tan(radiansY);

        var tx = -centerY * xTan;
        var ty = -centerX * yTan;

        return new float3x2(
            1, yTan,
            xTan, 1,
            tx, ty);
    }

    static rotation(radians: number, centerX = 0, centerY = 0) {
        radians = radians % float3x2._2pi;

        var epsilon = 0.001 * float3x2._pi / 180.0; // 0.1% of a degree

        if (radians < 0)
            radians += float3x2._2pi;

        var c: number, s: number;

        if (radians < epsilon || radians > float3x2._2pi - epsilon) {
            // Exact case for zero rotation.
            c = 1;
            s = 0;
        } else if (radians > float3x2._piD2 - epsilon && radians < float3x2._piD2 + epsilon) {
            // Exact case for 90 degree rotation.
            c = 0;
            s = 1;
        } else if (radians > float3x2._pi - epsilon && radians < float3x2._pi + epsilon) {
            // Exact case for 180 degree rotation.
            c = -1;
            s = 0;
        } else if (radians > float3x2._pi + float3x2._piD2 - epsilon && radians < float3x2._pi + float3x2._piD2 + epsilon) {
            // Exact case for 270 degree rotation.
            c = 0;
            s = -1;
        } else {
            // Arbitrary rotation.
            c = Math.cos(radians);
            s = Math.sin(radians);
        }

        var x = centerX * (1 - c) + centerY * s;
        var y = centerY * (1 - c) - centerX * s;

        return new float3x2(
            c, s,
            -s, c,
            x, y);
    }

    static add(value1: float3x2, value2: float3x2) {
        return new float3x2(
            value1.m11 + value2.m11, value1.m12 + value2.m12,
            value1.m21 + value2.m21, value1.m22 + value2.m22,
            value1.m31 + value2.m31, value1.m32 + value2.m32);
    }

    static minus(value1: float3x2, value2: float3x2) {
        return new float3x2(
            value1.m11 - value2.m11, value1.m12 - value2.m12,
            value1.m21 - value2.m21, value1.m22 - value2.m22,
            value1.m31 - value2.m31, value1.m32 - value2.m32);
    }

    static multiply(value1: float3x2, value2: float3x2) {
        return new float3x2(
            // First row
            value1.m11 * value2.m11 + value1.m12 * value2.m21,
            value1.m11 * value2.m12 + value1.m12 * value2.m22,

            // Second row
            value1.m21 * value2.m11 + value1.m22 * value2.m21,
            value1.m21 * value2.m12 + value1.m22 * value2.m22,

            // Third row
            value1.m31 * value2.m11 + value1.m32 * value2.m21 + value2.m31,
            value1.m31 * value2.m12 + value1.m32 * value2.m22 + value2.m32
        );
    }

    static multiply_scalar(value1: float3x2, value2: number) {
        return new float3x2(
            value1.m11 * value2, value1.m12 * value2,
            value1.m21 * value2, value1.m22 * value2,
            value1.m31 * value2, value1.m32 * value2);
    }

    static negative(value: float3x2) {
        return new float3x2(
            -value.m11, -value.m12,
            -value.m21, -value.m22,
            -value.m31, -value.m32);
    }

    static equals(value1: float3x2, value2: float3x2) {
        return value1.m11 === value2.m11 && value1.m22 === value2.m22 && // Check diagonal element first for early out.
            value1.m12 === value2.m12 &&
            value1.m21 === value2.m21 &&
            value1.m31 === value2.m31 && value1.m32 === value2.m32;
    }

    static not_equals(value1: float3x2, value2: float3x2) {
        return value1.m11 !== value2.m11 || value1.m12 !== value2.m12 ||
            value1.m21 !== value2.m21 || value1.m22 !== value2.m22 ||
            value1.m31 !== value2.m31 || value1.m32 !== value2.m32;
    }

    static is_identity(value: float3x2) {
        return value.m11 === 1 && value.m22 === 1 && // Check diagonal element first for early out.
            value.m12 === 0 &&
            value.m21 === 0 &&
            value.m31 === 0 && value.m32 === 0;
    }

    static determinant(value: float3x2) {
        return (value.m11 * value.m22) - (value.m21 * value.m12);
    }

    static invert(matrix: float3x2) {
        var det = float3x2.determinant(matrix);

        var floatEpsilon = 1.192092896e-07;

        if (Math.abs(det) < floatEpsilon) {
            return null;
        }

        var invDet = 1.0 / det;

        return new float3x2(
            // First row
            matrix.m22 * invDet,
            -matrix.m12 * invDet,

            // Second row
            -matrix.m21 * invDet,
            matrix.m11 * invDet,

            // Third row
            (matrix.m21 * matrix.m32 - matrix.m31 * matrix.m22) * invDet,
            (matrix.m31 * matrix.m12 - matrix.m11 * matrix.m32) * invDet
        );
    }

    static lerp(matrix1: float3x2, matrix2: float3x2, amount: number) {
        // return matrix1 + (matrix2 - matrix1) * amount;
        return float3x2.add(matrix1,
            float3x2.multiply_scalar(float3x2.minus(matrix2, matrix1), amount));
    }
}