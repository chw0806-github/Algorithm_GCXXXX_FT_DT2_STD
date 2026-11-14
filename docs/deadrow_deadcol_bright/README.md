# 白帧坏行坏列 Deadrow_Deadcol_bright

- 动画：`index.html`（支持离线打开）；不执行脚本时可阅读 `storyboard.html`。
- 拖动行、列滑杆，模拟背景均值为 200 时目标线的均值变化。演示仅模拟居中孤立线，假设其他通道、DOL 和断线计数为零，并非原始图像处理。

## 代码流程（V1.0）

1. 白帧已拆分为 R、Gr、Gb、B 四通道；行检测、列检测各自启动四个通道线程。
2. 每行/列求均值，再与两侧各 `radius=5` 条邻线的平均值比较。比例模式下计算两侧相对差，取最大值；严格大于 TA 阈值则计数。边缘邻线不足时，该侧使用当前线均值；相邻均值为 0 时差值记为 -1。
3. 另外检查线内像素标准差（DOL），以及半行/半列均值序列的二次差（断线）；超限分别计数。
4. 任一类计数大于 0 即失败。代码依次处理列、行、DOL、断线；后项的 grade 须严格大于当前 grade 才会覆盖结果。还有列、行的低等级 MA 分支，见源码。

## 示例配置

`GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps/LineDefect.data`：`open_mode=1`、`radius=5`、四通道行 TA/MA 阈值均为 0.025，列 TA/MA 均为 0.030。DOL 阈值 R/B 为 80、Gr/Gb 为 90；断线阈值为 10。异常列常见分支 bin 2208 / grade 7；异常行 2210 / 7；DOL 2212 / 6；断线 2213 / 5。在本配置 TA 和 MA 阈值相同时，TA 已触发意味着最大差达到对应 MA 值。配置不同，结果可能不同。

## 源码位置

- `TestItems/BrightTest.cpp`：调用及最终子项汇总。
- `FTAlgorithm.cpp`、`Params/parammanage.cpp`：V1.0 / V1.1 配置加载及版本选择。
- `Params/algorithmitems.cpp`：V1.0 的结果判定、grade 和 bin。
- `Params/testitems.cpp`、`Params/multithread.cpp`：线程及四通道结果汇总。
- `Params/cpalgorithmraw.cpp`：行、列的均值、邻域差、DOL 与断线计算。
