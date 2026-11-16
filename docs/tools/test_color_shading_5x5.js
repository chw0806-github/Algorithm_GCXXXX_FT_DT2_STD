/* 模拟播放与算法边界测试；不依赖浏览器或第三方库。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../color_shading_5x5/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);assert.strictEqual(scripts.length,3);
let drawn=[],frame,inputHandler;const buttons={},gradient={addColorStop(){}};
const ctx=new Proxy({fillText(s){drawn.push(String(s));}}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
const input={value:'100',addEventListener:(ev,fn)=>{inputHandler=fn;}},out={textContent:''};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}},bval:input,bout:out};
for(const id of ['sample','pass'])elements[id]={addEventListener:(ev,fn)=>{buttons[id]=fn;}};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'shading5:script'+i}).runInContext(sandbox));
const run=s=>vm.runInContext(s,sandbox),close=(a,b)=>assert.ok(Math.abs(a-b)<1e-12,`${a} != ${b}`);
const dir=path.join(__dirname,'../../STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps');
function config(name){return Object.fromEntries(Array.from(fs.readFileSync(path.join(dir,name),'utf8').matchAll(/^\[([^\]]+)\]=([^\r\n]+)/gm),m=>[m[1],m[2]]));}
const conf=config('ColorShading_5x5.data'),basic=config('Basic.data');
for(const [k,v] of Object.entries({Skip:'0',ROI_X_Cnt:'5',ROI_Y_Cnt:'5',bin_shading:'158',grade_shading:'5',open_mode:'1'}))assert.strictEqual(conf[k],v,k);
assert.strictEqual(basic.width,'3840');assert.strictEqual(basic.height,'2160');
for(const k of ['RG1','RG2','RG3','BG1','BG2','BG3']){assert.strictEqual(conf[k+'_min'],'0.9');assert.strictEqual(conf[k+'_max'],'1.1');assert.deepStrictEqual(Array.from(run(`LIMITS.${k}`)),[.9,1.1]);}
assert.strictEqual(run('W'),384);assert.strictEqual(run('H'),216);assert.strictEqual(run('N'),82944);
assert.deepStrictEqual(Array.from(run('POINTS.map(p=>p.id)')),[0,4,12,20,24]);
const q=Array.from(run('pointRatios(100)')),rv=run('calculate(pointRatios(100))');
for(let i=0;i<5;i++){const v=[[100,120],[108,125],[100,120],[100,120],[100,100]][i];const green=200*82944+.1;close(q[i].rg,(v[0]*82944+.1)/green);close(q[i].bg,(v[1]*82944+.1)/green);}
for(const channel of ['rg','bg']){
 const prefix=channel.toUpperCase(),[a,b,c,d,e]=q.map(x=>x[channel]);
 close(rv.result[prefix+'1'],a/e);close(rv.result[prefix+'2'],b/d);close(rv.result[prefix+'3'],4*c/(a+b+d+e));
}
assert.ok(rv.guard);assert.strictEqual(run('firstFail(calculate(pointRatios(100)).result,LIMITS)'),'BG1');
assert.strictEqual(run('firstFail(calculate(pointRatios(120)).result,LIMITS)'),null);
assert.strictEqual(run('firstFail(calculate(pointRatios(70)).result,LIMITS)'),'BG3');
// 输入边界与 else-if 的第一个失败顺序；NaN 按 JS/C++ 普通比较语义不触发越界。
for(const [expr,expected] of [
 ["({RG1:.9,RG2:1.1,RG3:1,BG1:1,BG2:1,BG3:1})",null],
 ["({RG1:2,RG2:2,RG3:2,BG1:2,BG2:2,BG3:2})",'RG3'],
 ["({RG1:2,RG2:2,RG3:1,BG1:2,BG2:2,BG3:2})",'RG1'],
 ["({RG1:1,RG2:1,RG3:1,BG1:2,BG2:2,BG3:2})",'BG3'],
 ["({RG1:1,RG2:1,RG3:NaN,BG1:1,BG2:1,BG3:1})",null]
])assert.strictEqual(run(`firstFail(${expr},LIMITS)`),expected);
// 还原源码的重复条件、默认 2、未检查 BG 右下及 -1 前置异常。
let z=run("calculate([{rg:1,bg:1},{rg:1,bg:1},{rg:1,bg:1},{rg:1,bg:1},{rg:0,bg:1}])");
assert.ok(!z.guard);for(const k of ['RG1','RG2','RG3','BG1','BG2','BG3'])assert.strictEqual(z.result[k],2);
z=run("calculate([{rg:1,bg:1},{rg:1,bg:1},{rg:1,bg:1},{rg:1,bg:1},{rg:1,bg:0}])");assert.ok(z.guard);assert.strictEqual(z.result.BG1,Infinity);
z=run("calculate(Array.from({length:5},()=>({rg:-1,bg:-1})))");assert.ok(z.guard);for(const k of ['RG1','RG2','RG3','BG1','BG2','BG3'])assert.strictEqual(z.result[k],1);sandbox.invalid=z.result;assert.strictEqual(run('firstFail(invalid,LIMITS)'),null);
const scenes=sandbox.__ANIM.scenes;assert.strictEqual(scenes.length,16);assert.strictEqual(sandbox.__CODE_GUIDE.length,16);
function renderAt(i,p){sandbox.__ANIM.gotoScene(i,true);drawn=[];sandbox.__ANIM.state.t=p*scenes[i].dur;sandbox.__ANIM.render();return drawn.slice();}
for(let i=0;i<16;i++){sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'));for(const p of [0,.2,.4,.72,1]){sandbox.__ANIM.state.t=p*scenes[i].dur;sandbox.__ANIM.render();}}
let overview=renderAt(0,0);assert.ok(!overview.includes('四角对角'));overview=renderAt(0,.8);assert.ok(overview.includes('四角对角'));assert.ok(overview.includes('五点：0 / 4 / 12 / 20 / 24'));
let interactive=renderAt(15,1);assert.ok(interactive.includes('按源码顺序首个超限：BG1'));
let center=renderAt(3,0);assert.ok(center.some(t=>t.startsWith('① 虚构像素')));assert.ok(!center.some(t=>t.startsWith('④ RG =')));
center=renderAt(3,.62);assert.ok(center.some(t=>t.startsWith('③ 绿色参考和')));assert.ok(center.some(t=>t.startsWith('④ RG =')));assert.ok(!center.some(t=>t.startsWith('⑤ BG =')));
center=renderAt(3,.8);assert.ok(center.some(t=>t.startsWith('⑤ BG =')));
for(let i=5;i<=10;i++){
 let text=renderAt(i,0);assert.ok(text.includes('① 取 '+(i<8?'RG':'BG')+' 比例'));assert.ok(!text.includes('② 代入公式'));assert.ok(!text.some(t=>t.startsWith('③ 范围预览')));
 text=renderAt(i,.4);assert.ok(text.includes('② 代入公式'));assert.ok(!text.some(t=>t.startsWith('③ 范围预览')));
 text=renderAt(i,.8);assert.ok(text.some(t=>t.startsWith('③ 范围预览')));
}
let text=renderAt(11,.8);assert.ok(text.includes('超限 → 失败'));assert.ok(text.includes('未执行'));
buttons.pass();assert.strictEqual(out.textContent,'120');assert.strictEqual(run('firstFail(calculate(pointRatios(state.b)).result,LIMITS)'),null);
input.value='70';inputHandler();assert.strictEqual(out.textContent,'70');assert.strictEqual(run('firstFail(calculate(pointRatios(state.b)).result,LIMITS)'),'BG3');
buttons.sample();assert.strictEqual(out.textContent,'100');assert.strictEqual(typeof frame,'function');frame(100);assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：16 幕、真实配置、五点比例、六项公式、顺序、异常分支与互动');
