# Relative_Illumination 动画讲解

与 [Light_Response](../light_response/index.html) 成对的第二个例子：**同一个白帧，换一个角度看**。

- 动画：`index.html`（7 幕，单文件可直接打开；源文件为同目录 `index.src.html`）
- 目录页：`../index.html`
- 怎么再加别的算法：`../HOWTO.md`

---

## 一句话理解

> 在 **RAW 全图** 上取 **四角 + 中心共 5 个窗口**，用「四角平均 ÷ 中心」算出边缘衰减百分比；
> 落在 `20% ~ 100%` 就算通过，否则 `bin = 2202`、`grade = 8`。

```cpp
items_results.push_back(Algorithm_all[site_].Relative_Illumination(test_manage));
```

---

## 和 Light_Response 的差别

| | Light_Response | Relative_Illumination |
|---|---|---|
| 读哪块缓冲 | `g_buf_r_ / gr_ / gb_ / b_`（拆分后的 4 个平面） | `g_buf_raw_`（RAW 全图，不拆通道） |
| 窗口 | 每通道 1 个中心 ROI（`ROI_X/Y/W/H`） | 四角 4 个 + 中心 1 个（`w_win / h_win`） |
| 坐标体系 | **平面坐标**（1920×1080） | **RAW 坐标**（3840×2160） |
| 指标 | 绝对亮度（DN） | 相对衰减（%） |
| 判据 | 四通道各自 80~220 等 | 单值 20% ~ 100% |
| 失败 bin | 2201（配置）/ 161（<50）/ 162（>241） | 2202（配置，grade 8） |
| 防呆 | ROI 越界返回 -1 | 中心 < darkest/10 → 1000；> darkest×10 → 10 |

> **最容易混的就是坐标体系**：同一个数字放进另一个子项里，量到的是不同尺寸的图。

---

## 算法流程（`cpalgorithmraw.cpp:1576`）

```
RAW (3840×2160)
   ├── C1 左上 [0..h_win) × [0..w_win)
   ├── C2 左下 [h-h_win..h) × [0..w_win)
   ├── C3 右上 [0..h_win) × [w-w_win..w)
   ├── C4 右下 [h-h_win..h) × [w-w_win..w)
   └── Center   [h/2-h_win/2 .. +h_win) × [w/2-w_win/2 .. +w_win)

每个窗口： mean = Σ DN / N      (N = 96×54 = 5184)

darkest = (C1 + C2 + C3 + C4) / 4

if (center < 0.001)          center = 0.001       // 防除零
if (center < darkest/10)     return 1000;         // 中心几乎全黑
else if (center > darkest*10) return 10;          // 中心远亮于边缘
else                          return 100 * darkest / center;
```

判定：

```
ratio < limit_min_ratio_(20)  或  ratio > limit_max_ratio_(100)
    → is_passed = false
      bin   = param.bin_      (2202)
      grade = param.grade_    (8)
```

---

## 本例用的真实配置

`STD_Tester_GCXXXX_XXX_DTSTD_XX_20260817_V1.4.1.23/GC8613T/CP_SPEC/param_4l_f_bright1_normal_30fps/RelativeIllumination.data`

| 参数 | 值 |
|---|---|
| w_win / h_win | 96 / 54（RAW 像素） |
| min_ratio / max_ratio | 20 / 100（%） |
| bin / grade | 2202 / 8 |
| open_mode | 1 |

---

## 几个容易踩的坑

1. **ratio 恰好 = 1000 或 10**：不是测量不准，是命中了防呆分支（中心全黑 / 中心远亮于边缘）。去查图像本身。
2. **ratio > 100%**：边缘比中心亮，通常意味着中心被遮挡、光源偏了，或 `w_win/h_win` 配错。
3. **窗口落在无效区**：RAW 若含 OB / dummy 行，角上取到的不是有效像素，比值会失真。
4. **两个子项都失败时**：`grade` 大的赢（Light_Response 9 > Relative_Illumination 8），所以最终 bin 可能是 2201，但相对照度确实也挂了——要看完整 log。
