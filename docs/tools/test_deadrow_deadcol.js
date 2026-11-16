/* 严格模式下测试成品动画的所有场景、滑杆与阈值；不依赖 npm 包。 */
const fs=require('fs'), path=require('path'), vm=require('vm'), assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../deadrow_deadcol_bright/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);
assert.strictEqual(scripts.length,3);
const gradient={addColorStop(){}};
const ctx=new Proxy({}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
const handlers={};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},bar:{style:{}},'code-guide':{innerHTML:''}};
for(const id of ['row','col']){
  elements[id+'val']={value:'200',addEventListener:(name,fn)=>{handlers[id]=fn;}};
  elements[id+'out']={textContent:''};
}
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
let frame;
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};
sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'deadrow:script'+i}).runInContext(sandbox));
assert.strictEqual(sandbox.__ANIM.scenes.length,14);
assert.strictEqual(sandbox.__CODE_GUIDE.length,14);
assert.ok(elements['code-guide'].innerHTML.includes('Deadrow_Deadcol_bright'));
sandbox.__ANIM.gotoScene(3,true);
assert.ok(elements['code-guide'].innerHTML.includes('each_row_.push_back'));
sandbox.__ANIM.gotoScene(10,true);
assert.ok(elements['code-guide'].innerHTML.includes('delta2-delta1'));
const row=vm.runInContext('rowExample()',sandbox);
const pixels=Array.from(vm.runInContext('ROW_PIXELS',sandbox));
assert.strictEqual(pixels.length,1920);
assert.strictEqual(new Set(pixels).size,20);
assert.strictEqual(pixels.reduce((a,b)=>a+b,0),363840);
assert.strictEqual(row.target,189.5);assert.strictEqual(row.top,200);assert.strictEqual(row.bottom,210);
assert.strictEqual(new Set(Array.from(vm.runInContext('TOP_MEANS',sandbox))).size,5);
assert.strictEqual(new Set(Array.from(vm.runInContext('BOTTOM_MEANS',sandbox))).size,5);
assert.ok(Math.abs(row.topDiff-.0525)<1e-12);
assert.ok(Math.abs(row.bottomDiff-20.5/210)<1e-12);
const dol=vm.runInContext('dolExample()',sandbox);
assert.strictEqual(new Set(Array.from(vm.runInContext('DOL_PIXELS',sandbox))).size,20);
assert.strictEqual(dol.mean,150);assert.strictEqual(dol.sumSquares,17526720);
assert.ok(Math.abs(dol.std-95.5680747145)<1e-9);
assert.strictEqual(vm.runInContext('Math.abs((HALF_MEANS[2]-HALF_MEANS[1])-(HALF_MEANS[1]-HALF_MEANS[0]))',sandbox),16);
for(let i=0;i<14;i++){
 sandbox.__ANIM.gotoScene(i,true);
 for(const p of [0,.3,.78,1]){sandbox.__ANIM.state.t=p*sandbox.__ANIM.scenes[i].dur;sandbox.__ANIM.render();}
}
let fills=0;ctx.fillRect=()=>{fills++;};
sandbox.__ANIM.scenes[3].draw(0);assert.strictEqual(fills,1920,'行均值画面应绘出完整 1920 像素');
fills=0;sandbox.__ANIM.scenes[3].draw(1);assert.strictEqual(fills,1940,'还应放大展示前 20 个像素');
fills=0;sandbox.__ANIM.scenes[8].draw(1);assert.strictEqual(fills,1920,'DOL 示例应绘出完整 1920 像素');
let texts=[];ctx.fillText=text=>texts.push(String(text));
for(const i of [3,4,5,6,7,8,10,11,12,13]){
 texts=[];sandbox.__ANIM.scenes[i].draw(0);const before=texts.length;
 texts=[];sandbox.__ANIM.scenes[i].draw(.78);assert.ok(texts.length>before,`第 ${i+1} 幕应逐步展开`);
 assert.notStrictEqual(sandbox.__ANIM.scenes[i].caption(0),sandbox.__ANIM.scenes[i].caption(.78));
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
console.log('OK：14 幕、滑杆、行列阈值边界与同级优先顺序');
