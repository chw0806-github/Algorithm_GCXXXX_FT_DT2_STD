#!/usr/bin/env python3
"""
把 shared/player.css 和 shared/player.js 内联进每个页面，生成「单文件、零依赖」的成品页。

为什么这么做：
  - 源文件（*.src.html）引用 ../shared/*，方便复用、只维护一份播放器；
  - 成品（index.html）把 CSS / JS 全部内联，双击就能打开，
    不依赖相对路径，也不怕被单独拷走或嵌入文件预览器。

用法：
  python3 docs/build.py          # 生成全部成品页
  python3 docs/build.py --check  # 只检查成品是否和源文件同步（CI 用）

规则：
  任意 <link rel="stylesheet" href="...shared/player.css">  →  <style>…</style>
  任意 <script src="...shared/player.js"></script>          →  <script>…</script>
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
CSS = (ROOT / 'shared' / 'player.css').read_text(encoding='utf-8').strip()
JS = (ROOT / 'shared' / 'player.js').read_text(encoding='utf-8').strip()
# 内联进 <script> 前必须转义结束标签，否则 HTML 解析器会提前截断脚本
JS_SAFE = JS.replace('</script>', '<\\/script>')

# (源文件, 成品文件)
PAGES = [
    (ROOT / 'light_response' / 'index.src.html',          ROOT / 'light_response' / 'index.html'),
    (ROOT / 'relative_illumination' / 'index.src.html',   ROOT / 'relative_illumination' / 'index.html'),
    (ROOT / 'non_uniformity' / 'index.src.html',          ROOT / 'non_uniformity' / 'index.html'),
    (ROOT / 'deadrow_deadcol_bright' / 'index.src.html', ROOT / 'deadrow_deadcol_bright' / 'index.html'),
    (ROOT / 'catalog.src.html',                            ROOT / 'index.html'),
]

FALLBACK = """
<script>
/* 兜底：脚本没跑起来时，别把「加载中…」和一个空画布丢给读者。
   做法：写一句人话 + 指向同一内容的静态分镜页（storyboard.html），并把已经点不动的控件关掉。 */
setTimeout(function () {
  if (window.__ANIM) return;
  var why = (window.__ERR && window.__ERR[0]) ? ('：' + window.__ERR[0]) : '';

  var c = document.getElementById('caption');
  if (c) c.textContent = '动画没有启动' + why + '　请看右侧讲解或 storyboard.html 静态分镜';

  var ex = document.getElementById('explain');
  if (ex) {
    var d = document.createElement('div');
    d.className = 'cue';
    var b = document.createElement('b');
    b.textContent = '动画没有启动' + why + '　';
    d.appendChild(b);
    d.appendChild(document.createTextNode('可能是脚本运行出错或当前环境不支持动画。可改看不依赖脚本的版本：'));
    var a = document.createElement('a');
    a.href = 'storyboard.html';
    a.textContent = 'storyboard.html 静态分镜';
    d.appendChild(a);
    d.appendChild(document.createTextNode('，每幕一张定格图 + 同样的字幕和讲解。'));
    ex.insertBefore(d, ex.firstChild);
  }

  var ctr = document.querySelector('.controls');
  if (ctr) {
    var btns = ctr.querySelectorAll('button');
    for (var i = 0; i < btns.length; i++) btns[i].disabled = true;
    ctr.style.opacity = '.5';
  }
  var tuner = document.querySelector('.tuner');
  if (tuner) tuner.style.display = 'none';
}, 1200);
</script>
"""


def build_one(src: Path, dst: Path) -> str:
    html = src.read_text(encoding='utf-8')

    html, n_css = re.subn(
        r'<link[^>]+href="[^"]*shared/player\.css"[^>]*>',
        lambda m: '<style>\n' + CSS + '\n</style>',
        html, count=1)

    html, n_js = re.subn(
        r'<script[^>]+src="[^"]*shared/player\.js"[^>]*>\s*</script>',
        lambda m: '<script>\n' + JS_SAFE + '\n</script>',
        html, count=1)

    # 目录页只用到 CSS，没有播放器脚本
    if n_css != 1 or n_js > 1:
        raise SystemExit(f'{src.name}: 期望 css=1、js<=1，实际 css={n_css} js={n_js}')

    if '</body>' in html:
        html = html.replace('</body>', FALLBACK + '</body>', 1)
    return html


def main() -> int:
    check = '--check' in sys.argv
    stale = []
    for src, dst in PAGES:
        out = build_one(src, dst)
        old = dst.read_text(encoding='utf-8') if dst.exists() else ''
        if old != out:
            stale.append(dst.relative_to(ROOT))
            if not check:
                dst.write_text(out, encoding='utf-8')
        print(('  ok  ' if old == out else ('stale ' if check else 'built ')) +
              str(dst.relative_to(ROOT)) + f'  ({len(out)//1024} KB)')
    if check and stale:
        print('\n以下成品页已过期，请运行 python3 docs/build.py：')
        for p in stale:
            print('  -', p)
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
