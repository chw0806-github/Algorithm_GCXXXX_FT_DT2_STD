/* 对真实配置、完整模拟数据、动画阶段、阈值、场景及互动做无浏览器回归。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../color_shading/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);
assert.strictEqual(scripts.length,3);
let drawn=[];const gradient={addColorStop(){}};
const ctx=new Proxy({fillText(s){drawn.push(String(s));}}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
let handler,leftHandler,inspectHandler,frame;const buttons={};
const input={value:'130',addEventListener:(ev,fn)=>{handler=fn;}},out={textContent:''};
const inspect={value:'110',addEventListener:(ev,fn)=>{inspectHandler=fn;}},inspectOut={textContent:''};
const left={value:'70',addEventListener:(ev,fn)=>{leftHandler=fn;}},leftOut={textContent:''};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}},rval:input,rout:out,lval:left,lout:leftOut,inspect,inspectout:inspectOut};
for(const id of ['sample','pass'])elements[id]={addEventListener:(ev,fn)=>{buttons[id]=fn;}};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'shading:script'+i}).runInContext(sandbox));
const run=s=>vm.runInContext(s,sandbox),close=(a,b)=>assert.ok(Math.abs(a-b)<1e-12,`${a} != ${b}`);
const configDir=path.join(__dirname,'../../STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps');
function config(name){return Object.fromEntries(Array.from(fs.readFileSync(path.join(configDir,name),'utf8').matchAll(/^\[([^\]]+)\]=([^\r\n]+)/gm),m=>[m[1],m[2]]));}
const c=config('ColorShading.data'),basic=config('Basic.data'),ratio=config('ColorRatio.data');
for(const [k,v] of Object.entries({Skip:'0',ROI_X_Cnt:'17',ROI_Y_Cnt:'13',Factor_Center_Class1:'0.2',Factor_Center_Class2:'0.2',bin_shading:'2227',grade_shading:'5',bin_shading_MA:'2228',grade_shading_MA:'2',open_mode:'1'}))assert.strictEqual(c[k],v,k);
assert.strictEqual(ratio.ratio_open_mode,'1');assert.strictEqual(basic.width,'3840');assert.strictEqual(basic.height,'2160');
assert.ok(!fs.existsSync(path.join(configDir,'ColorShading_V1.1.data')));
assert.strictEqual(run('COUNT'),221);assert.strictEqual(run('N'),9296);assert.strictEqual(Math.floor(1920/17),112);assert.strictEqual(Math.floor(1080/13),83);
assert.strictEqual(1920-17*112,16);assert.strictEqual(1080-13*83,1);assert.strictEqual(Math.floor(221/2),110);
// 按每个像素独立累加，与每一块的画面公式比较；再独立算出全 221 块极值。
const rg=[],bg=[];
for(let i=0;i<221;i++){
 const v={r:i===0?70:i===220?130:100,gr:200,gb:200,b:i===16?100:i===204?140:120};
 let sums={r:.1,gr:.1,gb:.1,b:.1};for(let n=0;n<9296;n++)for(const ch of Object.keys(sums))sums[ch]+=v[ch];
 const g=(sums.gr+sums.gb)/2,actual=run(`roiRatios(${i},130)`);
 for(const [a,b] of [['R','r'],['Gr','gr'],['Gb','gb'],['B','b']])assert.ok(Math.abs(actual[a]-sums[b])<1e-6);
 close(actual.rg,sums.r/g);close(actual.bg,sums.b/g);rg.push(actual.rg);bg.push(actual.bg);
}
const d=run('calc(130)');
assert.deepStrictEqual([d.R.minIndex,d.R.maxIndex,d.B.minIndex,d.B.maxIndex],[0,220,16,204]);
close(d.R.min,Math.min(...rg));close(d.R.max,Math.max(...rg));close(d.B.min,Math.min(...bg));close(d.B.max,Math.max(...bg));
close(d.R.shade,Math.max(Math.abs(Math.max(...rg)/rg[110]-1),Math.abs(Math.min(...rg)/rg[110]-1)));
close(d.B.shade,Math.max(Math.abs(Math.max(...bg)/bg[110]-1),Math.abs(Math.min(...bg)/bg[110]-1)));
assert.ok(Math.abs(d.R.shade-.3)<1e-6);assert.ok(Math.abs(d.B.shade-1/6)<1e-6);assert.ok(d.csFail);assert.ok(!d.maFail);
for(const [r,b,c1,c2,cs,ma] of [[.2,.2,.2,.2,false,false],[.200001,0,.2,.2,true,false],[0,.200001,.2,.2,true,false],[.15,0,.1,.2,false,true],[.21,.15,.1,.2,true,false]]){
 const v=run(`decide(${r},${b},${c1},${c2})`);assert.strictEqual(v.csFail,cs);assert.strictEqual(v.maFail,ma);
}
const scenes=sandbox.__ANIM.scenes;assert.strictEqual(scenes.length,14);assert.strictEqual(sandbox.__CODE_GUIDE.length,14);
function renderAt(i,p){sandbox.__ANIM.gotoScene(i,true);drawn=[];sandbox.__ANIM.state.t=p*scenes[i].dur;sandbox.__ANIM.render();return drawn.slice();}
for(let i=0;i<14;i++){
 sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'));
 for(const p of [0,.18,.3,.42,.55,.6,.76,.82,1]){sandbox.__ANIM.state.t=p*scenes[i].dur;sandbox.__ANIM.render();}
}
let text=renderAt(4,0);assert.ok(text.includes('① 四通道模拟值'));assert.ok(!text.some(x=>x.startsWith('④ RG =')));
text=renderAt(4,.61);assert.ok(text.some(x=>x.startsWith('③ G 参考和')));assert.ok(text.some(x=>x.startsWith('④ RG =')));assert.ok(!text.some(x=>x.startsWith('⑤ BG =')));
text=renderAt(4,.8);assert.ok(text.some(x=>x.startsWith('⑤ BG =')));assert.ok(!text.some(x=>x.includes('刚累加')));
text=renderAt(5,0);assert.ok(text.includes('正在看 ROI 0：1 / 221'));assert.ok(!text.includes('完成：全部 221 块都已算过'));
text=renderAt(5,1);assert.ok(text.includes('正在看 ROI 220：221 / 221'));assert.ok(text.includes('完成：全部 221 块都已算过'));
for(const [idx,min,max] of [[6,0,220],[7,16,204]]){
 text=renderAt(idx,0);assert.ok(!text.includes('最小　ROI '+min));assert.ok(!text.includes('最大　ROI '+max));
 text=renderAt(idx,.4);assert.ok(text.includes('最小　ROI '+min));assert.ok(!text.includes('最大　ROI '+max));
 text=renderAt(idx,.8);assert.ok(text.includes('最小　ROI '+min));assert.ok(text.includes('最大　ROI '+max));
}
for(const i of [8,9]){
 text=renderAt(i,0);assert.ok(!text.some(x=>x.includes('÷')));assert.ok(!text.some(x=>x.includes('shading =')));
 text=renderAt(i,.3);assert.strictEqual(text.filter(x=>x.includes('÷')).length,1);
 text=renderAt(i,.5);assert.strictEqual(text.filter(x=>x.includes('÷')).length,2);
 text=renderAt(i,.8);assert.ok(text.some(x=>x.includes('shading =')));
}
inspect.value='37';inspectHandler();assert.strictEqual(inspectOut.textContent,'37');assert.strictEqual(sandbox.__ANIM.state.scene,4);drawn=[];sandbox.__ANIM.state.t=.8*scenes[4].dur;sandbox.__ANIM.render();assert.ok(drawn.includes('选中 ROI 37（模拟）'));
buttons.pass();assert.strictEqual(out.textContent,'100');assert.strictEqual(leftOut.textContent,'100');assert.ok(!run('calc(state.r,state.l).csFail'));
input.value='160';handler();assert.strictEqual(out.textContent,'160');assert.strictEqual(run('calc(state.r,state.l).R.maxIndex'),220);assert.ok(run('calc(state.r,state.l).csFail'));
left.value='70';leftHandler();assert.strictEqual(leftOut.textContent,'70');assert.strictEqual(run('calc(state.r,state.l).R.minIndex'),0);
buttons.sample();assert.strictEqual(out.textContent,'130');assert.strictEqual(leftOut.textContent,'70');assert.strictEqual(typeof frame,'function');frame(100);
assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：14 幕、221 ROI、9296 像素、分阶段动画、极值、阈值与互动');
