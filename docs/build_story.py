#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""把 story/ 里的定格帧 + meta.json 打包成一页「静态分镜」HTML。

为什么需要它：
  动画页（index.html）是单文件、自带脚本的，但有些阅读器 / 预览环境压根不执行
  页面脚本，屏幕上只会剩下一行兜底提示。本脚本生成 storyboard.html —— 纯 HTML
  + CSS + base64 内嵌图片，零脚本、零外部请求，任何环境都能看。

用法：
  python3 docs/build_story.py           # 生成两页
  python3 docs/build_story.py --check   # 只校验产物是否与素材一致（CI 用）

素材来源（docs/<page>/story/）：
  sNN.png     —— 动画第 NN 幕的一帧定格（1000x620，96 色量化，约 60 KB）
  meta.json   —— [{i, file, kicker, caption, dur, html}, ...]，由导出脚本生成
"""

import base64
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))

PAGES = [
    {
        "dir": "deadrow_deadcol_bright",
        "title": "Deadrow_Deadcol_bright",
        "sub": "白帧坏行坏列：按通道比较邻近行列与线异常",
        "crumb": "图像传感器 FT 测试 · 白帧测试子项",
        "lead": "动画的 7 幕各定格成一帧，图下配字幕和同一份讲解文字。",
    },
    {
        "dir": "non_uniformity",
        "title": "Non_Uniformity",
        "sub": "分通道、分格，检查局部波动及对称差",
        "crumb": "图像传感器 FT 测试 · 白帧测试子项",
        "lead": "动画的 7 幕各定格成一帧，图下配字幕和同一份讲解文字。",
    },
    {
        "dir": "light_response",
        "title": "Light_Response",
        "sub": "白帧测试（Bright Test）里的第一个子项",
        "crumb": "图像传感器 FT 测试 · 白帧测试子项",
        "lead": "这一页把动画的 8 幕各定格成一帧，图下边配该幕字幕和同一份讲解文字。"
                "内容和 index.html 里的动画完全一致，只是不会动。",
    },
    {
        "dir": "relative_illumination",
        "title": "Relative_Illumination",
        "sub": "用 5 个窗口比较画面明暗差异（暗角 / 阴影）",
        "crumb": "图像传感器 FT 测试 · 白帧测试子项",
        "lead": "这一页把动画的 7 幕各定格成一帧，图下边配该幕字幕和同一份讲解文字。"
                "内容和 index.html 里的动画完全一致，只是不会动。",
    },
]

CSS = """
:root{
  --bg:#0b0e14; --panel:#12161f; --line:#1e2430; --line2:#2a3444;
  --fg:#d8dee9; --dim:#8b96a8; --dim2:#6b7688;
  --acc:#7aa2ff; --vio:#a78bfa; --ok:#4ade80; --warn:#fbbf24;
  --mono:ui-monospace,SFMono-Regular,Menlo,Consolas,"Liberation Mono",monospace;
  --sans:-apple-system,BlinkMacSystemFont,"Segoe UI","PingFang SC","Hiragino Sans GB",
         "Microsoft YaHei","Source Han Sans SC","Noto Sans SC",sans-serif;
}
*{box-sizing:border-box}
html{scroll-behavior:smooth}
body{
  margin:0; background:var(--bg); color:var(--fg);
  font-family:var(--sans); font-size:15px; line-height:1.75;
  -webkit-text-size-adjust:100%;
}
a{color:var(--acc); text-decoration:none}
a:hover{text-decoration:underline}
.wrap{max-width:1060px; margin:0 auto; padding:0 20px}

/* ---------- 头部 ---------- */
header.hero{
  border-bottom:1px solid var(--line);
  background:radial-gradient(1200px 400px at 20% -10%, rgba(122,162,255,.10), transparent 70%), var(--bg);
  padding:34px 0 22px;
}
.crumb{font-family:var(--mono); font-size:12px; letter-spacing:.12em; color:var(--dim2); text-transform:uppercase}
h1{margin:8px 0 4px; font-size:30px; line-height:1.3; letter-spacing:.5px}
h1 .fn{font-family:var(--mono); color:var(--vio)}
h1 .zh{font-size:20px; color:var(--dim); font-weight:500; margin-left:10px}
.sub{margin:0 0 14px; color:var(--acc); font-size:14px}
.lead{margin:0 0 16px; color:var(--dim); max-width:78ch}
.banner{
  border:1px solid var(--line2); border-left:3px solid var(--warn);
  background:rgba(251,191,36,.06); border-radius:8px;
  padding:10px 14px; margin:0 0 18px; font-size:13.5px; color:#e6d9b8;
}
.banner b{color:var(--warn)}
nav.chips{display:flex; flex-wrap:wrap; gap:8px}
nav.chips a{
  font-family:var(--mono); font-size:12px; letter-spacing:.03em;
  padding:5px 10px; border:1px solid var(--line2); border-radius:999px;
  color:var(--dim); background:rgba(255,255,255,.02);
}
nav.chips a:hover{color:var(--fg); border-color:var(--acc); text-decoration:none}
nav.chips a .n{color:var(--acc); margin-right:6px}

/* ---------- 每一幕 ---------- */
main{padding:26px 0 10px}
section.card{
  border:1px solid var(--line); border-radius:14px; background:var(--panel);
  margin:0 0 28px; overflow:hidden; scroll-margin-top:16px;
}
.card .head{
  display:flex; align-items:baseline; gap:12px; flex-wrap:wrap;
  padding:16px 20px 12px; border-bottom:1px solid var(--line);
}
.card .step{
  font-family:var(--mono); font-size:11.5px; letter-spacing:.14em; color:var(--dim2);
}
.card h2{margin:0; font-size:19px; font-weight:600}
.card h2 .k{color:var(--acc)}
figure{margin:0; padding:18px 20px 0}
figure img{
  display:block; width:100%; height:auto; border-radius:10px;
  border:1px solid var(--line); background:#0a0d13;
}
.cap{
  display:flex; gap:9px; align-items:flex-start;
  margin:12px 20px 0; padding:10px 14px;
  border:1px solid var(--line2); border-radius:9px; background:rgba(122,162,255,.05);
  font-size:14px; color:#c9d4e4;
}
.cap .dot{
  flex:0 0 auto; width:7px; height:7px; margin-top:8px; border-radius:50%;
  background:var(--acc); box-shadow:0 0 8px var(--acc);
}

/* ---------- 讲解文字（来自 scene.html） ---------- */
.explain{padding:6px 20px 20px}
.explain h3{
  margin:18px 0 8px; font-size:15px; font-weight:600; color:var(--acc);
  letter-spacing:.02em;
}
.explain h3:first-child{margin-top:14px}
.explain p{margin:9px 0; color:#c3ccdb}
.explain b{color:#eaf0fb}
.explain ol,.explain ul{margin:9px 0; padding-left:1.5em; color:#c3ccdb}
.explain li{margin:5px 0}
.explain code{
  font-family:var(--mono); font-size:12.5px;
  background:rgba(255,255,255,.06); border:1px solid var(--line2);
  border-radius:5px; padding:1px 5px; color:#dbe4f3;
}
.explain pre{
  margin:12px 0; padding:13px 15px; overflow-x:auto;
  background:#0a0d13; border:1px solid var(--line); border-radius:9px;
  line-height:1.65;
}
.explain pre code{
  background:none; border:0; padding:0; font-size:12.5px; color:#cbd6e6;
}
.explain p.tip{
  margin:12px 0 0; padding:10px 14px; border-radius:8px;
  border:1px solid rgba(251,191,36,.28); border-left:3px solid var(--warn);
  background:rgba(251,191,36,.07); color:#e8ddc0; font-size:13.5px;
}
.explain table{border-collapse:collapse; width:100%; margin:12px 0; font-size:13.5px}
.explain th,.explain td{
  border:1px solid var(--line2); padding:6px 10px; text-align:left;
}
.explain th{background:rgba(255,255,255,.04); color:#dbe4f3}
.explain .kbd{font-family:var(--mono); font-size:12px; color:var(--dim)}

/* ---------- 尾部 ---------- */
footer{
  border-top:1px solid var(--line); margin-top:14px; padding:20px 0 40px;
  color:var(--dim2); font-size:13px;
}
footer p{margin:6px 0}
footer code{font-family:var(--mono); font-size:12.5px; color:var(--dim)}

@media (max-width:640px){
  h1{font-size:23px} h1 .zh{display:block; margin:2px 0 0; font-size:16px}
  .wrap{padding:0 14px}
  .card .head,figure,.explain{padding-left:14px; padding-right:14px}
  .cap{margin-left:14px; margin-right:14px}
}
"""

PAGE = """<!doctype html>
<html lang="zh-CN">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>{title} 静态分镜（无脚本版） · 图像传感器 FT 测试</title>
<style>{css}</style>
</head>
<body>
<header class="hero">
  <div class="wrap">
    <div class="crumb">{crumb}</div>
    <h1><span class="fn">{title}</span><span class="zh">静态分镜</span></h1>
    <p class="sub">{sub}</p>
    <p class="lead">{lead}</p>
    <p class="banner">
      <b>为什么有这一页？</b>配套的 <code>index.html</code> 是带脚本的动画。
      如果你打开它只看到「脚本没有运行起来」之类的提示，说明当前阅读器（预览框 / 沙箱 / 邮件客户端）
      屏蔽了页面脚本。本页是同一份内容的<strong>定格版</strong>：纯 HTML + CSS，图片直接内嵌，
      <strong>不执行任何脚本、不请求任何外部文件</strong>。
    </p>
    <nav class="chips">{chips}</nav>
  </div>
</header>
<main class="wrap">
{cards}
</main>
<footer class="wrap">
  <p>每张图是动画在该幕 <code>78%</code> 进度处的定格；字幕与讲解文字直接取自动画的同一份数据
  （<code>scene.caption(p)</code> / <code>scene.html</code>），因此不会和动画说法不一致。</p>
  <p>想看会动的版本：<code>{dir}/index.html</code>　·　重新生成本页：<code>python3 docs/build_story.py</code></p>
</footer>
</body>
</html>
"""

CARD = """  <section class="card" id="s{n}">
    <div class="head">
      <span class="step">STEP {n:02d} / {total:02d}</span>
      <h2><span class="k">{kicker}</span></h2>
    </div>
    <figure>
      <img src="data:image/png;base64,{b64}" alt="{title} 第 {n} 幕：{kicker}">
    </figure>
    <p class="cap"><span class="dot"></span><span>{caption}</span></p>
    <div class="explain">
{html}
    </div>
  </section>
"""


def indent(text, pad="      "):
    out = []
    for line in text.splitlines():
        out.append((pad + line) if line.strip() else "")
    return "\n".join(out)


def build(page):
    root = os.path.join(HERE, page["dir"], "story")
    meta_path = os.path.join(root, "meta.json")
    if not os.path.exists(meta_path):
        raise SystemExit("missing " + meta_path)
    with open(meta_path, encoding="utf-8") as fh:
        meta = json.load(fh)

    total = len(meta)
    cards, chips = [], []
    for item in meta:
        n = item["i"]
        png = os.path.join(root, item["file"])
        with open(png, "rb") as fh:
            b64 = base64.b64encode(fh.read()).decode("ascii")
        chips.append(
            '<a href="#s{n}"><span class="n">{n:02d}</span>{k}</a>'.format(
                n=n, k=item["kicker"]
            )
        )
        cards.append(
            CARD.format(
                n=n,
                total=total,
                kicker=item["kicker"],
                b64=b64,
                title=page["title"],
                caption=item["caption"],
                html=indent(item.get("html", "")),
            )
        )

    return PAGE.format(
        title=page["title"],
        sub=page["sub"],
        crumb=page["crumb"],
        lead=page["lead"],
        css=CSS.strip(),
        chips="\n      ".join(chips),
        cards="\n".join(cards),
        dir=page["dir"],
    )


def main(argv):
    check = "--check" in argv
    ok = True
    for page in PAGES:
        out = os.path.join(HERE, page["dir"], "storyboard.html")
        html = build(page)
        if check:
            if not os.path.exists(out):
                print("MISSING %s" % out)
                ok = False
                continue
            with open(out, encoding="utf-8") as fh:
                cur = fh.read()
            if cur != html:
                print("STALE   %s" % os.path.relpath(out, HERE))
                ok = False
            else:
                print("ok      %s (%.0f KB)" % (os.path.relpath(out, HERE), len(html) / 1024.0))
        else:
            with open(out, "w", encoding="utf-8") as fh:
                fh.write(html)
            print("wrote   %s (%.0f KB)" % (os.path.relpath(out, HERE), len(html) / 1024.0))
    if check and not ok:
        print("\n静态分镜与素材不一致，请运行： python3 docs/build_story.py")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
