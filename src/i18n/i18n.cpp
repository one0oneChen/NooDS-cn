/*
    NooDS-cn i18n module - implementation
    See i18n.h for the public interface.
*/

#include "i18n.h"

I18n& I18n::instance() {
    static I18n inst;
    return inst;
}

I18n::I18n(): currentLang(LANG_EN) {
    registerTranslations();
}

void I18n::setLanguage(Language lang) {
    currentLang = lang;
}

void I18n::setLanguageFromInt(int value) {
    currentLang = (value == LANG_ZH) ? LANG_ZH : LANG_EN;
}

const char* I18n::t(const char* key) const {
    if (!key) return "";
    if (currentLang == LANG_ZH) {
        auto it = zh.find(key);
        if (it != zh.end()) return it->second.c_str();
    }
    auto it = en.find(key);
    if (it != en.end()) return it->second.c_str();
    return key;
}

std::string I18n::ts(const std::string &key) const {
    if (currentLang == LANG_ZH) {
        auto it = zh.find(key);
        if (it != zh.end()) return it->second;
    }
    auto it = en.find(key);
    if (it != en.end()) return it->second;
    return key;
}

void I18n::add(const char* key, const char* english, const char* chinese) {
    en[key] = english;
    zh[key] = chinese;
}

void I18n::registerTranslations() {
    // ============== Main menu bar ==============
    add("MENU_FILE", "&File", "文件(&F)");
    add("MENU_SYSTEM", "&System", "系统(&S)");
    add("MENU_SETTINGS", "&Settings", "设置(&S)");

    // ============== File menu ==============
    add("FILE_LOAD_ROM", "&Load ROM", "加载 ROM(&L)");
    add("FILE_BOOT_FIRMWARE", "&Boot Firmware", "启动固件(&B)");
    add("FILE_SAVE_STATE", "&Save State", "保存状态(&S)");
    add("FILE_LOAD_STATE", "&Load State", "读取状态(&L)");
    add("FILE_TRIM_ROM", "&Trim ROM", "精简 ROM(&T)");
    add("FILE_CHANGE_SAVE", "&Change Save Type", "更改存档类型(&C)");
    add("FILE_QUIT", "&Quit", "退出(&Q)");

    // ============== System menu ==============
    add("SYSTEM_RESUME", "&Resume", "继续(&R)");
    add("SYSTEM_PAUSE", "&Pause", "暂停(&P)");
    add("SYSTEM_RESTART", "&Restart", "重启(&R)");
    add("SYSTEM_STOP", "&Stop", "停止(&S)");
    add("SYSTEM_ACTION_REPLAY", "&Action Replay", "金手指(&A)");
    add("SYSTEM_ADD_SYSTEM", "&Add System", "添加系统(&A)");

    // ============== Settings submenus ==============
    add("SETTINGS_GENERAL", "&General Settings", "常规设置(&G)");
    add("SETTINGS_GRAPHICS", "&Graphics Settings", "图形设置(&G)");
    add("SETTINGS_AUDIO", "&Audio Settings", "音频设置(&A)");
    add("SETTINGS_EXPERIMENTAL", "&Experimental Settings", "实验性设置(&E)");
    add("SETTINGS_PATH", "&Path Settings", "路径设置(&P)");
    add("SETTINGS_LAYOUT", "&Screen Layout", "屏幕布局(&S)");
    add("SETTINGS_INPUT", "&Input Bindings", "按键绑定(&I)");
    add("SETTINGS_LANGUAGE", "&Language", "语言(&L)");

    // ============== Language options ==============
    add("LANG_ENGLISH", "&English", "英文(&E)");
    add("LANG_CHINESE", "&Chinese", "中文(&C)");

    // ============== General settings ==============
    add("GEN_DIRECT_BOOT", "&Direct Boot", "直接启动(&D)");
    add("GEN_ROM_IN_RAM", "&Keep ROM in RAM", "ROM 常驻内存(&K)");
    add("GEN_FPS_LIMITER", "&FPS Limiter", "帧率限制(&F)");

    // ============== Graphics settings ==============
    add("GFX_SKIP_FRAMES", "&Skip Frames", "跳帧(&S)");
    add("GFX_THREADED_2D", "&Threaded 2D", "2D 多线程(&T)");
    add("GFX_THREADED_3D", "&Threaded 3D", "3D 多线程(&T)");
    add("GFX_HIGH_RES_3D", "&High-Resolution 3D", "3D 高分辨率(&H)");
    add("GFX_SCREEN_GHOST", "Simulate Ghosting", "模拟残影");

    add("FRAMESKIP_NONE", "&None", "无(&N)");
    add("FRAMESKIP_1", "&1 Frame", "1 帧(&1)");
    add("FRAMESKIP_2", "&2 Frames", "2 帧(&2)");
    add("FRAMESKIP_3", "&3 Frames", "3 帧(&3)");
    add("FRAMESKIP_4", "&4 Frames", "4 帧(&4)");
    add("FRAMESKIP_5", "&5 Frames", "5 帧(&5)");

    add("THREADED_3D_DISABLED", "&Disabled", "禁用(&D)");
    add("THREADED_3D_1", "&1 Thread", "1 线程(&1)");
    add("THREADED_3D_2", "&2 Threads", "2 线程(&2)");
    add("THREADED_3D_3", "&3 Threads", "3 线程(&3)");
    add("THREADED_3D_4", "&4 Threads", "4 线程(&4)");

    // ============== Audio settings ==============
    add("AUDIO_EMULATE", "&Audio Emulation", "音频模拟(&A)");
    add("AUDIO_16BIT", "&16-bit Audio Output", "16 位音频输出(&1)");
    add("AUDIO_MIC", "&Use Microphone", "使用麦克风(&U)");

    // ============== Experimental settings ==============
    add("EXP_ARM7_HLE", "&High-Level ARM7", "ARM7 高级模拟(&H)");
    add("EXP_DSI_MODE", "&DSi Homebrew Mode", "DSi 自制模式(&D)");

    // ============== Path dialog ==============
    add("PATH_TITLE", "Path Settings", "路径设置");
    add("PATH_ARM9_BIOS", "ARM9 BIOS:", "ARM9 BIOS:");
    add("PATH_ARM7_BIOS", "ARM7 BIOS:", "ARM7 BIOS:");
    add("PATH_FIRMWARE", "Firmware:", "固件:");
    add("PATH_GBA_BIOS", "GBA BIOS:", "GBA BIOS:");
    add("PATH_SD_IMAGE", "SD Image:", "SD 镜像:");
    add("PATH_SEPARATE_FOLDERS", "Separate Folders For:", "为以下使用单独目录:");
    add("PATH_SAVES", "Saves", "存档");
    add("PATH_STATES", "States", "状态");
    add("PATH_CHEATS", "Cheats", "金手指");
    add("PATH_BROWSE", "Browse", "浏览");
    add("PATH_OPEN_FOLDER", "Open Folder", "打开目录");
    add("PATH_CANCEL", "Cancel", "取消");
    add("PATH_CONFIRM", "Confirm", "确认");
    add("PATH_SELECT_ARM9_BIOS", "Select ARM9 BIOS File", "选择 ARM9 BIOS 文件");
    add("PATH_SELECT_ARM7_BIOS", "Select ARM7 BIOS File", "选择 ARM7 BIOS 文件");
    add("PATH_SELECT_FIRMWARE", "Select Firmware File", "选择固件文件");
    add("PATH_SELECT_GBA_BIOS", "Select GBA BIOS File", "选择 GBA BIOS 文件");
    add("PATH_SELECT_SD_IMAGE", "Select SD Image File", "选择 SD 镜像文件");

    // ============== Layout dialog ==============
    add("LAYOUT_TITLE", "Screen Layout", "屏幕布局");
    add("LAYOUT_POSITION", "Position:", "位置:");
    add("LAYOUT_POSITION_CENTER", "Center", "居中");
    add("LAYOUT_POSITION_TOP", "Top", "顶部");
    add("LAYOUT_POSITION_BOTTOM", "Bottom", "底部");
    add("LAYOUT_POSITION_LEFT", "Left", "左");
    add("LAYOUT_POSITION_RIGHT", "Right", "右");
    add("LAYOUT_ROTATION", "Rotation:", "旋转:");
    add("LAYOUT_ROTATION_NONE", "None", "无");
    add("LAYOUT_ROTATION_CW", "Clockwise", "顺时针");
    add("LAYOUT_ROTATION_CCW", "Counter-Clockwise", "逆时针");
    add("LAYOUT_ARRANGEMENT", "Arrangement:", "排列:");
    add("LAYOUT_ARRANGEMENT_AUTO", "Automatic", "自动");
    add("LAYOUT_ARRANGEMENT_VERT", "Vertical", "垂直");
    add("LAYOUT_ARRANGEMENT_HORI", "Horizontal", "水平");
    add("LAYOUT_ARRANGEMENT_SING", "Single Screen", "单屏");
    add("LAYOUT_SIZING", "Sizing:", "尺寸:");
    add("LAYOUT_SIZING_EVEN", "Even", "均匀");
    add("LAYOUT_SIZING_TOP", "Enlarge Top", "放大上屏");
    add("LAYOUT_SIZING_BOT", "Enlarge Bottom", "放大下屏");
    add("LAYOUT_GAP", "Gap:", "间距:");
    add("LAYOUT_GAP_NONE", "None", "无");
    add("LAYOUT_GAP_QUART", "Quarter", "四分之一");
    add("LAYOUT_GAP_HALF", "Half", "一半");
    add("LAYOUT_GAP_FULL", "Full", "完全");
    add("LAYOUT_FILTER", "Filter:", "滤镜:");
    add("LAYOUT_FILTER_NEAREST", "Nearest", "邻近");
    add("LAYOUT_FILTER_UPSCALE", "Upscaled", "放大优化");
    add("LAYOUT_FILTER_LINEAR", "Linear", "线性");
    add("LAYOUT_ASPECT", "Aspect Ratio:", "宽高比:");
    add("LAYOUT_ASPECT_DEFAULT", "Default", "默认");
    add("LAYOUT_ASPECT_16_10", "16:10", "16:10");
    add("LAYOUT_ASPECT_16_9", "16:9", "16:9");
    add("LAYOUT_ASPECT_18_9", "18:9", "18:9");
    add("LAYOUT_INT_SCALE", "Integer Scale", "整数倍缩放");
    add("LAYOUT_GBA_CROP", "GBA Crop", "GBA 裁剪");
    add("LAYOUT_SPLIT_SCREENS", "Split Screens", "分屏显示");
    add("LAYOUT_CANCEL", "Cancel", "取消");
    add("LAYOUT_CONFIRM", "Confirm", "确认");

    // ============== Input dialog ==============
    add("INPUT_TITLE", "Input Bindings", "按键绑定");
    add("INPUT_BUTTONS_TAB", "&Buttons", "按键(&B)");
    add("INPUT_HOTKEYS_TAB", "&Hotkeys", "热键(&H)");
    add("INPUT_CLEAR", "Clear", "清除");
    add("INPUT_CANCEL", "Cancel", "取消");
    add("INPUT_CONFIRM", "Confirm", "确认");
    add("INPUT_PRESS_KEY", "Press a key", "请按下一个键");

    add("KEY_A", "A:", "A:");
    add("KEY_B", "B:", "B:");
    add("KEY_X", "X:", "X:");
    add("KEY_Y", "Y:", "Y:");
    add("KEY_START", "Start:", "Start:");
    add("KEY_SELECT", "Select:", "Select:");
    add("KEY_UP", "Up:", "上:");
    add("KEY_DOWN", "Down:", "下:");
    add("KEY_LEFT", "Left:", "左:");
    add("KEY_RIGHT", "Right:", "右:");
    add("KEY_L", "L:", "L:");
    add("KEY_R", "R:", "R:");
    add("KEY_FAST_HOLD", "Fast Forward Hold:", "长按快进:");
    add("KEY_FAST_TOGGLE", "Fast Forward Toggle:", "快进切换:");
    add("KEY_FULL_SCREEN", "Full Screen Toggle:", "全屏切换:");
    add("KEY_SCREEN_SWAP", "Screen Swap Toggle:", "屏幕切换:");
    add("KEY_SYSTEM_PAUSE", "System Pause Toggle:", "系统暂停切换:");

    // ============== Cheat dialog ==============
    add("CHEAT_TITLE", "Action Replay Cheats", "金手指作弊码");
    add("CHEAT_ADD", "Add", "添加");
    add("CHEAT_REMOVE", "Remove", "删除");
    add("CHEAT_CANCEL", "Cancel", "取消");
    add("CHEAT_CONFIRM", "Confirm", "确认");
    add("CHEAT_NEW", "New Cheat", "新建金手指");

    // ============== Save dialog ==============
    add("SAVE_TITLE", "Change Save Type", "更改存档类型");
    add("SAVE_NONE", "None", "无");
    add("SAVE_EEPROM_512B", "EEPROM 0.5KB", "EEPROM 0.5KB");
    add("SAVE_EEPROM_8KB", "EEPROM 8KB", "EEPROM 8KB");
    add("SAVE_EEPROM_64KB", "EEPROM 64KB", "EEPROM 64KB");
    add("SAVE_EEPROM_128KB", "EEPROM 128KB", "EEPROM 128KB");
    add("SAVE_SRAM_32KB", "SRAM 32KB", "SRAM 32KB");
    add("SAVE_FRAM_32KB", "FRAM 32KB", "FRAM 32KB");
    add("SAVE_FLASH_64KB", "FLASH 64KB", "FLASH 64KB");
    add("SAVE_FLASH_128KB", "FLASH 128KB", "FLASH 128KB");
    add("SAVE_FLASH_256KB", "FLASH 256KB", "FLASH 256KB");
    add("SAVE_FLASH_512KB", "FLASH 512KB", "FLASH 512KB");
    add("SAVE_FLASH_1024KB", "FLASH 1024KB", "FLASH 1024KB");
    add("SAVE_FLASH_8192KB", "FLASH 8192KB", "FLASH 8192KB");
    add("SAVE_CANCEL", "Cancel", "取消");
    add("SAVE_CONFIRM", "Confirm", "确认");
    add("SAVE_CHANGE_WARNING", "Are you sure? This may result in data loss!", "确定要更改吗？这可能会导致存档丢失！");
    add("SAVE_CHANGE_TITLE", "Changing Save Type", "更改存档类型");

    // ============== Error messages ==============
    add("ERR_BIOS_TITLE", "Error Loading BIOS", "BIOS 加载错误");
    add("ERR_BIOS_MSG", "Make sure the path settings point to valid BIOS files and try again.", "请确保路径设置指向有效的 BIOS 文件，然后重试。");
    add("ERR_FIRM_TITLE", "Error Loading Firmware", "固件加载错误");
    add("ERR_FIRM_MSG", "Make sure the path settings point to a bootable firmware file or try another boot method.", "请确保路径设置指向可启动的固件文件或尝试其他启动方式。");
    add("ERR_ROM_TITLE", "Error Loading ROM", "ROM 加载错误");
    add("ERR_ROM_MSG", "Make sure the ROM file is accessible and try again.", "请确保 ROM 文件可访问，然后重试。");

    // ============== ROM operations ==============
    add("ROM_SELECT_FILE", "Select ROM File", "选择 ROM 文件");
    add("ROM_FILTER", "NDS/GBA ROM files (*.nds, *.gba)|*.nds;*.gba", "NDS/GBA ROM 文件 (*.nds, *.gba)|*.nds;*.gba");
    add("ROM_LOADING_NDS_TITLE", "Loading NDS ROM", "加载 NDS ROM");
    add("ROM_LOADING_NDS_MSG", "Load the current GBA ROM alongside this ROM?", "是否同时加载当前的 GBA ROM？");
    add("ROM_LOADING_GBA_TITLE", "Loading GBA ROM", "加载 GBA ROM");
    add("ROM_LOADING_GBA_MSG", "Load the current NDS ROM alongside this ROM?", "是否同时加载当前的 NDS ROM？");
    add("ROM_TRIM_TITLE", "Trimming ROM", "精简 ROM");
    add("ROM_TRIM_MSG", "Trim the current ROM to save space?", "是否精简当前 ROM 以节省空间？");
    add("ROM_TRIM_RESULT", "ROM trimmed from %.2fMB to %.2fMB!", "ROM 已从 %.2fMB 精简到 %.2fMB！");
    add("ROM_TRIM_NOCHANGE", "This ROM is already trimmed!", "该 ROM 已精简过！");
    add("ROM_TRIMMED", "ROM Trimmed", "ROM 已精简");

    // ============== Save state ==============
    add("STATE_SAVE_TITLE", "Save State", "保存状态");
    add("STATE_SAVE_NEW", "Saving and loading states is dangerous and can lead to data loss. States are also not guaranteed to be compatible across emulator versions. Please rely on in-game saving to keep your progress, and back up .sav files before using this feature. Do you want to save the current state?", "保存和读取状态存在风险，可能导致数据丢失。状态文件不保证在不同模拟器版本之间兼容。请使用游戏内存档来保存进度，并使用此功能前备份 .sav 文件。是否保存当前状态？");
    add("STATE_SAVE_OVERWRITE", "Do you want to overwrite the saved state with the current state? This can't be undone!", "是否用当前状态覆盖已保存状态？此操作无法撤销！");
    add("STATE_LOAD_TITLE", "Load State", "读取状态");
    add("STATE_LOAD_CONFIRM", "Do you want to load the saved state and lose the current state? This can't be undone!", "是否读取已保存状态并丢失当前状态？此操作无法撤销！");
    add("STATE_LOAD_FILE_FAIL_TITLE", "Error", "错误");
    add("STATE_LOAD_FILE_FAIL", "The state file doesn't exist or couldn't be opened.", "状态文件不存在或无法打开。");
    add("STATE_LOAD_FORMAT_FAIL", "The state file doesn't have a valid format.", "状态文件格式无效。");
    add("STATE_LOAD_VERSION_FAIL", "The state file isn't compatible with this version of NooDS.", "状态文件与此版本的 NooDS 不兼容。");

    // ============== Language change ==============
    add("LANG_RESTART_TITLE", "Restart Required", "需要重启");
    add("LANG_RESTART_MSG", "The language change will take effect after restart. Do you want to restart now?", "语言更改将在重启后生效。是否立即重启？");
    add("LANG_RESTART_NOW", "Restart Now", "立即重启");
    add("LANG_RESTART_LATER", "Later", "稍后");

    // ============== File filters ==============
    add("FILTER_BIN", "Binary files (*.bin)|*.bin", "二进制文件 (*.bin)|*.bin");
    add("FILTER_IMG", "Image files (*.img)|*.img", "镜像文件 (*.img)|*.img");

    // ============== Key labels for keyToString ==============
    add("KEY_NONE", "None", "无");
    add("KEY_BACKSPACE", "Backspace", "退格");
    add("KEY_TAB", "Tab", "Tab");
    add("KEY_RETURN", "Return", "回车");
    add("KEY_ESCAPE", "Escape", "Esc");
    add("KEY_SPACE", "Space", "空格");
    add("KEY_DELETE", "Delete", "Delete");
    add("KEY_START", "Start", "Start");
    add("KEY_LBUTTON", "Left Button", "鼠标左键");
    add("KEY_RBUTTON", "Right Button", "鼠标右键");
    add("KEY_CANCEL", "Cancel", "取消");
    add("KEY_MBUTTON", "Middle Button", "鼠标中键");
    add("KEY_CLEAR", "Clear", "清除");
    add("KEY_SHIFT", "Shift", "Shift");
    add("KEY_ALT", "Alt", "Alt");
    add("KEY_CONTROL", "Control", "Ctrl");
    add("KEY_MENU", "Menu", "菜单");
    add("KEY_PAUSE", "Pause", "暂停");
    add("KEY_CAPITAL", "Caps Lock", "大写锁定");
    add("KEY_END", "End", "End");
    add("KEY_HOME", "Home", "Home");
    add("KEY_LEFT", "Left", "左");
    add("KEY_UP", "Up", "上");
    add("KEY_RIGHT", "Right", "右");
    add("KEY_DOWN", "Down", "下");
    add("KEY_SELECT", "Select", "Select");
    add("KEY_PRINT", "Print", "打印");
    add("KEY_EXECUTE", "Execute", "执行");
    add("KEY_SNAPSHOT", "Snapshot", "截屏");
    add("KEY_INSERT", "Insert", "Insert");
    add("KEY_HELP", "Help", "帮助");
    add("KEY_NUMPAD", "Numpad ", "数字键盘 ");
    add("KEY_F", "F", "F");
    add("KEY_NUMLOCK", "Numlock", "数字锁定");
    add("KEY_SCROLL", "Scroll", "滚动锁定");
    add("KEY_PAGEUP", "Page Up", "上页");
    add("KEY_PAGEDOWN", "Page Down", "下页");
    add("KEY_MULTIPLY", "Multiply", "乘");
    add("KEY_ADD", "Add", "加");
    add("KEY_SEPARATOR", "Separator", "分隔符");
    add("KEY_SUBTRACT", "Subtract", "减");
    add("KEY_DECIMAL", "Decimal", "小数点");
    add("KEY_DIVIDE", "Divide", "除");
    add("KEY_AXIS", "Axis ", "轴 ");
    add("KEY_BUTTON", "Button ", "按键 ");
    add("KEY_PLUS", " +", " +");
    add("KEY_MINUS", " -", " -");
}