# 白帧像素缺陷 Pixel_Defect_Test（V1.0）

动画页和静态分镜页顶部均展示本示例对应的 `.data` 与 `Basic.data` 原文（框内可滚动）；静态分镜每幕也展示对应源码摘录。配置为 GC8613T 白帧示例，不等于实测数据。

- 动画：`index.html`；静态分镜：`storyboard.html`。
- 14 幕从目标、四块 5×5 邻域、均值和四次比较，一步步讲到标记图和后续统计；每幕同步显示对应的真实源码。
- 拖动滑杆，模拟一个位于内部的 Gr 点。四块各有 25 个不同的模拟值，均值依次为 180、190、200、210 DN；演示不运行真实全图、团簇识别或最终 bin 汇总。
- 默认目标 245 DN，四次差异约为 36.11%、28.95%、22.50%、16.67%；左上超过 30%，主图标记。方向差为 245 − 195 = +50 DN。预设 234 则在左上刚好等于 30%，不进入主图。

## 处理流程

1. 白帧一条测试分支调用 `Pixel_Defect_Test(test_manage, External_Configuration)`。示例 `PixelDefect.data` 对应 V1.0、`open_mode=1`、`type=1`，即按白帧比例模式处理。V1.1～V1.4 的算法与配置不同。
2. 四个通道独立处理。对每个像素，以四个 `radius×radius` 邻域（分别在左上、右上、左下、右下）计算均值。配置 radius 为 5；四块都不包含目标点所在行和列。
3. 分别计算 `abs(像素 − 邻域均值) / 邻域均值`；任一块严格大于 0.30 时写入主缺陷图。若未进入主缺陷图，则比较 blemish 阈值并可能写入另一张候选图；Gr 示例阈值为 0.12。代码没有对该比例计算单独设置零分母保护；本演示假设邻域均值非零。
4. 四通道结果映射回 Bayer 坐标，识别不同形状的团簇，统计亮暗单点和 blemish。亮暗方向由目标值减去四个邻域均值的平均值确定。团簇和 blemish 计数可能改变 bin；动画没有实现这些检测。
5. 假设无团簇和其他点、blemish 不超限：无主缺陷点时子项 bin 2001 / grade 0；一个亮点时 2002 / 1；一个暗点时 2006 / 1。这只是 V1.0 众多分支中的简化例子。

**注意子项和最终结果不同。** V1.0 将 `is_passed` 初始化为 `false` 且本函数没有将其改为 `true`。最终汇总仅在 `!is_passed && grade > final_result.grade` 时覆盖初始结果，因此子项 grade 0 不覆盖初始最终结果；实际最终 bin 还受其他子项影响。

## 源码与配置

- `Algorithm_GCXXXX_FT_DT2_STD/TestItems/BrightTest.cpp`：白帧调用。
- `Algorithm_GCXXXX_FT_DT2_STD/FTAlgorithm.cpp`、`Params/parammanage.cpp`：参数加载和版本。
- `Algorithm_GCXXXX_FT_DT2_STD/Params/algorithmitems.cpp`：V1.0 的分类分支及最终汇总。
- `Algorithm_GCXXXX_FT_DT2_STD/Params/testitems.cpp`、`Params/multithread.cpp`：四通道处理、Bayer 合图、团簇和计数。
- `Algorithm_GCXXXX_FT_DT2_STD/Params/cpalgorithmraw.cpp`：`bychannel_pixel_MapBinarization` 的四邻域比例与候选图。
- `STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps/PixelDefect.data`：示例配置。
