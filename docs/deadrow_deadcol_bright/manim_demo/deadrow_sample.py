"""亮场坏行检测：以 GC8613T V1.0 为背景的单条目标行教学样片。

画面数值均为模拟：只演示居中目标行，不推断整幅图像的缺陷计数。
运行方法见同目录 README。使用 Text 而非 LaTeX，依赖系统中文字体。
"""
from manim import *

BG = "#09111f"
INK = "#edf3ff"
DIM = "#a8bad3"
BLUE = "#70aaf5"
YELLOW = "#ffd166"
GREEN = "#58d7ac"
FONT = "Noto Sans CJK SC"


def text(value, size=27, color=INK):
    return Text(value, font=FONT, font_size=size, color=color)


def bar(width, height, fill):
    return Rectangle(width=width, height=height, stroke_width=1,
                     stroke_color=fill, fill_color=fill, fill_opacity=0.38)


class DeadrowSample(Scene):
    """约一分钟的逐步讲解（无内嵌语音；旁白稿单独提供）。"""

    def construct(self):
        self.camera.background_color = BG
        title = text("白帧坏行检测：先看一条目标行", 36)
        title.to_edge(UP, buff=0.30)
        foot = text("GC8613T V1.0 · 所有数值为模拟 · 仅演示一个目标行", 18, DIM)
        foot.to_edge(DOWN, buff=0.18)
        self.add(title, foot)

        # 通道图仅为缩略示意。实际每行 1920 个像素，共 1080 行。
        channel = Rectangle(width=7.2, height=4.05, color=BLUE, fill_color=BLUE,
                            fill_opacity=0.07, stroke_width=2)
        channel.shift(LEFT * 2.1 + DOWN * 0.05)
        info = VGroup(text("R 通道图", 31, BLUE),
                      text("1920 列 × 1080 行", 24),
                      text("横着一行：1920 个像素", 22, DIM)).arrange(DOWN, aligned_edge=LEFT, buff=.23)
        info.next_to(channel, RIGHT, buff=.35)
        target = Rectangle(width=7.05, height=.075, color=YELLOW,
                           fill_color=YELLOW, fill_opacity=1).move_to(channel.get_center())
        self.play(Create(channel), FadeIn(info), run_time=2)
        self.wait(2)
        self.play(FadeIn(target), run_time=1)
        self.wait(2)

        # 两个色块分别代表 960 个像素，不是只含两个像素的简化行。
        self.play(FadeOut(channel), FadeOut(info),
                  target.animate.scale(1.1).move_to(UP * 1.45), run_time=1.5)
        left = bar(5.15, .42, BLUE).shift(LEFT * 2.575 + UP * 1.05)
        right = bar(5.15, .42, GREEN).shift(RIGHT * 2.575 + UP * 1.05)
        pixel_line = VGroup(left, right)
        labels = VGroup(text("前 960 个像素：各 180 DN", 22, BLUE),
                        text("后 960 个像素：各 200 DN", 22, GREEN))
        labels[0].next_to(left, DOWN, buff=.22)
        labels[1].next_to(right, DOWN, buff=.22)
        note = text("两块是压缩示意；一整行实际有 1920 个像素", 21, DIM).next_to(labels, DOWN, buff=.35)
        self.play(ReplacementTransform(target, pixel_line), FadeIn(labels), FadeIn(note), run_time=2)
        self.wait(3)
        calculation = VGroup(
            text("先把像素值全部加起来，再除以像素个数", 25),
            text("(960 × 180 + 960 × 200) ÷ 1920", 28),
            text("目标行均值 = 190 DN", 34, YELLOW),
        ).arrange(DOWN, buff=.25).shift(DOWN * 1.65)
        for line in calculation:
            self.play(FadeIn(line, shift=UP*.12), run_time=1)
            self.wait(1.3)
        self.wait(2)
        self.play(FadeOut(pixel_line), FadeOut(labels), FadeOut(note), FadeOut(calculation))

        # 展示实际 radius=5 的十个邻行；每个框是一个行均值。
        title2 = text("目标行上下各取 5 行，分别求平均", 34).move_to(title)
        self.play(Transform(title, title2), run_time=1)
        rows = VGroup()
        for i in range(11):
            fill = BLUE if i < 5 else YELLOW if i == 5 else GREEN
            rect = bar(4.5, .35, fill)
            label = text("目标行：190" if i == 5 else ("上方行：200" if i < 5 else "下方行：210"), 19)
            label.move_to(rect)
            rows.add(VGroup(rect, label))
        rows.arrange(DOWN, buff=.055).shift(LEFT * 3.35 + DOWN * .05)
        self.play(FadeIn(rows[5]), run_time=1)
        self.wait(1)
        self.play(LaggedStart(*(FadeIn(rows[i]) for i in range(4, -1, -1)), lag_ratio=.16), run_time=2)
        top = text("上方 5 行平均 = 200 DN", 24, BLUE).shift(RIGHT * 2.4 + UP * 1.65)
        self.play(FadeIn(top), run_time=1)
        self.wait(2)
        self.play(LaggedStart(*(FadeIn(rows[i]) for i in range(6, 11)), lag_ratio=.16), run_time=2)
        bottom = text("下方 5 行平均 = 210 DN", 24, GREEN).next_to(top, DOWN, aligned_edge=LEFT, buff=.28)
        self.play(FadeIn(bottom), run_time=1)
        self.wait(2)
        non_target = text("不包含目标行；两侧分开算", 21, DIM).next_to(bottom, DOWN, aligned_edge=LEFT, buff=.32)
        self.play(FadeIn(non_target), run_time=.8)
        self.wait(2)

        # 两侧分别以本侧平均作为分母，取最大值。
        self.play(FadeOut(rows), FadeOut(top), FadeOut(bottom), FadeOut(non_target))
        title3 = text("分别比较两侧，再取较大值", 34).move_to(title)
        self.play(Transform(title, title3), run_time=1)
        equations = VGroup(
            text("上方：|190 − 200| ÷ 200 = 5%", 28, BLUE),
            text("下方：|190 − 210| ÷ 210 ≈ 9.52%", 28, GREEN),
            text("两侧取较大：9.52%", 31, YELLOW),
            text("行阈值 2.5%：9.52% > 2.5%", 28),
            text("该目标行计为异常行", 34, YELLOW),
        ).arrange(DOWN, buff=.34).move_to(DOWN*.05)
        for line in equations:
            self.play(FadeIn(line, shift=UP*.1), run_time=1)
            self.wait(1.7)
        self.wait(3)
        self.play(FadeOut(equations))
        reminder = VGroup(text("实际会逐行、逐通道重复检测", 31),
                          text("本样片没有模拟整幅图像或总异常计数", 23, DIM),
                          text("其他检查：列、DOL、断线及最终判定", 23, DIM)).arrange(DOWN, buff=.4)
        self.play(FadeIn(reminder), run_time=1)
        self.wait(4)
