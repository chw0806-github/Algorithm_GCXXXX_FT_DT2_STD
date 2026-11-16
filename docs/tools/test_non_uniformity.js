/* 无 npm 依赖：严格模式运行成品页中的播放器和场景，检查边界与滑杆。 */
const fs = require('fs');
const path = require('path');
const vm = require('vm');
const assert = require('assert');
const html = fs.readFileSync(path.join(__dirname, '../non_uniformity/index.html'), 'utf8');
const scripts = Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi), m => m[1]);
assert.strictEqual(scripts.length, 3);
const gradient = { addColorStop() {} };
const ctx = new Proxy({}, { get(t, k) {
  if (k in t) return t[k];
  if (k === 'createLinearGradient' || k === 'createRadialGradient') return () => gradient;
  return () => {};
} });
const canvas = { style: {}, getContext: () => ctx, getBoundingClientRect: () => ({width:1000,height:620}) };
const handlers = {};
const slider = { value: '7.91', addEventListener: (name, fn) => { handlers[name] = fn; } };
const out = { textContent: '' }, baseOut = {textContent:''}, caption = {textContent:''}, explain = {innerHTML:''};
const baseHandlers = {};
const baseSlider = {value:'2',addEventListener:(name,fn)=>{baseHandlers[name]=fn;}};
const elements = {stage:canvas, cellvalue:slider, cellout:out, basevalue:baseSlider, baseout:baseOut, caption, explain, bar:{style:{}}};
const presetHandlers = {};
['preset-example', 'preset-normal', 'preset-max', 'preset-mean', 'preset-mirror'].forEach(id => {
  elements[id] = { addEventListener: (name, fn) => { presetHandlers[id] = fn; } };
});
const document = {getElementById: id => elements[id] || null, querySelectorAll: () => [],
  querySelector: () => null, addEventListener() {}};
let frame;
const sandbox = {document, performance:{now:()=>0}, requestAnimationFrame: fn => {frame=fn;},
  setTimeout(){}, addEventListener(){}, devicePixelRatio:1};
sandbox.window=sandbox;
vm.createContext(sandbox);
scripts.forEach((s,i) => new vm.Script(s,{filename:'non_uniformity:script'+i}).runInContext(sandbox));
assert.strictEqual(sandbox.__ANIM.scenes.length,13);
assert.strictEqual(vm.runInContext('state.cell',sandbox), Number(vm.runInContext('PIXEL_EXAMPLE.percent.toFixed(2)',sandbox)), '默认左上格应来自像素算例四舍五入');
const initial=vm.runInContext('metrics()',sandbox);
assert.ok(Math.abs(initial.m[0]-2.00369375)<1e-10);
assert.ok(Math.abs(initial.m[2]-5.91)<1e-10);
let tilesDrawn = 0;
ctx.fillRect = () => { tilesDrawn++; };
sandbox.__ANIM.scenes[4].draw(0.1875);
assert.strictEqual(tilesDrawn,400, '逐行动画在铺格进度 25% 时显示 10 行');
tilesDrawn = 0;
sandbox.__ANIM.scenes[4].draw(1);
assert.strictEqual(tilesDrawn,1600, '分格画面须绘制完整 40×40 格');
tilesDrawn = 0;
sandbox.__ANIM.scenes[5].draw(0);
assert.strictEqual(tilesDrawn,1600, '放大前应显示 40×40 个格子');
tilesDrawn = 0;
sandbox.__ANIM.scenes[5].draw(0.4);
assert.strictEqual(tilesDrawn,1600+1296, '放大后应显示完整 48×27 个像素');
assert.notStrictEqual(sandbox.__ANIM.scenes[5].caption(0),sandbox.__ANIM.scenes[5].caption(0.6));
let words=[];
ctx.fillText = text => { words.push(String(text)); };
for(const i of [6,7,8,9,10,11,12]){
  words=[]; sandbox.__ANIM.scenes[i].draw(0);
  const early=words.slice();
  words=[]; sandbox.__ANIM.scenes[i].draw(0.78);
  assert.ok(words.length>early.length, `第 ${i+1} 幕应逐步呈现内容`);
  assert.notStrictEqual(sandbox.__ANIM.scenes[i].caption(0),sandbox.__ANIM.scenes[i].caption(0.78));
}
for(const i of [10,11]){
  words=[];sandbox.__ANIM.scenes[i].draw(0.4);const first=words.find(s=>s.includes('扫描第'));
  words=[];sandbox.__ANIM.scenes[i].draw(0.7);const later=words.find(s=>s.includes('扫描第'));
  assert.ok(first && later && first!==later,'镜像对扫描应随时间移动');
}
let bridge=[];ctx.arc=(x,y)=>bridge.push([x,y]);
sandbox.__ANIM.scenes[8].draw(0.50);const position=bridge[0];
bridge=[];sandbox.__ANIM.scenes[8].draw(0.68);
assert.notDeepStrictEqual(bridge[0],position,'格内结果应动态移动回原来的格子');
let circles=[];
ctx.arc=(x,y)=>{circles.push([x,y]);};
sandbox.__ANIM.scenes[2].draw(0);
const start=circles.slice(); circles=[];
sandbox.__ANIM.scenes[2].draw(1);
assert.strictEqual(start.length,4);
assert.strictEqual(circles.length,4);
assert.notDeepStrictEqual(circles,start, '四个通道的像素应随播放移动');
assert.notStrictEqual(sandbox.__ANIM.scenes[2].caption(0),sandbox.__ANIM.scenes[2].caption(1));
assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
for(let i=0;i<13;i++){
  sandbox.__ANIM.gotoScene(i,true);
  for(const p of [0,0.3,0.78,1]){
    sandbox.__ANIM.state.t=p*sandbox.__ANIM.scenes[i].dur;
    sandbox.__ANIM.render();
  }
}
assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
assert.strictEqual(typeof frame,'function');
frame(100);
function change(v){ slider.value=String(v); handlers.input(); return vm.runInContext('metrics()',sandbox); }
function changeBase(v){ baseSlider.value=String(v); baseHandlers.input(); return vm.runInContext('metrics()',sandbox); }
assert.strictEqual(vm.runInContext('vals().length',sandbox),1600);
let r=change(12); assert.ok(Math.abs(r.m[0]-2.00625)<1e-12);
r=change(30); assert.strictEqual(r.bin,1); assert.strictEqual(out.textContent,'30%');
r=change(31); assert.strictEqual(r.bin,2204);
r=change(52); assert.strictEqual(r.m[2],50); assert.strictEqual(r.failed[2],false);
r=change(53); assert.strictEqual(r.m[2],51); assert.strictEqual(r.failed[2],true); assert.strictEqual(r.bin,2204);
change(5);r=changeBase(5);assert.strictEqual(r.m[0],5);assert.strictEqual(r.bin,1);
r=change(6);assert.ok(r.m[0]>5);assert.strictEqual(r.bin,2203);
r=changeBase(6);assert.strictEqual(r.bin,2203);assert.strictEqual(baseOut.textContent,'6%');
const ex = vm.runInContext('pixelExample()',sandbox);
assert.strictEqual(ex.pixels.length,1296);
for (const value of [180,190,210,220]) assert.strictEqual(ex.pixels.filter(x=>x===value).length,324);
assert.strictEqual(ex.mean,200);
assert.strictEqual(ex.squared.reduce((a,b)=>a+b,0),324000);
assert.strictEqual(ex.variance,250);
assert.ok(Math.abs(ex.std-Math.sqrt(250))<1e-12);
assert.ok(Math.abs(ex.percent-Math.sqrt(250)/200*100)<1e-12);
presetHandlers['preset-max']();
assert.strictEqual(out.textContent,'31%');assert.strictEqual(baseOut.textContent,'2%');
assert.strictEqual(vm.runInContext('metrics().bin',sandbox),2204);
presetHandlers['preset-mean']();
assert.strictEqual(baseOut.textContent,'6%');assert.strictEqual(vm.runInContext('metrics().bin',sandbox),2203);
presetHandlers['preset-mirror']();
assert.strictEqual(vm.runInContext('metrics().m[2]',sandbox),51);
assert.strictEqual(vm.runInContext('metrics().bin',sandbox),2204);
presetHandlers['preset-normal']();
assert.strictEqual(vm.runInContext('metrics().bin',sandbox),1);
presetHandlers['preset-example']();
assert.strictEqual(out.textContent,'7.91%');
assert.strictEqual(baseOut.textContent,'2%');
assert.ok(Math.abs(vm.runInContext('metrics().m[0]',sandbox)-2.00369375)<1e-10);
assert.strictEqual(vm.runInContext('pixelExample().mean',sandbox),200);
assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：13 幕、拆通道动画、逐行铺格、1296 像素、双滑杆及阈值边界');
