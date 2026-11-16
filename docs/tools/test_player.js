/* 播放器严格模式回归测试：不依赖浏览器或 npm 包。
   用 Node vm 的普通全局对象避免某些 DOM 模拟器把未声明赋值误当成全局变量。
   运行：node docs/tools/test_player.js */
const assert = require('assert');
const fs = require('fs');
const path = require('path');
const vm = require('vm');

// 所有动画的源文件和构建产物都应提供同一组倍速按钮。
const pages = ['light_response', 'relative_illumination', 'non_uniformity',
  'deadrow_deadcol_bright', 'edge_deadrow_deadcol_bright', 'pixel_defect', 'color_ratio'];
for (const page of pages) for (const file of ['index.src.html', 'index.html']) {
  const html = fs.readFileSync(path.join(__dirname, '..', page, file), 'utf8');
  const buttons = Array.from(html.matchAll(/<button data-speed="([\d.]+)"(?: class="([^"]+)")?>([^<]+)<\/button>/g));
  assert.deepStrictEqual(buttons.map(b => [b[1], b[3]]), [['0.5','0.5×'],['1','1×'],['2','2×'],['3','3×']], `${page}/${file} 倍速按钮`);
  assert.deepStrictEqual(buttons.filter(b => b[2] === 'on').map(b => b[1]), ['1'], `${page}/${file} 默认倍速`);
}
const speedButtons = ['0.5', '1', '2', '3'].map(speed => {
  const classes = new Set(speed === '1' ? ['on'] : []);
  return { dataset: {speed}, classList: {
    toggle(name, on) { if (on) classes.add(name); else classes.delete(name); },
    contains: name => classes.has(name)
  }, addEventListener(event, fn) { if (event === 'click') this.click = fn; } };
});

const gradient = { addColorStop() {} };
const ctx = new Proxy({}, {
  get(target, key) {
    if (key in target) return target[key];
    if (key === 'createLinearGradient' || key === 'createRadialGradient') return () => gradient;
    return () => {};
  }
});
const canvas = {
  style: {}, width: 0, height: 0,
  getContext: () => ctx,
  getBoundingClientRect: () => ({ width: 1000, height: 620 })
};
const caption = { textContent: '' };
const bar = { style: {} };
const explain = { innerHTML: '' };
const guide = { innerHTML: '' };
const elements = { stage: canvas, caption, bar, explain, 'code-guide': guide };
const document = {
  getElementById: id => elements[id] || null,
  querySelectorAll: selector => selector === '.speed button' ? speedButtons : [],
  querySelector: () => null,
  addEventListener() {}
};
let nextFrame;
const sandbox = {
  document,
  performance: { now: () => 0 },
  requestAnimationFrame: fn => { nextFrame = fn; },
  addEventListener() {},
  devicePixelRatio: 1
};
sandbox.window = sandbox;
sandbox.__CODE_GUIDE = [
  [{path:'Params/first.cpp', first:1, last:1, note:'第一幕', code:'if (a < b && c > 0)'}],
  [{path:'Params/second.cpp', first:2, last:2, note:'第二幕', code:'second();'}]
];
vm.createContext(sandbox);
const source = fs.readFileSync(path.join(__dirname, '../shared/player.js'), 'utf8');
new vm.Script(source, { filename: 'player.js' }).runInContext(sandbox);
let draws = 0;
sandbox.AlgoAnim.start({ scenes: [
  { kicker: '第一幕', dur: 2, html: '讲解一', caption: () => '字幕一', draw: () => { draws++; } },
  { kicker: '第二幕', dur: 2, html: '讲解二', caption: () => '字幕二', draw: () => { draws++; } }
] });
assert.deepStrictEqual(Array.from(sandbox.__ERR), [], '启动时出现错误');
assert.strictEqual(sandbox.AlgoAnim.canvas, canvas);
assert.strictEqual(sandbox.AlgoAnim.ctx, ctx);
assert.strictEqual(caption.textContent, '字幕一');
assert.strictEqual(explain.innerHTML, '讲解一');
assert.ok(guide.innerHTML.includes('Params/first.cpp:1–1'));
assert.ok(guide.innerHTML.includes('a &lt; b &amp;&amp; c &gt; 0'), '代码中的 HTML 字符必须转义');
assert.ok(draws > 0, '第一幕未绘制');
assert.strictEqual(typeof nextFrame, 'function', '动画帧未安排');
nextFrame(100);
assert.strictEqual(sandbox.AlgoAnim.state.t, 0.05);
speedButtons[3].click();
assert.strictEqual(sandbox.AlgoAnim.state.speed, 3);
assert.ok(speedButtons[3].classList.contains('on'));
assert.ok(!speedButtons[1].classList.contains('on'));
nextFrame(150);
assert.ok(Math.abs(sandbox.AlgoAnim.state.t - 0.20) < 1e-10, '3× 在 50ms 内应推进 150ms');
speedButtons[1].click();
assert.strictEqual(sandbox.AlgoAnim.state.speed, 1);
assert.ok(speedButtons[1].classList.contains('on'));
assert.ok(!speedButtons[3].classList.contains('on'));
nextFrame(200);
assert.ok(Math.abs(sandbox.AlgoAnim.state.t - 0.25) < 1e-10, '切回 1× 后正常推进');
sandbox.AlgoAnim.gotoScene(1, true);
assert.strictEqual(caption.textContent, '字幕二');
assert.strictEqual(explain.innerHTML, '讲解二');
assert.ok(guide.innerHTML.includes('Params/second.cpp:2–2'), '章节切换后应显示对应代码');
assert.ok(!guide.innerHTML.includes('Params/first.cpp'), '切换章节应移除旧代码');
assert.deepStrictEqual(Array.from(sandbox.__ERR), [], '播放过程中出现错误');
console.log('OK：严格模式下启动、绘制、动画帧和切换章节均通过');
