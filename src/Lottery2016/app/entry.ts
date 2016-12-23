import { Scene } from "./engine/Scene";
import { NN } from "./engine/NN";
import { FpsRenderer } from "./engine/FpsRenderer";
import { AlignToWindowUtil } from "./engine/AlignToWindowUtil";
import { LotteryRenderer } from "./lottery/index";

var s = new Scene(<HTMLCanvasElement>NN(document.querySelector("#canvas")));
s.addRenderer(new FpsRenderer(s.getCanvas()));
s.addRenderer(new AlignToWindowUtil(s.getCanvas()));
s.addRenderer(new LotteryRenderer(s.getCanvas()));