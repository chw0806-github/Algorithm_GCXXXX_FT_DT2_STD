/* 测试假设模型及播放器；不替代生产 C++ 图像检测。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../single_pixel_defect/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);assert.strictEqual(scripts.length,3);
let drawn=[],frame;const gradient={addColorStop(){}};
const ctx=new Proxy({fillText(s){drawn.push(String(s));},measureText(s){return {width:String(s).length*7};}},{get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}}},events={};
for(const id of ['wp','dp']){elements[id]={value:'3',addEventListener:(e,fn)=>events[id]=fn};elements[id+'out']={textContent:''};}
for(const id of ['tie','override','normal'])elements[id]={addEventListener:(e,fn)=>events[id]=fn};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'single:script'+i}).runInContext(sandbox));const run=s=>vm.runInContext(s,sandbox);
const repo=path.join(__dirname,'../..'),cfg=path.join(repo,'STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps');
assert.ok(!fs.existsSync(path.join(cfg,'PD.data')));assert.ok(html.includes('仓库没有 PD.data'));
const basic=fs.readFileSync(path.join(cfg,'Basic.data'),'utf8');for(const v of ['[width]=3840','[height]=2160','[bayer]=RG'])assert.ok(basic.includes(v));
const loader=fs.readFileSync(path.join(repo,'Algorithm_GCXXXX_FT_DT2_STD/Params/parammanage.cpp'),'utf8').split('bool ParamManage::load_Pixel_Defect_Single_Param')[1].split('bool ParamManage::load_Pixel_Defect_Channel_single_Param')[0];
for(const v of ['blemish_spec_r_','blemish_dn_spec_','blemish_global_size_limit_','blemish_bychannel_size_limit_'])assert.ok(!loader.includes(v),`专用加载器未读取 ${v}`);
for(const [n,t,bin] of [[0,0,1],[1,1,101],[2,1,101],[3,2,102],[5,2,102],[6,3,103],[10,3,103]]){assert.strictEqual(run(`tier(${n},HYP.wpBins,HYP.wpGrades).tier`),t);assert.strictEqual(run(`tier(${n},HYP.wpBins,HYP.wpGrades).bin`),bin);}
for(const [n,bin] of [[0,1],[1,201],[2,201],[3,202],[5,202],[6,203]])assert.strictEqual(run(`tier(${n},HYP.dpBins,HYP.dpGrades).bin`),bin);
let r=run('judge(3,3)');assert.strictEqual(r.result.bin,102);assert.strictEqual(r.overridden,false);
r=run('judge(1,3)');assert.strictEqual(r.result.bin,202);assert.strictEqual(r.result.grade,3);
r=run('judge(6,2)');assert.strictEqual(r.count,8);assert.strictEqual(r.result.bin,103);assert.strictEqual(r.overridden,false);
r=run('judge(6,3)');assert.strictEqual(r.chosen.grade,5);assert.strictEqual(r.result.bin,999);assert.strictEqual(r.result.grade,2);assert.strictEqual(r.overridden,true);
assert.strictEqual(run('judge(10,0,0).count'),0);assert.strictEqual(run('judge(0,10,1).count'),0);assert.strictEqual(run('judge(5,4,2).count'),9);assert.strictEqual(run('judge(5,4,9).count'),0);
assert.strictEqual(run('point(245).primary'),true);assert.strictEqual(run('point(234).primary'),false);assert.strictEqual(run('point(245).delta'),50);assert.ok(Math.abs(run('point(245).diff[0]')-65/180)<1e-14);
const scenes=sandbox.__ANIM.scenes;assert.strictEqual(scenes.length,14);assert.strictEqual(sandbox.__CODE_GUIDE.length,14);
for(let i=0;i<scenes.length;i++){sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'));for(const p of [0,.2,.45,.78,1]){sandbox.__ANIM.state.t=p*scenes[i].dur;sandbox.__ANIM.render();}}
function at(i,p){drawn=[];scenes[i].draw(p);return drawn.slice();}
assert.ok(!at(0,0).includes('分别分档'));assert.ok(at(0,1).includes('分别分档'));
assert.ok(!at(5,0).some(s=>s.includes('36.11% > 30%')));assert.ok(at(5,1).some(s=>s.includes('36.11% > 30%')));
events.override();assert.strictEqual(elements.wpout.textContent,'6');assert.strictEqual(elements.dpout.textContent,'3');assert.ok(at(13,1).some(s=>s.includes('bin 999 / grade 2')));
events.tie();assert.strictEqual(run('judge(state.wp,state.dp).result.bin'),102);events.normal();assert.strictEqual(run('judge(state.wp,state.dp).result.pass'),true);
elements.wp.value='5';events.wp();assert.strictEqual(run('state.wp'),5);assert.strictEqual(elements.wpout.textContent,'5');
assert.strictEqual(typeof frame,'function');frame(100);assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：14 幕、缺失配置、假设像素、分档边界、同级与覆盖、互动及源码');
