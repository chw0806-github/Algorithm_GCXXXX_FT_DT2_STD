# 白帧色彩比例 Color_Ratio（V1.0）

- 动画：`index.html`；无需脚本的静态分镜：`storyboard.html`。顶部显示 `ColorRatio.data`、`Basic.data`、前置 `LightResponse.data` 原文；每幕都有对应源码摘录和解释。
- 本页仅对应 GC8613T `CP_SPEC/param_4l_f_bright1_normal_30fps` 的示例 `ColorRatio.data`，加载器选 V1.0；其他配置及 V1.1 不一定相同。模拟像素、均值和互动均非实测，不执行整图计算或计算最终 bin。
- `Color_Ratio` 返回一个向量；调用处 `insert(..., begin(), end())` 把所有返回子项追加到 `items_results`，不会重新计算。

## 分区与前置条件

先由 `Light_Response` 得到其 ROI 的 Gr、Gb 均值；`test_roi_colorratio` 要求两者都大于零，否则比值向量和选点写 −1。这两个值是前置条件，不是本算法各 ROI 的比值分母。此算例假设前置条件满足。

示例 Basic.data 为 3840×2160 RG Bayer，分通道后 1920×1080；Skip=0，横纵各 9 个 ROI。整数除法得每通道每块 213×120=25560 像素，81 块只覆盖 1917 列，右侧余 3 列不在 ROI 内。81 块均计算 RG/BG/GG 并用于统计日志；五点比较使用左上、右上、中心、左下、右下（从 0 编号分别为 0、8、40、72、80）。

## 中心算例（虚构）

各通道每个数字都出现 1278 次：R 81～100、Gr 180～199、Gb 182～201、B 108～127，各 20 个值。像素总数分别为 25560；均值依次是 90.5、189.5、191.5、117.5。实际算法不要求重复或等差。

源码将各通道求和初始为 **0.1**：R=2313180.1、Gr=4843620.1、Gb=4894740.1、B=3003300.1，绿色平均和为 4869180.1。RG=R/绿色平均和≈0.47507，BG=B/绿色平均和≈0.61680，GG=Gr/Gb≈0.98956；绿色绝对差为 2 DN。此例不触发底层极端比例截断，五点映射中的大于 10 转 −1 也不触发。

中心 RG 加 offset 0.07 后约 0.54507；目标 0.55，Class2=0.2，范围 [0.44,0.66]。中心 BG 加 offset −0.008 后约 0.60880，目标 0.61，范围 [0.488,0.732]；GG 范围 [0.8,1.2]，还有中心独立加严范围 [0.92,1.08]。这些仅是中心条件，四角未构造，不能推断完整 ratio 子项。

## 特别注意源码顺序

1. 普通五点 Class2 先判断 ratio；如果此时 ratio 通过才执行 Class1 / ratio_MA，随后再对中心 GG 作单独加严。本配置 Class1/2 系数中心均 0.2、四角均 0.5，所以正常有限比值情况下 Class1 不会产生额外失败。其他配置可能不同。
2. 五个 `G_G_diff` 大于 10 会先变为 −1，再与配置阈值 50 比较。因此此配置下 V1.0 的 ratio_grid 失败条件不能触发，应请开发人员确认设计意图；不代表绿色差异不存在。
3. 中心 RG、BG 加 offset 后分别严格进入 target 的 ±2% 区间时，将各自内部 golden 标志设为 true。两者都 true 时，源码又将 `result_golden_RG.is_passed` 设为 false，并赋 2222 / grade 0；仅 RG true、BG false 时 RG 标志保持 true。初始为 false，BG 对象不单独返回。不要仅凭字段名称解释为普通 PASS。
4. 返回顺序为 ratio、ratio_MA、ratio_grid、ratio_golden_RG；其它测试项仍参与最终汇总。范围比较用未舍入数值，普通上下限相等不失败，golden 的边界严格不含等号。

## 代码位置

- `TestItems/BrightTest.cpp:48–49`：调用与批量追加。
- `FTAlgorithm.cpp:329–335`、`Params/parammanage.cpp:6650`：配置加载及 V1.0 选择。
- `Params/algorithmitems.cpp:1935–2459`：各结果判定、golden 状态与返回向量。
- `Params/testitems.cpp:1765–1897`：前置条件、9×9 ROI、五点和异常值转换。
- `Params/cpalgorithmraw.cpp:7825–7867`：ROI 四通道求和、三种比例及绿色差。
