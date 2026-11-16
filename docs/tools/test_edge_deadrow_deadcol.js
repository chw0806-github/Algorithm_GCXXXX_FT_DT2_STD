/* 验证边缘选择、样本数、阈值边界、交互和所有画面绘制。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../edge_deadrow_deadcol_bright/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);
assert.strictEqual(scripts.length,3);
const gradient={addColorStop(){}};
const ctx=new Proxy({}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
let handler,frame;const buttons={};
const input={value:'189.5',addEventListener:(event,fn)=>{handler=fn;}},out={textContent:''};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}},rowmean:input,rowout:out};
for(const id of ['actual','hypothetical','normal','ta','ma','example'])elements[id]={addEventListener:(event,fn)=>{buttons[id]=fn;}};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'edge:script'+i}).runInContext(sandbox));
const run=s=>vm.runInContext(s,sandbox);
assert.strictEqual(sandbox.__ANIM.scenes.length,13);
assert.strictEqual(sandbox.__CODE_GUIDE.length,13);
const pixels=Array.from(run('samplePixels()'));
assert.strictEqual(pixels.length,1920);assert.strictEqual(new Set(pixels).size,20);
for(let n=180;n<200;n++)assert.strictEqual(pixels.filter(v=>v===n).length,96);
assert.strictEqual(pixels.reduce((a,b)=>a+b,0),363840);assert.strictEqual(run('average(samplePixels())'),189.5);
assert.strictEqual(run('average(neighbors)'),200);
function selected(size,strip){return Array.from({length:size},(_,i)=>i).filter(i=>i<Math.floor(strip/2)||i>=size-Math.floor(strip/2));}
assert.deepStrictEqual(selected(1080,0),[]);assert.deepStrictEqual(selected(1080,1),[]);assert.deepStrictEqual(selected(1080,2),[0,1079]);
assert.deepStrictEqual(selected(1920,2),[0,1919]);
let r=run('lineResult(189.5,0)');assert.strictEqual(r.selected,false);assert.strictEqual(r.hit,false);
r=run('lineResult(195,2)');assert.strictEqual(r.hit,false);
r=run('lineResult(194.5,2)');assert.strictEqual(r.hit,true);assert.strictEqual(r.bin,2217);assert.strictEqual(r.grade,2);
r=run('lineResult(193,2)');assert.strictEqual(r.bin,2216);assert.strictEqual(r.grade,6);
const dol=[];for(let i=0;i<96;i++)for(let v=50;v<=59;v++)dol.push(v);for(let i=0;i<96;i++)for(let v=241;v<=250;v++)dol.push(v);
assert.strictEqual(dol.length,1920);assert.strictEqual(dol.reduce((a,b)=>a+b,0)/dol.length,150);
const squared=dol.reduce((sum,v)=>sum+(v-150)**2,0);assert.strictEqual(squared,17526720);assert.ok(Math.sqrt(squared/1919)>95.56&&Math.sqrt(squared/1919)<95.58);
for(let i=0;i<13;i++){
 sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'),`第 ${i+1} 幕缺源码`);
 for(const p of [0,.3,.78,1]){sandbox.__ANIM.state.t=p*sandbox.__ANIM.scenes[i].dur;sandbox.__ANIM.render();}
}
// 行像素数量一开始即完整呈现，右侧按步骤展示总和和均值。
let fills=0,texts=[];ctx.fillRect=()=>{fills++;};ctx.fillText=s=>texts.push(String(s));
function lineAt(p){fills=0;texts=[];sandbox.__ANIM.scenes[4].draw(p);return {fills,texts:texts.slice()};}
let first=lineAt(0),last=lineAt(1);assert.strictEqual(first.fills,1920);assert.strictEqual(last.fills,1920);
assert.ok(!first.texts.includes('总和 = 363840 DN'));assert.ok(last.texts.includes('行均值 = 363840 ÷ 1920 = 189.5 DN'));
buttons.hypothetical();assert.strictEqual(run('state.strip'),2);buttons.ma();assert.strictEqual(out.textContent,'193');assert.strictEqual(run('lineResult(state.mean,state.strip).bin'),2216);
buttons.ta();assert.strictEqual(run('lineResult(state.mean,state.strip).bin'),2217);buttons.actual();assert.strictEqual(run('state.strip'),0);
input.value='195';handler();assert.strictEqual(out.textContent,'195');assert.strictEqual(run('lineResult(state.mean,2).hit'),false);
assert.strictEqual(typeof frame,'function');frame(100);assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：13 幕、实际零条带、假设选区、1920 像素、DOL、阈值及交互');
