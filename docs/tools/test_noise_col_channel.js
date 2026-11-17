/* 动画计算与 UI 模拟测试；不替代真实 C++ 或浏览器测试。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../noise_col_channel/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);assert.strictEqual(scripts.length,3);
let drawn=[],frame;const gradient={addColorStop(){}};
const ctx=new Proxy({fillText(s){drawn.push(String(s));},measureText(s){return {width:String(s).length*7};}},{get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
const events={},input={value:'3',addEventListener:(e,f)=>events.amp=f},out={textContent:''};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}},amp:input,ampout:out};
for(const id of ['normal','example','flat'])elements[id]={addEventListener:(e,f)=>events[id]=f};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:f=>{frame=f;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'noise:script'+i}).runInContext(sandbox));const run=s=>vm.runInContext(s,sandbox);
const dir=path.join(__dirname,'../../STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_dark1_normal_30fps');
function config(name){return Object.fromEntries(Array.from(fs.readFileSync(path.join(dir,name),'utf8').matchAll(/^\[([^\]]+)\]=([^\r\n]+)/gm),m=>[m[1],m[2].trim()]));}
const c=config('Channel_Noise.data'),b=config('Basic.data');for(const [k,v] of Object.entries({Column_Noise_Max:'3.2',bin:'2075',grade:'5',open_mode:'1'}))assert.strictEqual(c[k],v,k);assert.strictEqual(b.width,'3840');assert.strictEqual(b.height,'2160');assert.strictEqual(b.bayer,'RG');assert.ok(!fs.existsSync(path.join(dir,'Channel_Noise_V1.1.data')));
// 独立生成四帧及 1920 个列均值，计算样本标准差和最大差。
function std(v){const avg=v.reduce((a,b)=>a+b,0)/v.length;return Math.sqrt(v.reduce((s,x)=>s+(x-avg)**2,0)/(v.length-1));}
for(let k=1;k<=4;k++){const base=std([97,99,101,103]),peak=std([100-3*k,100-k,100+k,100+3*k]);const cols=Array(1920).fill(base);cols[300]=peak;const avg=cols.reduce((a,b)=>a+b,0)/cols.length,diff=Math.max(...cols)-avg;const got=run(`evaluate(${k})`);assert.ok(Math.abs(got.base-base)<1e-12);assert.ok(Math.abs(got.peak-peak)<1e-12);assert.ok(Math.abs(got.avg-avg)<1e-10);assert.ok(Math.abs(got.diff-diff)<1e-10);assert.strictEqual(got.fail,diff>3.2&&diff>0);}
assert.strictEqual(run('limit(3.2)'),false);assert.strictEqual(run('limit(3.2000001)'),true);assert.strictEqual(run('limit(0)'),false);assert.strictEqual(run('limit(NaN)'),false);
const scenes=sandbox.__ANIM.scenes;assert.strictEqual(scenes.length,14);assert.strictEqual(sandbox.__CODE_GUIDE.length,14);
for(let i=0;i<scenes.length;i++){sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'));for(const p of [0,.2,.45,.78,1]){sandbox.__ANIM.state.t=p*scenes[i].dur;sandbox.__ANIM.render();}}
function at(i,p){drawn=[];scenes[i].draw(p);return drawn.slice();}assert.ok(!at(0,0).includes('最大列 − 通道平均'));assert.ok(at(0,1).includes('最大列 − 通道平均'));assert.ok(at(8,1).some(s=>s.includes('5.16129')));assert.ok(at(10,1).some(s=>s.includes('2075 / grade 5')));
events.normal();assert.strictEqual(out.textContent,'2');assert.strictEqual(run('evaluate(state.amp).fail'),false);assert.ok(at(13,1).includes('模拟未超限'));events.example();assert.strictEqual(out.textContent,'3');assert.strictEqual(run('evaluate(state.amp).fail'),true);events.flat();assert.strictEqual(run('evaluate(state.amp).diff'),0);input.value='4';events.amp();assert.strictEqual(out.textContent,'4');assert.strictEqual(run('evaluate(state.amp).fail'),true);
assert.strictEqual(typeof frame,'function');frame(100);assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);console.log('OK：14 幕、真实配置、帧间样本 STD、列平均、严格边界和互动');
