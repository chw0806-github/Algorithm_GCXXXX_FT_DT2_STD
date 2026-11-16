# ROI_PRNU 区域像素不均匀动画

[动画](index.html) · [静态分镜](storyboard.html)。14 幕展示通道 ROI、上游 Delta、平方量、原始均值和四通道判定；配有逐幕源码和配置原文。互动仅模拟一个棋盘图案，不执行完整的 C++ 图像检测。

## 配置与前置

- GC8613T 白帧示例 `ROI_PRNU.data`：`X_ROI=768`、`Y_ROI=432`、`W_ROI=384`、`H_ROI=216`，`SpecR/SpecG1/SpecG2/SpecB=3`，`bin=2238`、`grade=3`、`open_mode=1`。示例 `Basic.data` 是 RG Bayer 3840×2160；各通道宽高为 1920×1080。
- ROI 参数直接用于通道图，区域为 X 768～1151、Y 432～647，384×216=82944 像素/通道。函数要求 `line+win_h<height` 且 `col+win_w<width`，恰好贴下边或右边也会返回 −1。
- 调用进入条件是 ROI_PRNU 开关为 1，并且 Pixel_Defect 或 Single_Pixel_Defect 的开关至少一个为 1；之后仅当 `mean_gr_roi>1` 才执行 ROI 计算。该均值由此前 Light_Response 提供。示例 `PixelDefect.data` 开启、半径为 5；仓库未找到 Single 的 `PD.data`，不能断言其开关或版本。文件打开成功后 loader 即赋 ROI_PRNU 版本，后续解析仍可能失败；实际加载状态需核查。

## 计算流程

1. 白帧示例调用链先分配 PRNU 缓冲，再调用 PKG、Blemish、Pixel_Defect、Single_Pixel_Defect，最后调用 ROI_PRNU。前面的多个路径可能写共享 Delta，最终哪个路径写入以及哪些位置有效，取决于版本、配置、开关及调用是否真正执行。开关条件并不验证缓冲是否为当前图像的有效结果。
2. 像素缺陷 V1.0 的内部位置 `Delta = 当前通道像素 − 四块同通道邻域均值的平均`。这不是当前像素减 ROI 均值。教程假设 Pixel_Defect 的 radius=5 产生并最后保留 ROI 内的 Delta；实际 Single 检测若启用可能再次覆盖。
3. 每通道独立算 `sigma = sqrt(sum(Delta²)/(N−1))`，原始像素 `mean = sum(raw)/N`，`PRNU = sigma/mean×100`。源码**没有先减 Delta 的平均值**，因此不能把 sigma 一概称为 Delta 的中心化样本标准差。例如所有 Delta 都为 5 时，该 sigma 不会为 0，而通常中心化的标准差为 0。
4. 任意通道严格大于对应 Spec 才令本子项失败；示例超限 bin 2238/grade 3。边界等于 3 不失败，判定使用未舍入数。该子项并非最终设备 bin。

## 虚构棋盘示例

假设整个 R 通道交替为 190/210 DN，ROI 内各 41472 个；邻域半径 5，四块各有 13 个与目标同值、12 个不同值。因此目标 190 的四块均值都是 199.6、Delta 约 −9.6，目标 210 的四块均值都是 200.4、Delta 约 +9.6。假设其他通道全为 200、Delta 为 0。

R 原始均值为 200 DN；按源码 float Delta 的平方与 N−1 分母计算，sigma≈9.60006 DN，PRNU≈4.80003%，高于 3%，于是模拟子项失败。滑杆的 A 表示棋盘的 200±A；A=5 时约 2.4%，A=10 时约 4.8%。像素、Delta 与其他通道结果均为教学假设，不是实测。

## 无效数据与边界

- ROI 越界（包括右下贴边）返回 −1；负坐标、非正尺寸、N≤1 没有完整防护，使用前需验证。原始均值为零时没有单独的零分母保护，可能产生 Inf 或 NaN。
- Gr 前置不满足时四值保持 −1；在正上限下 −1 不会超限。NaN 普通 `>` 比较为 false。无效输入不能被理解为有效通过。
- 不满足开关条件时 `g_msg=false`；未知版本返回 false 且 `g_msg=false`。这里没有修改生产算法。

构建：`python3 docs/build.py`；使用 `docs/tools/export_story.js` 导出分镜后运行 `python3 docs/build_story.py`。专项测试：`node docs/tools/test_roi_prnu.js`。
