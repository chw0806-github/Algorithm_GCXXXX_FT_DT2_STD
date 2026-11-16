# Single_Pixel_Defect_Test 单点缺陷动画

[动画](index.html) · [静态分镜](storyboard.html)。14 幕包含逐幕源码、实际 Basic.data 原文及假设参数说明；区域、比较条、计数与结果随步骤展示，互动滑杆仅模拟分档，不运行图像检测。

## 真实配置与假设

- 调用见 `TestItems/BrightTest.cpp:157`；同一函数也被其他测试路径调用。它重新调用 `find_pixel_defect`，不是直接取前一个 `Pixel_Defect_Test` 子项的结果。
- 仓库没有 `PD.data`。加载器成功打开后赋 `PD_V1.0`，但后续仍可能解析失败；无法从仓库示例确认版本、开关、类型、检测阈值、分档、bin 或 grade。`PixelDefect.data` 是另一个参数集，不能代替 `PD.data`。
- 本页实际引用 Basic.data 的 RAW 3840×2160、RG Bayer；假设 V1.0、open_mode=1、type=1、radius=5、Gr 主比例阈值 0.30。像素、计数及以下数值都是假设，不是实测或真实配置。
- 假设 WP/DP 分档下限均为 1、3、6；WP bin 为 101/102/103、grade 为 1/3/5；DP bin 为 201/202/203、grade 为 1/3/4。假设 single_pix_type=2、single_pix_limit=8、bin_single_pix=999、grade10=2。这些只是用于解释分支的虚构值。

## 数据流程

1. 配置 type=1 强制 bright，使用各通道比例阈值；type=0 强制 dark，使用各通道 DN 阈值。其他 type 保留传入的 `External_Configuration`，此时具体行为取决于传入枚举值。代码初始比较模式为 Ratio，仅当类型是 dark 时改为 DN。
2. 四通道分别比较内部像素与四块 radius×radius 对角邻域。假设均值 180、190、200、210，目标 245，比例分别约 36.11%、28.95%、22.50%、16.67%；任一严格大于主阈值则标记主图。比值分母为对应邻域均值。主图没命中时才判断 blemish 图；方向差为目标减四块均值的平均，严格大于零时记亮，否则记暗。
3. 主图、blemish 图和方向差合回 Bayer。主图经团簇识别后，按方向遍历计数；然后对亮点数和暗点数分别减去 `cnt_1x2_2x1_pix` 与 `cnt_2x2_diagonal_pix`，负数归零。不能概括为剔除全部团簇后的孤立点总数；请参看源码。
4. 对处理后的 WP 和 DP 分别进行三档判断（下限含等号，上限不含等号；最高档只比较下限）。暗点 grade **严格大于**亮点才选暗；同 grade 选亮。之后 single_pix_type 选择暗、亮或两者和，严格大于上限时直接用 `bin_single_pix/grade10` 覆盖，不检查 grade 是否更高。
5. 假设 WP=3、DP=3：两者 grade 都为 3，选择 WP bin 102；总数 6 不超过 8。假设 WP=6、DP=3：先选 WP bin 103 / grade 5，但总数 9 大于 8，直接覆盖为 bin 999 / grade 2。等于总数上限不覆盖。最终 bin 仍需与其他子项汇总。

## 风险和范围

- `load_Pixel_Defect_Single_Param` 未读取此路径使用的 blemish 阈值与尺寸限制字段；代码仍将其传给检测函数。应核查初始化、其他赋值路径以及实际配置，不能假定它们来自 `PixelDefect.data`。
- 比例模式未单独处理零邻域均值；边界邻域、skip、输入和缓冲区合法性需要结合真实数据核查。本页面只模拟非零均值的内部点，不修改生产代码。
- V1.0 开关关闭时保留初始化通过结果但 `g_msg=false`；未知版本返回 `false` 且 `g_msg=false`。这些都不意味着已经完成有效检测。

构建：`python3 docs/build.py`；分镜使用 `docs/tools/export_story.js` 导出后运行 `python3 docs/build_story.py`。专项检查：`node docs/tools/test_single_pixel_defect.js`。
