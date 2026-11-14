/* ==========================================================================
   导出「静态分镜」素材：把动画的每一幕定格成一帧 PNG，并导出该幕的字幕 / 讲解文字。

   产物（供 docs/build_story.py 组装成 storyboard.html）：
     docs/<page>/story/sNN.png     每幕一帧定格（1000x620）
     docs/<page>/story/meta.json   [{i, file, kicker, caption, dur, html}, ...]

   这一步不属于日常构建，只有改了动画的场景/画面、需要重新出静态分镜时才跑；
   它依赖两个 npm 包（不是页面依赖，只是离线截图工具）：
       npm i jsdom @napi-rs/canvas
   并在 NODE_PATH 里能 require 到它们；中文字体用 @expo-google-fonts/noto-sans-sc，
   路径可用 FONT_DIR 覆盖（默认 <cwd>/node_modules/@expo-google-fonts/noto-sans-sc）。

   用法：
     node docs/tools/export_story.js docs/light_response/index.html docs/light_response/story 0.78
   之后（可选）把 PNG 量化到 96 色，单帧从 ~220 KB 降到 ~60 KB：
     python3 - <<'EOF'
     from PIL import Image; import glob
     for f in glob.glob('docs/*/story/*.png'):
         im = Image.open(f).convert('RGB')
         im.quantize(colors=96, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE) \
           .save(f, optimize=True, compress_level=9)
     EOF
   最后：python3 docs/build_story.py
   ========================================================================== */
const fs = require('fs');
const path = require('path');
const { JSDOM } = require('jsdom');
const { createCanvas, GlobalFonts } = require('@napi-rs/canvas');

const F = process.env.FONT_DIR || path.join(process.cwd(), 'node_modules/@expo-google-fonts/noto-sans-sc');
GlobalFonts.registerFromPath(F + '/400Regular/NotoSansSC_400Regular.ttf', 'Noto Sans SC');
GlobalFonts.registerFromPath(F + '/500Medium/NotoSansSC_500Medium.ttf', 'Noto Sans SC');
GlobalFonts.registerFromPath(F + '/700Bold/NotoSansSC_700Bold.ttf', 'Noto Sans SC');

const pagePath = process.argv[2];
const outDir = process.argv[3];
const p = Number(process.argv[4] || 0.78);
const html = fs.readFileSync(pagePath, 'utf8');
fs.mkdirSync(outDir, { recursive: true });

let backing = null;
const dom = new JSDOM(html, {
  url: 'file://' + pagePath,
  runScripts: 'dangerously',
  pretendToBeVisual: true,
  beforeParse(window) {
    const proto = window.HTMLCanvasElement.prototype;
    const store = new WeakMap();
    const mk = (w, h) => {
      const c = createCanvas(Math.max(1, Math.round(w)), Math.max(1, Math.round(h)));
      if (!backing) backing = c;
      return c;
    };
    proto.getContext = function (t) {
      let c = store.get(this);
      if (!c) { c = mk(this.__w || 1000, this.__h || 620); store.set(this, c); }
      return c.getContext(t);
    };
    Object.defineProperty(proto, 'width', {
      get() { return this.__w || 1000; },
      set(v) { this.__w = v; let c = store.get(this); if (!c) { c = mk(v, this.__h || 620); store.set(this, c); } else { c.width = v; } }
    });
    Object.defineProperty(proto, 'height', {
      get() { return this.__h || 620; },
      set(v) { this.__h = v; let c = store.get(this); if (!c) { c = mk(this.__w || 1000, v); store.set(this, c); } else { c.height = v; } }
    });
    proto.getBoundingClientRect = function () {
      return { width: 1000, height: 620, left: 0, top: 0, right: 1000, bottom: 620, x: 0, y: 0 };
    };
  }
});

setTimeout(() => {
  const w = dom.window;
  if (!w.__ANIM) { console.error('no __ANIM'); process.exit(1); }
  const scenes = w.__ANIM.scenes;
  const meta = [];
  scenes.forEach((sc, i) => {
    w.__ANIM.state.scene = i;
    w.__ANIM.state.t = p * sc.dur;
    w.__ANIM.state.playing = false;
    w.__ANIM.render();
    const file = 's' + String(i + 1).padStart(2, '0') + '.png';
    fs.writeFileSync(path.join(outDir, file), backing.toBuffer('image/png'));
    meta.push({
      i: i + 1, file,
      kicker: sc.kicker,
      caption: sc.caption(p),
      dur: sc.dur,
      html: (sc.html || '').trim()
    });
  });
  fs.writeFileSync(path.join(outDir, 'meta.json'), JSON.stringify(meta, null, 2));
  console.log(JSON.stringify(meta.map(m => m.kicker)));
  process.exit(0);
}, 400);
