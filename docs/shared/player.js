/* ==========================================================================
   AlgoAnim · 算法动画共用播放器
   --------------------------------------------------------------------------
   页面里只需要写「场景」，播放器负责：画布自适应、章节、控制条、字幕、快捷键。

   用法：
     <link rel="stylesheet" href="../shared/player.css">
     <script src="../shared/player.js"></script>
     <script>
       const state = AlgoAnim.state;          // 共用状态（scene/t/speed + 自定义字段）
       state.xxx = ...;
       function drawStep1(p){ ... }           // p = 0..1，用全局 ctx / txt / card 画
       const SCENES = [{ kicker, dur, draw:drawStep1, caption:p=>'...', html:'...' }];
       AlgoAnim.start({ scenes: SCENES });
     </script>

   绘图工具箱（全部是全局函数）：txt card arrow rr para wrapLines withA
   常量：C（颜色） CH（通道色） CHLIST VW VH UI MONO
   ========================================================================== */
'use strict';


/* =====================================================================
   基础设置
   ===================================================================== */
const VW = 1000, VH = 620;
const UI = '"PingFang SC","Microsoft YaHei","Noto Sans SC",system-ui,sans-serif';
const MONO = 'ui-monospace,SFMono-Regular,Menlo,Consolas,monospace';

const C = {
  ink:'#eef3ff', dim:'#93a3c4', faint:'#63718f', line:'rgba(255,255,255,0.10)',
  accent:'#6ea8fe', accent2:'#ffd166', pass:'#3ddc97', fail:'#ff5f7e',
  panel:'rgba(255,255,255,0.035)'
};
const CH = { R:'#ff7a7a', Gr:'#7fe08f', Gb:'#3ddcbf', B:'#6ea8fe' };
const CHLIST = ['R','Gr','Gb','B'];


/* =====================================================================
   小工具
   ===================================================================== */
const clamp01 = v => v<0?0:(v>1?1:v);
const lerp = (a,b,t) => a+(b-a)*t;
const seg = (p,a,b) => clamp01((p-a)/(b-a));
const ease = t => t<0.5 ? 2*t*t : 1-Math.pow(-2*t+2,2)/2;
const easeOut = t => 1-Math.pow(1-t,3);
const withA = (hex,a) => {
  const n = parseInt(hex.slice(1),16);
  return 'rgba('+((n>>16)&255)+','+((n>>8)&255)+','+(n&255)+','+a+')';
};

function rr(x,y,w,h,r){
  const q = Math.min(r, Math.abs(w)/2, Math.abs(h)/2);
  ctx.beginPath();
  ctx.moveTo(x+q,y);
  ctx.arcTo(x+w,y,x+w,y+h,q);
  ctx.arcTo(x+w,y+h,x,y+h,q);
  ctx.arcTo(x,y+h,x,y,q);
  ctx.arcTo(x,y,x+w,y,q);
  ctx.closePath();
}

function txt(s,x,y,o){
  o = o || {};
  ctx.save();
  ctx.font = (o.weight||400)+' '+(o.size||14)+'px '+(o.font||UI);
  ctx.fillStyle = o.color || C.ink;
  ctx.textAlign = o.align || 'left';
  ctx.textBaseline = o.base || 'alphabetic';
  if(o.alpha != null) ctx.globalAlpha = clamp01(o.alpha);
  ctx.fillText(String(s), x, y);
  ctx.restore();
}

function card(x,y,w,h,o){
  o = o || {};
  const r = o.r == null ? 14 : o.r;
  ctx.save();
  if(o.glow){ ctx.shadowColor = o.glow; ctx.shadowBlur = o.blur==null?20:o.blur; }
  rr(x,y,w,h,r);
  ctx.fillStyle = o.fill || C.panel;
  ctx.fill();
  ctx.restore();
  if(o.stroke !== false){
    rr(x,y,w,h,r);
    ctx.strokeStyle = o.stroke || C.line;
    ctx.lineWidth = o.lw || 1;
    ctx.stroke();
  }
}

function arrow(x1,y1,x2,y2,color,width,dash){
  ctx.save();
  ctx.strokeStyle = color || C.line;
  ctx.lineWidth = width || 1.5;
  if(dash) ctx.setLineDash(dash);
  ctx.beginPath(); ctx.moveTo(x1,y1); ctx.lineTo(x2,y2); ctx.stroke();
  ctx.setLineDash([]);
  const a = Math.atan2(y2-y1, x2-x1);
  ctx.beginPath();
  ctx.moveTo(x2,y2);
  ctx.lineTo(x2-9*Math.cos(a-0.42), y2-9*Math.sin(a-0.42));
  ctx.lineTo(x2-9*Math.cos(a+0.42), y2-9*Math.sin(a+0.42));
  ctx.closePath();
  ctx.fillStyle = color || C.line;
  ctx.fill();
  ctx.restore();
}

function wrapLines(s, maxW, font){
  ctx.save();
  ctx.font = font;
  const lines = [];
  let cur = '';
  for(const ch of s){
    const test = cur + ch;
    if(ctx.measureText(test).width > maxW && cur){ lines.push(cur); cur = ch; }
    else cur = test;
  }
  if(cur) lines.push(cur);
  ctx.restore();
  return lines;
}

function para(s, x, y, maxW, lh, o){
  o = o || {};
  const size = o.size || 15;
  const font = (o.weight||400)+' '+size+'px '+(o.font||UI);
  const lines = wrapLines(s, maxW, font);
  lines.forEach((ln,i) => txt(ln, x, y+i*lh, o));
  return lines.length;
}

/* 背景 */
function drawBg(){
  const g = ctx.createLinearGradient(0,0,0,VH);
  g.addColorStop(0,'#0a1120');
  g.addColorStop(0.55,'#080d18');
  g.addColorStop(1,'#06090f');
  ctx.fillStyle = g;
  ctx.fillRect(0,0,VW,VH);

  ctx.strokeStyle = 'rgba(110,168,254,0.05)';
  ctx.lineWidth = 1;
  ctx.beginPath();
  for(let x=0;x<=VW;x+=40){ ctx.moveTo(x+0.5,0); ctx.lineTo(x+0.5,VH); }
  for(let y=0;y<=VH;y+=40){ ctx.moveTo(0,y+0.5); ctx.lineTo(VW,y+0.5); }
  ctx.stroke();

  const glow = ctx.createRadialGradient(140,60,0,140,60,420);
  glow.addColorStop(0,'rgba(110,168,254,0.10)');
  glow.addColorStop(1,'rgba(110,168,254,0)');
  ctx.fillStyle = glow;
  ctx.fillRect(0,0,VW,VH);
}

function header(idx, kicker, title, sub){
  txt('STEP ' + String(idx+1).padStart(2,'0') + ' / ' + String(scenes.length).padStart(2,'0') + ' · ' + kicker, 40, 38,
      {size:11.5, color:C.accent, weight:700});
  txt(title, 40, 72, {size:23.5, color:'#f4f8ff', weight:700});
  if(sub) txt(sub, 40, 96, {size:12.5, color:C.dim});
  ctx.strokeStyle = C.line;
  ctx.lineWidth = 1;
  ctx.beginPath(); ctx.moveTo(40,116); ctx.lineTo(960,116); ctx.stroke();
}



/* ==========================================================================
   播放器
   ========================================================================== */
const PState = { scene:0, t:0, playing:true, speed:1 };
let scenes = [];
let elCaption = null, elBar = null;

function resize(){
  const r = canvas.getBoundingClientRect();
  dpr = Math.min(2, window.devicePixelRatio || 1);
  canvas.width = Math.max(1, Math.round(r.width * dpr));
  canvas.height = Math.max(1, Math.round(r.height * dpr));
}

function render(){
  if(!ctx || !scenes.length) return;
  ctx.setTransform(canvas.width / VW, 0, 0, canvas.height / VH, 0, 0);
  ctx.clearRect(0, 0, VW, VH);
  drawBg();
  const sc = scenes[PState.scene];
  const p = clamp01(PState.t / sc.dur);
  sc.draw(p);
  if(elCaption){
    const cap = sc.caption(p);
    if(elCaption.textContent !== cap) elCaption.textContent = cap;
  }
}

let lastTime = performance.now();

function tick(now){
  const dt = Math.min(0.05, (now - lastTime)/1000);
  lastTime = now;
  const sc = scenes[PState.scene];
  if(sc && PState.playing){
    PState.t += dt * PState.speed;
    if(PState.t >= sc.dur){
      if(PState.scene < scenes.length - 1){
        gotoScene(PState.scene + 1, true);
      } else {
        PState.t = sc.dur;
        PState.playing = false;
        syncPlayButton();
      }
    }
  }
  render();
  updateProgress();
  requestAnimationFrame(tick);
}

function updateProgress(){
  if(!elBar || !scenes.length) return;
  const sc = scenes[PState.scene];
  const overall = (PState.scene + clamp01(PState.t/sc.dur)) / scenes.length;
  elBar.style.width = (overall*100).toFixed(2) + '%';
}

function syncPlayButton(){
  const btn = document.getElementById('play');
  if(btn) btn.textContent = PState.playing ? '⏸ 暂停' : '▶ 播放';
}

function gotoScene(i, autoplay){
  if(!scenes.length) return;
  PState.scene = Math.max(0, Math.min(scenes.length - 1, i));
  PState.t = 0;
  PState.playing = autoplay !== false;
  syncPlayButton();
  const ex = document.getElementById('explain');
  if(ex) ex.innerHTML = scenes[PState.scene].html || '';
  document.querySelectorAll('.chapter').forEach((b,k)=>{
    b.classList.toggle('active', k === PState.scene);
  });
  const activeBtn = document.querySelector('.chapter.active');
  if(activeBtn && activeBtn.scrollIntoView) activeBtn.scrollIntoView({block:'nearest'});
  render();
}

function buildChapters(){
  const box = document.getElementById('chapters');
  if(!box) return;
  box.innerHTML = '';
  scenes.forEach((s,i)=>{
    const b = document.createElement('button');
    b.className = 'chapter';
    b.innerHTML = '<span class="num">' + String(i+1).padStart(2,'0') +
                  '</span><span class="name">' + s.kicker + '</span>';
    b.addEventListener('click', ()=> gotoScene(i, true));
    box.appendChild(b);
  });
}

function bindControls(){
  const play = document.getElementById('play');
  if(play) play.addEventListener('click', ()=>{
    if(!PState.playing && PState.t >= scenes[PState.scene].dur - 0.01) PState.t = 0;
    PState.playing = !PState.playing;
    syncPlayButton();
  });
  const prev = document.getElementById('prev');
  if(prev) prev.addEventListener('click', ()=> gotoScene(PState.scene-1, true));
  const next = document.getElementById('next');
  if(next) next.addEventListener('click', ()=> gotoScene(PState.scene+1, true));
  const replay = document.getElementById('replay');
  if(replay) replay.addEventListener('click', ()=> gotoScene(0, true));

  document.querySelectorAll('.speed button').forEach(b=>{
    b.addEventListener('click', ()=>{
      PState.speed = parseFloat(b.dataset.speed);
      document.querySelectorAll('.speed button').forEach(x=>x.classList.toggle('on', x===b));
    });
  });

  document.addEventListener('keydown', e=>{
    if(e.target && /INPUT|TEXTAREA|SELECT/.test(e.target.tagName)) return;
    if(e.code === 'Space'){ e.preventDefault(); if(play) play.click(); }
    if(e.code === 'ArrowRight'){ e.preventDefault(); gotoScene(PState.scene+1, true); }
    if(e.code === 'ArrowLeft'){ e.preventDefault(); gotoScene(PState.scene-1, true); }
  });
}

const AlgoAnim = {
  get state(){ return PState; },
  get ctx(){ return ctx; },
  get canvas(){ return canvas; },
  get scenes(){ return scenes; },
  gotoScene: gotoScene,
  render: render,
  start: function(opts){
    opts = opts || {};
    canvas = document.getElementById(opts.canvas || 'stage');
    if(!canvas) return;
    ctx = canvas.getContext('2d');
    scenes = opts.scenes || [];
    Object.assign(PState, opts.state || {});
    elCaption = document.getElementById('caption');
    elBar = document.getElementById('bar');
    buildChapters();
    bindControls();
    if(window.ResizeObserver) new ResizeObserver(resize).observe(canvas);
    else window.addEventListener('resize', resize);
    resize();
    gotoScene(opts.start || 0, true);
    requestAnimationFrame(tick);
    window.__ANIM = { state:PState, scenes:scenes, render:render, gotoScene:gotoScene };
  }
};
window.AlgoAnim = AlgoAnim;
