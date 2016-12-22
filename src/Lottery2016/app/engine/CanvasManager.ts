import { float3x2 } from "./float3x2";
import { NN } from "./NN";

type IBrushType = string | CanvasGradient | CanvasPattern;
type ITextBaseLineType = "top" | "hanging" | "middle" | "alphabetic" | "ideographic" | "bottom";

export class CanvasManager {
    ctx: CanvasRenderingContext2D;

    width() {
        return this.canvas.width;
    }

    height() {
        return this.canvas.height;
    }

    get font() {
        return this.ctx.font;
    }
    set font(font: string) {
        this.ctx.font = font;
    }

    get textBaseLine() {
        return <ITextBaseLineType>this.ctx.textBaseline;
    }
    set textBaseLine(baseline: ITextBaseLineType) {
        this.ctx.textBaseline = baseline;
    }

    resizeTo(width: number, height: number) {
        let font = this.font;
        let textBaseLine = this.textBaseLine;

        this.canvas.width = width;
        this.canvas.height = height;

        this.font = font;
        this.textBaseLine = textBaseLine;
    }

    fillRect(x: number, y: number, w: number, h: number, brush: IBrushType) {
        this.ctx.fillStyle = brush;
        this.ctx.fillRect(x, y, w, h);
    }

    //drawImage(image: HTMLImageElement | HTMLCanvasElement | HTMLVideoElement,
    //    offsetX: number, offsetY: number, width?: number, height?: number,
    //    canvasOffsetX?: number, canvasOffsetY?: number, canvasImageWidth?: number, canvasImageHeight?: number) {
    //    this.ctx.drawImage(image,
    //        offsetX, offsetY, width, height,
    //        canvasOffsetX, canvasOffsetY, canvasImageWidth, canvasImageHeight);
    //}

    strokeRect(x: number, y: number, w: number, h: number, brush: IBrushType, lineWidth?: number) {
        if (lineWidth) this.ctx.lineWidth = lineWidth;
        this.ctx.strokeStyle = brush;
        this.ctx.strokeRect(x, y, w, h);
    }

    drawText(text: string, x: number, y: number, brush: IBrushType, maxWidth?: number): void {
        this.ctx.fillStyle = brush;
        maxWidth && this.ctx.fillText(text, x, y, maxWidth);
        maxWidth || this.ctx.fillText(text, x, y);
    }

    drawTextAtCenter(text: string, x: number, y: number, brush: IBrushType): void {
        this.ctx.fillStyle = brush;
        let width = this.ctx.measureText(text).width;

        let textBaseLine = this.textBaseLine;
        this.textBaseLine = "middle";
        x -= width / 2;
        this.ctx.fillText(text, x, y);
        this.textBaseLine = textBaseLine;
    }

    strokeText(text: string, x: number, y: number, brush: IBrushType, maxWidth?: number): void {
        this.ctx.strokeStyle = brush;
        maxWidth && this.ctx.strokeText(text, x, y, maxWidth);
        maxWidth || this.ctx.strokeText(text, x, y);
    }

    line(p1x: number, p1y: number, p2x: number, p2y: number) {
        this.ctx.moveTo(p1x, p1y);
        this.ctx.lineTo(p2x, p2y);
    }

    moveTo(x: number, y: number) {
        this.ctx.moveTo(x, y);
    }

    lineTo(x: number, y: number) {
        this.ctx.lineTo(x, y);
    }

    beginPath() {
        this.ctx.beginPath();
    }

    drawLine(p1x: number, p1y: number, p2x: number, p2y: number, brush?: IBrushType, lineWidth?: number) {
        if (lineWidth) this.ctx.lineWidth = lineWidth;
        if (brush) this.ctx.strokeStyle = brush;
        this.ctx.beginPath();
        this.ctx.moveTo(p1x, p1y);
        this.ctx.lineTo(p2x, p2y);
        this.ctx.stroke();
    }

    fillCircle(x: number, y: number, r: number, brush?: IBrushType) {
        if (brush) this.ctx.fillStyle = brush;
        this.ctx.beginPath();
        this.ctx.moveTo(x, 0);
        this.ctx.arc(x, y, r, 0, Math.PI * 2);
        this.ctx.fill();
    }

    circle(x: number, y: number, r: number) {
        this.ctx.moveTo(x, 0);
        this.ctx.arc(x, y, r, 0, Math.PI * 2);
    }

    drawCircle(x: number, y: number, r: number, brush?: IBrushType, lineWidth?: number) {
        if (lineWidth) this.ctx.lineWidth = lineWidth;
        if (brush) this.ctx.strokeStyle = brush;
        this.ctx.beginPath();
        this.ctx.arc(x, y, r, 0, Math.PI * 2);
        this.ctx.stroke();
    }

    stroke(brush?: IBrushType, lineWidth?: number) {
        if (brush) this.ctx.strokeStyle = brush;
        if (lineWidth) this.ctx.lineWidth = lineWidth;
        this.ctx.stroke();
    }

    rotate(angle: number, centerX?: number, centerY?: number) {
        this.ctx.rotate(angle % 360);
        centerX && this.ctx.translate(-centerX, -centerY);
    }

    setIdentity() {
        this.ctx.setTransform(1, 0, 0, 1, 0, 0);
    }

    clear() {
        this.setIdentity();
        this.ctx.clearRect(0, 0, this.width(), this.height());
    }

    setTransform(matrix: float3x2) {
        this.ctx.setTransform.apply(this.ctx, matrix.get());
    }

    createLinearGradient(x0: number, y0: number, x1: number, y1: number) {
        return this.ctx.createLinearGradient(x0, y0, x1, y1);
    }

    createRadialGradient(...stops: Array<{ offset: number, color: string }>) {
        let gradient = this.ctx.createRadialGradient(0, 0, 0, 0, 0, 1);
        for (let stop of stops) {
            gradient.addColorStop(stop.offset, stop.color);
        }
        return gradient;
    }

    constructor(private canvas: HTMLCanvasElement) {
        this.ctx = NN(canvas.getContext("2d"));
        this.ctx.textBaseline = "top";
        this.ctx.font = "14pt Consolas";
    }
}