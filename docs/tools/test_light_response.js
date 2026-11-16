/* 严格模式验证均值画面的步骤、滑杆交互、判定与源码。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../light_response/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);
assert.strictEqual(scripts.length,3);
let texts=[],frame;const gradient={addColorStop(){}};
const ctx=new Proxy({measureText(s){return {width:String(s).length*7};},fillText(s){texts.push(String(s));}}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
const sliders=[],buttons=[];
function node(tag){
 const input={value:'',dataset:{},addEventListener(event,fn){this.handler=fn;}},out={textContent:''};
 const el={style:{setProperty(){}},appendChild(){},querySelector(s){return s==='input'?input:out;},addEventListener(event,fn){this.handler=fn;}};
 input.parentElement=el;el.input=input;el.out=out;
 if(tag==='label')sliders.push(el);if(tag==='button')buttons.push(el);return el;
}
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}},sliders:{appendChild(){}},presets:{appendChild(){}}};
const document={getElementById:id=>elements[id]||null,createElement:node,querySelectorAll:s=>s==='.slider input'?sliders.map(x=>x.input):[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'light:script'+i}).runInContext(sandbox));
const run=s=>vm.runInContext(s,sandbox);
assert.strictEqual(sandbox.__ANIM.scenes.length,8);assert.strictEqual(sandbox.__CODE_GUIDE.length,8);
assert.strictEqual(run('SPEC.roi.w * SPEC.roi.h'),82944);
for(let i=0;i<8;i++){
 sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'));
 for(const p of [0,.3,.78,1]){sandbox.__ANIM.state.t=p*sandbox.__ANIM.scenes[i].dur;sandbox.__ANIM.render();}
}
function at(p){texts=[];sandbox.__ANIM.scenes[5].draw(p);return texts.slice();}
let t=at(0);assert.ok(t.includes('384 × 216 = 82944 个像素 / 通道'));assert.ok(!t.some(s=>s.startsWith('均值 =')));
t=at(.25);assert.ok(t.includes('均值 = ROI 像素亮度总和 ÷ 82944'));assert.ok(!t.some(s=>s.startsWith('R 通道模拟均值')));
t=at(.45);assert.ok(t.includes('R 通道模拟均值 = 182.4 DN'));assert.ok(!t.includes('196.7 DN'));
t=at(1);for(const v of ['182.4 DN','196.7 DN','193.2 DN','171.8 DN'])assert.ok(t.includes(v));
assert.ok(!t.some(s=>/累加器|扫描到第/.test(s)));
sliders[0].input.value='40';sliders[0].input.handler();assert.strictEqual(run('judge().bin'),161);assert.strictEqual(sliders[0].out.textContent,'40.0');assert.ok(at(.45).includes('R 通道模拟均值 = 40.0 DN'));
sliders.forEach((v,i)=>{v.input.dataset.ch=['R','Gr','Gb','B'][i];});buttons[0].handler();assert.strictEqual(run('judge().pass'),true);assert.strictEqual(sliders[0].input.value,182.4);
assert.strictEqual(typeof frame,'function');frame(100);assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：8 幕、ROI 均值阶段、滑杆、判定和源码');
