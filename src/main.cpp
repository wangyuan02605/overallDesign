#include <QtWidgets>

namespace {

const char *kRetroStyle = R"(
* {
    font-family: "Geneva", "Arial", "Helvetica";
    font-size: 12px;
    color: #000000;
}
QMainWindow, QDialog, QWidget { background: #c0c0c0; }
QMenuBar {
    background: #c0c0c0;
    border-bottom: 1px solid #808080;
    spacing: 4px;
}
QMenuBar::item { padding: 4px 8px; background: transparent; }
QMenuBar::item:selected, QMenu::item:selected {
    background: #000080; color: white;
}
QMenu {
    background: #c0c0c0;
    border-top: 2px solid white; border-left: 2px solid white;
    border-right: 2px solid #404040; border-bottom: 2px solid #404040;
}
QMenu::item { padding: 4px 26px 4px 22px; }
QToolBar {
    background: #c0c0c0; border: 0; border-bottom: 1px solid #808080;
    spacing: 2px; padding: 3px;
}
QToolButton, QPushButton {
    background: #c0c0c0;
    border-top: 2px solid white; border-left: 2px solid white;
    border-right: 2px solid #404040; border-bottom: 2px solid #404040;
    padding: 3px 7px;
}
QToolButton:hover, QPushButton:hover { background: #d4d0c8; }
QToolButton:pressed, QPushButton:pressed {
    border-top: 2px solid #404040; border-left: 2px solid #404040;
    border-right: 2px solid white; border-bottom: 2px solid white;
    padding-left: 8px; padding-top: 4px;
}
QLineEdit, QSpinBox, QComboBox, QTextEdit, QPlainTextEdit {
    background: white;
    border-top: 2px solid #404040; border-left: 2px solid #404040;
    border-right: 2px solid white; border-bottom: 2px solid white;
    padding: 2px;
}
QTreeWidget, QTableWidget, QTableView {
    background: white;
    alternate-background-color: #f1f1e8;
    border-top: 2px solid #404040; border-left: 2px solid #404040;
    border-right: 2px solid white; border-bottom: 2px solid white;
    gridline-color: #808080;
    selection-background-color: #000080;
    selection-color: white;
}
QHeaderView::section {
    background: #c0c0c0;
    border-top: 1px solid white; border-left: 1px solid white;
    border-right: 1px solid #404040; border-bottom: 1px solid #404040;
    padding: 3px;
}
QTabWidget::pane {
    border-top: 2px solid white; border-left: 2px solid white;
    border-right: 2px solid #404040; border-bottom: 2px solid #404040;
}
QTabBar::tab {
    background: #c0c0c0;
    border-top: 2px solid white; border-left: 2px solid white;
    border-right: 2px solid #404040;
    padding: 5px 12px;
}
QTabBar::tab:selected { background: #e4e4e4; }
QGroupBox {
    border: 1px solid #808080; margin-top: 8px; padding-top: 7px;
}
QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 3px; }
QDockWidget { titlebar-close-icon: none; titlebar-normal-icon: none; }
QDockWidget::title {
    background: #000080; color: white; padding: 4px; font-weight: bold;
}
QStatusBar {
    background: #c0c0c0; border-top: 1px solid white;
}
QStatusBar::item {
    border-top: 1px solid #808080; border-left: 1px solid #808080;
    border-right: 1px solid white; border-bottom: 1px solid white;
}
QSplitter::handle { background: #c0c0c0; }
QProgressBar {
    background: white; border: 2px inset #c0c0c0; text-align: center;
}
QProgressBar::chunk { background: #000080; }
)";

QIcon pixelIcon(const QColor &color, int kind)
{
    QPixmap pm(16, 16);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setPen(Qt::black);
    p.setBrush(color);
    if (kind == 0) {
        p.drawRect(1, 4, 13, 10);
        p.setBrush(color.lighter(135));
        p.drawRect(2, 2, 6, 3);
    } else if (kind == 1) {
        p.drawRect(2, 1, 11, 13);
        p.setBrush(Qt::white);
        p.drawRect(4, 3, 7, 4);
        p.setBrush(QColor("#808080"));
        p.drawRect(4, 9, 7, 4);
    } else if (kind == 2) {
        p.drawEllipse(2, 2, 11, 11);
        p.setBrush(Qt::white);
        p.drawRect(6, 4, 3, 5);
        p.drawRect(7, 10, 1, 1);
    } else if (kind == 3) {
        p.drawRect(1, 2, 13, 10);
        p.setPen(Qt::white);
        p.drawLine(3, 5, 12, 5);
        p.drawLine(3, 8, 9, 8);
        p.setPen(Qt::black);
        p.drawLine(4, 14, 11, 14);
    } else {
        p.drawRect(2, 2, 11, 11);
        p.setBrush(Qt::white);
        p.drawRect(4, 4, 7, 7);
        p.setPen(color);
        p.drawLine(5, 7, 7, 9);
        p.drawLine(7, 9, 10, 5);
    }
    p.setPen(Qt::white);
    p.drawPoint(2, 5);
    return QIcon(pm);
}

QLabel *sunkenLabel(const QString &text, int minWidth = 100)
{
    auto *label = new QLabel(text);
    label->setMinimumWidth(minWidth);
    label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label->setMargin(2);
    return label;
}

class MainWindow final : public QMainWindow
{
public:
    MainWindow()
    {
        setWindowTitle(QStringLiteral("第三方通信管理系统 - 工程师站 [DEMO]"));
        setWindowIcon(pixelIcon(QColor("#000080"), 3));
        resize(1320, 820);
        setMinimumSize(1024, 680);
        createMenus();
        createToolbar();
        createWorkspace();
        createStatusBar();
        startDemoTimer();
    }

private:
    QTabWidget *tabs_ = nullptr;
    QTableWidget *overview_ = nullptr;
    QTableWidget *frames_ = nullptr;
    QLabel *clock_ = nullptr;
    QLabel *rxRate_ = nullptr;
    int tick_ = 0;

    void createMenus()
    {
        const QStringList menus = {
            QStringLiteral("工程(&P)"), QStringLiteral("编辑(&E)"),
            QStringLiteral("组态(&C)"), QStringLiteral("运行(&R)"),
            QStringLiteral("监视(&M)"), QStringLiteral("工具(&T)"),
            QStringLiteral("窗口(&W)"), QStringLiteral("帮助(&H)")
        };
        for (const auto &name : menus) {
            auto *menu = menuBar()->addMenu(name);
            if (name.startsWith(QStringLiteral("工程"))) {
                menu->addAction(pixelIcon(QColor("#c8a000"), 0), QStringLiteral("打开工程..."));
                menu->addAction(pixelIcon(QColor("#000080"), 1), QStringLiteral("保存全部"));
                menu->addSeparator();
                menu->addAction(QStringLiteral("退出"), this, &QWidget::close);
            } else if (name.startsWith(QStringLiteral("运行"))) {
                menu->addAction(pixelIcon(QColor("#008000"), 4), QStringLiteral("启动所选通道"));
                menu->addAction(pixelIcon(QColor("#800000"), 2), QStringLiteral("停止所选通道"));
            } else {
                menu->addAction(QStringLiteral("演示命令"));
            }
        }
    }

    void createToolbar()
    {
        auto *tb = addToolBar(QStringLiteral("主工具栏"));
        tb->setMovable(false);
        const QList<QPair<QString, QIcon>> actions = {
            {QStringLiteral("打开"), pixelIcon(QColor("#c8a000"), 0)},
            {QStringLiteral("保存"), pixelIcon(QColor("#000080"), 1)},
            {QStringLiteral("校验"), pixelIcon(QColor("#008080"), 4)},
            {QStringLiteral("发布"), pixelIcon(QColor("#800080"), 3)},
            {QStringLiteral("启动"), pixelIcon(QColor("#008000"), 4)},
            {QStringLiteral("停止"), pixelIcon(QColor("#800000"), 2)},
            {QStringLiteral("报文"), pixelIcon(QColor("#000080"), 3)}
        };
        for (const auto &a : actions) {
            auto *act = tb->addAction(a.second, a.first);
            act->setToolTip(a.first);
        }
        tb->addSeparator();
        auto *project = new QLabel(QStringLiteral(" 当前工程: 烟气处理通信工程  "));
        project->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        tb->addWidget(project);
    }

    void createWorkspace()
    {
        tabs_ = new QTabWidget;
        tabs_->setTabsClosable(true);
        tabs_->setMovable(true);
        connect(tabs_, &QTabWidget::tabCloseRequested, this, [this](int i) {
            if (tabs_->count() > 1) tabs_->removeTab(i);
        });
        tabs_->addTab(createOverviewPage(), pixelIcon(QColor("#000080"), 3), QStringLiteral("系统总览"));
        tabs_->addTab(createPointPage(), pixelIcon(QColor("#008080"), 1), QStringLiteral("Modbus点表 *"));
        tabs_->addTab(createFramePage(), pixelIcon(QColor("#800080"), 3), QStringLiteral("报文监视"));
        setCentralWidget(tabs_);

        auto *projectDock = new QDockWidget(QStringLiteral("工程管理器"), this);
        projectDock->setObjectName(QStringLiteral("projectDock"));
        projectDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        projectDock->setWidget(createProjectTree());
        addDockWidget(Qt::LeftDockWidgetArea, projectDock);

        auto *propertyDock = new QDockWidget(QStringLiteral("属性 / 检查器"), this);
        propertyDock->setObjectName(QStringLiteral("propertyDock"));
        propertyDock->setWidget(createPropertyPanel());
        addDockWidget(Qt::RightDockWidgetArea, propertyDock);

        auto *logDock = new QDockWidget(QStringLiteral("问题与运行日志"), this);
        logDock->setObjectName(QStringLiteral("logDock"));
        logDock->setWidget(createLogPanel());
        addDockWidget(Qt::BottomDockWidgetArea, logDock);

        resizeDocks({projectDock, propertyDock}, {235, 275}, Qt::Horizontal);
        resizeDocks({logDock}, {175}, Qt::Vertical);
    }

    QWidget *createProjectTree()
    {
        auto *box = new QWidget;
        auto *layout = new QVBoxLayout(box);
        layout->setContentsMargins(4, 4, 4, 4);
        auto *filter = new QLineEdit;
        filter->setPlaceholderText(QStringLiteral("查找工程对象"));
        layout->addWidget(filter);

        auto *tree = new QTreeWidget;
        tree->setHeaderLabel(QStringLiteral("对象名称"));
        tree->setAnimated(false);
        auto add = [&](QTreeWidgetItem *parent, const QString &name, QColor c, int kind) {
            auto *item = parent ? new QTreeWidgetItem(parent) : new QTreeWidgetItem(tree);
            item->setText(0, name);
            item->setIcon(0, pixelIcon(c, kind));
            return item;
        };
        auto *root = add(nullptr, QStringLiteral("烟气处理通信工程"), QColor("#c8a000"), 0);
        auto *channels = add(root, QStringLiteral("通信通道"), QColor("#000080"), 3);
        auto *mb = add(channels, QStringLiteral("Modbus 主站"), QColor("#008000"), 4);
        add(mb, QStringLiteral("锅炉房 PLC [运行]"), QColor("#008000"), 4);
        add(mb, QStringLiteral("脱硫 RTU [运行]"), QColor("#008000"), 4);
        add(mb, QStringLiteral("水处理 PLC [故障]"), QColor("#800000"), 2);
        auto *iec = add(channels, QStringLiteral("IEC104 主站"), QColor("#008080"), 3);
        add(iec, QStringLiteral("厂用电保护 [运行]"), QColor("#008000"), 4);
        add(channels, QStringLiteral("OPC UA 客户端"), QColor("#800080"), 3);
        add(root, QStringLiteral("点目录 (20,436)"), QColor("#c8a000"), 0);
        add(root, QStringLiteral("扫描组"), QColor("#000080"), 1);
        add(root, QStringLiteral("Python 脚本"), QColor("#008080"), 1);
        add(root, QStringLiteral("版本与发布"), QColor("#800080"), 1);
        root->setExpanded(true);
        channels->setExpanded(true);
        mb->setExpanded(true);
        tree->setCurrentItem(mb->child(0));
        layout->addWidget(tree);

        connect(filter, &QLineEdit::textChanged, tree, [tree](const QString &text) {
            for (auto *item : tree->findItems(QStringLiteral("*"), Qt::MatchWildcard | Qt::MatchRecursive))
                item->setHidden(!text.isEmpty() && !item->text(0).contains(text, Qt::CaseInsensitive));
        });
        return box;
    }

    QWidget *createOverviewPage()
    {
        auto *page = new QWidget;
        auto *layout = new QVBoxLayout(page);
        auto *title = new QLabel(QStringLiteral(" 通信运行总览"));
        title->setStyleSheet(QStringLiteral("background:#000080;color:white;font-weight:bold;padding:5px;"));
        layout->addWidget(title);

        overview_ = new QTableWidget(5, 7);
        overview_->setHorizontalHeaderLabels({
            QStringLiteral("状态"), QStringLiteral("通道"), QStringLiteral("协议/角色"),
            QStringLiteral("对端"), QStringLiteral("收/发帧"), QStringLiteral("成功率"), QStringLiteral("响应")
        });
        const QList<QStringList> rows = {
            {QStringLiteral("● 运行"), QStringLiteral("锅炉房 PLC"), QStringLiteral("Modbus TCP / 主站"), QStringLiteral("10.20.1.31:502"), QStringLiteral("18542 / 9231"), QStringLiteral("99.98%"), QStringLiteral("12 ms")},
            {QStringLiteral("● 运行"), QStringLiteral("脱硫 RTU"), QStringLiteral("Modbus RTU / 主站"), QStringLiteral("/dev/tty.usbserial"), QStringLiteral("8230 / 4120"), QStringLiteral("99.91%"), QStringLiteral("38 ms")},
            {QStringLiteral("● 故障"), QStringLiteral("水处理 PLC"), QStringLiteral("Modbus TCP / 主站"), QStringLiteral("10.20.2.16:502"), QStringLiteral("320 / 211"), QStringLiteral("82.34%"), QStringLiteral("超时")},
            {QStringLiteral("● 运行"), QStringLiteral("厂用电保护"), QStringLiteral("IEC104 / 主站"), QStringLiteral("10.30.0.8:2404"), QStringLiteral("7210 / 402"), QStringLiteral("100%"), QStringLiteral("8 ms")},
            {QStringLiteral("○ 停止"), QStringLiteral("历史数据 UA"), QStringLiteral("OPC UA / 客户端"), QStringLiteral("opc.tcp://127.0.0.1"), QStringLiteral("0 / 0"), QStringLiteral("--"), QStringLiteral("--")}
        };
        for (int r = 0; r < rows.size(); ++r)
            for (int c = 0; c < rows[r].size(); ++c)
                overview_->setItem(r, c, new QTableWidgetItem(rows[r][c]));
        overview_->item(0, 0)->setForeground(QColor("#008000"));
        overview_->item(1, 0)->setForeground(QColor("#008000"));
        overview_->item(2, 0)->setForeground(QColor("#c00000"));
        overview_->item(3, 0)->setForeground(QColor("#008000"));
        overview_->setAlternatingRowColors(true);
        overview_->setSelectionBehavior(QAbstractItemView::SelectRows);
        overview_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        overview_->horizontalHeader()->setStretchLastSection(true);
        overview_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        layout->addWidget(overview_);

        auto *summary = new QHBoxLayout;
        for (const auto &pair : QList<QPair<QString, QString>>{
                 {QStringLiteral("运行通道"), QStringLiteral("3 / 5")},
                 {QStringLiteral("点更新速率"), QStringLiteral("18,642 /s")},
                 {QStringLiteral("通信告警"), QStringLiteral("1")},
                 {QStringLiteral("CPU / 内存"), QStringLiteral("18% / 412 MB")}}) {
            auto *g = new QGroupBox(pair.first);
            auto *l = new QVBoxLayout(g);
            auto *v = new QLabel(pair.second);
            v->setAlignment(Qt::AlignCenter);
            v->setStyleSheet(QStringLiteral("font-size:18px;font-weight:bold;color:#000080;"));
            l->addWidget(v);
            summary->addWidget(g);
        }
        layout->addLayout(summary);
        return page;
    }

    QWidget *createPointPage()
    {
        auto *page = new QWidget;
        auto *layout = new QVBoxLayout(page);
        auto *controls = new QHBoxLayout;
        auto *search = new QLineEdit;
        search->setPlaceholderText(QStringLiteral("点名/地址/描述"));
        controls->addWidget(new QLabel(QStringLiteral("扫描组:")));
        auto *group = new QComboBox;
        group->addItems({QStringLiteral("快速组 (100 ms)"), QStringLiteral("普通组 (1 s)"), QStringLiteral("慢速组 (5 s)")});
        controls->addWidget(group);
        controls->addSpacing(8);
        controls->addWidget(new QLabel(QStringLiteral("筛选:")));
        controls->addWidget(search, 1);
        controls->addWidget(new QPushButton(QStringLiteral("导入...")));
        controls->addWidget(new QPushButton(QStringLiteral("分包预览")));
        layout->addLayout(controls);

        auto *table = new QTableWidget(12, 9);
        table->setHorizontalHeaderLabels({
            QStringLiteral("启用"), QStringLiteral("点名"), QStringLiteral("地址"),
            QStringLiteral("类型"), QStringLiteral("读写"), QStringLiteral("工程值"),
            QStringLiteral("质量"), QStringLiteral("周期"), QStringLiteral("描述")
        });
        for (int r = 0; r < table->rowCount(); ++r) {
            auto *enabled = new QTableWidgetItem;
            enabled->setCheckState(Qt::Checked);
            table->setItem(r, 0, enabled);
            table->setItem(r, 1, new QTableWidgetItem(QStringLiteral("BLR_%1").arg(r % 2 ? QStringLiteral("PRESS") : QStringLiteral("TEMP")) + QString::number(r + 1)));
            table->setItem(r, 2, new QTableWidgetItem(QStringLiteral("400%1%2").arg(10 + r / 2).arg(r % 2 ? QStringLiteral(".1") : QString())));
            table->setItem(r, 3, new QTableWidgetItem(r % 2 ? QStringLiteral("DM/Bit") : QStringLiteral("AM/Float32")));
            table->setItem(r, 4, new QTableWidgetItem(r == 7 ? QStringLiteral("写") : QStringLiteral("读")));
            table->setItem(r, 5, new QTableWidgetItem(r % 2 ? QStringLiteral("1") : QString::number(80.0 + r * 2.35, 'f', 2)));
            table->setItem(r, 6, new QTableWidgetItem(r == 4 ? QStringLiteral("Bad_Comm") : QStringLiteral("Good")));
            table->setItem(r, 7, new QTableWidgetItem(QStringLiteral("100 ms")));
            table->setItem(r, 8, new QTableWidgetItem(QStringLiteral("锅炉工艺演示测点 %1").arg(r + 1)));
            if (r == 4) table->item(r, 6)->setForeground(QColor("#c00000"));
        }
        table->setAlternatingRowColors(true);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
        layout->addWidget(table);
        auto *hint = new QLabel(QStringLiteral("  校验结果: 11 个有效点，1 个通信质量异常  |  自动分为 2 个读取块，预计周期占用 43%"));
        hint->setStyleSheet(QStringLiteral("background:#ffffc0;border:1px solid #808000;padding:4px;"));
        layout->addWidget(hint);
        return page;
    }

    QWidget *createFramePage()
    {
        auto *page = new QWidget;
        auto *layout = new QVBoxLayout(page);
        auto *bar = new QHBoxLayout;
        auto *pause = new QPushButton(QStringLiteral("Ⅱ 暂停"));
        auto *clear = new QPushButton(QStringLiteral("清空"));
        bar->addWidget(pause);
        bar->addWidget(clear);
        bar->addWidget(new QLabel(QStringLiteral("协议:")));
        auto *protocol = new QComboBox;
        protocol->addItems({QStringLiteral("全部"), QStringLiteral("Modbus"), QStringLiteral("IEC104"), QStringLiteral("OPC UA")});
        bar->addWidget(protocol);
        bar->addWidget(new QLabel(QStringLiteral("方向:")));
        auto *direction = new QComboBox;
        direction->addItems({QStringLiteral("收 + 发"), QStringLiteral("接收"), QStringLiteral("发送")});
        bar->addWidget(direction);
        bar->addStretch();
        rxRate_ = sunkenLabel(QStringLiteral("642 帧/s"), 90);
        bar->addWidget(rxRate_);
        layout->addLayout(bar);

        auto *split = new QSplitter(Qt::Vertical);
        frames_ = new QTableWidget(10, 6);
        frames_->setHorizontalHeaderLabels({
            QStringLiteral("时间"), QStringLiteral("方向"), QStringLiteral("通道"),
            QStringLiteral("摘要"), QStringLiteral("长度"), QStringLiteral("结果")
        });
        for (int r = 0; r < frames_->rowCount(); ++r) addFrameRow(r);
        frames_->setAlternatingRowColors(true);
        frames_->setSelectionBehavior(QAbstractItemView::SelectRows);
        frames_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        frames_->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
        frames_->selectRow(0);
        split->addWidget(frames_);

        auto *detail = new QSplitter(Qt::Horizontal);
        auto *hex = new QPlainTextEdit;
        hex->setReadOnly(true);
        hex->setFont(QFont(QStringLiteral("Monaco"), 11));
        hex->setPlainText(QStringLiteral(
            "0000  00 01 00 00 00 06 01 03 00 0A 00 04  ............\n"
            "0010  41 A2 00 00 00 01 42 18 00 00        A.....B..."));
        auto *parse = new QTreeWidget;
        parse->setHeaderLabels({QStringLiteral("字段"), QStringLiteral("值")});
        auto *mbap = new QTreeWidgetItem(parse, {QStringLiteral("MBAP Header"), QString()});
        new QTreeWidgetItem(mbap, {QStringLiteral("Transaction ID"), QStringLiteral("0x0001")});
        new QTreeWidgetItem(mbap, {QStringLiteral("Unit ID"), QStringLiteral("1")});
        auto *pdu = new QTreeWidgetItem(parse, {QStringLiteral("PDU"), QString()});
        new QTreeWidgetItem(pdu, {QStringLiteral("Function"), QStringLiteral("03 Read Holding Registers")});
        new QTreeWidgetItem(pdu, {QStringLiteral("Start Address"), QStringLiteral("10")});
        new QTreeWidgetItem(pdu, {QStringLiteral("Quantity"), QStringLiteral("4")});
        parse->expandAll();
        detail->addWidget(hex);
        detail->addWidget(parse);
        split->addWidget(detail);
        split->setSizes({300, 180});
        layout->addWidget(split);
        connect(clear, &QPushButton::clicked, frames_, [this] { frames_->setRowCount(0); });
        connect(pause, &QPushButton::clicked, pause, [pause] {
            pause->setText(pause->text().contains(QStringLiteral("暂停")) ? QStringLiteral("▶ 继续") : QStringLiteral("Ⅱ 暂停"));
        });
        return page;
    }

    void addFrameRow(int r)
    {
        const bool rx = r % 2 == 0;
        const QStringList vals = {
            QTime::currentTime().addMSecs(-r * 84).toString(QStringLiteral("HH:mm:ss.zzz")),
            rx ? QStringLiteral("← RX") : QStringLiteral("→ TX"),
            r % 3 ? QStringLiteral("锅炉房 PLC") : QStringLiteral("厂用电保护"),
            r % 3 ? QStringLiteral("FC03  地址 40010  数量 4") : QStringLiteral("M_ME_NC_1  遥测"),
            QString::number(12 + r * 2),
            r == 7 ? QStringLiteral("超时") : QStringLiteral("成功")
        };
        for (int c = 0; c < vals.size(); ++c) frames_->setItem(r, c, new QTableWidgetItem(vals[c]));
        frames_->item(r, 1)->setForeground(rx ? QColor("#0000c0") : QColor("#008000"));
        if (r == 7) frames_->item(r, 5)->setForeground(QColor("#c00000"));
    }

    QWidget *createPropertyPanel()
    {
        auto *scroll = new QScrollArea;
        scroll->setWidgetResizable(true);
        auto *panel = new QWidget;
        auto *layout = new QVBoxLayout(panel);
        auto addForm = [&](const QString &title, const QList<QPair<QString, QString>> &items) {
            auto *group = new QGroupBox(title);
            auto *form = new QFormLayout(group);
            form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
            for (const auto &item : items) {
                auto *edit = new QLineEdit(item.second);
                form->addRow(item.first, edit);
            }
            layout->addWidget(group);
        };
        addForm(QStringLiteral("基本"), {
            {QStringLiteral("通道名称"), QStringLiteral("锅炉房 PLC")},
            {QStringLiteral("协议"), QStringLiteral("Modbus TCP")},
            {QStringLiteral("角色"), QStringLiteral("主站")}
        });
        addForm(QStringLiteral("端点"), {
            {QStringLiteral("IP 地址"), QStringLiteral("10.20.1.31")},
            {QStringLiteral("端口"), QStringLiteral("502")},
            {QStringLiteral("Unit ID"), QStringLiteral("1")}
        });
        addForm(QStringLiteral("时序"), {
            {QStringLiteral("响应超时"), QStringLiteral("1000 ms")},
            {QStringLiteral("重试次数"), QStringLiteral("2")},
            {QStringLiteral("重连间隔"), QStringLiteral("3000 ms")}
        });
        auto *test = new QPushButton(QStringLiteral("测试连接"));
        layout->addWidget(test);
        layout->addStretch();
        scroll->setWidget(panel);
        return scroll;
    }

    QWidget *createLogPanel()
    {
        auto *tabs = new QTabWidget;
        auto *problems = new QTableWidget(3, 4);
        problems->setHorizontalHeaderLabels({QStringLiteral("级别"), QStringLiteral("对象"), QStringLiteral("代码"), QStringLiteral("说明")});
        const QList<QStringList> rows = {
            {QStringLiteral("错误"), QStringLiteral("水处理 PLC"), QStringLiteral("CONNECT_FAILED"), QStringLiteral("连接 10.20.2.16:502 超时，3 秒后重试")},
            {QStringLiteral("警告"), QStringLiteral("点 BLR_TEMP5"), QStringLiteral("BAD_COMM"), QStringLiteral("数据已陈旧 12.4 秒")},
            {QStringLiteral("信息"), QStringLiteral("工程版本 27"), QStringLiteral("VALID"), QStringLiteral("组态静态校验通过")}
        };
        for (int r = 0; r < rows.size(); ++r)
            for (int c = 0; c < rows[r].size(); ++c)
                problems->setItem(r, c, new QTableWidgetItem(rows[r][c]));
        problems->item(0, 0)->setForeground(QColor("#c00000"));
        problems->item(1, 0)->setForeground(QColor("#806000"));
        problems->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
        problems->setEditTriggers(QAbstractItemView::NoEditTriggers);
        auto *log = new QPlainTextEdit;
        log->setReadOnly(true);
        log->setFont(QFont(QStringLiteral("Monaco"), 10));
        log->setPlainText(QStringLiteral(
            "14:26:07 [INFO ] comm-managerd 配置版本 27 已加载\n"
            "14:26:08 [INFO ] Modbus/主站 锅炉房 PLC 进入 RUNNING\n"
            "14:26:12 [WARN ] 水处理 PLC 连接超时 retry=2\n"
            "14:26:13 [INFO ] IEC104 总召确认，收到 842 个对象"));
        tabs->addTab(problems, pixelIcon(QColor("#c8a000"), 2), QStringLiteral("问题 (2)"));
        tabs->addTab(log, pixelIcon(QColor("#000080"), 3), QStringLiteral("运行日志"));
        return tabs;
    }

    void createStatusBar()
    {
        statusBar()->setSizeGripEnabled(true);
        statusBar()->addWidget(sunkenLabel(QStringLiteral("● 服务已连接"), 125));
        statusBar()->addWidget(sunkenLabel(QStringLiteral("工程版本: 27  |  草稿已修改"), 190), 1);
        statusBar()->addPermanentWidget(sunkenLabel(QStringLiteral("用户: engineer"), 115));
        statusBar()->addPermanentWidget(sunkenLabel(QStringLiteral("本地模式"), 80));
        clock_ = sunkenLabel(QString(), 150);
        statusBar()->addPermanentWidget(clock_);
    }

    void startDemoTimer()
    {
        auto *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [this] {
            ++tick_;
            clock_->setText(QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd  HH:mm:ss")));
            if (rxRate_) rxRate_->setText(QStringLiteral("%1 帧/s").arg(620 + (tick_ * 17) % 55));
            if (overview_ && tick_ % 2 == 0)
                overview_->item(0, 4)->setText(QStringLiteral("%1 / %2").arg(18542 + tick_ * 4).arg(9231 + tick_ * 2));
        });
        timer->start(1000);
        clock_->setText(QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd  HH:mm:ss")));
    }
};

} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("Retro DCS Demo"));
    app.setStyle(QStringLiteral("Fusion"));
    app.setStyleSheet(QString::fromLatin1(kRetroStyle));
    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#c0c0c0"));
    palette.setColor(QPalette::Button, QColor("#c0c0c0"));
    palette.setColor(QPalette::Base, Qt::white);
    palette.setColor(QPalette::Highlight, QColor("#000080"));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    app.setPalette(palette);
    MainWindow window;
    window.show();
    const QStringList args = app.arguments();
    const int screenshotArg = args.indexOf(QStringLiteral("--screenshot"));
    if (screenshotArg >= 0 && screenshotArg + 1 < args.size()) {
        const QString path = args.at(screenshotArg + 1);
        QTimer::singleShot(350, &window, [&window, path] {
            window.grab().save(path);
            QApplication::quit();
        });
    }
    return app.exec();
}
