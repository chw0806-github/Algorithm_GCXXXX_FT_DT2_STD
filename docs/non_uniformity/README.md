# 白帧不均匀性 Non_Uniformity

- 动画：`index.html`（双击或通过本地服务打开）
- 静态分镜：`storyboard.html`（无脚本也可阅读）
- 拖动滑杆可改变示意网格左上格的百分比，观察平均值、最大值、左右及上下差异与判定的变化。

## 流程

1. 白帧测试先拆出 R、Gr、Gb、B；四个通道各自以完整格子处理。
2. 每个格子计算像素的均值和总体标准差，局部不均匀性 = 标准差 / 均值 × 100。均值不大于 0.001 时分母取 0.001。
3. 对各格的百分比求平均和最大值；将每格与其左右、上下镜像格的百分比作差，分别取最大绝对差。
4. 四项指标依次和各通道配置的上限比较，任一严格大于上限即失败。后续失败项只有 grade 严格更高才能覆盖当前 bin。

示例配置 `GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps/NonUniformity.data`：RAW 格 96×54（通道图上为 48×27），各通道平均上限 5、最大上限 30、左右差和上下差上限各 50；对应 bin 2203、2204、2205、2206，grade 均为 6。其他配置可能不同。

动画的 4×3 网格及百分比均为教学示意，只模拟一个通道、假设其他通道通过。格子数量和数值不是实际测量。若版本不匹配或该项未开启，处理路径不同；最终 bin 还要在测试项之间汇总。

## 源码位置

- `Algorithm_GCXXXX_FT_DT2_STD/TestItems/BrightTest.cpp`：白帧调用。
- `Algorithm_GCXXXX_FT_DT2_STD/Params/algorithmitems.cpp`：版本选择、配置开关、四类判定和 bin。
- `Algorithm_GCXXXX_FT_DT2_STD/Params/testitems.cpp`：四通道线程与尺寸。
- `Algorithm_GCXXXX_FT_DT2_STD/Params/multithread.cpp`：各通道调用底层计算。
- `Algorithm_GCXXXX_FT_DT2_STD/Params/cpalgorithmraw.cpp`：格内均值、标准差和镜像差计算。
