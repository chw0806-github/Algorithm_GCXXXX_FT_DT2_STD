# Light_Response 动画讲解

动画页和静态分镜页顶部均展示本示例对应的 `.data` 与 `Basic.data` 原文（框内可滚动）；静态分镜每幕也展示对应源码摘录。配置为 GC8613T 白帧示例，不等于实测数据。

面向初学者的交互式动画，讲清楚这一行代码：

```cpp
items_results.push_back(Algorithm_all[site_].Light_Response(test_manage));
```

- 动画：`docs/light_response/index.html`（单文件、零依赖、可离线；源文件为同目录 `index.src.html`）
- 静态分镜：`docs/light_response/storyboard.html`（**不执行脚本也能看**：8 幕各一张定格图 + 字幕 + 讲解）
- 均值画面先定位中心区域，再展示公式与四通道模拟均值；不播放逐像素累加。滑杆值是模拟结果，非实测。
- 讲解：本文件（算法流程、判定树、真实配置、常见坑）
- 目录：`../index.html`；相对照度对照页：`../relative_illumination/index.html`

---

## 怎么用（使用说明）

### 打开

| 方式 | 操作 | 适用 |
|---|---|---|
| 直接双击 | 用 Chrome / Edge / Firefox 打开 `docs/light_response/index.html` | 自己看，最快；不联网也能跑 |
| 本地起服务 | 仓库根目录执行 `python3 -m http.server 8000 --directory docs/light_response`，访问 `http://localhost:8000/` | 手机 / 同网段同事一起看（Windows 用 `py -m http.server`） |
| 在线预览 | 打开会话里的 LIVE PREVIEW | 在当前环境里直接点开 |

### 界面分区

```
┌───────────────────────────────────────────────┬──────────────────┐
│  动画舞台（Canvas）：顶部是步骤号+标题         │  8 个章节按钮     │
│  中间是当前这一幕的动画                        ├──────────────────┤
│                                               │  本章讲解        │
│  画布下方：字幕条（这一步在干什么）            │  + 对应 C++ 片段 │
├───────────────────────────────────────────────┤                  │
│  ◀上一节  ⏸暂停  下一节▶  ↺重播  进度  0.5/1/2×│                  │
├───────────────────────────────────────────────┤                  │
│  模拟测量值：R / Gr / Gb / B 四条滑杆 + 预设   │                  │
└───────────────────────────────────────────────┴──────────────────┘
```

### 操作

| 操作 | 效果 |
|---|---|
| `空格` / 点「播放·暂停」 | 播放或暂停当前幕 |
| `←` `→` / 「上一节·下一节」 | 切换章节 |
| 点右侧章节按钮 | 直接跳到某一幕（01~08） |
| `0.5×` `1×` `2×` | 播放速度 |
| 拖 R / Gr / Gb / B 滑杆 | 修改四个通道的 ROI 均值，**第 06、07、08 幕实时重算**（表盘位置、PASS/FAIL、bin 都跟着变） |
| 预设按钮 | 一键切换：正常片 / 偏暗 / 极暗<50 / 偏亮 / 过曝>241 / 只有 R 超限 |

### 打开后只显示「动画没有启动」怎么办

动画可能因为脚本运行出错、浏览器不支持 Canvas，或阅读器屏蔽脚本而无法启动。
如果页面显示错误信息，请将其反馈给维护者。同一份内容有两条不依赖脚本的路：

| 想看什么 | 打开 | 说明 |
|---|---|---|
| 每幕的定格画面 + 字幕 + 讲解 | `storyboard.html` | 纯 HTML + CSS，图片 base64 内嵌，零脚本、零外部请求 |
| 纯文字版流程 / 判定树 / 配置 | `README.md`（本文件） | 最省事，复制粘贴也方便 |

`index.html` 的页头也有一个「看不了动画？→ 静态分镜」的链接，点它即可。
静态分镜与动画共用同一份字幕/讲解数据，两者说法一致。

### 三种典型用法

1. **自己 5 分钟入门**：看 01（定位）→ 02（拆代码）→ 05~07（ROI、均值、判定）。03、04 是背景知识，可以后看。
2. **给新人讲**：按 01→08 顺序，每幕暂停后指着画布讲；到 07 幕把某个通道拉过 220，让新人自己说出 bin 应该是多少（2201），再拉过 241 看它变成 162。
3. **现场排障对照**：遇到"图像很亮却报 bin 161"，打开 05 幕确认 ROI 量的是 **1920×1080 平面坐标**，再对照 `LightResponse.data` 的 `ROI_X/Y/W/H`；遇到"这项没卡住"，看 08 幕的 `g_msg=false` 说明。

> 动画里的像素值、Σ/N、均值都是示意数据，用来讲清"怎么算、怎么判"，不代表产线实测。

---

## 一句话理解

> 让 **第 `site_` 号工位** 的算法对象，用 `test_manage` 里那套参数，跑一遍「光照响应」子项；
> 得到一张 `Subitem_Result` 体检单，**追加**到本次 Bright Test 的结果清单里，等着最后汇总。

它不是"判一颗芯片好不好"，而是"往清单里放一张体检单"。

---

## 逐段拆解

| 片段 | 含义 | 关键点 |
|---|---|---|
| `items_results` | `std::vector<Subitem_Result>` | 在 `BrightTest::TestFunc_Normal_Bright_ALL()` 开头创建，装整帧图所有子项的结论 |
| `.push_back(...)` | 追加到清单末尾 | 返回 `void`，**这一行本身没有返回值** |
| `Algorithm_all[site_]` | 全局数组下标取对象 | `AlgorithmItems Algorithm_all[Parallel_num]`，`Parallel_num = 4`；每个工位一份独立缓冲 |
| `.Light_Response(test_manage)` | 真正干活 | 参数全在 `ParamManage` 里：宽高、Bayer、ROI、上下限、bin、grade |
| 返回值 `Subitem_Result` | `{name, is_passed, bin, grade, g_msg}` | `g_msg = false` 表示"不表态"，汇总时会被跳过 |

代码位置：

- `Algorithm_GCXXXX_FT_DT2_STD/TestItems/BrightTest.cpp:120`
- `Algorithm_GCXXXX_FT_DT2_STD/Params/algorithmitems.cpp:880`（`Light_Response` 版本分发）、`:890`（`Light_Response_V1_0`）
- `Algorithm_GCXXXX_FT_DT2_STD/Params/testitems.cpp:740`（`test_roi_channel_mean`）
- `Algorithm_GCXXXX_FT_DT2_STD/Params/cpalgorithmraw.cpp:7949`（`GetROIMean`）
- `Algorithm_GCXXXX_FT_DT2_STD/Params/algorithmitems.cpp:5903`（`Calculate_The_Final_Bin`）

---

## 算法流程（LightResponse_V1.0）

```
RAW 白帧 (W×H, Bayer)
      │  Split_Channel —— 每 2×2 一个周期
      ▼
R / Gr / Gb / B 四个平面（各 W/2 × H/2）
      │  Light_Response
      ▼
① response_open_mode_ == 1 ?          否 → g_msg = false，本子项不参与汇总
② 版本 == "LightResponse_V1.0" ?      否 → 同上
③ 在四个平面上各取同一 ROI        test_roi_channel_mean()
④ 每通道求均值                    mean = Σ DN / N
⑤ 四组均值逐一比对 Min / Max
⑥ 越界 → is_passed = false，再按 error_MIN / error_MAX / bin_response 定 bin
      ▼
Subitem_Result → push_back 进 items_results
```

### 均值公式

```
mean_ch = ( Σ_{ROI 内像素} DN ) / N        ch ∈ {R, Gr, Gb, B}
```

四个通道各起一个线程并行算（`MultiThread::get_roi_channel_mean`）。

### 判定树（顺序很重要）

```
任一通道越界（> Max 或 < Min）
├─ error_min_open && 任一通道 < error_MIN(50)   → bin = 161, grade = 9   // 死黑 / 不响应
├─ error_max_open && 任一通道 > error_MAX(241)  → bin = 162, grade = 9   // 饱和 / 过曝
└─ 否则                                         → bin = bin_response, grade = grade_response
```

全部在范围内：`is_passed` 保持 `true`，`bin/grade` 保持构造时的初值 `1 / 0`（良品）。

### 最终 bin

`Calculate_The_Final_Bin()` 遍历整张清单：

- `g_msg == false` → 跳过；
- 只保留 **`grade` 更大** 的失败项（同级取先出现的）；
- 初始值 `{"The initial value", true, 1, 0, true}`，所以全通过时返回 `bin = 1`。

`BrightTest` 最后把这个 `bin` 直接 `return` 给机台做分选。

---

## 本例用的真实配置

`STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps/LightResponse.data`
（`Basic.data`: `width=3840, height=2160, bayer=RG`）

| 参数 | 值 | 说明 |
|---|---|---|
| MinMeanR / MaxMeanR | 80 / 220 | R 通道上下限 |
| MinMeanGr / MaxMeanGr | 144 / 216 | Gr 通道上下限 |
| MinMeanGb / MaxMeanGb | 144 / 216 | Gb 通道上下限 |
| MinMeanB / MaxMeanB | 80 / 220 | B 通道上下限 |
| ROI_X / Y / W / H | 768 / 432 / 384 / 216 | **平面坐标**（1920×1080 上） |
| bin_response / grade_response | 2201 / 9 | 越界但未触发 161/162 时的 bin |
| error_MIN / error_MAX | 50 / 241 | <50 → 161；>241 → 162 |
| response_open_mode | 1 | 0 则不测（`g_msg = false`） |

---

## 初学者最容易踩的 6 个坑

1. **ROI 是平面坐标，不是 RAW 坐标。** `ROI_X=768` 是在 1920 宽的平面上量的，写 `1536` 之类按 3840 换算的值就会越界。
2. **越界直接返回 -1。** `GetROIMean()` 里 `if (line+win_h >= height || col+win_w >= width) return -1;`，均值 -1 必然判 Fail，通常直接落 **bin 161**。看到"图像很亮却报 161"，先查 ROI。
3. **`g_msg = false` 不等于通过。** `open_mode ≠ 1` 或版本号不匹配时返回 `g_msg=false`，汇总时被跳过——现象是"这项没卡住"。
4. **分支顺序决定 bin。** 先判 `error_min` 再判 `error_max`；两端同时超时只会落 161。
5. **`site_` 必须和机台通道对应。** 4 个工位共用一套代码、各自的 `g_buf_*`，`site_` 传错就会拿别的工位的图来算。
6. **这一行没有返回值。** `push_back` 返回 `void`；返回 `Subitem_Result` 的是 `Light_Response()`。调试时别指望 `push_back` 的"返回结果"。

---

## 动画分镜 ↔ 代码

| 幕 | 主题 | 对应实现 |
|---|---|---|
| 01 | 总览：这一行在流水线的哪一站 | `BrightTest::TestFunc_Normal_Bright_ALL()` |
| 02 | 拆解：一行代码 5 个部件 | `Subitem_Result` 结构体 |
| 03 | 多工位：`Algorithm_all[site_]` | `AlgorithmItems Algorithm_all[Parallel_num]` |
| 04 | 分通道：Bayer → 4 个平面 | `TestItem::split_channel()` |
| 05 | 取 ROI：中心区域的理由 | `Param_Light_Response` + `GetROIMean()` |
| 06 | 算均值：Σ DN / N | `test_roi_channel_mean()` |
| 07 | 判 Spec：161 / 162 / 2201 | `Light_Response_V1_0()` |
| 08 | 归档：清单与最终 bin | `Calculate_The_Final_Bin()` |

动画里的像素值、Σ/N 数字是示意，用于讲清楚"怎么算、怎么判"，不代表产线实测数据。
