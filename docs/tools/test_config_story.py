#!/usr/bin/env python3
"""比较两种页面的配置原文和静态分镜逐幕源码；不依赖浏览器。"""
from html import unescape
import re
import sys
from pathlib import Path

DOCS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(DOCS))
from config_guides import CONFIGS, get_config
from code_guides import EXPECTED_KICKERS, get_guides

for page in CONFIGS:
    files, fields = get_config(page)
    for filename in ('index.html', 'storyboard.html'):
        html = (DOCS / page / filename).read_text(encoding='utf-8')
        assert html.index('class="config-panel"') < html.index('<main'), (page, filename)
        blocks = re.findall(r'<section class="config-file">(.*?)</section>', html, re.S)
        assert len(blocks) == len(files), (page, filename)
        for block, file in zip(blocks, files):
            assert file['path'] in unescape(block)
            code = re.search(r'<pre\b[^>]*><code>(.*?)</code></pre>', block, re.S)
            assert code and unescape(code.group(1)) == file['text'], (page, filename, file['name'])
        for key, value, note in fields:
            assert f'[{key}]={value}' in unescape(html), (page, filename, key)
    story = (DOCS / page / 'storyboard.html').read_text(encoding='utf-8')
    assert not re.search(r'<script\b', story, re.I), page
    cards = re.findall(r'<section class="card" id="s(\d+)">(.*?)</section>', story, re.S)
    guides = get_guides(page)
    assert len(cards) == len(guides) == len(EXPECTED_KICKERS[page])
    for n, ((number, card), guide, kicker) in enumerate(zip(cards, guides, EXPECTED_KICKERS[page]), 1):
        assert int(number) == n and f'<span class="k">{kicker}</span>' in card
        blocks = re.findall(r'<div class="source-block">(.*?)</div>', card, re.S)
        assert len(blocks) == len(guide), (page, n)
        for raw, expected in zip(blocks, guide):
            assert f"{expected['path']}:{expected['first']}–{expected['last']}" in unescape(raw)
            assert expected['note'] in unescape(raw)
            match = re.search(r'<pre\b[^>]*><code>(.*?)</code></pre>', raw, re.S)
            assert match and unescape(match.group(1)) == expected['code'], (page, n)
    print(f'OK: {page} 两页的配置原文及 {len(guides)} 幕源码')
