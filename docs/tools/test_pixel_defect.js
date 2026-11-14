/* 严格模式运行成品页，测试场景、滑杆及两个阈值；无 npm 依赖。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../pixel_defect/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);
assert.strictEqual(scripts.length,3);
const gradient={addColorStop(){}};
const ctx=new Proxy({}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
let handler,frame;
const input={value:'180',addEventListener:(name,fn)=>{handler=fn;}}, output={textContent:''};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},bar:{style:{}},pixelval:input,pixelout:output};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};
sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'pixel:script'+i}).runInContext(sandbox));
assert.strictEqual(sandbox.__ANIM.scenes.length,7);
for(let i=0;i<7;i++){sandbox.__ANIM.gotoScene(i,true);for(const p of [0,.3,.78,1]){sandbox.__ANIM.state.t=p*sandbox.__ANIM.scenes[i].dur;sandbox.__ANIM.render();}}
assert.strictEqual(typeof frame,'function');frame(100);
function set(v){input.value=String(v);handler();return vm.runInContext('result()',sandbox);}
let r=set(180);assert.strictEqual(r.primary,false);assert.strictEqual(r.blemish,false);assert.strictEqual(r.bin,2001);
r=set(201);assert.strictEqual(r.blemish,false);r=set(202);assert.strictEqual(r.blemish,true);
r=set(234);assert.strictEqual(r.primary,false);assert.strictEqual(r.blemish,true);assert.strictEqual(r.bin,2001);
r=set(235);assert.strictEqual(r.primary,true);assert.strictEqual(r.blemish,false);assert.strictEqual(r.bin,2002);assert.strictEqual(output.textContent,'235');
r=set(126);assert.strictEqual(r.primary,false);r=set(125);assert.strictEqual(r.primary,true);assert.strictEqual(r.bin,2006);
assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：7 幕、滑杆、主缺陷和 blemish 阈值边界');
