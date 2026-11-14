/* 播放器严格模式回归测试：不依赖浏览器或 npm 包。
   用 Node vm 的普通全局对象避免某些 DOM 模拟器把未声明赋值误当成全局变量。
   运行：node docs/tools/test_player.js */
const assert = require('assert');
const fs = require('fs');
const path = require('path');
const vm = require('vm');

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
const elements = { stage: canvas, caption, bar, explain };
const document = {
  getElementById: id => elements[id] || null,
  querySelectorAll: () => [],
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
assert.ok(draws > 0, '第一幕未绘制');
assert.strictEqual(typeof nextFrame, 'function', '动画帧未安排');
nextFrame(100);
assert.strictEqual(sandbox.AlgoAnim.state.t, 0.05);
sandbox.AlgoAnim.gotoScene(1, true);
assert.strictEqual(caption.textContent, '字幕二');
assert.strictEqual(explain.innerHTML, '讲解二');
assert.deepStrictEqual(Array.from(sandbox.__ERR), [], '播放过程中出现错误');
console.log('OK：严格模式下启动、绘制、动画帧和切换章节均通过');
