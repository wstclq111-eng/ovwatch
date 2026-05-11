# LabVIEW 随机数生成器 VI 作业说明

## 1. 前面板控件

新建一个 VI，在前面板放置以下控件和显示器：

| 类型 | 名称 | 作用 |
| --- | --- | --- |
| Button | 生成随机数 | 每点击一次生成 0~100 的随机数 |
| Button | 清零 | 当前随机数和生成次数清零 |
| Button | 停止 | 结束程序 |
| Numeric Indicator | 当前随机数 | 显示本次生成的随机数 |
| Numeric Indicator | 生成次数 | 显示生成随机数的次数 |
| Waveform Chart | 随机数波形图 | 显示每次生成的随机数变化 |

建议把三个按钮的机械动作设置为 `Latch When Released`，中文界面一般是“释放时锁存”。

## 2. 程序框图总体结构

程序框图使用：

- 一个 `While Loop`
- 一个 `Event Structure`
- 两个移位寄存器：
  - `count`：保存生成次数
  - `value`：保存当前随机数

初始化：

- `count` 左侧移位寄存器接常量 `0`
- `value` 左侧移位寄存器接常量 `0`

## 2.1 控件和函数位置

前面板：

- 按钮：`Controls` -> `Boolean` -> `Push Button`
- 数值显示：`Controls` -> `Numeric` -> `Numeric Indicator`
- 波形图：`Controls` -> `Graph` -> `Waveform Chart`

程序框图：

- While Loop：`Functions` -> `Programming` -> `Structures` -> `While Loop`
- Event Structure：`Functions` -> `Programming` -> `Structures` -> `Event Structure`
- 随机数：`Functions` -> `Programming` -> `Numeric` -> `Random Number (0-1)`
- 乘法：`Functions` -> `Programming` -> `Numeric` -> `Multiply`
- 加法：`Functions` -> `Programming` -> `Numeric` -> `Add`
- 取整：`Functions` -> `Programming` -> `Numeric` -> `Conversion` -> `To Long Integer`，或使用 `Floor`
- 等待：`Functions` -> `Programming` -> `Timing` -> `Wait (ms)`

## 3. While Loop 内部连线

把 `Event Structure` 放进 `While Loop` 中。

在 While Loop 里建议加一个 `Wait (ms)`，常量接 `20` 或 `50`，避免空转占用 CPU。

While Loop 的停止条件由事件结构输出的布尔值控制：

- 默认情况输出 `False`
- 点击“停止”时输出 `True`

## 4. 事件结构设置

### 事件 1：生成随机数 Value Change

事件源选择：`生成随机数`

在该事件分支中：

1. 放置 `Random Number (0-1)` 函数。
2. 将随机数乘以 `101`。
3. 接入 `Floor` 或 `Round To Nearest`，得到整数。
4. 若使用 `Floor`，结果范围是 `0~100`。
5. 将结果连接到：
   - `当前随机数` 显示器
   - `随机数波形图`
   - `value` 右侧移位寄存器
6. 将 `count` 左侧移位寄存器的值加 `1`。
7. 加 1 后连接到：
   - `生成次数` 显示器
   - `count` 右侧移位寄存器
8. While 停止输出接 `False`。

随机数公式：

```text
随机数 = floor(Random Number (0-1) * 101)
```

### 事件 2：清零 Value Change

事件源选择：`清零`

在该事件分支中：

1. 常量 `0` 连接到 `当前随机数`。
2. 常量 `0` 连接到 `生成次数`。
3. 常量 `0` 连接到 `随机数波形图`。
4. 常量 `0` 连接到 `value` 右侧移位寄存器。
5. 常量 `0` 连接到 `count` 右侧移位寄存器。
6. While 停止输出接 `False`。

说明：波形图如果只接 0，会在图上追加一个 0 点。若老师要求图表也完全清空，可以创建 `随机数波形图` 的属性节点，选择 `History Data`，在清零事件里给它写入空数组。

### 事件 3：停止 Value Change

事件源选择：`停止`

在该事件分支中：

1. `当前随机数` 可以继续接 `value` 左侧移位寄存器。
2. `生成次数` 可以继续接 `count` 左侧移位寄存器。
3. `value` 右侧移位寄存器接原 `value`。
4. `count` 右侧移位寄存器接原 `count`。
5. While 停止输出接 `True`。

### 事件 4：Timeout

给事件结构的 Timeout 端子接 `-1`，表示没有事件时一直等待。

在 Timeout 分支中保持数据不变：

1. `当前随机数` 接 `value` 左侧移位寄存器。
2. `生成次数` 接 `count` 左侧移位寄存器。
3. `value` 右侧移位寄存器接原 `value`。
4. `count` 右侧移位寄存器接原 `count`。
5. While 停止输出接 `False`。

## 5. 评分点检查

- 点击“生成随机数”：当前随机数显示 `0~100`，波形图追加该随机数。
- 每点击一次“生成随机数”：生成次数加 `1`。
- 点击“清零”：当前随机数和生成次数变为 `0`。
- 点击“停止”：While Loop 结束，VI 停止运行。
- 程序框图中能看到 `While Loop + Event Structure + 移位寄存器`。
