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
const slider = { value: '12', addEventListener: (name, fn) => { handlers[name] = fn; } };
const out = { textContent: '' }, caption = {textContent:''}, explain = {innerHTML:''};
const elements = {stage:canvas, cellvalue:slider, cellout:out, caption, explain, bar:{style:{}}};
const document = {getElementById: id => elements[id] || null, querySelectorAll: () => [],
  querySelector: () => null, addEventListener() {}};
let frame;
const sandbox = {document, performance:{now:()=>0}, requestAnimationFrame: fn => {frame=fn;},
  setTimeout(){}, addEventListener(){}, devicePixelRatio:1};
sandbox.window=sandbox;
vm.createContext(sandbox);
scripts.forEach((s,i) => new vm.Script(s,{filename:'non_uniformity:script'+i}).runInContext(sandbox));
assert.strictEqual(sandbox.__ANIM.scenes.length,7);
assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
for(let i=0;i<7;i++){
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
let r=change(30); assert.strictEqual(r.bin,1); assert.strictEqual(out.textContent,'30%');
r=change(31); assert.strictEqual(r.bin,2204); // 单格超过 30，均值仍未超过 5
r=change(38); assert.strictEqual(r.m[0],5); assert.strictEqual(r.bin,2204); // 恰好等于 5
r=change(39); assert.strictEqual(r.bin,2203); // 均值与单格同时失败，先触发者保留
r=change(52); assert.strictEqual(r.m[2],50); assert.strictEqual(r.failed[2],false);
r=change(53); assert.strictEqual(r.m[2],51); assert.strictEqual(r.failed[2],true);
assert.strictEqual(r.failed[3],true);
assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：7 幕、滑杆、阈值边界与同级优先顺序');
