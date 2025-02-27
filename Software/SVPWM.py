import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button
from matplotlib.animation import FuncAnimation

# 常量定义
SQRT_3 = np.sqrt(3)  # 根号3
PWM_PERIOD = 3400      # PWM周期
T = PWM_PERIOD * 4    # TIM1 ARR值的4倍
T_SQRT3 = int(T * SQRT_3)

# 扇区定义
SECTOR_1, SECTOR_2, SECTOR_3, SECTOR_4, SECTOR_5, SECTOR_6 = 1, 2, 3, 4, 5, 6

# SVPWM计算函数
def calc_svpwm(Valpha, Vbeta, zeta):
    # 计算中间变量
    wUAlpha = Valpha * T_SQRT3
    wUBeta = -(Vbeta * T)

    wX = wUBeta
    wY = (wUBeta + wUAlpha) / 2
    wZ = (wUBeta - wUAlpha) / 2

    # 查找扇区号
    if wY < 0:
        if wZ < 0:
            bSector = SECTOR_5
        else:
            if wX <= 0:
                bSector = SECTOR_4
            else:
                bSector = SECTOR_3
    else:
        if wZ >= 0:
            bSector = SECTOR_2
        else:
            if wX <= 0:
                bSector = SECTOR_6
            else:
                bSector = SECTOR_1

    # 计算三相占空比
    if bSector in [SECTOR_1, SECTOR_4]:
        hTimePhA = (T / 8) + (((T + wX) - wZ) / 2) / zeta
        hTimePhB = hTimePhA + wZ / zeta
        hTimePhC = hTimePhB - wX / zeta
    elif bSector in [SECTOR_2, SECTOR_5]:
        hTimePhA = (T / 8) + (((T + wY) - wZ) / 2) / zeta
        hTimePhB = hTimePhA + wZ / zeta
        hTimePhC = hTimePhA - wY / zeta
    elif bSector in [SECTOR_3, SECTOR_6]:
        hTimePhA = (T / 8) + (((T - wX) + wY) / 2) / zeta
        hTimePhC = hTimePhA - wY / zeta
        hTimePhB = hTimePhC + wX / zeta
    else:
        hTimePhA, hTimePhB, hTimePhC = 0, 0, 0

    return hTimePhA, hTimePhB, hTimePhC

# 动态调节和图像化显示
def main():
    # 初始值
    Valpha_init = 0.5
    Vbeta_init = 0.5
    zeta_init = 1000

    # 创建图形和滑动条
    fig = plt.figure(figsize=(10, 6))
    gs = fig.add_gridspec(2, 1, height_ratios=[2, 1])

    ax_main = fig.add_subplot(gs[0])
    ax_vector = fig.add_subplot(gs[1])

    plt.subplots_adjust(left=0.1, bottom=0.3)
    ax_Valpha = plt.axes([0.2, 0.25, 0.6, 0.03])
    ax_Vbeta = plt.axes([0.2, 0.2, 0.6, 0.03])
    ax_zeta = plt.axes([0.2, 0.15, 0.6, 0.03])

    slider_Valpha = Slider(ax_Valpha, 'Valpha', -100.0, 100.0, valinit=Valpha_init)
    slider_Vbeta = Slider(ax_Vbeta, 'Vbeta', -100.0, 100.0, valinit=Vbeta_init)
    slider_zeta = Slider(ax_zeta, 'Zeta', 1, 10000, valinit=zeta_init, valstep=1)

    # 初始化绘图
    time = np.arange(0, 100, 1)
    hTimePhA_vals = np.zeros_like(time)
    hTimePhB_vals = np.zeros_like(time)
    hTimePhC_vals = np.zeros_like(time)

    line_A, = ax_main.plot(time, hTimePhA_vals, label='hTimePhA')
    line_B, = ax_main.plot(time, hTimePhB_vals, label='hTimePhB')
    line_C, = ax_main.plot(time, hTimePhC_vals, label='hTimePhC')
    ax_main.set_ylim(0, T/4)
    ax_main.legend()

    # 向量图初始化
    circle = plt.Circle((0, 0), 100, color='lightblue', fill=False)
    vector_line, = ax_vector.plot([], [], marker='o', markersize=5, linestyle='-')

    ax_vector.add_artist(circle)
    ax_vector.set_xlim(-110, 110)
    ax_vector.set_ylim(-110, 110)
    ax_vector.set_aspect('equal')
    ax_vector.grid(True)
    ax_vector.set_xlabel('Valpha')
    ax_vector.set_ylabel('Vbeta')

    # 自动模式状态
    auto_mode = False

    # 更新函数
    def update(val):
        nonlocal auto_mode
        if not auto_mode:
            Valpha = slider_Valpha.val
            Vbeta = slider_Vbeta.val
            zeta = slider_zeta.val

            # 计算SVPWM
            hTimePhA, hTimePhB, hTimePhC = calc_svpwm(Valpha, Vbeta, zeta)

            # 更新数据
            hTimePhA_vals[:-1] = hTimePhA_vals[1:]
            hTimePhA_vals[-1] = hTimePhA

            hTimePhB_vals[:-1] = hTimePhB_vals[1:]
            hTimePhB_vals[-1] = hTimePhB

            hTimePhC_vals[:-1] = hTimePhC_vals[1:]
            hTimePhC_vals[-1] = hTimePhC

            # 更新绘图
            line_A.set_ydata(hTimePhA_vals)
            line_B.set_ydata(hTimePhB_vals)
            line_C.set_ydata(hTimePhC_vals)
            fig.canvas.draw_idle()

            # 更新向量图
            vector_line.set_data([0, Valpha], [0, Vbeta])
            ax_vector.relim()
            ax_vector.autoscale_view()

    # 绑定滑动条事件
    slider_Valpha.on_changed(update)
    slider_Vbeta.on_changed(update)
    slider_zeta.on_changed(update)

    # 自动模式切换函数
    def toggle_auto_mode(event):
        nonlocal auto_mode
        auto_mode = not auto_mode
        button.label.set_text('Stop' if auto_mode else 'Start')

    # 添加开始/停止按钮
    ax_button = plt.axes([0.7, 0.05, 0.1, 0.04])
    button = Button(ax_button, 'Start')
    button.on_clicked(toggle_auto_mode)

    # 自动模式更新函数
    t = 0

    def animate(i):
        nonlocal t, auto_mode
        if auto_mode:
            Valpha = 100 * np.cos(t)
            Vbeta = 100 * np.sin(t)
            zeta = slider_zeta.val

            # 计算SVPWM
            hTimePhA, hTimePhB, hTimePhC = calc_svpwm(Valpha, Vbeta, zeta)

            # 更新数据
            hTimePhA_vals[:-1] = hTimePhA_vals[1:]
            hTimePhA_vals[-1] = hTimePhA

            hTimePhB_vals[:-1] = hTimePhB_vals[1:]
            hTimePhB_vals[-1] = hTimePhB

            hTimePhC_vals[:-1] = hTimePhC_vals[1:]
            hTimePhC_vals[-1] = hTimePhC

            # 更新绘图
            line_A.set_ydata(hTimePhA_vals)
            line_B.set_ydata(hTimePhB_vals)
            line_C.set_ydata(hTimePhC_vals)
            fig.canvas.draw_idle()

            # 更新向量图
            vector_line.set_data([0, Valpha], [0, Vbeta])
            ax_vector.relim()
            ax_vector.autoscale_view()

            t += 0.1  # 时间步长

    # 启动动画
    ani = FuncAnimation(fig, animate, interval=100)

    # 显示图形
    plt.show()

if __name__ == "__main__":
    main()



