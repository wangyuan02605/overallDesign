# Retro DCS Qt5 界面 Demo

这是依据《第三方通信管理软件总体设计方案》制作的界面风格验证原型：

- Qt 5 Widgets，macOS 可直接编译运行；
- MACS/传统 DCS 工程师站式工作台；
- Win98 风格的灰色 3D 控件、深蓝标题、紧凑表格和 16×16 像素图标；
- 包含工程树、通道总览、点表、报文监视、属性检查器、问题/日志面板；
- 所有数据均为演示数据，不连接真实设备。

## macOS 构建

### 使用 Qt Creator / qmake

在 Qt Creator 中直接打开 `RetroDcsDemo.pro`，选择 Qt 5 Desktop Kit 后运行即可。

也可以在终端构建：

```bash
mkdir -p build-qmake
cd build-qmake
/opt/homebrew/opt/qt@5/bin/qmake ../RetroDcsDemo.pro
make -j4
./retro_dcs_demo
```

### 使用 CMake

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt@5)"
cmake --build build -j
./build/retro_dcs_demo
```

如果 CMake 找不到 Qt5，也可先执行：

```bash
export PATH="/opt/homebrew/opt/qt@5/bin:$PATH"
```

## 风格说明

界面刻意不调用 macOS 原生 Aqua 外观，而通过 Qt StyleSheet 与自绘像素图标保持跨平台一致。字体优先使用 `Geneva`，不存在时由系统回退。当前原型以 1280×800 为基准，最小窗口尺寸为 1024×680。
