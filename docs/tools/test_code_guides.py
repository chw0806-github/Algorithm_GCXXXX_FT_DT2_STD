#!/usr/bin/env python3
"""检查所有成品页包含与章节一一对应的源码原文。无需浏览器或第三方依赖。"""
import json
import re
import sys
from pathlib import Path

DOCS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(DOCS))
from code_guides import GUIDES, get_guides  # noqa: E402

for page in GUIDES:
    html = (DOCS / page / 'index.html').read_text(encoding='utf-8')
    match = re.search(r'window\.__CODE_GUIDE = (.*?);\s*</script>', html, re.S)
    assert match, f'{page}: 未内联代码映射'
    actual = json.loads(match.group(1).replace('<\\/', '</'))
    expected = get_guides(page)  # 含行范围及锚点验证，重新从源码提取
    assert actual == expected, f'{page}: 摘录不是当前源码'
    assert 'id="code-guide"' in html, f'{page}: 缺少可见区域'
    assert all(blocks and all(b['note'] and b['code'] for b in blocks) for blocks in actual)
    print(f'OK: {page}，{len(actual)} 幕逐章源码摘录')
