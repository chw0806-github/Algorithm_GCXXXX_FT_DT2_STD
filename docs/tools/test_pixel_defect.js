/* 严格模式测试完整内部 11×11 样例、邻域均值、阈值和页面章节。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../pixel_defect/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);
assert.strictEqual(scripts.length,3);
const gradient={addColorStop(){}};
const ctx=new Proxy({}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
let handler,frame;
const input={value:'245',addEventListener:(name,fn)=>{handler=fn;}}, output={textContent:''};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}},pixelval:input,pixelout:output};
const presets={};
for(const id of ['preset-normal','preset-blemish','preset-limit','preset-bright','preset-dark'])
 elements[id]={addEventListener:(name,fn)=>{presets[id]=fn;}};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};
sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'pixel:script'+i}).runInContext(sandbox));
assert.strictEqual(sandbox.__ANIM.scenes.length,14);
assert.strictEqual(sandbox.__CODE_GUIDE.length,14);
assert.deepStrictEqual(Array.from(vm.runInContext('means()',sandbox)),[180,190,200,210]);
for(let q=0;q<4;q++){
 const v=Array.from(vm.runInContext(`quadValues(${q})`,sandbox));
 assert.strictEqual(v.length,25);assert.strictEqual(new Set(v).size,25);
 assert.strictEqual(v.reduce((a,b)=>a+b,0)/25,[180,190,200,210][q]);
}
const counts=[0,0,0,0];let cross=0,center=0;
for(let y=0;y<11;y++)for(let x=0;x<11;x++){
 const q=vm.runInContext(`quadrant(${x},${y})`,sandbox);
 if(x===5&&y===5)center++;else if(q<0)cross++;else counts[q]++;
}
assert.deepStrictEqual(counts,[25,25,25,25]);assert.strictEqual(cross,20);assert.strictEqual(center,1);
for(let q=0;q<4;q++){
 const values=[];
 for(let y=0;y<11;y++)for(let x=0;x<11;x++){
  if(vm.runInContext(`quadrant(${x},${y})`,sandbox)===q)values.push(vm.runInContext(`patchValue(${x},${y})`,sandbox));
 }
 assert.deepStrictEqual(values,Array.from(vm.runInContext(`quadValues(${q})`,sandbox)),`第 ${q+1} 块显示值与计算值不一致`);
}
assert.strictEqual(vm.runInContext('patchValue(5,5)',sandbox),245);
let r=vm.runInContext('result()',sandbox);
assert.strictEqual(r.primary,true);assert.strictEqual(r.blemish,false);
assert.deepStrictEqual(Array.from(r.diffs, v=>v>.3),[true,false,false,false]);
assert.strictEqual(r.ref,195);assert.strictEqual(r.delta,50);assert.strictEqual(r.bin,2002);
for(let i=0;i<14;i++){
 sandbox.__ANIM.gotoScene(i,true);
 assert.ok(elements['code-guide'].innerHTML.includes('code-block'),`第 ${i+1} 幕缺少源码`);
 for(const p of [0,.3,.78,1]){sandbox.__ANIM.state.t=p*sandbox.__ANIM.scenes[i].dur;sandbox.__ANIM.render();}
}
assert.strictEqual(typeof frame,'function');frame(100);
function set(v){input.value=String(v);handler();return vm.runInContext('result()',sandbox);}
r=set(195);assert.strictEqual(r.primary,false);assert.strictEqual(r.blemish,false);
r=set(185);assert.strictEqual(r.blemish,false);r=set(202);assert.strictEqual(r.blemish,true);
r=set(220);assert.strictEqual(r.primary,false);assert.strictEqual(r.blemish,true);
r=set(234);assert.strictEqual(r.primary,false);assert.strictEqual(r.blemish,true);assert.strictEqual(r.bin,2001);
r=set(235);assert.strictEqual(r.primary,true);assert.strictEqual(r.blemish,false);assert.strictEqual(r.bin,2002);assert.strictEqual(output.textContent,'235');
r=set(147);assert.strictEqual(r.primary,false);r=set(146);assert.strictEqual(r.primary,true);assert.strictEqual(r.bin,2006);
presets['preset-bright']();assert.strictEqual(output.textContent,'245');
presets['preset-dark']();assert.strictEqual(output.textContent,'140');
assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：14 幕、121 格局部、四个 25 像素邻域、阈值、互动与源码');
