/* 15×15 教学假设、完整数据、角/边方向、阈值、互动和所有画面。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../color_shading_15x15/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);assert.strictEqual(scripts.length,3);
let drawn=[],frame;const gradient={addColorStop(){}};
const ctx=new Proxy({fillText(s){drawn.push(String(s));},measureText(s){return {width:String(s).length*7};}}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
const handlers={},buttons={};const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}}};
for(const [id,value] of [['corner','120'],['bottom','80']]){elements[id]={value,addEventListener:(event,fn)=>{handlers[id]=fn;}};elements[id+'out']={textContent:''};}
for(const id of ['sample','normal'])elements[id]={addEventListener:(event,fn)=>{buttons[id]=fn;}};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'shading15:script'+i}).runInContext(sandbox));
const run=s=>vm.runInContext(s,sandbox),close=(a,b)=>assert.ok(Math.abs(a-b)<1e-10,`${a} != ${b}`);
assert.strictEqual(sandbox.__ANIM.scenes.length,16);assert.strictEqual(sandbox.__CODE_GUIDE.length,16);
const base=path.join(__dirname,'../../STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps');
assert.ok(!fs.existsSync(path.join(base,'ColorShading_15x15.data')));
assert.ok(html.includes('仓库没有 ColorShading_15x15.data'));assert.ok(html.includes('教学假设'));
const basic=fs.readFileSync(path.join(base,'Basic.data'),'utf8');for(const line of ['[width]=3840','[height]=2160','[bayer]=RG'])assert.ok(basic.includes(line));
assert.strictEqual(1920/15,128);assert.strictEqual(1080/15,72);assert.strictEqual(128*72,9216);
const a=run('calculate()'),corners=[[0,1,15,16],[195,196,210,211],[13,14,28,29],[208,209,223,224]],edges=[[5,6,7,8,9],[215,216,217,218,219],[75,90,105,120,135],[89,104,119,134,149]];
assert.deepStrictEqual(Array.from(run('CORNERS'),x=>Array.from(x)),corners);assert.deepStrictEqual(Array.from(run('EDGES'),x=>Array.from(x)),edges);
assert.strictEqual(new Set([...corners.flat(),...edges.flat()]).size,36);
assert.strictEqual(a.values.length,225);
const ratios=[];
for(let i=0;i<225;i++){
 let r=100;for(let k=0;k<4;k++){if(corners[k].includes(i))r=[80,90,100,120][k];if(edges[k].includes(i))r=[100,80,100,110][k];}
 const v=a.values[i];assert.deepStrictEqual([v.r,v.gr,v.gb,v.b],[r,200,200,120]);ratios.push(r*120/(200*200+1e-8));close(a.ratios[i],ratios[i]);
}
const avg=ids=>ids.reduce((s,i)=>s+ratios[i],0)/ids.length;
const c=corners.map(avg),e=edges.map(avg),diff=(x,y)=>Math.abs(x/(y+1e-8)-1)*100;
const pairs=[[0,1],[0,2],[0,3],[1,2],[1,3],[2,3]].map(([x,y])=>diff(c[x],c[y]));
for(let i=0;i<4;i++){close(a.corners[i],c[i]);close(a.edges[i],e[i]);}for(let i=0;i<6;i++)close(a.pairs[i],pairs[i]);
close(a.dc,Math.max(...pairs));close(a.tb,diff(e[0],e[1]));close(a.rl,diff(e[3],e[2]));close(a.edge,Math.max(a.tb,a.rl));
assert.ok(Math.abs(a.dc-100/3)<.00001);assert.ok(Math.abs(a.tb-25)<.00001);assert.ok(Math.abs(a.rl-10)<.00001);assert.strictEqual(a.fail,true);
assert.strictEqual(run('exceeds(30,20)'),false);assert.strictEqual(run('exceeds(30.00001,20)'),true);assert.strictEqual(run('exceeds(30,20.00001)'),true);
assert.strictEqual(run('calculate(100,100).fail'),false);
for(let i=0;i<16;i++){
 sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'));
 for(const p of [0,.2,.5,.78,1]){sandbox.__ANIM.state.t=p*sandbox.__ANIM.scenes[i].dur;sandbox.__ANIM.render();}
}
function at(i,p){drawn=[];sandbox.__ANIM.scenes[i].draw(p);return drawn.slice();}
let early=at(4,0),late=at(4,1);assert.ok(!early.some(x=>x.includes('比例 = 9600')));assert.ok(late.some(x=>x.includes('比例 = 9600')));
late=at(8,1);assert.ok(late.some(x=>x.includes('左上 ÷ 右下')));assert.ok(late.some(x=>x.includes('DiffCorner ≈ 33.3333%')));
late=at(11,1);assert.ok(late.includes('右 ÷ (左 + 1e−8)'));assert.ok(late.some(x=>x.includes('Maxdiff = max')));
buttons.normal();assert.strictEqual(run('calculate(state.corner,state.bottom).fail'),false);assert.strictEqual(elements.cornerout.textContent,'100');
buttons.sample();assert.strictEqual(run('calculate(state.corner,state.bottom).fail'),true);assert.strictEqual(elements.bottomout.textContent,'80');
elements.corner.value='100';handlers.corner();assert.strictEqual(run('state.corner'),100);assert.strictEqual(elements.cornerout.textContent,'100');
elements.bottom.value='100';handlers.bottom();assert.strictEqual(run('calculate(state.corner,state.bottom).fail'),false);
assert.strictEqual(typeof frame,'function');frame(100);assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：16 幕、假设配置、225 块、方向公式、阈值边界与交互');
