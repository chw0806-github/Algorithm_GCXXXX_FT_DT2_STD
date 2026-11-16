"""为动画和分镜读取相同的 CP_SPEC 配置，生成不依赖 JS 的原文面板。"""
from html import escape
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parent.parent
CONFIG_DIR = Path('STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps')
# 字段解释只是阅读提示；数值始终从原文件读取，不写死在讲解里。
CONFIGS = {
    'light_response': ('LightResponse.data', [
        ('MinMeanR', 'R 均值下限'), ('MaxMeanR', 'R 均值上限'),
        ('MinMeanGr', 'Gr 均值下限'), ('MaxMeanGr', 'Gr 均值上限'),
        ('ROI_X', 'ROI 起始 X'), ('ROI_Y', 'ROI 起始 Y'),
        ('ROI_W', 'ROI 宽'), ('ROI_H', 'ROI 高'), ('bin_response', '响应失败 bin')]),
    'relative_illumination': ('RelativeIllumination.data', [
        ('w_win', '窗口宽'), ('h_win', '窗口高'), ('min_ratio', '比例下限（%）'),
        ('max_ratio', '比例上限（%）'), ('bin', '失败 bin'), ('grade', '失败 grade')]),
    'non_uniformity': ('NonUniformity.data', [
        ('w_cell', 'RAW 格宽'), ('h_cell', 'RAW 格高'), ('SpecR', 'R 平均上限'),
        ('MaxR', 'R 单格最大上限'), ('Max_Mirror_Diff_R', 'R 左右镜像差上限'),
        ('Max_Updown_Diff_R', 'R 上下镜像差上限')]),
    'deadrow_deadcol_bright': ('LineDefect.data', [
        ('radius', '每侧邻行/邻列数'), ('Spec_DeadRow_R_TA', 'R 行 TA 比例阈值'),
        ('Spec_DeadCol_R_TA', 'R 列 TA 比例阈值'), ('SpecDOL_Row_R', 'R 行 DOL 阈值'),
        ('breakrow_spec_r', 'R 断行阈值'), ('bin_dead_row', '坏行 bin'), ('bin_dead_col', '坏列 bin')]),
    'edge_deadrow_deadcol_bright': ('EdgeLineDefect.data', [
        ('open_mode', '子项开关'), ('StripRow', '边缘行条带'), ('StripCol', '边缘列条带'),
        ('radius', '向内参考的邻线数'), ('Spec_DeadRow_R_TA', 'R 行 TA'),
        ('Spec_DeadRow_R_MA', 'R 行 MA'), ('Spec_DeadCol_R_TA', 'R 列 TA'),
        ('Spec_DeadCol_R_MA', 'R 列 MA'), ('SpecDOL_Row_R', 'R 行 DOL 阈值')]),
    'pixel_defect': ('PixelDefect.data', [
        ('type', '1 为白帧类型'), ('open_mode', '检测开关'), ('radius', '邻域半径'),
        ('ratio_diff_gr', 'Gr 主缺陷比例阈值'), ('blemish_SpecGr', 'Gr blemish 比例阈值'),
        ('single_pix_limit', '单点数量限制'), ('bin_blemish', 'blemish bin')]),
}

CSS = '''
.config-panel{margin:18px 0 22px;padding:16px 18px;border:1px solid #53658b;border-radius:14px;background:#111a2c;color:#e0e9fa;min-width:0}
.config-panel h2{font-size:17px;margin:0 0 6px;color:#ffe2a0}
.config-panel p{margin:6px 0;font-size:13px;line-height:1.6;color:#bbc9df}
.config-keys{display:flex;flex-wrap:wrap;gap:7px;margin:10px 0 14px}
.config-key{border:1px solid #405578;border-radius:7px;padding:3px 8px;font-size:12px;background:#17243a}
.config-key code{color:#ffe2a0;font-family:ui-monospace,monospace}
.config-layout{display:grid;grid-template-columns:minmax(0,1fr) minmax(0,1fr);gap:12px}
.config-file{min-width:0;border:1px solid #344664;border-radius:9px;background:#0b1220;padding:10px 12px}
.config-file h3{font-size:14px;color:#b9d3ff;margin:0 0 3px}
.config-file .config-path{font-family:ui-monospace,monospace;font-size:11px;color:#a7b7cf;overflow-wrap:anywhere}
.config-file pre{margin:8px 0 0;padding:10px;border:1px solid #273653;border-radius:7px;background:#050a14;max-height:190px;overflow:auto;white-space:pre;color:#dbe9ff;font:12px/1.55 ui-monospace,monospace}
.config-file pre code{font:inherit;color:inherit;background:none;padding:0;border:0}
@media(max-width:700px){.config-layout{grid-template-columns:minmax(0,1fr)}}
'''


def get_config(page):
    filename, fields = CONFIGS[page]
    files = []
    for name in (filename, 'Basic.data'):
        path = CONFIG_DIR / name
        text = (ROOT / path).read_text(encoding='utf-8')
        files.append({'path': str(path), 'name': name, 'text': text})
    values = dict(re.findall(r'^\[([^\]]+)\]\s*=\s*([^\r\n]*)', files[0]['text'], re.M))
    missing = [key for key, _ in fields if key not in values]
    if missing:
        raise ValueError(f'{page}: 配置缺少字段 {missing}')
    return files, [(key, values[key], note) for key, note in fields]


def render_config(page):
    files, fields = get_config(page)
    key_html = ''.join('<span class="config-key">{}：<code>[{}]={}</code></span>'.format(
        escape(note), escape(key), escape(value)) for key, value, note in fields)
    file_html = ''.join('''<section class="config-file"><h3>{name} · 文件原文</h3>
<div class="config-path">{path}</div><pre tabindex="0" aria-label="{name} 文件原文，可滚动"><code>{text}</code></pre></section>'''.format(
        name=escape(f['name']), path=escape(f['path']), text=escape(f['text'])) for f in files)
    return '''<section class="config-panel" aria-label="示例配置文件">
<h2>本页使用的 .data 配置</h2>
<p>来源：GC8613T / CP_SPEC / param_4l_f_bright1_normal_30fps。下方是仓库中的文件原文，框内可滚动查看全部内容；不同配置或版本可能不同。动画里的像素数值是模拟值，不是配置或实测；拖动滑杆不会修改配置文件。</p>
<div class="config-keys">{keys}</div><div class="config-layout">{files}</div>
</section>'''.format(keys=key_html, files=file_html)
