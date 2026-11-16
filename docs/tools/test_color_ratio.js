/* 测试真实 ROI 尺寸、模拟值、源码公式与 27 幕播放。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../color_ratio/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);assert.strictEqual(scripts.length,3);
const gradient={addColorStop(){}};
let drawn=[];
const ctx=new Proxy({fillText(s){drawn.push(String(s));}}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
let handler,frame;const buttons={};
const input={value:'90.5',addEventListener:(event,fn)=>{handler=fn;}},out={textContent:''};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}},rmean:input,rout:out};
for(const id of ['sample','notgolden','fail'])elements[id]={addEventListener:(event,fn)=>{buttons[id]=fn;}};
// 从实际生成的页面读取倍速按钮，模拟点击和选中状态。
const speedButtons=Array.from(html.matchAll(/<button data-speed="([\d.]+)"(?: class="([^"]+)")?>([^<]+)<\/button>/g),m=>{
 const classes=new Set((m[2]||'').split(/\s+/).filter(Boolean));
 return {dataset:{speed:m[1]},label:m[3],classList:{toggle(name,on){if(on)classes.add(name);else classes.delete(name);},contains:name=>classes.has(name)},addEventListener(event,fn){if(event==='click')this.click=fn;}};
});
const document={getElementById:id=>elements[id]||null,querySelectorAll:selector=>selector==='.speed button'?speedButtons:[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'color:script'+i}).runInContext(sandbox));
const run=s=>vm.runInContext(s,sandbox);
assert.strictEqual(sandbox.__ANIM.scenes.length,27);assert.strictEqual(sandbox.__CODE_GUIDE.length,27);
assert.deepStrictEqual(speedButtons.map(b=>b.label),['0.5×','1×','2×','3×']);
const triple=speedButtons.find(b=>b.dataset.speed==='3'),normal=speedButtons.find(b=>b.dataset.speed==='1');
triple.click();assert.strictEqual(sandbox.__ANIM.state.speed,3);assert.ok(triple.classList.contains('on'));assert.ok(!normal.classList.contains('on'));
frame(50);assert.ok(Math.abs(sandbox.__ANIM.state.t-.15)<1e-10,'3× 播放应在 50ms 内推进 150ms');
normal.click();assert.strictEqual(sandbox.__ANIM.state.speed,1);assert.ok(normal.classList.contains('on'));assert.ok(!triple.classList.contains('on'));
frame(100);assert.ok(Math.abs(sandbox.__ANIM.state.t-.2)<1e-10,'切回 1× 后应正常推进');
assert.strictEqual(run('N'),25560);
for(const [ch,low,avg] of [['R',90,90.5],['Gr',189,189.5],['Gb',191,191.5],['B',117,117.5]]){
 const a=Array.from(run(`sample('${ch}')`));assert.strictEqual(a.length,25560);
 assert.ok(a.every((v,i)=>v===low+i%2));
 assert.strictEqual(a.reduce((x,y)=>x+y,0)/a.length,avg);
}
const pitchX=Math.floor(1920/9),pitchY=Math.floor(1080/9),roi=[];
for(let i=0;i<9;i++)for(let j=0;j<9;j++)roi.push({x:j*pitchX,y:i*pitchY,w:pitchX,h:pitchY});
assert.strictEqual(pitchX,213);assert.strictEqual(pitchY,120);assert.strictEqual(roi.length,81);assert.strictEqual(1920-(roi[8].x+roi[8].w),3);
assert.deepStrictEqual([0,8,40,72,80].map(n=>[roi[n].x,roi[n].y]),[[0,0],[1704,0],[852,480],[0,960],[1704,960]]);
const configDir=path.join(__dirname,'../../STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps');
const data=fs.readFileSync(path.join(configDir,'ColorRatio.data'),'utf8');
const config=Object.fromEntries(Array.from(data.matchAll(/^\[([^\]]+)\]=([^\r\n]+)/gm),m=>[m[1],Number(m[2])]));
for(const [key,value] of Object.entries({Skip:0,ROI_X_Cnt:9,ROI_Y_Cnt:9,TargetRatioRG_center:.55,TargetRatioBG_center:.61,TargetRatioGG_center:1,TargetRatioRG_center_offset:.07,TargetRatioBG_center_offset:-.008,Factor_Center_Class1:.2,Factor_Center_Class2:.2,Factor_Corner_Class1:.5,Factor_Corner_Class2:.5,Factor_Center_Gr_Gb_Ratio:.08,Factor_Golden:.02,Gr_Gb_Diff:50,bin_ratio:2220,bin_ratio_MA:2221,bin_GI:2219,bin_ratio_Golden:2222,ratio_open_mode:1}))assert.strictEqual(config[key],value,key);
const close=(a,b)=>assert.ok(Math.abs(a-b)<1e-10,`${a} vs ${b}`);
let r=run('ratios(90.5)');
close(r.rg,2313180.1/4869180.1);close(r.bg,3003300.1/4869180.1);close(r.gg,4843620.1/4894740.1);close(r.gdiff,2);
assert.strictEqual(r.centerRGPass,true);assert.strictEqual(r.goldenRG,true);assert.strictEqual(r.goldenBG,true);
r=run('ratios(100)');assert.strictEqual(r.centerRGPass,true);assert.strictEqual(r.goldenRG,false);assert.strictEqual(r.goldenBG,true);
r=run('ratios(125)');assert.strictEqual(r.centerRGPass,false);
// 反推中心比例的均值并在每个边界两侧测试；浮点边界不依赖显示值。
const green=190.5*25560+.1;
const rForCorrected=corrected=>((corrected-.07)*green-.1)/25560;
for(const [value,property,expected] of [[.44-1e-6,'centerRGPass',false],[.44+1e-6,'centerRGPass',true],[.66-1e-6,'centerRGPass',true],[.66+1e-6,'centerRGPass',false],[.539-1e-6,'goldenRG',false],[.539+1e-6,'goldenRG',true],[.561-1e-6,'goldenRG',true],[.561+1e-6,'goldenRG',false]])assert.strictEqual(run(`ratios(${rForCorrected(value)}).${property}`),expected);
// 五个点的目标、偏移及因子都必须对应真实配置，不靠舍入值判定。
const points=Array.from(run('FIVE'));
assert.deepStrictEqual(points.map(p=>p.index),[0,8,40,72,80]);
const keys=['leftup','rightup','center','leftdown','rightdown'];
for(let i=0;i<5;i++){
 const pt=points[i],key=keys[i],factor=i===2?config.Factor_Center_Class2:config.Factor_Corner_Class2;
 assert.strictEqual(pt.rgTarget,config['TargetRatioRG_'+key]);
 assert.strictEqual(pt.bgTarget,config['TargetRatioBG_'+key]);
 assert.strictEqual(pt.rgOffset,config['TargetRatioRG_'+key+'_offset']);
 assert.strictEqual(pt.bgOffset,config['TargetRatioBG_'+key+'_offset']);
 assert.strictEqual(pt.factor,factor);assert.strictEqual(config['TargetRatioGG_'+key],1);assert.strictEqual(config['TargetRatioGG_'+key+'_offset'],0);
 for(const kind of ['GG','RG','BG']){
  const c=run(`regionCompare(FIVE[${i}],'${kind}')`);
  const R=pt.r*25560+.1, G1=pt.gr*25560+.1, G2=pt.gb*25560+.1, B=pt.b*25560+.1;
  const raw=kind==='GG'?G1/G2:kind==='RG'?R/((G1+G2)/2):B/((G1+G2)/2);
  const target=config['TargetRatio'+kind+'_'+key],offset=config['TargetRatio'+kind+'_'+key+'_offset'];
  close(c.raw,raw);close(c.offset,offset);close(c.target,target);close(c.low,target*(1-factor));close(c.high,target*(1+factor));
  assert.strictEqual(c.pass,!(raw+offset>target*(1+factor)||raw+offset<target*(1-factor)));
  assert.strictEqual(c.pass,!(i===4&&kind==='BG'),`错误的五点判定: ${key} ${kind}`);
 }
}
close(run("regionCompare(FIVE[2],'RG').raw"),run('ratios(90.5).rg'));
close(run("regionCompare(FIVE[2],'BG').raw"),run('ratios(90.5).bg'));
close(run("regionCompare(FIVE[2],'GG').raw"),run('ratios(90.5).gg'));
// 使用实际按位置遍历的像素逐个相加，独立核验前缀和、行边界及最终和。
for(let i=0;i<5;i++)for(const ch of ['r','gr','gb','b']){
 const m=points[i][ch],low=Math.floor(m);
 let integerSum=0;
 for(let n=0;n<=25560;n++){
  if([0,1,2,3,4,212,213,214,12779,12780,25559,25560].includes(n))
   close(run(`pixelSum(FIVE[${i}],'${ch}',${n})`),integerSum+.1);
  if(n<25560){
   const value=run(`regionPixel(FIVE[${i}],'${ch}',${n})`);
   assert.strictEqual(value,low+(Number.isInteger(m)?0:n%2));
   assert.ok(Number.isInteger(value)&&value>=0&&value<=255);
   integerSum+=value;
  }
 }
 close(integerSum+.1,m*25560+.1);
 if(i===2)assert.deepStrictEqual(Array.from({length:20},(_,n)=>run(`regionPixel(FIVE[${i}],'${ch}',${n})`)),Array.from(run(`sample('${{r:'R',gr:'Gr',gb:'Gb',b:'B'}[ch]}').slice(0,20)`)));
}
assert.strictEqual(run('pixelCount(0)'),0);assert.strictEqual(run('pixelCount(.08)'),0);
assert.strictEqual(run('pixelCount(.09)'),1);assert.strictEqual(run('pixelCount(.115)'),2);
assert.strictEqual(run('pixelCount(.14)'),3);assert.strictEqual(run('pixelCount(.165)'),4);
assert.strictEqual(run('pixelCount(.36)'),25560);
// 逐幕核验文字确实随进度依次出现，而不是只列出结果。
for(let i=0;i<5;i++){
 const index=12+i;
 assert.strictEqual(sandbox.__ANIM.scenes[index].kicker,`${points[i].name}逐步计算`);
 assert.ok(sandbox.__ANIM.scenes[index].dur>=60);
 sandbox.__ANIM.gotoScene(index,true);
 function renderAt(p){drawn=[];sandbox.__ANIM.state.t=p*sandbox.__ANIM.scenes[index].dur;sandbox.__ANIM.render();return drawn.slice();}
 let text=renderAt(0);assert.ok(text.includes('等待第一个像素'));assert.ok(text.includes('已累加：0 / 25560 个 / 通道'));assert.ok(text.includes('0.1（初始）'));
 assert.ok(!text.some(x=>x.startsWith('加偏移 ')));
 text=renderAt(.09);assert.ok(text.includes('刚累加：第 1 行，第 1 列'));assert.ok(text.includes(`0.1 + ${Math.floor(points[i].r)} = ${(0.1+Math.floor(points[i].r)).toFixed(1)}`));
 text=renderAt(.115);assert.ok(text.includes('刚累加：第 1 行，第 2 列'));
 text=renderAt(.2);assert.ok(text.some(x=>x.startsWith('刚累加：第 ')));assert.ok(!text.includes('已累加：25560 / 25560 个 / 通道'));
 text=renderAt(.38);assert.ok(text.includes('刚累加：第 120 行，第 213 列'));assert.ok(text.includes('已累加：25560 / 25560 个 / 通道'));assert.ok(text.some(x=>x.startsWith('= (')));
 for(const [progress,offsets,ranges] of [[.43,0,0],[.47,1,0],[.52,1,1],[.56,1,1],[.60,2,1],[.65,2,2],[.69,2,2],[.73,3,2],[.78,3,3]]){
  text=renderAt(progress);
  assert.strictEqual(text.filter(x=>x.startsWith('加偏移 ')).length,offsets,`${points[i].name} ${progress} offset`);
  assert.strictEqual(text.filter(x=>x.startsWith('目标 ')).length,ranges,`${points[i].name} ${progress} range`);
 }
}
// 右下 BG 通过与失败区间的邻近边界；仅在本演示的比例非截断范围内。
const edgeRaw=.62*1.5+.007;
const boundaryB=(edgeRaw*((200*25560+.1+192*25560+.1)/2)-.1)/25560;
sandbox.boundaryB=boundaryB;
assert.strictEqual(run("regionCompare({...FIVE[4],b:boundaryB-0.001},'BG').pass"),true);
assert.strictEqual(run("regionCompare({...FIVE[4],b:boundaryB+0.001},'BG').pass"),false);
// V1.0 的绿色差先替换再比较，当前阈值 50；包括边界 10。
for(const value of [0,2,10,10.001,50,51,100])assert.strictEqual((value>10?-1:value)>50,false);
for(let i=0;i<27;i++){
 sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'),`第 ${i+1} 幕缺源码`);
 for(const p of [0,.3,.78,1]){sandbox.__ANIM.state.t=p*sandbox.__ANIM.scenes[i].dur;sandbox.__ANIM.render();}
}
buttons.fail();assert.strictEqual(out.textContent,'125');assert.strictEqual(run('ratios(state.r).centerRGPass'),false);
buttons.notgolden();assert.strictEqual(run('ratios(state.r).goldenRG'),false);buttons.sample();assert.strictEqual(out.textContent,'90.5');
input.value='100';handler();assert.strictEqual(out.textContent,'100');assert.strictEqual(typeof frame,'function');frame(100);
assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：27 幕、81 ROI、25560 像素、四通道比例、偏移、特殊绿色差、互动');
