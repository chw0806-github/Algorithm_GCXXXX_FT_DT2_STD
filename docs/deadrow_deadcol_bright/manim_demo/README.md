# Manim 样片：白帧坏行检测

本样片只讲 **一个居中目标行**：R 通道与真实配置尺寸、行内 1920 像素的平均、上下各 5 个邻行、两侧比例差及行阈值。教学数值均为模拟，不是实测；这不是完整算法动画，也没有模拟全图计数。`旁白稿.md` 是待录制的讲解文本，脚本目前不会生成或自动配入语音。

算例：行内 960 个 180 DN 和 960 个 200 DN，均值 190 DN；上方 5 行均值各 200 DN，下方 5 行均值各 210 DN。两侧比例差分别为 5% 和约 9.52%，取大值；示例行阈值 0.025（2.5%），严格大于才计数。

## 安装并渲染（Ubuntu / Debian 示例）

需要 Python 3.11、FFmpeg、Cairo/Pango 开发包和支持中文的字体；不同系统包名可能不同：

```bash
sudo apt-get update
sudo apt-get install -y python3-venv python3-dev build-essential pkg-config libcairo2-dev libpango1.0-dev ffmpeg fonts-noto-cjk
python3 -m venv .venv
. .venv/bin/activate
python -m pip install 'manim==0.19.0'
python -m manim -pqh --media_dir /tmp/manim-deadrow docs/deadrow_deadcol_bright/manim_demo/deadrow_sample.py DeadrowSample
```

运行命令应从仓库根目录执行。`-pqh` 为高质量并尝试自动播放；不需要自动打开时用 `-qh`。输出在 `/tmp/manim-deadrow/videos/` 下，具体目录由 Manim 生成。使用 `-ql` 可较快预览。字体名 `Noto Sans CJK SC` 不存在时，可在脚本顶部修改 `FONT` 为本机中文字体。导出 MP4 不会自动配音；可依据旁白稿后期配音并调整节奏。

本工作环境尝试安装 Manim 时因缺少系统 Cairo/Pango 开发依赖而失败，无法在此渲染验证；本脚本仅完成 Python 语法检查，动画布局和实际视频输出仍需在有依赖的环境中验证。
