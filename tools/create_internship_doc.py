from pathlib import Path
import sys


TITLE = "ovwatch 智能手表项目：五月中旬实习冲刺计划"


def add_heading(doc, text, level):
    p = doc.add_heading(text, level=level)
    return p


def add_bullets(doc, items):
    for item in items:
        doc.add_paragraph(item, style="List Bullet")


def add_numbered(doc, items):
    for item in items:
        doc.add_paragraph(item, style="List Number")


def build_docx(output_path):
    from docx import Document
    from docx.shared import Pt, Cm
    from docx.enum.text import WD_ALIGN_PARAGRAPH
    from docx.oxml.ns import qn

    doc = Document()
    section = doc.sections[0]
    section.top_margin = Cm(1.8)
    section.bottom_margin = Cm(1.8)
    section.left_margin = Cm(2.0)
    section.right_margin = Cm(2.0)

    styles = doc.styles
    styles["Normal"].font.name = "Microsoft YaHei"
    styles["Normal"]._element.rPr.rFonts.set(qn("w:eastAsia"), "Microsoft YaHei")
    styles["Normal"].font.size = Pt(10.5)

    title = doc.add_heading(TITLE, 0)
    title.alignment = WD_ALIGN_PARAGRAPH.CENTER
    title.runs[0].font.name = "Microsoft YaHei"
    title.runs[0]._element.rPr.rFonts.set(qn("w:eastAsia"), "Microsoft YaHei")

    subtitle = doc.add_paragraph("目标日期：2026 年 5 月 15 日前完成可演示、可投递、可讲解的实习作品版本")
    subtitle.alignment = WD_ALIGN_PARAGRAPH.CENTER

    add_heading(doc, "一、项目定位", 1)
    doc.add_paragraph(
        "把 ovwatch 包装成一个基于 STM32F411 的智能手表原型系统。重点不是追求所有功能完整，"
        "而是在五月中旬前做出一个稳定 demo，并能向面试官讲清楚显示、触摸、传感器、任务架构和后续 LVGL/FreeRTOS 规划。"
    )

    add_heading(doc, "二、五月中旬前必须完成的版本", 1)
    add_bullets(
        doc,
        [
            "Keil 工程可稳定编译，目标为 0 error / 0 warning。",
            "ST7789 屏幕显示稳定：首页时间、菜单、画板、触摸测试、MPU6050 页面可演示。",
            "CST816T 触摸稳定：点击、上滑、下滑、右滑返回、画板触摸绘制可用。",
            "MPU6050 可读取六轴数据，并保留摇动/运动唤醒逻辑。",
            "MAX30102 至少完成读 ID、初始化、FIFO 原始 RED/IR 数据读取。",
            "健康页先显示原始数据或波形，不强行承诺医学级心率/血氧准确性。",
            "补齐 README、硬件引脚表、项目结构图、演示视频和简历项目描述。",
        ],
    )

    add_heading(doc, "三、暂时不做或只写规划的内容", 1)
    add_bullets(
        doc,
        [
            "完整 FreeRTOS 重构：五月中旬前只完成最小任务拆分或架构说明，不做大规模推倒重来。",
            "完整 LVGL 全页面替换：可以保留 SquareLine 设计稿和移植计划，优先保证当前真机 demo 稳定。",
            "复杂心率/血氧算法：先读出 MAX30102 原始数据，再做滤波和峰值检测作为后续优化。",
            "SPI DMA、低功耗深度优化、PC 模拟器：放到面试时的后续计划中讲。",
        ],
    )

    add_heading(doc, "四、10 天冲刺计划", 1)
    table = doc.add_table(rows=1, cols=4)
    table.style = "Table Grid"
    hdr = table.rows[0].cells
    hdr[0].text = "日期"
    hdr[1].text = "主题"
    hdr[2].text = "当天任务"
    hdr[3].text = "验收标准"

    rows = [
        ("5 月 5 日", "工程整理", "确认当前功能、清理无关生成文件、补齐 ui.h 函数声明、确认最新构建日志。", "工程能编译，知道当前缺口。"),
        ("5 月 6 日", "README 初版", "写硬件清单、引脚表、功能列表、项目结构、构建方法。", "别人打开仓库能看懂项目。"),
        ("5 月 7 日", "MAX30102 基础驱动", "完成 I2C 读写、读 PART ID、基础寄存器配置。", "串口能打印芯片 ID。"),
        ("5 月 8 日", "MAX30102 FIFO", "读取 RED/IR FIFO 原始数据，做简单均值/范围检查。", "健康页或串口能看到连续数据。"),
        ("5 月 9 日", "健康页接入", "健康页显示 RED/IR 数值或简单波形，补错误提示。", "健康页不再是空页面。"),
        ("5 月 10 日", "交互稳定", "测试触摸、滑动、画板、返回、息屏、唤醒，修明显 bug。", "连续演示 3 次不崩。"),
        ("5 月 11 日", "MPU6050 展示", "整理六轴数据显示，写清楚摇动唤醒逻辑。", "能讲清楚采样和阈值判断。"),
        ("5 月 12 日", "最小架构文档", "画系统分层图，写 FreeRTOS/LVGL 后续迁移方案。", "README 有架构图和任务规划。"),
        ("5 月 13 日", "演示材料", "拍 3-5 张照片，录 30-60 秒 demo 视频。", "能直接投递给 HR/面试官。"),
        ("5 月 14 日", "简历包装", "写项目描述、技术要点、面试问答。", "简历项目经历可以直接使用。"),
        ("5 月 15 日", "最终检查", "重新编译、按演示脚本完整跑一遍，整理提交记录。", "形成五月中旬投递版本。"),
    ]
    for date, theme, task, check in rows:
        cells = table.add_row().cells
        cells[0].text = date
        cells[1].text = theme
        cells[2].text = task
        cells[3].text = check

    add_heading(doc, "五、每天学习和开发节奏", 1)
    add_numbered(
        doc,
        [
            "上午：看手册和资料，只解决当天功能需要的知识点。",
            "下午：写代码和上板测试，优先让功能跑起来。",
            "晚上：记录问题、截图、提交 Git、更新 README。",
            "每天结束前必须留下一个可见成果：代码提交、截图、串口日志、视频片段或文档更新。",
        ],
    )

    add_heading(doc, "六、简历项目描述", 1)
    doc.add_paragraph("项目名称：基于 STM32F411 的智能手表原型系统")
    doc.add_paragraph(
        "项目描述：基于 STM32F411、ST7789、CST816T、MPU6050、MAX30102 设计智能手表原型，"
        "完成显示、触摸、RTC、姿态检测、息屏唤醒和健康数据采集原型，并规划 FreeRTOS + LVGL 架构升级。"
    )
    add_bullets(
        doc,
        [
            "基于 SPI 驱动 ST7789，实现表盘时间显示、菜单页面、画板和传感器数据展示。",
            "基于 I2C + EXTI 驱动 CST816T，实现点击、滑动、画板绘制和页面切换。",
            "基于 I2C 驱动 MPU6050，实现六轴数据读取和摇动唤醒检测。",
            "接入 MAX30102，完成芯片识别、初始化和 FIFO 原始 RED/IR 数据读取。",
            "设计息屏/唤醒逻辑，并整理 FreeRTOS 多任务拆分和 LVGL 移植方案。",
        ],
    )

    add_heading(doc, "七、面试时主讲顺序", 1)
    add_numbered(
        doc,
        [
            "先展示实物 demo：开机、表盘、滑动菜单、画板、MPU6050、健康页。",
            "再讲硬件链路：SPI 屏幕、I2C 触摸、I2C 传感器、EXTI 中断。",
            "接着讲关键问题：触摸坐标映射、息屏唤醒、传感器采样、UI 刷新。",
            "最后讲升级计划：FreeRTOS 任务拆分、LVGL 显示和触摸移植、DMA 刷屏优化。",
        ],
    )

    add_heading(doc, "八、五月中旬交付清单", 1)
    add_bullets(
        doc,
        [
            "Keil 可编译工程。",
            "README.md。",
            "硬件引脚表。",
            "项目架构图。",
            "30-60 秒演示视频。",
            "3-5 张实物运行图。",
            "简历项目描述。",
            "面试问答笔记。",
        ],
    )

    doc.add_paragraph("")
    end = doc.add_paragraph("一句话原则：五月中旬前先做出稳定可演示版本，再用 FreeRTOS 和 LVGL 作为明确的工程升级方向。")
    end.alignment = WD_ALIGN_PARAGRAPH.CENTER

    doc.save(output_path)


def build_markdown(output_path):
    content = """# ovwatch 智能手表项目：五月中旬实习冲刺计划

目标日期：2026 年 5 月 15 日前完成可演示、可投递、可讲解的实习作品版本。

## 一、项目定位

把 ovwatch 包装成一个基于 STM32F411 的智能手表原型系统。重点不是追求所有功能完整，而是在五月中旬前做出一个稳定 demo，并能向面试官讲清楚显示、触摸、传感器、任务架构和后续 LVGL/FreeRTOS 规划。

## 二、五月中旬前必须完成的版本

- Keil 工程可稳定编译，目标为 0 error / 0 warning。
- ST7789 屏幕显示稳定：首页时间、菜单、画板、触摸测试、MPU6050 页面可演示。
- CST816T 触摸稳定：点击、上滑、下滑、右滑返回、画板触摸绘制可用。
- MPU6050 可读取六轴数据，并保留摇动/运动唤醒逻辑。
- MAX30102 至少完成读 ID、初始化、FIFO 原始 RED/IR 数据读取。
- 健康页先显示原始数据或波形，不强行承诺医学级心率/血氧准确性。
- 补齐 README、硬件引脚表、项目结构图、演示视频和简历项目描述。

## 三、10 天冲刺计划

| 日期 | 主题 | 当天任务 | 验收标准 |
|---|---|---|---|
| 5 月 5 日 | 工程整理 | 确认当前功能、清理无关生成文件、补齐 ui.h 函数声明、确认最新构建日志。 | 工程能编译，知道当前缺口。 |
| 5 月 6 日 | README 初版 | 写硬件清单、引脚表、功能列表、项目结构、构建方法。 | 别人打开仓库能看懂项目。 |
| 5 月 7 日 | MAX30102 基础驱动 | 完成 I2C 读写、读 PART ID、基础寄存器配置。 | 串口能打印芯片 ID。 |
| 5 月 8 日 | MAX30102 FIFO | 读取 RED/IR FIFO 原始数据，做简单均值/范围检查。 | 健康页或串口能看到连续数据。 |
| 5 月 9 日 | 健康页接入 | 健康页显示 RED/IR 数值或简单波形，补错误提示。 | 健康页不再是空页面。 |
| 5 月 10 日 | 交互稳定 | 测试触摸、滑动、画板、返回、息屏、唤醒，修明显 bug。 | 连续演示 3 次不崩。 |
| 5 月 11 日 | MPU6050 展示 | 整理六轴数据显示，写清楚摇动唤醒逻辑。 | 能讲清楚采样和阈值判断。 |
| 5 月 12 日 | 最小架构文档 | 画系统分层图，写 FreeRTOS/LVGL 后续迁移方案。 | README 有架构图和任务规划。 |
| 5 月 13 日 | 演示材料 | 拍 3-5 张照片，录 30-60 秒 demo 视频。 | 能直接投递给 HR/面试官。 |
| 5 月 14 日 | 简历包装 | 写项目描述、技术要点、面试问答。 | 简历项目经历可以直接使用。 |
| 5 月 15 日 | 最终检查 | 重新编译、按演示脚本完整跑一遍，整理提交记录。 | 形成五月中旬投递版本。 |

## 四、简历项目描述

项目名称：基于 STM32F411 的智能手表原型系统

项目描述：基于 STM32F411、ST7789、CST816T、MPU6050、MAX30102 设计智能手表原型，完成显示、触摸、RTC、姿态检测、息屏唤醒和健康数据采集原型，并规划 FreeRTOS + LVGL 架构升级。
"""
    output_path.write_text(content, encoding="utf-8")


def main():
    if len(sys.argv) != 2:
        raise SystemExit("usage: create_internship_doc.py OUTPUT_PATH")
    output_path = Path(sys.argv[1])
    output_path.parent.mkdir(parents=True, exist_ok=True)
    if output_path.suffix.lower() == ".docx":
        try:
            build_docx(output_path)
        except Exception:
            markdown_path = output_path.with_suffix(".md")
            build_markdown(markdown_path)
            raise
    else:
        build_markdown(output_path)


if __name__ == "__main__":
    main()
