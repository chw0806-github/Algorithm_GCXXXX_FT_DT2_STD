# Color_GI（V1.0）

- [动画](index.html) · [静态分镜](storyboard.html)：14 幕分阶段说明区域、GG 比例、极值、GI 和判定。画面不逐像素播放累加；像素与互动均为虚构，不是实测。
- 页面顶部展示 GC8613T `param_4l_f_bright1_normal_30fps` 目录中的 `GI.data`、`Basic.data`、`ColorRatio.data`、`LightResponse.data` 原文。此目录 GI.data 有 Skip=0、9×9、GI_Ratio=0.08、ratio_open_mode=1、bin_GI=157、grade_GI=3。示例目录不含 GI_V1.1.data；在此配置下加载 V1.0。其他目录可能加载 V1.1，不能套用这里的判定阈值。

## 流程

1. 白帧调用 `Color_GI(test_manage)`；版本 V1.0、V1.1 分派到不同实现。V1.0 检查的是 GI 自己的 `ratio_open_mode`，不检查 Color_Ratio 的开关。此前 Light_Response 求得的 `mean_gr_roi`、`mean_gb_roi` 被传给共用 ROI 函数；不要求 Light_Response 子项本身通过。
2. Basic.data 的 RAW 为 3840×2160 RG Bayer，分通道为 1920×1080。GI.data 以 Skip=0、9×9 整数分区，每块 213×120=25560 个通道像素，共 81 块；右侧 3 列不在 ROI 内。
3. 两个前置均值都大于零时，对每块计算 GG：四通道和均从 0.1 开始，`GG=abs(Gr和/Gb和)`；`Gb和×100 < Gr和` 时底层设置 GG=100。其他通道也在共用函数中处理，但 GI 的极值和公式只用 GG。
4. 从全部区域的 **原始 GG 向量**取最大和最小，再检查 `mean_gr_roi>10`。满足时 `GI=max(abs(GG最大−1), abs(GG最小−1))`。它不是 Gr/Gb 的 DN 差，也不是最大减最小，不仅检查四角或中心。
5. 示例 V1.0 仅在 `GI > 0.08` 时失败；等于边界不失败。超过上限赋子项 157 / grade 3。实际比较使用未舍入值。V1.1 的 Class1 / Class2 是另一种判定：`GI > class1 && GI < class2`，否则 `GI >= class2`；没有在此示例中使用。

## 模拟数据

所有 81 块每通道内像素固定：Gb=200、R=100、B=120；ROI 10 的 Gr=190，ROI 70 的 Gr=220，其余 79 块 Gr=200。按行优先从 0 编号，两处异常只是为教学人为构造。ROI 10 的 Gr 和为 4856400.1，Gb 和为 5112000.1，GG≈0.95；ROI 70 的 GG≈1.10，其他区域为 1。GI≈0.10，超过示例 0.08，模拟子项失败 157 / 3。

滑杆只在最后一幕改变 ROI 70 的 Gr，前置均值假设有效且独立于滑杆。当 Gr=216 时显示 GI≈0.080000，但由于和各自从 0.1 开始，未舍入的 GI 略小于 0.08，严格比较不超限；不要拿屏幕显示的六位小数代替实际判断。

## 异常和返回

- `mean_gr_roi<=0` 或 `mean_gb_roi<=0` 时，共用函数将 81 个原始 GG 写为 −1；计算尺寸和极值发生在 GI 额外检查之前。Gr≤10 时 GI 不再比较，日志写 −1，返回的子项仍保持初始化通过、bin 1 / grade 0、g_msg=true；**这不证明有效测量**。
- 若 Gr>10 但 Gb≤0，GG 向量仍为 −1，而仅检查 Gr 的 GI 分支会计算 GI=2；在示例阈值下失败。这是异常数据路径，不能当作实际绿色不平衡测量。
- 共用函数另生成五点映射，对映射中大于 10 的比例替换为 −1；GI 使用的是原始 GG 向量，不应用该替换。ColorRatio 的五点 target 参与映射键，offset 和 target 不参与此 GI 极值与阈值公式。
- 区域数或尺寸不合法可能产生除零、空向量极值解引用或无效索引；代码未在此入口完整保护。需要核实配置及输入有效性。
- 开关关闭时 V1.0 将 `g_msg=false`；未知版本返回 false 且 `g_msg=false`。V1.0 返回一个 GI 子项；调用方 `insert` 追加整个向量，最终 bin 仍需结合其他子项。`Color_GI_5_5` 和 Color_Ratio 内的 GI 子项与此函数不同。

源码见 `TestItems/BrightTest.cpp:291–413`、`Params/testitems.cpp:1765–1889`、`Params/cpalgorithmraw.cpp:7825–7867`、`Params/parammanage.cpp:6221–6339` 与 `FTAlgorithm.cpp:358–364`。
