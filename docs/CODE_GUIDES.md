# 动画中的源码讲解

七个动画页面的右侧「本幕对应源码」会随章节自动切换，显示仓库中的原文摘录、路径、行号与中文说明。较长代码可以在代码框内滚动。数字模拟和简化动画不代表源码里的固定数据；标记为 `.data` 的摘录是示例配置，不是 C++。

映射定义在 `docs/code_guides.py`。构建时读取仓库对应文件的指定连续行并内联到离线 HTML：

```bash
python3 docs/build.py
python3 docs/tools/test_code_guides.py
node docs/tools/test_player.js
python3 docs/build.py --check
```

每个范围包含锚点校验，章节名与顺序也会校验；若源码或章节改变，请重新核对 `code_guides.py` 里的范围、锚点和解释再构建。锚点不保证语义一定正确，仍需人工对照源码审查。静态分镜每幕也显示相同的源码摘录、行号和中文解释；无需脚本。构建时校验分镜章节与映射顺序。

七个动画和分镜页的顶部均显示 GC8613T 白帧示例的对应 `.data` 和 `Basic.data` 原文；Color_Ratio 额外展示作为前置条件的 `LightResponse.data`。配置文件路径和重点字段解释在 `docs/config_guides.py` 中维护，字段值和全文在构建时读取仓库原文件；原文框可滚动。不要把这些示例配置或动画模拟值误认为任意产线实测。更新配置后同时构建动画和分镜：

```bash
python3 docs/build.py
python3 docs/build_story.py
python3 docs/tools/test_config_story.py
python3 docs/build.py --check
python3 docs/build_story.py --check
```
