"""构建各动画章节的源码摘录：行号直接指向仓库文件，重新构建会重新读取原文。"""
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
P = 'Algorithm_GCXXXX_FT_DT2_STD/Params/'
RAW = P + 'cpalgorithmraw.cpp'
ITEM = P + 'algorithmitems.cpp'
TEST = P + 'testitems.cpp'
BRIGHT = 'Algorithm_GCXXXX_FT_DT2_STD/TestItems/BrightTest.cpp'
CFG = 'STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps/Basic.data'

# 片段为连续原文，解释为教学文字；每个片段带锚点用于构建时发现行号漂移。
def S(path, a, b, anchor, note):
    return (path, a, b, anchor, note)

calllight=S(BRIGHT,35,40,'Light_Response','push_back 把此工位的子项结果放入结果集合；site_ 是工位索引。')
callri=S(BRIGHT,38,40,'Relative_Illumination','这一步调用相对照度子项；下面章节进入具体计算。')
callnon=S(BRIGHT,38,40,'Non_Uniformity','这一步调用不均匀性子项；画面数值只是教学示例。')
calldead=S(BRIGHT,44,46,'Deadrow_Deadcol_bright','调用结果被放入 items_results；true 是 log_flag，并非判定开关。')
callpixel=S(BRIGHT,154,157,'Pixel_Defect_Test','这条白帧分支调用 Pixel_Defect_Test；项目存在其他测试分支。')
bayer=S(TEST,27,33,'case RG','RG 排列将四个位置映射到 R、Gr、Gb、B 缓冲区。')
splitraw=S(TEST,64,78,'*pb1++','循环每次跨两行、两列，把 2×2 各位置写入对应通道。')
finalbin=S(ITEM,5903,5926,'item.grade>final_result.grade','遍历子项：忽略不参与汇总的项，失败且 grade 严格更高时覆盖最终结果。')
basic=S(CFG,1,3,'[bayer]=RG','这里是示例配置，不是 C++：图像 3840×2160，Bayer 顺序为 RG。')
split=S(TEST,1422,1425,'w_cell/2','四通道尺寸和格尺寸均传入一半；这是不均匀性线程的参数。')
roi=S(ITEM,908,915,'test_roi_channel_mean','这里将 ROI 的位置和尺寸以及四个通道缓冲区传给计算函数。')
roimean=S(RAW,7951,7963,'mean_t/cnt','循环累加 ROI 像素并计数；返回总和除以个数。')
riwin=S(RAW,1587,1595,'ROI_3+=','ROI_3 的循环展示左上窗口的取样与求均值；其他窗口同理。')
riformula=S(RAW,1640,1649,'100*darkest/ROI_2','ROI_2 是中心均值，darkest 实际是四角均值的平均；先处理极端情况，再返回百分比。')
rowparams=S(TEST,3636,3648,'w/2,h/2','行检测分别为四个通道设置半尺寸参数并启动线程，随后等待。')
rowmean=S(RAW,4940,4957,'each_row_.push_back','逐行读取像素：mean_t 是一行之和，除以 2*N 得整行均值；另外保存两半均值。')
rowneighbors=S(RAW,5025,5049,'mean_top','i 是目标行，r 是邻域半径；上下各自累加并除以计数。边界条件会阻止不足 r 条时进入对应循环。')
rowratio=S(RAW,5064,5071,'std::max','比例模式下分别除以上下均值，std::max 选较大的一侧。')
rowcount=S(RAW,5079,5087,'diff>spec_line','diff 是两侧较大比例差；严格大于阈值时计数加一。')
dol=S(RAW,5096,5103,'GetDatasStd','先收集行内像素，再计算标准差；严格超过 DOL 阈值时计数。')
std=S(RAW,18,28,'datas.size ()-1','源码计算样本标准差：先求平均，再求平方差之和，除以 N−1 后开平方。')
halves=S(RAW,4946,4957,'mean_t_half1','每次读取相邻两个像素，同时分别累加左半与右半，最后保存半行均值。')
breakline=S(RAW,4964,4977,'std::abs(delta2-delta1)','delta1、delta2 是相邻变化；两者差的绝对值大于阈值时计数。')
edge=S(RAW,5025,5034,'mean_top','先默认参考均值为当前行；只有上方足够 r 条时才改算上方邻行。')
zero=S(RAW,5058,5063,'mean_top==0','任一侧均值为零时，源码将两个比例差和最终差都设为 −1。')
decision=S(ITEM,5110,5124,'cnt_dead_col','先汇总 DOL 和断线计数；任何类型大于零就失败，然后优先检查列。')
priority=S(ITEM,5132,5140,'grade_dead_row_>result_line.grade','后续行分支要求行 grade 严格大于当前 grade 才能覆盖；其余分支也要分别查看。')
cellcount=S(RAW,7582,7591,'cnt_vertical','h/h_cell 与 w/w_cell 决定格子行列数；双重循环逐格处理。')
cellpixels=S(RAW,7594,7604,'mean_tmp/=cnt_tmp','遍历格内像素，累加并计数；均值保存在 mean_each_cell。')
cellstd=S(RAW,7605,7616,'stdv_tmp/=cnt_tmp','每个像素与格内均值之差平方；除以像素数后开平方，是总体标准差。')
cellratio=S(RAW,7621,7632,'stdv_each_cell.at (i)/mean_tmp*100','格内标准差除以均值再乘 100；均值过小时仅对分母设置下限。')
mirror=S(RAW,7639,7645,'n_uniform_mirror_eachcell.replace','同一格的数据按反转列或反转行写入镜像数组。')
mirrordiff=S(RAW,7649,7663,'std::abs','原数组与镜像数组逐格相减取绝对值，然后分别查找最大差。')
pixneighbor=S(RAW,9164,9184,'sum_t[n]/cnt','四个滑动邻域的和除以 r*r 得均值，再计算目标与各邻域的比例差。')
pixratio=S(RAW,9175,9198,'compareValue[n]>spec','Ratio 模式计算绝对差与邻域均值的比；任何一个超过 spec 就增加 flag。')
pixmask=S(RAW,9195,9217,'out_blemish','主缺陷 flag>0 时写主图；否则才检查 blemish 阈值，两个标记互斥。')

EXPECTED_KICKERS = {
    'light_response': ['总览','拆解代码','多工位','分通道','取 ROI','算均值','判 Spec','归档汇总'],
    'relative_illumination': ['定位','取窗口','求均值','算 ratio','防呆','判 Spec','归档'],
    'non_uniformity': ['定位','认识 RAW','拆四通道','一格从哪来','逐行铺格','放大像素','求均值','求标准差','格内百分比','平均与最大','左右镜像','上下镜像','判定与互动'],
    'deadrow_deadcol_bright': ['任务','分通道','认识行列','行均值','取邻行','算比例差','扫描行','扫描列','线内 DOL','半行均值','断线二次差','边界与零','汇总结果','互动判定'],
    'pixel_defect': ['总览','拆通道','目标与邻域','一块求均值','四块均值','四次比较','主缺陷阈值','两张标记图','亮暗方向','逐点扫描','合回 Bayer','单点与团簇','子项与汇总','互动模拟'],
}

GUIDES = {
 'light_response': [
  [calllight], [calllight], [S('Algorithm_GCXXXX_FT_DT2_STD/FTAlgorithm.cpp',25,29,'Algorithm_all','全局数组用工位索引选择对应算法对象。')], [bayer,splitraw],
  [roi], [roimean], [S(ITEM,933,942,'limit_r_max_','四通道均值与各自上下限比较；超出任一范围则失败。')],
  [S(ITEM,960,964,'res.bin','对应分支把配置的 bin 和 grade 写入子项结果。'),finalbin]],
 'relative_illumination': [
  [callri], [riwin,S(RAW,1630,1639,'ROI_2+=','中心窗口按中心位置计算；四角与中心共五个窗口。')],
  [riwin], [riformula], [S(RAW,1640,1647,'return 1000','中心均值的下限与极端比例返回值用于特殊情况。')],
  [S(ITEM,1332,1340,'limit_max_ratio_','计算结果与上下限比较；超出则设置失败、bin、grade。')],
  [S(ITEM,1342,1346,'g_str_log_all','计算结果与子项名称被追加到日志。')]],
 'non_uniformity': [
  [callnon], [basic,bayer], [bayer,splitraw], [split], [cellcount], [cellpixels], [cellpixels], [cellstd], [cellratio],
  [S(RAW,7628,7635,'n_uniform_eachcell_max','每格百分比求平均，并查找最大值与位置。')], [mirror,mirrordiff], [mirror,mirrordiff],
  [S(ITEM,1562,1569,'spec_n_uniform_r_','四通道平均分别与阈值比较，超限时设置子项结果；其他指标还有后续分支。')]],
 'deadrow_deadcol_bright': [
  [calldead], [bayer,splitraw], [rowparams,basic], [rowmean], [rowneighbors], [rowratio,rowcount], [rowcount],
  [S(RAW,5225,5271,'mean_left','列检测用左右邻列；与行检测方向不同。')],
  [dol,std], [halves], [breakline], [edge,zero], [decision,priority], [rowratio,rowcount,priority]],
 'pixel_defect': [
  [callpixel], [basic,bayer,splitraw],
  [S(RAW,9155,9164,'sum_t[0]','源码数组顺序是左上、左下、右上、右下；画面为便于阅读按左上、右上、左下、右下展示。本幕只示意内部点。'),pixneighbor],
  [S(RAW,9175,9178,'sum_t[n]/cnt','r=5 时 cnt=25；每块的求和结果除以像素数得到该块均值。')],
  [pixneighbor], [pixratio],
  [S(RAW,9195,9204,'compareValue[n]>spec','循环比较四个比例；只要 flag 大于零就写入主缺陷图。')],
  [pixmask],
  [S(RAW,9192,9193,'mean_ref','四块均值的平均用于计算带符号差值；此值和比例判断用途不同。')],
  [S(RAW,9143,9151,'vec_up','沿行扫描并通过增减更新邻域的列和；随后还要逐列扫描。')],
  [S(TEST,2875,2881,'RGGB_Map_Integrate_chao','主缺陷、blemish 和差值图分别按 Bayer 排列合回原图位置。')],
  [S(TEST,2905,2922,'FindALLClusterBayer_in_binary','对合成主缺陷图调用团簇统计；本幕仅作概念解释。')],
  [S(ITEM,3633,3649,'res.bin = 2001','满足外围条件时先赋 2001；符合亮单点数量范围可改为 2002。'),S(ITEM,3661,3665,'res.bin = 2006','暗单点符合相应范围时改为 2006；其他范围与后续 blemish 条件可能再次改变结果。'),S(ITEM,3680,3697,'blemish_cnt','超过单点限制或 blemish 条件时可能覆盖先前 bin。'),finalbin],
  [pixratio,pixmask]]
}

def get_guides(page):
    guides = []
    for snippets in GUIDES[page]:
        blocks = []
        for path, first, last, anchor, note in snippets:
            lines = (ROOT / path).read_text(encoding='utf-8').splitlines()
            if first < 1 or last > len(lines) or first > last:
                raise ValueError(f'代码范围无效: {path}:{first}-{last}')
            extract = lines[first-1:last]
            if not any(anchor in line for line in extract):
                raise ValueError(f'源码已变化，请复核: {path}:{first}-{last} 缺少 {anchor}')
            blocks.append({'path': path, 'first': first, 'last': last, 'note': note,
                           'code': '\n'.join(f'{i:>5}  {line}' for i, line in enumerate(extract, first))})
        guides.append(blocks)
    return guides
