/* 仅验证教学模型和画面，不替代生产 OpenCV 检测。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../blemish_test/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);assert.strictEqual(scripts.length,3);
let drawn=[],frame;const gradient={addColorStop(){}};
const ctx=new Proxy({fillText(s){drawn.push(String(s));},measureText(s){return {width:String(s).length*7};}}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}}},events={};
for(const id of ['pd','offset','width']){elements[id]={value:'',addEventListener:(e,fn)=>{events[id]=fn;}};elements[id+'out']={textContent:''};}
for(const id of ['matched','removed','unmatched','large'])elements[id]={addEventListener:(e,fn)=>{events[id]=fn;}};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'blemish:script'+i}).runInContext(sandbox));const run=s=>vm.runInContext(s,sandbox);
const dir=path.join(__dirname,'../../STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps');
const config=fs.readFileSync(path.join(dir,'Blemish_V1.1.data'),'utf8'),entries=Array.from(config.matchAll(/^\[([^\]]+)\]=(.+)$/gm));assert.strictEqual(entries.length,53);assert.ok(!fs.existsSync(path.join(dir,'Blemish.data')));
const vals=Object.fromEntries(entries.map(([,k,v])=>[k,Number(v)]));for(const [k,v] of Object.entries({winSmall:3,winBig:55,threshold_center:.01,threshold_margin:.03,limit_size:3,max_size:38,winSmall_enhace:7,winBig_enhace:55,threshold_center_enhace:.02,threshold_margin_enhace:.03,limit_size_enhace:3,max_size_enhace:1000,search_area_x:100,search_area_y:100,min_enhace_size:3,max_enhace_size:40,magnification:2,pd_area_x:3,pd_area_y:3,bin_blemish:183,grade_blemish:3,open_mode:1,pd_single_open_mode:1}))assert.strictEqual(vals[k],v,k);
assert.ok(run('model(3,51,8).removed'));assert.strictEqual(run('model(3,51,8).failure'),false);assert.strictEqual(run('model(4,50,8).matched'),true);assert.strictEqual(run('model(4,51,8).matched'),false);assert.strictEqual(run('model(4,51,8).failure'),true);
assert.strictEqual(run('model(4,0,20).size'),40);assert.strictEqual(run('model(4,0,20).failure'),true);assert.strictEqual(run('model(4,0,21).size'),42);assert.strictEqual(run('model(4,0,21).failure'),false);
assert.strictEqual(run('model(4,0,1).size'),2);assert.strictEqual(run('model(4,0,1).accepted'),false);assert.strictEqual(run('model(4,0,8).code'),183);
const scenes=sandbox.__ANIM.scenes;assert.strictEqual(scenes.length,15);assert.strictEqual(sandbox.__CODE_GUIDE.length,15);
for(let i=0;i<15;i++){sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'));for(const p of [0,.2,.45,.78,1]){sandbox.__ANIM.state.t=p*scenes[i].dur;sandbox.__ANIM.render();}}
function at(i,p){drawn=[];scenes[i].draw(p);return drawn.slice();}
assert.ok(!at(9,0).some(x=>x.includes('(505,400)')));assert.ok(at(9,.5).some(x=>x.includes('1010,800')));assert.ok(at(9,.8).some(x=>x.includes('横差 10')));
events.removed();assert.strictEqual(elements.pdout.textContent,'3');assert.strictEqual(run('model(state.pd,state.offset,state.width).removed'),true);
events.unmatched();assert.strictEqual(elements.offsetout.textContent,'51');assert.strictEqual(run('model(state.pd,state.offset,state.width).matched'),false);
events.large();assert.strictEqual(elements.widthout.textContent,'21');assert.strictEqual(run('model(state.pd,state.offset,state.width).failure'),false);
elements.width.value='20';events.width();assert.strictEqual(elements.widthout.textContent,'20');assert.strictEqual(run('model(state.pd,state.offset,state.width).failure'),true);
events.matched();assert.strictEqual(run('model(state.pd,state.offset,state.width).failure'),true);assert.strictEqual(typeof frame,'function');frame(100);assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：15 幕、示例配置、PD 排除、搜索及尺寸边界、互动');
