import { Scene } from "./engine/Scene";
import { NN } from "./engine/NN";
import { FpsRenderer } from "./engine/FpsRenderer";
import { ClockRenderer } from "./engine/ClockRenderer";
import { AlignToWindowUtil } from "./engine/AlignToWindowUtil";

var s = new Scene(<HTMLCanvasElement>NN(document.querySelector("#canvas")));
s.addRenderer(new FpsRenderer(s.getCanvas()));
s.addRenderer(new ClockRenderer(s.getCanvas()));
s.addRenderer(new AlignToWindowUtil(s.getCanvas()));