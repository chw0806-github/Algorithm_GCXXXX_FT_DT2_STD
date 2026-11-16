/* 测试真实 ROI 尺寸、模拟值、源码公式与 16 幕播放。 */
const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync(path.join(__dirname,'../color_ratio/index.html'),'utf8');
const scripts=Array.from(html.matchAll(/<script(?:\s[^>]*)?>([\s\S]*?)<\/script>/gi),m=>m[1]);assert.strictEqual(scripts.length,3);
const gradient={addColorStop(){}};
const ctx=new Proxy({}, {get(t,k){if(k in t)return t[k];if(k==='createLinearGradient'||k==='createRadialGradient')return ()=>gradient;return ()=>{};}});
const canvas={style:{},getContext:()=>ctx,getBoundingClientRect:()=>({width:1000,height:620})};
let handler,frame;const buttons={};
const input={value:'90.5',addEventListener:(event,fn)=>{handler=fn;}},out={textContent:''};
const elements={stage:canvas,caption:{textContent:''},explain:{innerHTML:''},'code-guide':{innerHTML:''},bar:{style:{}},rmean:input,rout:out};
for(const id of ['sample','notgolden','fail'])elements[id]={addEventListener:(event,fn)=>{buttons[id]=fn;}};
const document={getElementById:id=>elements[id]||null,querySelectorAll:()=>[],querySelector:()=>null,addEventListener(){}};
const sandbox={document,performance:{now:()=>0},requestAnimationFrame:fn=>{frame=fn;},setTimeout(){},addEventListener(){},devicePixelRatio:1};sandbox.window=sandbox;vm.createContext(sandbox);
scripts.forEach((s,i)=>new vm.Script(s,{filename:'color:script'+i}).runInContext(sandbox));
const run=s=>vm.runInContext(s,sandbox);
assert.strictEqual(sandbox.__ANIM.scenes.length,16);assert.strictEqual(sandbox.__CODE_GUIDE.length,16);
assert.strictEqual(run('N'),25560);
for(const [ch,min,avg] of [['R',81,90.5],['Gr',180,189.5],['Gb',182,191.5],['B',108,117.5]]){
 const a=Array.from(run(`sample('${ch}')`));assert.strictEqual(a.length,25560);assert.strictEqual(new Set(a).size,20);
 for(let v=min;v<min+20;v++)assert.strictEqual(a.filter(x=>x===v).length,1278);
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
// V1.0 的绿色差先替换再比较，当前阈值 50；包括边界 10。
for(const value of [0,2,10,10.001,50,51,100])assert.strictEqual((value>10?-1:value)>50,false);
for(let i=0;i<16;i++){
 sandbox.__ANIM.gotoScene(i,true);assert.ok(elements['code-guide'].innerHTML.includes('code-block'),`第 ${i+1} 幕缺源码`);
 for(const p of [0,.3,.78,1]){sandbox.__ANIM.state.t=p*sandbox.__ANIM.scenes[i].dur;sandbox.__ANIM.render();}
}
buttons.fail();assert.strictEqual(out.textContent,'125');assert.strictEqual(run('ratios(state.r).centerRGPass'),false);
buttons.notgolden();assert.strictEqual(run('ratios(state.r).goldenRG'),false);buttons.sample();assert.strictEqual(out.textContent,'90.5');
input.value='100';handler();assert.strictEqual(out.textContent,'100');assert.strictEqual(typeof frame,'function');frame(100);
assert.deepStrictEqual(Array.from(sandbox.__ERR),[]);
console.log('OK：16 幕、81 ROI、25560 像素、四通道比例、偏移、特殊绿色差、互动');
