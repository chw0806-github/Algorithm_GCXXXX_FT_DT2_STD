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
    'roi_prnu': ['一眼看懂','真实示例配置','前置与开关','通道与区域','Delta 来源','模拟 ROI','原始像素均值','Delta 平方','计算 PRNU','四通道判定','共享缓冲','边界与无效值','返回与追加','互动模拟'],
    'single_pixel_defect': ['一眼看懂','配置与版本','模式选择','分通道','四块邻域','比例比较','标记与方向','合图与计数','亮点分档','亮暗竞争','总数覆盖','返回与追加','风险与边界','互动分档'],
    'blemish_test': ['一眼看懂','配置与版本','图像预处理','亮度修正与增强','大小窗口找暗斑','区域阈值','提取连通候选','找 PD 标记','排除附近 PD','匹配增强候选','增强包围框','没匹配到增强','返回与特殊情况','边界与版本','互动观察'],
    'color_gi_5x5': ['一眼看懂','配置与假设','前置与开关','假设分成 25 块','准备五点输入','一块的计算方法','左上逐步比较','右上逐步比较','中心逐步比较','左下逐步比较','右下逐步比较','五点汇总','异常值与映射','返回与追加','互动观察'],
    'color_gi': ['一眼看懂','配置与入口','先检查前置条件','分通道和区域','一块怎么算','得到 81 个比例','找到最大和最小','两端离 1 多远','得到 GI','对照 GI 阈值','低亮度与无效前置','原始向量与特殊值','返回与追加','互动观察'],
    'color_shading_15x15': ['一眼看懂','配置与入口','分通道与分区','一块求通道均值','一块算比例','得到比例图','四角区域','一对角怎么比较','六对取最大','四边中间区域','上下边比较','左右边比较','对照上限','异常与风险','返回与追加','互动观察'],
    'color_shading_5x5': ['一眼看懂','配置与入口','分区与五点','一块先算 RG / BG','先得到五点比例','RG1 对角比较','RG2 对角比较','RG3 中心与四角','BG1 对角比较','BG2 对角比较','BG3 中心与四角','依次对照六个范围','源码的除法条件','前置均值异常','返回与追加','互动观察'],
    'color_shading': ['一眼看懂','版本与开关','前置与低亮度','把画面分区','每块算颜色比例','逐块计算','RG 找中心与极值','BG 找中心与极值','RG 和中心比较','BG 和中心比较','对照阈值','特殊情况','返回并追加结果','互动模拟'],
    'light_response': ['总览','拆解代码','多工位','分通道','取 ROI','算均值','判 Spec','归档汇总'],
    'relative_illumination': ['定位','取窗口','求均值','算 ratio','防呆','判 Spec','归档'],
    'non_uniformity': ['定位','认识 RAW','拆四通道','一格从哪来','逐行铺格','放大像素','求均值','求标准差','格内百分比','平均与最大','左右镜像','上下镜像','判定与互动'],
    'deadrow_deadcol_bright': ['任务','分通道','认识行列','行均值','取邻行','算比例差','扫描行','扫描列','线内 DOL','半行均值','断线二次差','边界与零','汇总结果','互动判定'],
    'color_ratio': ['调用与返回','前置条件','区域尺寸','完整 9×9','选出五块','中心像素','四个累加和','三种比值','中心 RG 判定','其他点与比值','区域怎么比较','五块模拟数据','左上逐步计算','右上逐步计算','中心逐步计算','左下逐步计算','右下逐步计算','五点 GG 比较','五点 RG 比较','五点 BG 比较','五点汇总','中心 GG 加严','绿色差值','Class1 与 Class2','Golden 特殊逻辑','四个子项','互动模拟'],
    'edge_deadrow_deadcol_bright': ['任务','实际配置','拆通道','假设选边缘','目标行像素','向内找邻行','比例差','两级阈值','边缘列','线内 DOL','汇总优先级','实际运行与风险','互动模拟'],
    'pixel_defect': ['总览','拆通道','目标与邻域','一块求均值','四块均值','四次比较','主缺陷阈值','两张标记图','亮暗方向','逐点扫描','合回 Bayer','单点与团簇','子项与汇总','互动模拟'],
}

GUIDES = {
 'roi_prnu': [
  [S(BRIGHT,154,162,'ROI_PRNU','调用方将单个子项追加，然后继续汇总。'),S(RAW,8320,8351,'return sigma/mean*100','同一 ROI 的 Delta 平方量除以原始均值。')],
  [S('Algorithm_GCXXXX_FT_DT2_STD/Params/parammanage.cpp',16994,17011,'ROI_PRNU_V1.0','打开文件后赋版本；后续解析未必成功。'),S(ITEM,5789,5796,'spec_prnu_r_','四通道分别比较各自上限，失败使用配置 bin 和 grade。')],
  [S(ITEM,5741,5762,'mean_gr_roi>1','ROI 开关与像素缺陷开关共同控制，Gr 大于 1 才计算。'),S(ITEM,897,915,'mean_gr_roi','Light_Response V1.0 初始化并计算此前的 Gr 均值。')],
  [basic,bayer,splitraw,S(TEST,1221,1234,'width/2,height/2','通道宽高减半，ROI 配置参数原样传入。')],
  [S(ITEM,3518,3524,'g_buf_delta_r_','Pixel_Defect V1.0 将 Delta 缓冲交给缺陷检测。'),S(RAW,9175,9193,'delta_data[w*i+j]','Delta 为像素减四块邻域均值的平均。')],
  [S(RAW,9155,9178,'sum_t[n]/cnt','四块邻域按 radius 的平方求均值；画面棋盘为假设。'),S(RAW,9192,9193,'mean_ref','本页的模拟 Delta 取自该定义。')],
  [S(RAW,8338,8349,'mean_t/cnt','在同一 ROI 内计算原始通道像素均值。')],
  [S(RAW,8323,8335,'Delta[width*i+j] * Delta[width*i+j]','对 Delta 平方求和，除以 N−1 后开平方，没有对 Delta 做均值中心化。')],
  [S(RAW,8335,8351,'return sigma/mean*100','零参考平方量除以原始均值并转百分数。')],
  [S(ITEM,5789,5796,'roi_prnu_gr>','四通道任一严格超过各自阈值时失败。')],
  [S(BRIGHT,149,160,'Single_Pixel_Defect_Test','缓冲分配后多个步骤可依序写入共享区域；ROI_PRNU 读取后才释放。'),S(ITEM,3285,3292,'g_buf_delta_r_','Single 像素检测也可能写入同一组 Delta。'),S(ITEM,5754,5762,'g_buf_delta_r_','本函数读取现有通道 Delta 缓冲。')],
  [S(RAW,8323,8351,'line+win_h>=height','贴边返回 −1，分母及输入边界缺乏完整防护。'),S(ITEM,5749,5755,'mean_gr_roi>1','Gr 不满足时保留 −1，之后仍比较阈值。'),S(ITEM,5789,5796,'roi_prnu_r>','普通浮点大于比较不是有效性检查。')],
  [S(ITEM,5723,5737,'g_msg = false','未知版本和 V1.0 初始化。'),S(ITEM,5789,5809,'result_real_roi_prnu.g_msg = false','超限设失败；关闭条件时不参与汇总。'),S(BRIGHT,156,162,'ROI_PRNU','调用方 push_back 后汇总。'),finalbin],
  [S(RAW,9175,9193,'delta_data[w*i+j]','假设棋盘在 radius=5 下产生局部 Delta。'),S(RAW,8323,8351,'return sigma/mean*100','对假设 ROI 数据计算；互动不运行完整 C++ 检测。'),S(ITEM,5789,5796,'roi_prnu_r>','使用真实示例上限 3 比较模拟值。')]],

 'single_pixel_defect': [
  [S(BRIGHT,154,158,'Single_Pixel_Defect_Test','调用此函数并追加一个子项；并非前一个 Pixel_Defect_Test 的返回值。'),S(ITEM,3285,3300,'find_pixel_defect','重新运行四通道像素检测并取得计数。')],
  [S('Algorithm_GCXXXX_FT_DT2_STD/FTAlgorithm.cpp',400,404,'PD.data','初始化参数时尝试加载 PD.data。'),S('Algorithm_GCXXXX_FT_DT2_STD/Params/parammanage.cpp',4988,5002,'PD_V1.0','打开成功后赋版本，解析尚未完成。'),S('Algorithm_GCXXXX_FT_DT2_STD/Params/parammanage.cpp',5387,5403,'cnt_param != 37','检查解析及字段数量；文件缺失时无法确定实际配置。')],
  [S(ITEM,3205,3209,'Pixel_Defect_bright','type 为 0 或 1 时覆盖传入类型。'),S(ITEM,3253,3282,'spec_mode_=DN','bright 使用比例阈值，dark 使用 DN 阈值。')],
  [basic,bayer,splitraw,S(TEST,2855,2877,'w/2,h/2','四通道分别检测并合回 Bayer。')],
  [S(RAW,9155,9164,'sum_t[0]','内部点四块邻域的和；假设半径为 5。'),pixneighbor],
  [pixratio,S(RAW,9195,9204,'flag>0','只要有一个严格超主阈值就标记主图。')],
  [S(RAW,9192,9217,'mean_ref','方向差使用四块均值的平均；主图和 blemish 标记互斥。'),S(TEST,2924,2937,'g_buf_delta_raw_','方向差严格大于零计亮，否则计暗。')],
  [S(TEST,2875,2881,'RGGB_Map_Integrate_chao','合并标记及差值。'),S(TEST,2905,2942,'cnt_white_pix = cnt_white_pix','团簇计数和亮暗计数的具体处理；并非删除所有团簇。')],
  [S(ITEM,3303,3325,'limit_wp1_','亮点三档分别比较，示例分档均是假设。'),S(ITEM,3327,3346,'limit_dp1_','暗点也采用三档。')],
  [S(ITEM,3348,3359,'res_singe_dp.grade > res_singe_wp.grade','暗 grade 严格更高才选暗；相等选亮。')],
  [S(ITEM,3362,3386,'pixel_count_to_check','选亮、暗或总数；超过上限时直接覆盖，无 grade 比较。')],
  [S(ITEM,3190,3203,'Single_Pixel_Defect','未知版本与 V1.0 初始结果不同。'),S(ITEM,3399,3407,'g_msg = false','关闭开关不参与汇总并返回。'),S(BRIGHT,154,158,'Single_Pixel_Defect_Test','调用方 push_back 一个子项。'),finalbin],
  [S(ITEM,3255,3300,'blemish_global_size_limit_','该路径读取 blemish 字段并传入检测函数；需检查其初始化。'),S('Algorithm_GCXXXX_FT_DT2_STD/Params/parammanage.cpp',5387,5403,'cnt_param != 37','专用加载器的结束处；核查完整 4988–5403 行有哪些字段被读取。'),S(RAW,9180,9189,'mean_t[n]','比例模式没有单独的零分母检查。')],
  [S(ITEM,3303,3359,'res_singe_dp.grade > res_singe_wp.grade','分别分档，再按 grade 竞争。'),S(ITEM,3362,3386,'pixel_count_to_check','仅以假设输入和参数模拟最终覆盖。')]],

 'blemish_test': [
  [S(BRIGHT,149,156,'Blemish_Test','申请资源后调用此子项并追加结果。'),S(ITEM,3031,3040,'PD_all_single_pix_find','V1.1 先准备 PD 标记，再调用双路污点检测。')],
  [S('Algorithm_GCXXXX_FT_DT2_STD/FTAlgorithm.cpp',423,429,'Blemish_V1.1.data','先尝试 V1.1 再尝试 V1.0，其他目录可能覆盖版本。'),S(ITEM,2968,2977,'blemish_V1.1','版本分派；未知版本返回 false 且不参与汇总。'),S(ITEM,3031,3050,'code_Blemish>0','V1.1 开关与结果。')],
  [S(RAW,2451,2479,'medianBlur','普通路转 float，亮度提取、滤波及缩放。'),S(RAW,2572,2598,'resize_w_enhace','增强路同样预处理，使用另一尺寸。')],
  [S(RAW,2486,2510,'mean_center/mean_corner','左上和中心参考构建 LSC 因子。'),S(RAW,2647,2665,'maxV','增强路再用最小最大值映射。')],
  [S(RAW,2525,2542,'cv::divide','大小窗口差除以大窗口均值；注意零分母。')],
  [S(RAW,2543,2564,'threshold_center_','中心 ROI 先阈值化，整图再阈值化，然后清边并提取候选。'),S(RAW,2670,2698,'threshold_center_enhace','增强路使用独立阈值和尺寸。')],
  [S(RAW,1767,1815,'cnt_pix','从二值图出发，八邻域查找连通块。'),S(RAW,1806,1826,'sizeLimit','按外接框最大边筛选并记录左上角。')],
  [S(ITEM,2927,2956,'find_all_single_pixel_defect','Blemish 使用 Ratio 模式和自己的 PD 参数。'),S(TEST,2769,2791,'RGGB_Map_Integrate_chao','四通道二值合回 Bayer。'),S(TEST,2816,2826,'pd_all_single_particle.push_back','每个 255 坐标加入列表，不检查孤立性。')],
  [S(RAW,2216,2248,'pd_area_x_','普通候选左上角与 PD 的横纵差都在范围内时移除。')],
  [S(RAW,2275,2298,'search_area_x','增强坐标乘倍率后按横纵范围匹配；无匹配记入列表。')],
  [S(RAW,2323,2350,'current_x_plus_w','对所有匹配增强框求整体边界。'),S(RAW,2359,2372,'max_p_enhace','生成对应普通候选的增强包围框。'),S(RAW,2381,2402,'min_enhace_size','最大边大于下限且不超过上限才命中。')],
  [S(RAW,2292,2298,'particle_not_in_enahce','没有匹配时记录普通候选。'),S(RAW,2412,2441,'particle_not_in_enahce.size()>0','未匹配非空也返回失败代码。')],
  [S(RAW,2204,2213,'particle.size()>1000','计时开始后提取两路，普通候选超过 1000 提前返回。'),S(RAW,2277,2290,'elapsed() > 300','仅在特定循环检查 300 毫秒。'),S(ITEM,3043,3050,'code_Blemish>0','正 code 才令子项失败。'),finalbin],
  [S(ITEM,3023,3037,'image.empty()','无效图像仅记录日志；开关开启时仍继续。'),S(RAW,2508,2510,'mean_center/mean_corner','参考和为零需核查。'),S(RAW,2647,2665,'maxV','增强拉伸要关注最大最小相等。'),S(ITEM,2968,2977,'g_msg = false','未知版本的结果。')],
  [S(RAW,2221,2248,'pd_area_x_','按横纵差排除。'),S(RAW,2275,2298,'search_area_x','增强候选按坐标匹配。'),S(RAW,2381,2389,'min_enhace_size','匹配后的包围框尺寸判定；互动只模拟单个候选。'),S(RAW,2412,2439,'particle_not_in_enahce.size()>0','没有匹配也失败。')]],
 'color_gi_5x5': [
  [S(BRIGHT,63,64,'ratio_GI_5x5','调用并追加整个向量。'),S(RAW,7895,7910,'sum_g1-sum_g2','5×5 路径的 GG 是绿色和之差除平均和。'),S(BRIGHT,613,622,'TargetGI_Leftup','五点各自与上限严格比较。')],
  [S('Algorithm_GCXXXX_FT_DT2_STD/Params/parammanage.cpp',6491,6503,'GI_5x5.data','文件打开失败返回 false；本仓库未找到专用文件。'),S(BRIGHT,532,556,'ratio_open_mode_','版本分派并检查此项开关；教程的 5×5 参数为假设。')],
  [S(ITEM,897,915,'mean_gr_roi','此前 Light_Response 对两个绿色前置均值赋值。'),S(TEST,1915,1919,'mean_gr>0','两个前置均值都大于零才计算。'),S(TEST,1998,2012,'GG_ratio','否则五点映射填 −1；本函数无 Gr>10 检查。')],
  [basic,bayer,S(TEST,1915,1929,'roi_x_pitch','根据专用配置的 Skip、行数和列数构造 ROI；本示例数量为假设。'),S(TEST,1948,1970,'ratio_g_g_center','选取中心、四角的索引。')],
  [S(TEST,1932,1945,'get_roi_colorratio_5X5','共用函数逐块计算并存入向量；本教程仅模拟五点。'),S(TEST,1948,1970,'ratio_g_g_center','从向量读取五点对应的比例。')],
  [S(RAW,7875,7889,'sum_g1 = 0.1','绿色和从 0.1 开始；画面直接展示，不播放累加。'),S(RAW,7895,7910,'sum_g1-sum_g2','绿色差绝对值除绿色平均和；极端条件设为 100。')],
  *[[S(TEST,1974,1978,'GG_ratio','从五点向量构造映射，值大于 10 时变成 −1。'),S(RAW,7895,7910,'sum_g1-sum_g2',name+'的绿色相对差由两个绿色通道的和计算。'),S(BRIGHT,613,622,'TargetGI_Leftup','五点各自与对应假设上限比较；实际值来自专用配置。')] for name in ('左上','右上','中心','左下','右下')],
  [S(BRIGHT,613,622,'TargetGI_Center','各点采用自己的上限，任一严格大于时失败。')],
  [S(TEST,1998,2012,'GG_ratio','前置任一非正时五点映射均为 −1。'),S(RAW,7907,7910,'g_g = 100','底层极端情况设为 100。'),S(TEST,1974,1978,'>10','五点大于 10 转 −1；正上限下 −1 不触发大于。'),S(BRIGHT,613,622,'TargetGI_Leftup','无效 −1 仍直接参与上限比较。')],
  [S(BRIGHT,532,541,'g_msg = false','未知版本返回 false 且不参与汇总。'),S(BRIGHT,640,653,'push_back','关闭时 g_msg=false；V1.0 返回一个子项。'),S(BRIGHT,63,64,'insert','调用方追加返回向量。'),finalbin],
  [S(RAW,7895,7910,'sum_g1-sum_g2','互动调整右下区域模拟 Gr 后重算相对差。'),S(BRIGHT,613,622,'TargetGI_Rightdown','使用假设的每点上限，仍按源码的严格大于比较。')]],
 'color_gi': [
  [S(BRIGHT,60,61,'ratio_GI_', '调用 Color_GI 并将返回范围追加到 items_results。'),S(BRIGHT,342,356,'GI = std::max','原始 GG 比例最大和最小分别与 1 比较，再取较大偏离。')],
  [S('Algorithm_GCXXXX_FT_DT2_STD/FTAlgorithm.cpp',358,364,'GI_V1.1.data','依次尝试加载 V1.0 与 V1.1 配置。'),S('Algorithm_GCXXXX_FT_DT2_STD/Params/parammanage.cpp',6221,6234,'gi_ratio_V1.0','文件成功打开后选择 V1.0。'),S(BRIGHT,291,314,'ratio_open_mode_','版本分派；V1.0 只检查 GI 自身开关。')],
  [S(ITEM,897,915,'mean_gr_roi','Light_Response 为两个绿色中心均值赋初值并计算。'),S(TEST,1780,1785,'mean_gr>0','要求两个绿色均值均大于零才计算区域比例。'),S(TEST,1863,1871,'roi_gg_ratio.append(-1)','否则向量填 −1。'),S(BRIGHT,342,356,'mean_gr_roi>10','先取极值，再检查 Gr 是否大于 10。')],
  [basic,bayer,S(TEST,1780,1794,'roi_x_pitch','根据 GI 的 ROI 数量与 Skip，以整数 pitch 构造通道 ROI。')],
  [S(RAW,7829,7844,'sum_g1 = 0.1','各通道和从 0.1 开始，画面直接展示模拟和。'),S(RAW,7851,7864,'g_g = std::abs','GG 是两绿色和的比；极端分支置 100。')],
  [S(TEST,1797,1810,'roi_gg_ratio.push_back','每个 ROI 分别计算比例并保存至原始向量。')],
  [S(BRIGHT,342,348,'max_element','对所有原始 GG 比例取最大和最小，而非只取五点映射。')],
  [S(BRIGHT,348,356,'gg_roi_min_1','两个极值分别减 1 取绝对值。')],
  [S(BRIGHT,350,356,'GI = std::max','两端偏离取最大作为 GI。')],
  [S(BRIGHT,378,389,'GI > manage.param_GI.GI_Ratio','V1.0 使用严格大于；超限时使用 GI 配置的 bin 和 grade。')],
  [S(TEST,1863,1871,'roi_gg_ratio.append(-1)','前置 Gr 或 Gb 不大于零时将向量设为 −1。'),S(BRIGHT,342,356,'mean_gr_roi>10','先取极值再检查 Gr；Gb 不在该条件中。'),S(BRIGHT,391,396,'not test GI','低 Gr 分支只记录日志，未修改初始化结果。')],
  [S(TEST,1804,1810,'roi_gg_ratio.push_back','原始比例先存入向量。'),S(TEST,1839,1843,'>10','五点映射将大于 10 的值改为 −1，不改变原始向量。'),S(RAW,7861,7864,'g_g = 100','底层极端绿色比例设置 100。')],
  [S(BRIGHT,291,302,'g_msg = false','未知版本返回一个不参与汇总的子项。'),S(BRIGHT,403,413,'push_back','开关关闭设置 g_msg=false；V1.0 返回一个子项。'),S(BRIGHT,60,61,'insert','追加返回结果。'),finalbin],
  [S(BRIGHT,342,356,'GI = std::max','互动模拟重新计算所有区域比例和 GI。'),S(BRIGHT,384,389,'GI >','与示例真实阈值严格比较，不能用舍入后的显示值判断。')]],
 'color_shading_15x15': [
  [S(BRIGHT,57,58,'cs_shading_15x15','调用并追加向量。'),S(TEST,5086,5092,'RB_GrGb[i]','每块用四通道均值计算一个比例。'),S(TEST,5108,5116,'out_DiffCorner','六对角比较后取最大值。'),S(TEST,5136,5140,'out_Maxdiff','上下、左右边差取较大值。')],
  [S('Algorithm_GCXXXX_FT_DT2_STD/Params/parammanage.cpp',17812,17824,'ColorShading_15x15.data','文件打开失败时直接返回 false；本仓库没有该文件。'),S(BRIGHT,1181,1200,'open_mode_','分派 V1.0 后，仅 open_mode=1 进入计算。')],
  [basic,bayer,splitraw,S(TEST,5057,5077,'roi_x_pitch','根据行列数、Skip 和整数 pitch 构造 ROI；示例 15×15 为假设。')],
  [S(TEST,5071,5083,'GetROIChannelMean','四个通道分别计算每块的均值。'),S(RAW,13609,13626,'mean_t / cnt','均值是像素和除以数量；画面不逐点播放。')],
  [S(TEST,5086,5092,'Gr_mean[i] * Gb_mean[i] + EPS','比例用 R×B 除以 Gr×Gb 加 EPS；并非 RG/BG。')],
  [S(TEST,5071,5092,'RB_GrGb[i]','每个区域得到四通道均值，再计算比例。')],
  [S(TEST,5094,5106,'corner_avg','每角取 2×2 个比例求均值。')],
  [S(TEST,5108,5116,'CornerLT / (CornerRB + EPS)','左上除右下加 EPS，再减一、取绝对值并乘 100。')],
  [S(TEST,5108,5116,'max_element','六个方向固定的比值差中取最大。')],
  [S(BRIGHT,1207,1217,'x_cnt / 2','中心索引来自区域数整数除二。'),S(TEST,5118,5134,'safe_x','中心索引裁到安全范围，每条边取连续五个比例求平均。')],
  [S(TEST,5122,5127,'Top /= 5.0','顶部和底部各五块求平均。'),S(TEST,5136,5138,'Diff_TB','上下差是 Top 除以 Bottom 加 EPS。')],
  [S(TEST,5129,5138,'Diff_RL','左右差是 Right 除以 Left 加 EPS，再与上下差取最大。')],
  [S(BRIGHT,1220,1225,'max_diff_corner','任一输出严格大于对应上限时设置失败；阈值和 bin 均为教学假设。')],
  [S(TEST,5057,5066,'return -1','不足 5 行或列时未赋输出即返回 −1。'),S(BRIGHT,1212,1224,'TestFunc_LSC','调用方忽略返回值并读取未初始化输出，存在未定义行为。'),S(RAW,13609,13626,'return -1','只检查末端边界；未验证负起点和零面积。')],
  [S(BRIGHT,1194,1200,'result_cs_15x15','初始化通过，只有开关等于 1 才进入处理。'),S(BRIGHT,1232,1240,'push_back','没有关闭分支更改 g_msg；返回一个子项。'),S(BRIGHT,1181,1190,'g_msg = false','未知版本返回不参与汇总的子项。'),S(BRIGHT,57,58,'insert','向量整体追加到结果列表。'),finalbin],
  [S(TEST,5094,5116,'diffcorner','互动改变右下角的模拟值后重新计算角差。'),S(TEST,5122,5138,'out_Maxdiff','互动改变下边中段并重新计算边差。'),S(BRIGHT,1220,1225,'max_diff_edge','两个假设上限按源码的或条件判断。')]],
 'color_shading_5x5': [
  [S(BRIGHT,54,55,'cs_shading_res_5x5','调用并把向量整体追加到结果列表。'),S(BRIGHT,1061,1075,'RG1 =','六项分别用五点比值作对角或中心比较。')],
  [S('Algorithm_GCXXXX_FT_DT2_STD/FTAlgorithm.cpp',346,348,'ColorShading_5x5.data','加载器读取本版本配置。'),S(BRIGHT,966,969,'color_shading_5_5_V1.0','按版本选择实现。'),S(BRIGHT,982,1012,'param_color_shading_5x5.open_mode_','只检查 5×5 开关，再用本配置重新求 ROI 比例。')],
  [basic,S(TEST,1780,1794,'roi_x_pitch','根据行列数及 Skip 按整数 pitch 建立 ROI。'),S(TEST,1813,1835,'ratio_r_g_center','取得中心与四角的索引及比例。')],
  [S(RAW,7829,7866,'sum_g_average','中心示例展示四通道和、绿色参考和与 RG/BG；画面不逐像素播放。')],
  [S(RAW,7829,7866,'sum_g_average','四通道按像素求和，RG 与 BG 共用绿色平均和；画面只展示结果。'),S(TEST,1797,1809,'roi_rg_ratio.push_back','逐块计算比例并保存。')],
  [S(BRIGHT,1061,1075,'RG1 = ratio_value.rg_leftup','RG1 左上除右下；六项共有同一个 if 条件。')],
  [S(BRIGHT,1061,1075,'RG2 = ratio_value.rg_rightup','RG2 右上除左下。')],
  [S(BRIGHT,1061,1075,'RG3 = (4*ratio_value.rg_center)','RG3 等价于中心除四角均值，但源码实际先乘 4 再除四角和。')],
  [S(BRIGHT,1061,1075,'BG1 = ratio_value.bg_leftup','BG1 左上除右下；右下 BG 未在该条件中检查。')],
  [S(BRIGHT,1061,1075,'BG2 = ratio_value.bg_rightup','BG2 右上除左下。')],
  [S(BRIGHT,1061,1075,'BG3 = (4*ratio_value.bg_center)','BG3 为中心相对四角平均；四角和未检查为非零。')],
  [S(BRIGHT,1124,1159,'if(RG3 >','源码依 RG3、RG1、RG2、BG3、BG1、BG2 的 else if 顺序进行严格越界检查。')],
  [S(BRIGHT,1061,1075,'ratio_value.rg_rightdown !=0','重复检查 RG 左下，未检查 BG 右下及四角和；六项先初始化为 2。')],
  [S(TEST,1863,1887,'append(-1)','前置 Gr 或 Gb 不大于零时向量和五点均置 −1。'),S(BRIGHT,1061,1075,'RG1 = ratio_value.rg_leftup','−1 非零会进入计算，六项均为 1；需检查数据有效性。'),S(TEST,1845,1854,'>10','五点映射的比值大于 10 会变为 −1。')],
  [S(BRIGHT,1167,1176,'push_back','关闭时 g_msg=false；V1.0 只返回一个子项。'),S(BRIGHT,966,976,'g_msg = false','未知版本返回一个 false 且 g_msg=false 的子项。'),S(BRIGHT,54,55,'insert','向量追加到 items_results。'),finalbin],
  [S(BRIGHT,1068,1075,'BG3 =','互动修改右下 BG，重新计算六项。'),S(BRIGHT,1124,1159,'BG1 >','按 else if 顺序严格判断范围。')]],
 'color_shading': [
  [S(BRIGHT,51,52,'cs_shading_res_17x13','调用 Color_Shading，并把返回向量追加到汇总列表。'),S(BRIGHT,695,710,'test_roi_colorratio','计算每块比例，随后取极值并与中心比较。'),S(BRIGHT,734,740,'rg_shading','计算 RG 与 BG 相对中心的最大偏离。')],
  [S('Algorithm_GCXXXX_FT_DT2_STD/FTAlgorithm.cpp',329,345,'ColorShading.data','按顺序尝试载入颜色比例和颜色阴影的版本配置。'),S(BRIGHT,660,665,'Color_Shading_V1_0','按已加载版本分派。'),S(BRIGHT,680,698,'ratio_open_mode_','两个开关均为 1 才进入，并以 shading 配置重新计算 ROI。')],
  [S(TEST,1780,1785,'mean_gr>0','先检查两个 Light_Response 均值。'),S(TEST,1863,1869,'append(-1)','任一均值不大于零，比例向量填 −1。'),S(BRIGHT,720,725,'mean_gr_roi>10','计算极值后，仅检查 Gr 均值是否大于 10。'),S(BRIGHT,785,790,'not test color shading','低 Gr 时只记录 −1，不更新两个初始结果。')],
  [basic,S(TEST,1780,1794,'roi_x_pitch','按配置的列数、行数及整数 pitch 构造 ROI。')],
  [S(RAW,7829,7866,'sum_g_average','源码逐像素求四通道和，再算 RG、BG、GG；画面不播放累加。')],
  [S(TEST,1786,1809,'roi_rg_ratio.push_back','逐行构造 ROI，依次计算每块并保存原始比值。')],
  [S(BRIGHT,712,722,'max_element','RG 极值从向量读取；中心从五点映射读取。')],
  [S(BRIGHT,712,722,'roi_bg_ratio_for_cs_min','BG 极值从向量读取；中心从五点映射读取。')],
  [S(BRIGHT,734,736,'rg_shading =','最大和最小各自除中心、减一、取绝对值后选较大者。')],
  [S(BRIGHT,738,740,'bg_shading =','BG 使用同一套归一化公式。')],
  [S(BRIGHT,762,775,'else if','先判 Class2，再以 else if 判 Class1，严格大于才失败。')],
  [S(TEST,1845,1854,'>10','中心五点映射中大于 10 的比值变为 −1，而原始向量没有此转换。'),S(BRIGHT,712,725,'mean_gr_roi>10','先取向量极值和映射中心，再检查 Gr > 10；未验证 Gb。'),S(BRIGHT,734,740,'/rg_center_for_cs','中心作为分母，此处没有显式的除零保护。')],
  [S(BRIGHT,797,810,'push_back','开关关闭时 g_msg=false；V1.0 依次返回两个子项。'),S(BRIGHT,660,672,'g_msg = false','未知版本返回一个 false 且 g_msg=false 的子项。'),S(BRIGHT,51,52,'insert','返回向量整体追加；最终汇总发生在后面。'),finalbin],
  [S(BRIGHT,712,722,'max_element','互动模拟改变 ROI 后重新查极值。'),S(BRIGHT,734,740,'rg_shading','分别计算 RG、BG 的最大偏离。'),S(BRIGHT,762,775,'else if','使用示例真实配置阈值判断。')]],
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
 'color_ratio': [
  [S(BRIGHT,48,49,'ratio_res_','调用返回结果向量；insert 将范围内的全部结果追加到列表。')],
  [S(ITEM,897,914,'mean_gr_roi','Light_Response 初始化并计算自己的通道 ROI 均值。'),S(TEST,1780,1785,'mean_gr>0','只有两个先前均值都大于 0 才构造 ROI；否则在函数后部写 −1。'),S(TEST,1863,1870,'append(-1)','不满足前置条件时把各 ROI 比值向量写为 −1。')],
  [basic,S(TEST,1780,1794,'roi_x_pitch','先用整数除法计算通道 ROI 宽高，再构造矩形。')],
  [S(TEST,1780,1809,'ROI.append','两重循环逐行构造 ROI，之后逐个计算三种比值。')],
  [S(TEST,1813,1835,'total_roi_cnt/2','从按行排列的向量取中心和四角；当前配置索引为 40、0、8、72、80。'),S(ITEM,2002,2014,'Get_Double_DatasStdandMean','对所有 ROI 的比值向量求统计量。')],
  [S(RAW,7829,7844,'sum_r = 0.1','各通道和从 0.1 开始，逐个累加 ROI 中的四通道像素。')],
  [S(RAW,7829,7849,'sum_g_average','各通道先累加，再将两个绿通道的和求平均。')],
  [S(RAW,7849,7867,'r_g = sum_r/sum_g_average','RG、BG 的分母为绿色平均和；GG 为两绿和之比，另算绿色差。')],
  [S(ITEM,2080,2091,'RG_Ratio_target_offset','按位置选择中心或四角 factor；修正值严格超出上下限才失败。')],
  [S(ITEM,2120,2130,'BG_Ratio_target_offset','BG 也查本位置的目标、偏移和 factor；GG 类似。'),S(ITEM,2056,2067,'GG_Ratio_target_offset','GG 的普通 Class2 比较。')],
  [S(TEST,1813,1835,'total_roi_cnt/2','先从 81 块各自的计算结果选出五点，并非区域两两相减。'),S(ITEM,2080,2091,'RG_Ratio_target_offset','每点修正后分别与该点目标范围比较。')],
  [S(RAW,7829,7849,'sum_g_average','每块 ROI 独立累加四通道并求绿色平均和；画面的四角使用模拟像素。')],
  *[[S(TEST,1813,1835,'total_roi_cnt/2',name+'是选出的五点之一；代码先得到每个区域的比值。'),
      S(RAW,7829,7849,'sum_g_average','源码按行列逐位置累加四通道，每个和从 0.1 开始；画面直接展示由模拟数据得到的和。'),
      S(RAW,7851,7867,'r_g = sum_r/sum_g_average','分别计算 RG、BG 和 GG；此算例没有触发极端比例保护。'),
      S(ITEM,2080,2091,'RG_Ratio_target_offset','RG 修正值和目标范围比较；BG、GG 使用对应类型配置。'),
      S(ITEM,2120,2130,'BG_Ratio_target_offset','BG 使用对应位置的偏移与目标范围。'),
      S(ITEM,2056,2067,'GG_Ratio_target_offset','GG 使用对应位置的偏移与目标范围。')] for name in ('左上','右上','中心','左下','右下')],
  [S(ITEM,2056,2067,'GG_Ratio_target_offset','对所有选点循环，分别根据位置选 factor，并比较 GG 修正值。')],
  [S(ITEM,2080,2091,'RG_Ratio_target_offset','对所有选点循环，分别比较 RG 修正值。')],
  [S(ITEM,2120,2130,'BG_Ratio_target_offset','对所有选点循环，分别比较 BG 修正值；任一点超范围将 ratio 标记失败。')],
  [S(ITEM,2120,2130,'result_ratio.is_passed = false','模拟右下 BG 超上限，ratio 失败。'),S(ITEM,2162,2174,'result_ratio.is_passed','Class1 仅在此时 ratio 通过时执行。'),S(ITEM,2431,2450,'bin_color_ratio_','失败赋 ratio 子项 bin；golden 单独判断。')],
  [S(ITEM,2245,2255,'factor_center_gr_gb_ratio','中心 GG 在普通比较之外还检查单独范围；此语句在 MA 分支之后。')],
  [S(TEST,1857,1861,'gr_gb_diff_center>10','五点绿色差大于 10 会转换为 −1。'),S(ITEM,2269,2275,'gr_gb_diff_spec','随后对转换值与配置阈值比较；此配置阈值为 50。')],
  [S(ITEM,2162,2174,'result_ratio.is_passed','只有普通 Class2 ratio 当时通过才进入 Class1 比较；GG 加严在此之后。')],
  [S(ITEM,1956,1965,'result_golden_BG.is_passed = false','内部两个 golden 状态初始化为 false。'),S(ITEM,2102,2107,'factor_golden','中心 RG 严格落入 golden 范围时设 true，BG 有对应判断。'),S(ITEM,2143,2148,'result_golden_BG.is_passed','中心 BG 同样严格比较。'),S(ITEM,2446,2450,'result_golden_RG.is_passed=false','两者都为 true 时重新将 RG 结果设为 false 并赋 golden bin。')],
  [S(ITEM,2431,2459,'result_ratio_test.push_back','依次返回四个子项，BG 标志不单独返回。'),S(BRIGHT,48,49,'ratio_res_.begin','把整个返回范围追加到测试结果列表。'),finalbin],
  [S(RAW,7849,7867,'r_g = sum_r/sum_g_average','模拟只计算中心 ROI；绿色和均为非零，保留每和 0.1。'),S(ITEM,2080,2107,'factor_golden','观察中心 RG 普通条件及 golden 的严格边界；其他点未模拟。')]],
 'edge_deadrow_deadcol_bright': [
  [S(BRIGHT,44,46,'Edge_Deadrow_Deadcol_bright','白帧调用该子项并保存返回值。')],
  [S(ITEM,5580,5585,'open_mode_','开关为 1 才进入子项。'),S(RAW,7238,7244,'stripCnt/2','条带宽为 0 时，顶部条件不成立；底部条件也不成立，随后 continue。'),S(RAW,7262,7284,'continue','底部条件和 else continue；0 时任何索引均不满足。')],
  [basic,bayer,S(TEST,4525,4543,'w/2,h/2','四个通道按半尺寸分别设置线程并等待。')],
  [S(RAW,7238,7245,'stripCnt/2','顶部按 stripCnt/2 选择，整数除法；该页后续使用假设值。'),S(RAW,7262,7274,'mean_top','底部方向相反：参考上方 r 行。')],
  [S(RAW,7223,7235,'mean_t/(2*N)','逐行遍历像素求均值；本例 w=1920，所以 2*N=1920。')],
  [S(RAW,7242,7255,'mean_down/=cnt_t1','顶部从下一行到 i+r 累加 r 条邻行均值再求平均。')],
  [S(RAW,7254,7260,'dif_down/mean_down','取目标与参考差的绝对值，Ratio 模式除以内侧参考。')],
  [S(RAW,7286,7295,'diff>spec_line','保存最大差位置；严格大于 TA 才增加计数。'),S(ITEM,5685,5699,'deadrow_spec_r_MA_','有计数后，最大差大于或等于 MA 选择对应结果。')],
  [S(RAW,7358,7381,'mean_right','左侧目标向右取参考列，计算同样的比例差。'),S(ITEM,5669,5683,'deadcol_spec_r_MA_','列有计数后，按 MA 判断高低等级。')],
  [S(RAW,7303,7314,'GetDatasStd','读取整条选中行，用其均值计算 DOL，严格大于阈值才计数。'),S(RAW,32,40,'datas.size ()-1','使用传入的均值，平方偏差和除以 N−1 再开平方。')],
  [S(ITEM,5663,5705,'grade_dead_row_>result_edge_line.grade','列先赋值；行先用高等级条件与现有 grade 比较，再选择高低结果；DOL 最后比较。'),finalbin],
  [S(RAW,7262,7284,'continue','StripRow=0 时两边都没有目标，跳过比例与 DOL。'),S(TEST,4583,4602,'delete p_row_defect','delete 后仍读取指针成员写日志，存在释放后读取的未定义行为；列函数也有相同问题。')],
  [S(RAW,7242,7258,'dif_down/mean_down','仅模拟顶部一行的参考与比例。'),S(RAW,7292,7295,'diff>spec_line','严格超过 TA 才计数。'),S(ITEM,5685,5699,'deadrow_spec_r_MA_','MA 判断包括等于；这里不模拟其他缺陷。')]],
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
            # 新增教程空白源码行只显示行号，避免分镜中的行尾空格；不改变非空行原文。
            code = '\n'.join((f'{i:>5}' if page in ('color_shading_15x15', 'color_gi', 'color_gi_5x5', 'blemish_test') and not line else f'{i:>5}  {line}')
                             for i, line in enumerate(extract, first))
            blocks.append({'path': path, 'first': first, 'last': last, 'note': note, 'code': code})
        guides.append(blocks)
    return guides
