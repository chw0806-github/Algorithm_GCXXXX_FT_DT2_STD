/* 严格模式下测试成品动画的所有场景、滑杆与阈值；不依赖 npm 包。 */
const fs=require('fs'), path=require('path'), vm=require('vm'), assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../deadrow_deadcol_bright/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);
assert.strictEqual(scripts.length,3);
const gradient={addColorStop(){}};
const ctx=new Proxy({}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
const handlers={};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},bar:{style:{}}};
for(const id of ['row','col']){
  elements[id+'val']={value:'200',addEventListener:(name,fn)=>{handlers[id]=fn;}};
  elements[id+'out']={textContent:''};
}
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
let frame;
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};
sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'deadrow:script'+i}).runInContext(sandbox));
assert.strictEqual(sandbox.__ANIM.scenes.length,7);
for(let i=0;i<7;i++){
 sandbox.__ANIM.gotoScene(i,true);
 for(const p of [0,.3,.78,1]){sandbox.__ANIM.state.t=p*sandbox.__ANIM.scenes[i].dur;sandbox.__ANIM.render();}
}
assert.strictEqual(typeof frame,'function');frame(100);
function set(id,val){elements[id+'val'].value=String(val);handlers[id]();return vm.runInContext('result()',sandbox);}
let r=set('row',195);assert.strictEqual(r.badRow,false);assert.strictEqual(r.bin,1);
r=set('row',194);assert.strictEqual(r.badRow,true);assert.strictEqual(r.bin,2210);
r=set('col',194);assert.strictEqual(r.badCol,false);assert.strictEqual(r.bin,2210);
r=set('col',193);assert.strictEqual(r.badCol,true);assert.strictEqual(r.bin,2208);assert.strictEqual(r.grade,7);
r=set('row',200);assert.strictEqual(r.bin,2208);
r=set('col',206);assert.strictEqual(r.badCol,false);assert.strictEqual(r.bin,1);
r=set('col',207);assert.strictEqual(r.badCol,true);assert.strictEqual(elements.colout.textContent,'207');
assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：7 幕、滑杆、行列阈值边界与同级优先顺序');
