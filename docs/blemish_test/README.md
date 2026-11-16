# Blemish_Test 教学动画

概览用两路候选流程图，窗口、区域、连通块和互动用图形示意；这些图形不是完整图像处理实现。

- 打开 [动画](index.html) 或无需脚本的 [静态分镜](storyboard.html)；动画和分镜都包含逐幕源码与配置原文。
- 示例配置来自 `GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps/Blemish_V1.1.data` 和同目录 `Basic.data`。加载器先尝试 V1.1 再尝试 V1.0；示例目录没有 `Blemish.data`。文件打开、完整解析及版本应在实际运行环境核查。
- 候选、坐标、灰度与互动输入均为教学假设，不是实测。互动只模拟一个已经提取的普通候选、一个 PD 和一个增强候选的后处理，假设无其他候选和超时；没有实现 OpenCV 图像处理或最终 bin 汇总。
- 普通候选按左上坐标排除邻近 PD；剩余候选匹配增强框并判断其整体外接框；没有增强匹配也返回失败。大于 1000 个普通候选及指定循环中的 300 ms 检查可能提前返回。
- 页面没有修改生产算法，不能取代完整产线检测。特殊输入、零分母、ROI 合法性和开关需结合源码确认。

更新：编辑 `index.src.html`，运行 `python3 docs/build.py`；按 `docs/tools/export_story.js` 中说明导出并量化分镜，运行 `python3 docs/build_story.py`。运行 `node docs/tools/test_blemish_test.js` 及 `docs/tools/test_player.js` 等回归检查。
