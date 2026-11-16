/* 模拟动画、棋盘数值和源码边界；不替代真实 C++ 测试。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../roi_prnu/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);assert.strictEqual(scripts.length,3);
let drawn=[],frame;const gradient={addColorStop(){}};
const ctx=new Proxy({fillText(s){drawn.push(String(s));},measureText(s){return {width:String(s).length*7};}},{get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
const events={},input={value:'10',addEventListener:(e,f)=>events.amp=f},out={textContent:''};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}},amp:input,ampout:out};
for(const id of ['normal','example','zero'])elements[id]={addEventListener:(e,f)=>events[id]=f};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:f=>{frame=f;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'roi-prnu:script'+i}).runInContext(sandbox));const run=s=>vm.runInContext(s,sandbox);
const dir=path.join(__dirname,'../../STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps');
function config(name){return Object.fromEntries(Array.from(fs.readFileSync(path.join(dir,name),'utf8').matchAll(/^\[([^\]]+)\]=([^\r\n]+)/gm),m=>[m[1],m[2].trim()]));}
const c=config('ROI_PRNU.data'),b=config('Basic.data'),p=config('PixelDefect.data'),l=config('LightResponse.data');
for(const [k,v] of Object.entries({X_ROI:'768',Y_ROI:'432',W_ROI:'384',H_ROI:'216',SpecR:'3',SpecG1:'3',SpecG2:'3',SpecB:'3',bin:'2238',grade:'3',open_mode:'1'}))assert.strictEqual(c[k],v,k);
assert.strictEqual(b.width,'3840');assert.strictEqual(b.height,'2160');assert.strictEqual(b.bayer,'RG');assert.strictEqual(p.radius,'5');assert.strictEqual(p.open_mode,'1');assert.strictEqual(l.response_open_mode,'1');
for(const [k,v] of Object.entries({W:1920,H:1080,X:768,Y:432,RW:384,RH:216,N:82944}))assert.strictEqual(run(k),v);
// 独立生成目标四个 5×5 象限，验证奇偶性而非照搬模型常数。
for(const target of [190,210])for(const sy of [-1,1])for(const sx of [-1,1]){let values=[];for(let y=1;y<=5;y++)for(let x=1;x<=5;x++)values.push((Math.abs(sx*x+sy*y)%2===0)?target:400-target);assert.strictEqual(values.filter(v=>v===target).length,13);assert.strictEqual(values.reduce((a,b)=>a+b,0)/25,target===190?199.6:200.4);}
const d=run('sample(10)'),N=82944,dl=Math.fround(190-Math.fround(199.6)),dh=Math.fround(210-Math.fround(200.4));
assert.strictEqual(d.dl,dl);assert.strictEqual(d.dh,dh);assert.strictEqual(d.squares,N/2*(Math.fround(dl*dl)+Math.fround(dh*dh)));assert.ok(Math.abs(d.sigma-9.60006)<.00002);assert.ok(Math.abs(d.prnu-4.80003)<.00002);
assert.strictEqual(run('sample(0).prnu'),0);assert.ok(run('sample(5).prnu')<3);assert.ok(run('sample(7).prnu')>3);
assert.strictEqual(run('classify(3,3,3,3)'),false);assert.strictEqual(run('classify(3.00001,0,0,0)'),true);assert.strictEqual(run('classify(NaN,0,0,0)'),false);assert.strictEqual(run('classify(Infinity,0,0,0)'),true);
// 复现源码右/下边界（包括恰好贴边被拒绝），不声称其他非法输入已被防护。
const rejected=(x,y,w,h)=>y+h>=1080||x+w>=1920;
assert.strictEqual(rejected(768,432,384,216),false);assert.strictEqual(rejected(1536,432,384,216),true);assert.strictEqual(rejected(768,864,384,216),true);assert.strictEqual(rejected(1535,863,384,216),false);
const scenes=sandbox.__ANIM.scenes;assert.strictEqual(scenes.length,14);assert.strictEqual(sandbox.__CODE_GUIDE.length,14);
for(let i=0;i<scenes.length;i++){sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'));for(const p of [0,.2,.45,.78,1]){sandbox.__ANIM.state.t=p*scenes[i].dur;sandbox.__ANIM.render();}}
function at(i,p){drawn=[];scenes[i].draw(p);return drawn.slice();}
assert.ok(!at(0,0).includes('原始像素均值'));assert.ok(at(0,1).includes('原始像素均值'));assert.ok(at(7,1).some(s=>s.includes('9.60006')));assert.ok(at(9,1).some(s=>s.includes('2238 / grade 3')));
events.normal();assert.strictEqual(out.textContent,'5');assert.ok(!run('classify(sample(state.amp).prnu,0,0,0)'));assert.ok(at(13,1).includes('四通道未超限（假设）'));
events.example();assert.strictEqual(out.textContent,'10');assert.ok(run('classify(sample(state.amp).prnu,0,0,0)'));events.zero();assert.strictEqual(run('sample(state.amp).prnu'),0);input.value='7';events.amp();assert.strictEqual(out.textContent,'7');
assert.strictEqual(typeof frame,'function');frame(100);assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：14 幕、真实配置、棋盘邻域、Delta 平方、阈值边界、无效数值与互动');
