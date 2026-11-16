/* Color_GI 示例配置、全区域 GG、严格阈值、异常路径与交互。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../color_gi/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);assert.strictEqual(scripts.length,3);
let drawn=[],frame,handler;const gradient={addColorStop(){}};
const ctx=new Proxy({fillText(s){drawn.push(String(s));},measureText(s){return {width:String(s).length*7};}}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
const input={value:'220',addEventListener:(e,fn)=>{handler=fn;}},out={textContent:''},buttons={};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}},gr:input,grout:out};
for(const id of ['sample','normal','near'])elements[id]={addEventListener:(e,fn)=>{buttons[id]=fn;}};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'gi:script'+i}).runInContext(sandbox));
const run=s=>vm.runInContext(s,sandbox),close=(a,b)=>assert.ok(Math.abs(a-b)<1e-12,`${a} != ${b}`);
const dir=path.join(__dirname,'../../STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps');
function config(name){return Object.fromEntries(Array.from(fs.readFileSync(path.join(dir,name),'utf8').matchAll(/^\[([^\]]+)\]=([^\r\n]+)/gm),m=>[m[1],m[2]]));}
const gi=config('GI.data'),basic=config('Basic.data');
for(const [k,v] of Object.entries({Skip:'0',ROI_X_Cnt:'9',ROI_Y_Cnt:'9',GI_Ratio:'0.08',bin_GI:'157',grade_GI:'3',ratio_open_mode:'1'}))assert.strictEqual(gi[k],v);
assert.strictEqual(basic.width,'3840');assert.strictEqual(basic.height,'2160');assert.strictEqual(basic.bayer,'RG');assert.ok(!fs.existsSync(path.join(dir,'GI_V1.1.data')));
assert.strictEqual(run('N'),25560);assert.strictEqual(1920-9*213,3);
for(let i=0;i<81;i++){
 const v=run(`values(${i})`),gr=i===10?190:i===70?220:200;
 assert.deepStrictEqual([v.r,v.gr,v.gb,v.b],[100,gr,200,120]);
 close(run(`gg(values(${i}))`),Math.abs((gr*25560+.1)/(200*25560+.1)));
}
const d=run('calc()');assert.strictEqual(d.ratios.length,81);assert.strictEqual(d.lo,10);assert.strictEqual(d.hi,70);
close(d.min,(190*25560+.1)/(200*25560+.1));close(d.max,(220*25560+.1)/(200*25560+.1));
close(d.gi,Math.max(Math.abs(d.max-1),Math.abs(d.min-1)));assert.strictEqual(d.fail,true);
assert.strictEqual(run('decide(.08)'),false);assert.strictEqual(run('decide(.08+1e-10)'),true);assert.strictEqual(run('decide(.08-1e-10)'),false);
assert.strictEqual(run('gg({gr:200,gb:0})'),100);assert.strictEqual(run('calc(216).fail'),false);
assert.strictEqual(run('calc(216).gi.toFixed(6)'),'0.080000');assert.ok(run('calc(216).gi')<.08);assert.strictEqual(run('calc(217).fail'),true);
// 独立复现仅与前置状态有关的 V1.0 分支；这些不是交互值。
function precondition(gr,gb,values){const ratios=(gr>0&&gb>0)?values:Array(81).fill(-1);const hi=Math.max(...ratios),lo=Math.min(...ratios);if(!(gr>10))return {tested:false,pass:true,log:-1};const value=Math.max(Math.abs(hi-1),Math.abs(lo-1));return {tested:true,value,pass:!(value>Number(gi.GI_Ratio))};}
assert.deepStrictEqual(precondition(10,200,[1]),{tested:false,pass:true,log:-1});
assert.deepStrictEqual(precondition(11,0,[1]),{tested:true,value:2,pass:false});
assert.deepStrictEqual(precondition(11,200,[1]),{tested:true,value:0,pass:true});
const scenes=sandbox.__ANIM.scenes;assert.strictEqual(scenes.length,14);assert.strictEqual(sandbox.__CODE_GUIDE.length,14);
for(let i=0;i<14;i++){sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'));for(const p of [0,.2,.45,.78,1]){sandbox.__ANIM.state.t=p*scenes[i].dur;sandbox.__ANIM.render();}}
function at(i,p){drawn=[];scenes[i].draw(p);return drawn.slice();}
let first=at(4,0),last=at(4,1);assert.ok(!first.some(x=>x.startsWith('GG =')));assert.ok(last.some(x=>x.includes('4856400.1')));assert.ok(last.some(x=>x.startsWith('GG =')));
first=at(8,0);last=at(8,1);assert.ok(!first.some(x=>x.startsWith('GI ≈')));assert.ok(last.some(x=>x.startsWith('GI ≈')));
// 极值标记在对应步骤出现，不提前展示最终结果。
let marks;const originalGrid=sandbox.grid;sandbox.grid=ids=>{marks=Array.from(ids);};
scenes[6].draw(0);assert.deepStrictEqual(marks,[]);
scenes[6].draw(.2);assert.deepStrictEqual(marks,[10]);
scenes[6].draw(.4);assert.deepStrictEqual(marks,[10,70]);sandbox.grid=originalGrid;
assert.notStrictEqual(scenes[6].caption(0),scenes[6].caption(.2));assert.notStrictEqual(scenes[6].caption(.2),scenes[6].caption(.4));
buttons.normal();assert.strictEqual(out.textContent,'200');assert.strictEqual(run('calc(state.gr).fail'),false);
buttons.near();assert.strictEqual(out.textContent,'216');assert.strictEqual(run('calc(state.gr).fail'),false);assert.ok(at(13,1).includes('GI ≈ 0.080000'));
input.value='217';handler();assert.strictEqual(out.textContent,'217');assert.strictEqual(run('calc(state.gr).fail'),true);
buttons.sample();assert.strictEqual(out.textContent,'220');assert.strictEqual(typeof frame,'function');frame(100);assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：14 幕、真实配置、81 块 GG、精确阈值、前置异常与交互');
