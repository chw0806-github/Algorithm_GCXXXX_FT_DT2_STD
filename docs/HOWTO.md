# 怎么给别的算法也做一个动画

> 结论：**能**，而且这套东西已经把「播放器」和「算法内容」拆开了，新加一个子项大约 300~500 行，基本只写画图。

```
docs/
├── index.html                 目录页
├── shared/
│   ├── player.js              播放器：画布自适应 / 章节 / 控制条 / 字幕 / 快捷键 / 绘图工具
│   └── player.css             统一视觉（深色舞台、侧栏、滑杆、按钮）
├── light_response/            一个算法 = 一个目录
│   ├── index.html             只写 SCENES（draw + caption + html）
│   └── README.md              文字版讲解
├── relative_illumination/     第二个算法，证明这套骨架可复用
└── HOWTO.md                   本文件
```

---

## 1. 先给算法分个类，选对可视化模板

不同子项的"数据形状"不同，画法也不同。按这 6 类套模板，基本不用重新设计：

| 类型 | 典型子项 | 画面要素 | 交互滑杆 |
|---|---|---|---|
| **窗口均值判定** | `Light_Response`、`Relative_Illumination`、`DarkCurrent` | 图像/平面 + ROI 框 + 均值 + Spec 刻度尺 | 均值或边缘亮度 |
| **网格统计** | `Non_Uniformity`、`Color_Shading`、`Color_GI`、`ROI_PRNU` | M×N 网格热力图 + max/min/ratio | 网格不均匀度 |
| **行列 / 几何** | `Deadrow_Deadcol_bright`、`Optical_Center` | 图像 + 高亮行列 / 质心标记 | 缺陷行数、偏心量 |
| **像素级异常** | `Pixel_Defect`、`DAG`、`ImageDirt` | 散点图 + 直方图 + 阈值线 | 坏点数、阈值 |
| **曲线 / 拟合** | `Light_Linearity`、DAG 分段 | 散点 + 拟合直线 + R² | 增益点、最大 DN |
| **多帧 / 时序** | 多帧平均、Dark 与 Bright 相减 | 帧堆叠动画 + 差值图 | 帧数、曝光 |

先判断新子项属于哪类，直接照抄同类的页面改，比从零画快得多。

---

## 2. 六步接入

### ① 读代码，找到真正的计算点

每个子项都长这样（`Params/algorithmitems.cpp`）：

```cpp
Subitem_Result AlgorithmItems::Xxx(const ParamManage& manage)
{
    if(manage.xxx_version == "Xxx_V1.0")   // 版本分发
        return Xxx_V1_0(manage);

    Subitem_Result res("Xxx", false);
    res.g_msg = false;                     // 未开启 / 版本不匹配
    return res;
}
```

进去以后找三样东西：**读数在哪算**（往往在 `Params/cpalgorithmraw.cpp` 的 `AlgorithmFuncs::` 里）、**和谁比**（`limit_*`）、**落哪个 bin**（`bin_` / `grade_`）。

### ② 抄真实配置，别自己编数

```
STD_Tester_.../GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps/Xxx.data
```

把里面的值原样写进页面顶部的 `const SPEC = {...}`，并在页脚注明来源文件。**数字对不上配置，动画就会教错人。**

### ③ 定故事线：5~8 幕，每幕只讲一件事

固定套路，直接套：

1. **定位**：它在整条 Bright Test 流程的哪一步，和相邻子项什么关系
2. **取数据**：ROI / 窗口 / 网格怎么切，坐标是 RAW 还是拆分平面
3. **计算**：Σ/N、比值、差值……把公式和中间量演出来
4. **判定**：Spec 刻度、分支顺序、bin / grade
5. **归档**：进 `items_results`，和别的子项一起竞争最终 bin

复杂算法再加 **防呆**、**多帧**、**拟合** 之类的专幕。

### ④ 写 `draw(p)`

`p` 是 0~1 的播放进度，所有坐标固定在 **1000×620** 的逻辑画布上（播放器自动缩放）。

```js
function drawStep1(p){
  header(0, '定位', '标题写在这一幕');
  const ap = ease(seg(p, 0.05, 0.30));       // 分段出现的常用写法
  card(60, 150, 400, 200, {r:14, glow:'rgba(110,168,254,.4)'});
  txt('Hello', 80, 190, {size:16, weight:700});
  arrow(480, 250, 560, 250, C.accent);
}
```

可用工具（都在 `player.js` 里，全局可直接用）：

| 工具 | 用途 |
|---|---|
| `txt(s,x,y,{size,color,weight,align,font,alpha})` | 文字 |
| `card(x,y,w,h,{fill,stroke,r,lw,glow})` | 圆角卡片 / 面板 |
| `arrow(x1,y1,x2,y2,color,w,dash)` | 箭头 |
| `rr(x,y,w,h,r)` | 圆角路径（配合 `ctx.fill()`） |
| `para(s,x,y,maxW,lh,opts)` | 自动换行段落 |
| `withA('#ff7a7a', .3)` | 十六进制色 → rgba |
| `C` / `CH` / `CHLIST` | 主题色 / 四通道色 |
| `clamp01 lerp seg ease easeOut` | 动画常用数学 |

> 经验：先画"静帧"（把 p 固定成 0.95 看最终画面），再补中间过程。避免文字互相压住。

### ⑤ 写 `caption(p)` 和右侧讲解 `html`

- `caption(p)`：画布下方那一行字幕，随进度换句子，一句话说清"现在在干什么"。
- `html`：右侧面板，用 `<p>/<ul>/<pre><code>`；**每幕至少留一条 `.tip`** 写坑（越界、坐标、`g_msg`、分支顺序这类）。

### ⑥ 加滑杆和预设

```js
state.center = 205;            // 自定义字段挂在共用 state 上
function calc(){ ... }          // 用 state 的值算，draw 里直接读
```

页面底部 `#sliders` / `#presets` 自己建 DOM 即可（参考两个现有页面的写法），播放器不干预。

预设一定要覆盖：**正常 / 临界 / 各类 Fail**，让人一拖就能看到分支变化。

---

## 3. 最小模板（复制即用）

```html
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Xxx 动画讲解</title>
<link rel="stylesheet" href="../shared/player.css">
</head>
<body>
<div class="app">
  <header class="top">
    <div class="brand">
      <div class="logo">◉</div>
      <div>
        <h1>图像传感器 FT 测试 · Xxx 动画讲解</h1>
        <p class="sub">逐行拆解 <code>items_results.push_back(Algorithm_all[site_].Xxx(test_manage));</code></p>
      </div>
    </div>
    <div class="badges"><span class="badge">C++ / Qt</span><span class="badge">GC8613T</span></div>
  </header>

  <main>
    <section class="stage-col">
      <div class="stage-card">
        <canvas id="stage"></canvas>
        <div class="caption"><span class="dot"></span><span id="caption">…</span></div>
      </div>
      <div class="controls">
        <button id="prev">◀ 上一节</button>
        <button id="play" class="primary">⏸ 暂停</button>
        <button id="next">下一节 ▶</button>
        <button id="replay">↺ 重播</button>
        <div class="progress"><div id="bar"></div></div>
        <div class="speed">
          <button data-speed="0.5">0.5×</button>
          <button data-speed="1" class="on">1×</button>
          <button data-speed="2">2×</button>
        </div>
      </div>
      <div class="tuner">
        <div class="tuner-head"><h2>模拟测量值</h2><span>拖动看判定变化</span></div>
        <div class="sliders" id="sliders"></div>
        <div class="presets" id="presets"></div>
      </div>
    </section>
    <aside class="side">
      <nav class="chapters" id="chapters"></nav>
      <div class="explain" id="explain"></div>
    </aside>
  </main>
</div>

<script src="../shared/player.js"></script>
<script>
const state = AlgoAnim.state;
state.xxx = 1;

const SPEC = { /* 抄 .data */ };

function drawStep1(p){
  header(0, '定位', '这一幕的标题');
  txt('…', 40, 200, {size:14, color:C.dim});
}

const SCENES = [
  { kicker:'定位', dur:12, draw:drawStep1,
    caption:p => '字幕随进度变化',
    html: `<h3>标题</h3><p>讲解…</p><p class="tip">坑…</p>` }
];

AlgoAnim.start({ scenes: SCENES });
</script>
</body>
</html>
```

---

## 4. 约定与检查清单

**约定**

- 一幕 **10~16 秒**，太长会困；每幕只讲一件事。
- 每幕一张"主角图"，别塞三张。
- 数字必须与 `.data` 一致，页脚注明来源。
- 文案里出现 C++ 标识符时用等宽字体（`font:MONO`），中文句子里别混等宽（部分环境缺字）。
- 不引用任何外部资源：可离线双击打开。

**自测**

- [ ] `node --check` 抽出内联脚本，语法通过
- [ ] 每幕取 `p = 0.15 / 0.55 / 0.95` 截图，检查文字不重叠、不出界
- [ ] 拖滑杆：第 5~7 幕的数值、PASS/FAIL、bin 都跟着变
- [ ] 窗口宽度 < 1120px 时，右侧栏自动落到下方
- [ ] 章节按钮顺序、字幕、右侧讲解三者对得上
- [ ] 页脚写明参数来源文件

**加完一个子项后**，在 `docs/index.html` 的卡片区加一张入口卡即可。

---

## 5. 想做成批量培训，还可以再往前走一步

- 把同类子项的 `draw` 抽成参数化模板（比如"窗口均值类"一个函数，传窗口数量和 Spec 就出图）；
- 用 `.data` 文件直接生成 `SPEC`（读文件 → 注入页面），换机种不用改代码；
- 再往上一层：给整条 Bright Test 做一个"总览页"，点某个子项跳进对应动画。

现有两个页面就是按这个方向写的，照着加就行。
