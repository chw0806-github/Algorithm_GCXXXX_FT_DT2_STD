/* 验证 5×5 模拟、相对差、五点阈值、异常映射和分阶段画面。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../color_gi_5x5/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);assert.strictEqual(scripts.length,3);
let drawn=[],frame,handler;const gradient={addColorStop(){}};
const ctx=new Proxy({fillText(s){drawn.push(String(s));},measureText(s){return {width:String(s).length*7};}}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
const input={value:'230',addEventListener:(e,fn)=>{handler=fn;}},out={textContent:''},buttons={};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}},gr:input,grout:out};
for(const id of ['sample','normal','dark'])elements[id]={addEventListener:(e,fn)=>{buttons[id]=fn;}};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'gi5:script'+i}).runInContext(sandbox));
const run=s=>vm.runInContext(s,sandbox),close=(a,b)=>assert.ok(Math.abs(a-b)<1e-12,`${a} != ${b}`);
const dir=path.join(__dirname,'../../STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps');
assert.ok(!fs.existsSync(path.join(dir,'GI_5x5.data')));assert.ok(html.includes('仓库没有 GI_5x5.data'));assert.ok(html.includes('教学假设'));
const basic=fs.readFileSync(path.join(dir,'Basic.data'),'utf8');for(const line of ['[width]=3840','[height]=2160','[bayer]=RG'])assert.ok(basic.includes(line));
assert.strictEqual(run('N'),82944);assert.strictEqual(1920/5,384);assert.strictEqual(1080/5,216);
assert.deepStrictEqual(Array.from(run('POINTS.map(v=>v.id)')),[0,4,12,20,24]);
assert.deepStrictEqual(Array.from(run('POINTS.map(v=>v.limit)')),[.05,.08,.10,.08,.12]);
const vals=Array.from(run('calculate().vals'));for(let i=0;i<5;i++){
 const g1=[200,210,220,190,230][i]*82944+.1,g2=200*82944+.1;
 close(vals[i].g1,g1);close(vals[i].g2,g2);close(vals[i].avg,(g1+g2)/2);close(vals[i].raw,Math.abs(g1-g2)/((g1+g2)/2));close(vals[i].mapped,vals[i].raw);
}
assert.strictEqual(vals[0].raw,0);close(vals[2].avg,17418240.1);assert.strictEqual(run('calculate().fail'),true);assert.strictEqual(run('calculate(200).fail'),false);assert.strictEqual(run('calculate(160).fail'),true);
close(run('measure(190,200).raw'),run('measure(200,190).raw'));
assert.strictEqual(run('measure(200,0).raw'),100);assert.strictEqual(run('measure(200,0).mapped'),-1);
assert.strictEqual(run('exceeds([-1,-1,-1,-1,-1],POINTS.map(v=>v.limit))'),false);
assert.strictEqual(run('exceeds([.05,.08,.10,.08,.12],POINTS.map(v=>v.limit))'),false);
for(let i=0;i<5;i++)assert.strictEqual(run(`exceeds(POINTS.map((v,j)=>v.limit+(j===${i}?1e-8:0)),POINTS.map(v=>v.limit))`),true);
const scenes=sandbox.__ANIM.scenes;assert.strictEqual(scenes.length,15);assert.strictEqual(sandbox.__CODE_GUIDE.length,15);
for(let i=0;i<15;i++){sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'));for(const p of [0,.2,.45,.78,1]){sandbox.__ANIM.state.t=p*scenes[i].dur;sandbox.__ANIM.render();}}
function at(i,p){drawn=[];scenes[i].draw(p);return drawn.slice();}
for(let i=6;i<=10;i++){
 let text=at(i,0);assert.ok(text.some(x=>x.startsWith('① 这块的输入')));assert.ok(!text.some(x=>x.startsWith('② Gr 和')));
 text=at(i,.25);assert.ok(text.some(x=>x.startsWith('② Gr 和')));assert.ok(!text.some(x=>x.startsWith('③ |Gr')));
 text=at(i,.5);assert.ok(text.some(x=>x.startsWith('③ |Gr')));assert.ok(!text.some(x=>x.startsWith('④ 假设上限')));
 text=at(i,.8);assert.ok(text.some(x=>x.startsWith('④ 假设上限')));assert.notStrictEqual(scenes[i].caption(0),scenes[i].caption(.8));
}
assert.ok(!at(0,0).includes('五点各比自己的上限'));assert.ok(at(0,1).includes('五点各比自己的上限'));
assert.ok(at(14,1).some(x=>x.includes('假设 9001 / 5')));
assert.ok(at(11,1).some(x=>x.includes('本例右下约 0.139535 > 0.12')));
buttons.normal();assert.strictEqual(out.textContent,'200');assert.strictEqual(run('calculate(state.gr).fail'),false);
buttons.dark();assert.strictEqual(out.textContent,'160');assert.strictEqual(run('calculate(state.gr).fail'),true);
input.value='220';handler();assert.strictEqual(out.textContent,'220');assert.strictEqual(run('calculate(state.gr).fail'),false);
buttons.sample();assert.strictEqual(out.textContent,'230');assert.strictEqual(typeof frame,'function');frame(100);assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：15 幕、缺失配置假设、五点相对差、异常映射、阈值和互动');
