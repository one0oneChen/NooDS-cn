/*
    Copyright 2019-2025 Hydr8gon

    This file is part of NooDS.

    NooDS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NooDS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NooDS. If not, see <https://www.gnu.org/licenses/>.
*/

#include <wx/notebook.h>

#include "input_dialog.h"
#include "../i18n/i18n.h"
#include "../settings.h"

enum InputEvent {
    REMAP_A = 1,
    REMAP_B,
    REMAP_X,
    REMAP_Y,
    REMAP_START,
    REMAP_SELECT,
    REMAP_UP,
    REMAP_DOWN,
    REMAP_LEFT,
    REMAP_RIGHT,
    REMAP_L,
    REMAP_R,
    REMAP_FAST_HOLD,
    REMAP_FAST_TOGGLE,
    REMAP_FULL_SCREEN,
    REMAP_SCREEN_SWAP,
    REMAP_SYSTEM_PAUSE,
    CLEAR_MAP,
    UPDATE_JOY
};

wxBEGIN_EVENT_TABLE(InputDialog, wxDialog)
EVT_BUTTON(REMAP_A, InputDialog::remapA)
EVT_BUTTON(REMAP_B, InputDialog::remapB)
EVT_BUTTON(REMAP_X, InputDialog::remapX)
EVT_BUTTON(REMAP_Y, InputDialog::remapY)
EVT_BUTTON(REMAP_START, InputDialog::remapStart)
EVT_BUTTON(REMAP_SELECT, InputDialog::remapSelect)
EVT_BUTTON(REMAP_UP, InputDialog::remapUp)
EVT_BUTTON(REMAP_DOWN, InputDialog::remapDown)
EVT_BUTTON(REMAP_LEFT, InputDialog::remapLeft)
EVT_BUTTON(REMAP_RIGHT, InputDialog::remapRight)
EVT_BUTTON(REMAP_L, InputDialog::remapL)
EVT_BUTTON(REMAP_R, InputDialog::remapR)
EVT_BUTTON(REMAP_FAST_HOLD, InputDialog::remapFastHold)
EVT_BUTTON(REMAP_FAST_TOGGLE, InputDialog::remapFastToggle)
EVT_BUTTON(REMAP_FULL_SCREEN, InputDialog::remapFullScreen)
EVT_BUTTON(REMAP_SCREEN_SWAP, InputDialog::remapScreenSwap)
EVT_BUTTON(REMAP_SYSTEM_PAUSE, InputDialog::remapSystemPause)
EVT_BUTTON(CLEAR_MAP, InputDialog::clearMap)
EVT_TIMER(UPDATE_JOY, InputDialog::updateJoystick)
EVT_BUTTON(wxID_OK, InputDialog::confirm)
EVT_CHAR_HOOK(InputDialog::pressKey)
wxEND_EVENT_TABLE()

std::string InputDialog::keyToString(int key) {
    // Handle joystick keys
    // 1000, 2000, and 3000 are offsets assigned to these keys to identify them and hopefully avoid conflict
    if (key >= 3000) {
        std::string prefix = I18n::instance().ts("KEY_AXIS");
        std::string suffix = I18n::instance().ts("KEY_MINUS");
        return prefix + std::to_string(key - 3000) + suffix;
    }
    else if (key >= 2000) {
        std::string prefix = I18n::instance().ts("KEY_AXIS");
        std::string suffix = I18n::instance().ts("KEY_PLUS");
        return prefix + std::to_string(key - 2000) + suffix;
    }
    else if (key >= 1000) {
        std::string prefix = I18n::instance().ts("KEY_BUTTON");
        return prefix + std::to_string(key - 1000);
    }

    // Convert special keys to words representing their respective keys
    switch (key) {
        case 0: return I18n::instance().ts("KEY_NONE");
        case WXK_BACK: return I18n::instance().ts("KEY_BACKSPACE");
        case WXK_TAB: return I18n::instance().ts("KEY_TAB");
        case WXK_RETURN: return I18n::instance().ts("KEY_RETURN");
        case WXK_ESCAPE: return I18n::instance().ts("KEY_ESCAPE");
        case WXK_SPACE: return I18n::instance().ts("KEY_SPACE");
        case WXK_DELETE: return I18n::instance().ts("KEY_DELETE");
        case WXK_START: return I18n::instance().ts("KEY_START");
        case WXK_LBUTTON: return I18n::instance().ts("KEY_LBUTTON");
        case WXK_RBUTTON: return I18n::instance().ts("KEY_RBUTTON");
        case WXK_CANCEL: return I18n::instance().ts("KEY_CANCEL");
        case WXK_MBUTTON: return I18n::instance().ts("KEY_MBUTTON");
        case WXK_CLEAR: return I18n::instance().ts("KEY_CLEAR");
        case WXK_SHIFT: return I18n::instance().ts("KEY_SHIFT");
        case WXK_ALT: return I18n::instance().ts("KEY_ALT");
        case WXK_RAW_CONTROL: return I18n::instance().ts("KEY_CONTROL");
        case WXK_MENU: return I18n::instance().ts("KEY_MENU");
        case WXK_PAUSE: return I18n::instance().ts("KEY_PAUSE");
        case WXK_CAPITAL: return I18n::instance().ts("KEY_CAPITAL");
        case WXK_END: return I18n::instance().ts("KEY_END");
        case WXK_HOME: return I18n::instance().ts("KEY_HOME");
        case WXK_LEFT: return I18n::instance().ts("KEY_LEFT");
        case WXK_UP: return I18n::instance().ts("KEY_UP");
        case WXK_RIGHT: return I18n::instance().ts("KEY_RIGHT");
        case WXK_DOWN: return I18n::instance().ts("KEY_DOWN");
        case WXK_SELECT: return I18n::instance().ts("KEY_SELECT");
        case WXK_PRINT: return I18n::instance().ts("KEY_PRINT");
        case WXK_EXECUTE: return I18n::instance().ts("KEY_EXECUTE");
        case WXK_SNAPSHOT: return I18n::instance().ts("KEY_SNAPSHOT");
        case WXK_INSERT: return I18n::instance().ts("KEY_INSERT");
        case WXK_HELP: return I18n::instance().ts("KEY_HELP");
        case WXK_NUMPAD0: return I18n::instance().ts("KEY_NUMPAD") + std::string("0");
        case WXK_NUMPAD1: return I18n::instance().ts("KEY_NUMPAD") + std::string("1");
        case WXK_NUMPAD2: return I18n::instance().ts("KEY_NUMPAD") + std::string("2");
        case WXK_NUMPAD3: return I18n::instance().ts("KEY_NUMPAD") + std::string("3");
        case WXK_NUMPAD4: return I18n::instance().ts("KEY_NUMPAD") + std::string("4");
        case WXK_NUMPAD5: return I18n::instance().ts("KEY_NUMPAD") + std::string("5");
        case WXK_NUMPAD6: return I18n::instance().ts("KEY_NUMPAD") + std::string("6");
        case WXK_NUMPAD7: return I18n::instance().ts("KEY_NUMPAD") + std::string("7");
        case WXK_NUMPAD8: return I18n::instance().ts("KEY_NUMPAD") + std::string("8");
        case WXK_NUMPAD9: return I18n::instance().ts("KEY_NUMPAD") + std::string("9");
        case WXK_MULTIPLY: return I18n::instance().ts("KEY_MULTIPLY");
        case WXK_ADD: return I18n::instance().ts("KEY_ADD");
        case WXK_SEPARATOR: return I18n::instance().ts("KEY_SEPARATOR");
        case WXK_SUBTRACT: return I18n::instance().ts("KEY_SUBTRACT");
        case WXK_DECIMAL: return I18n::instance().ts("KEY_DECIMAL");
        case WXK_DIVIDE: return I18n::instance().ts("KEY_DIVIDE");
        case WXK_F1: return I18n::instance().ts("KEY_F") + std::string("1");
        case WXK_F2: return I18n::instance().ts("KEY_F") + std::string("2");
        case WXK_F3: return I18n::instance().ts("KEY_F") + std::string("3");
        case WXK_F4: return I18n::instance().ts("KEY_F") + std::string("4");
        case WXK_F5: return I18n::instance().ts("KEY_F") + std::string("5");
        case WXK_F6: return I18n::instance().ts("KEY_F") + std::string("6");
        case WXK_F7: return I18n::instance().ts("KEY_F") + std::string("7");
        case WXK_F8: return I18n::instance().ts("KEY_F") + std::string("8");
        case WXK_F9: return I18n::instance().ts("KEY_F") + std::string("9");
        case WXK_F10: return I18n::instance().ts("KEY_F") + std::string("10");
        case WXK_F11: return I18n::instance().ts("KEY_F") + std::string("11");
        case WXK_F12: return I18n::instance().ts("KEY_F") + std::string("12");
        case WXK_F13: return I18n::instance().ts("KEY_F") + std::string("13");
        case WXK_F14: return I18n::instance().ts("KEY_F") + std::string("14");
        case WXK_F15: return I18n::instance().ts("KEY_F") + std::string("15");
        case WXK_F16: return I18n::instance().ts("KEY_F") + std::string("16");
        case WXK_F17: return I18n::instance().ts("KEY_F") + std::string("17");
        case WXK_F18: return I18n::instance().ts("KEY_F") + std::string("18");
        case WXK_F19: return I18n::instance().ts("KEY_F") + std::string("19");
        case WXK_F20: return I18n::instance().ts("KEY_F") + std::string("20");
        case WXK_F21: return I18n::instance().ts("KEY_F") + std::string("21");
        case WXK_F22: return I18n::instance().ts("KEY_F") + std::string("22");
        case WXK_F23: return I18n::instance().ts("KEY_F") + std::string("23");
        case WXK_F24: return I18n::instance().ts("KEY_F") + std::string("24");
        case WXK_NUMLOCK: return I18n::instance().ts("KEY_NUMLOCK");
        case WXK_SCROLL: return I18n::instance().ts("KEY_SCROLL");
        case WXK_PAGEUP: return I18n::instance().ts("KEY_PAGEUP");
        case WXK_PAGEDOWN: return I18n::instance().ts("KEY_PAGEDOWN");
        case WXK_NUMPAD_SPACE: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_SPACE");
        case WXK_NUMPAD_TAB: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_TAB");
        case WXK_NUMPAD_ENTER: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_RETURN");
        case WXK_NUMPAD_F1: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_F") + std::string("1");
        case WXK_NUMPAD_F2: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_F") + std::string("2");
        case WXK_NUMPAD_F3: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_F") + std::string("3");
        case WXK_NUMPAD_F4: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_F") + std::string("4");
        case WXK_NUMPAD_HOME: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_HOME");
        case WXK_NUMPAD_LEFT: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_LEFT");
        case WXK_NUMPAD_UP: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_UP");
        case WXK_NUMPAD_RIGHT: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_RIGHT");
        case WXK_NUMPAD_DOWN: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_DOWN");
        case WXK_NUMPAD_PAGEUP: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_PAGEUP");
        case WXK_NUMPAD_PAGEDOWN: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_PAGEDOWN");
        case WXK_NUMPAD_END: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_END");
        case WXK_NUMPAD_BEGIN: return I18n::instance().ts("KEY_NUMPAD") + "Begin";
        case WXK_NUMPAD_INSERT: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_INSERT");
        case WXK_NUMPAD_DELETE: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_DELETE");
        case WXK_NUMPAD_EQUAL: return I18n::instance().ts("KEY_NUMPAD") + "=";
        case WXK_NUMPAD_MULTIPLY: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_MULTIPLY");
        case WXK_NUMPAD_ADD: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_ADD");
        case WXK_NUMPAD_SEPARATOR: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_SEPARATOR");
        case WXK_NUMPAD_SUBTRACT: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_SUBTRACT");
        case WXK_NUMPAD_DECIMAL: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_DECIMAL");
        case WXK_NUMPAD_DIVIDE: return I18n::instance().ts("KEY_NUMPAD") + I18n::instance().ts("KEY_DIVIDE");
    }

    // Directly use the key character for regular keys
    std::string regular;
    regular = (char)key;
    return regular;
}

InputDialog::InputDialog(wxJoystick *joystick): wxDialog(nullptr, wxID_ANY, TR_I18N("INPUT_TITLE")), joystick(joystick) {
    // Load the key bindings
    memcpy(keyBinds, NooApp::keyBinds, sizeof(keyBinds));

    // Determine the height of a button
    // Borders are measured in pixels, so this value can be used to make values that scale with the DPI/font size
    wxButton *dummy = new wxButton(this, wxID_ANY, "");
    int size = dummy->GetSize().y;
    delete dummy;

    // Create separate tabs for buttons and hotkeys
    wxNotebook *notebook = new wxNotebook(this, wxID_ANY);
    wxPanel* buttonTab = new wxPanel(notebook, wxID_ANY);
    wxPanel* hotkeyTab = new wxPanel(notebook, wxID_ANY);
    notebook->AddPage(buttonTab, TR_I18N("INPUT_BUTTONS_TAB"));
    notebook->AddPage(hotkeyTab, TR_I18N("INPUT_HOTKEYS_TAB"));

    // Set up the A button setting
    wxBoxSizer *aSizer = new wxBoxSizer(wxHORIZONTAL);
    aSizer->Add(new wxStaticText(buttonTab, wxID_ANY, TR_I18N("KEY_A")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    aSizer->Add(keyA = new wxButton(buttonTab, REMAP_A, keyToString(keyBinds[0]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the B button setting
    wxBoxSizer *bSizer = new wxBoxSizer(wxHORIZONTAL);
    bSizer->Add(new wxStaticText(buttonTab, wxID_ANY, TR_I18N("KEY_B")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    bSizer->Add(keyB = new wxButton(buttonTab, REMAP_B, keyToString(keyBinds[1]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the X button setting
    wxBoxSizer *xSizer = new wxBoxSizer(wxHORIZONTAL);
    xSizer->Add(new wxStaticText(buttonTab, wxID_ANY, TR_I18N("KEY_X")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    xSizer->Add(keyX = new wxButton(buttonTab, REMAP_X, keyToString(keyBinds[10]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the Y button setting
    wxBoxSizer *ySizer = new wxBoxSizer(wxHORIZONTAL);
    ySizer->Add(new wxStaticText(buttonTab, wxID_ANY, TR_I18N("KEY_Y")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    ySizer->Add(keyY = new wxButton(buttonTab, REMAP_Y, keyToString(keyBinds[11]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the Start button setting
    wxBoxSizer *startSizer = new wxBoxSizer(wxHORIZONTAL);
    startSizer->Add(new wxStaticText(buttonTab, wxID_ANY, TR_I18N("KEY_START")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    startSizer->Add(keyStart = new wxButton(buttonTab, REMAP_START, keyToString(keyBinds[3]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the Select button setting
    wxBoxSizer *selectSizer = new wxBoxSizer(wxHORIZONTAL);
    selectSizer->Add(new wxStaticText(buttonTab, wxID_ANY, TR_I18N("KEY_SELECT")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    selectSizer->Add(keySelect = new wxButton(buttonTab, REMAP_SELECT, keyToString(keyBinds[2]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the Up button setting
    wxBoxSizer *upSizer = new wxBoxSizer(wxHORIZONTAL);
    upSizer->Add(new wxStaticText(buttonTab, wxID_ANY, TR_I18N("KEY_UP")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    upSizer->Add(keyUp = new wxButton(buttonTab, REMAP_UP, keyToString(keyBinds[6]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the Down button setting
    wxBoxSizer *downSizer = new wxBoxSizer(wxHORIZONTAL);
    downSizer->Add(new wxStaticText(buttonTab, wxID_ANY, TR_I18N("KEY_DOWN")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    downSizer->Add(keyDown = new wxButton(buttonTab, REMAP_DOWN, keyToString(keyBinds[7]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the Left button setting
    wxBoxSizer *leftSizer = new wxBoxSizer(wxHORIZONTAL);
    leftSizer->Add(new wxStaticText(buttonTab, wxID_ANY, TR_I18N("KEY_LEFT")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    leftSizer->Add(keyLeft = new wxButton(buttonTab, REMAP_LEFT, keyToString(keyBinds[5]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the Right button setting
    wxBoxSizer *rightSizer = new wxBoxSizer(wxHORIZONTAL);
    rightSizer->Add(new wxStaticText(buttonTab, wxID_ANY, TR_I18N("KEY_RIGHT")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    rightSizer->Add(keyRight = new wxButton(buttonTab, REMAP_RIGHT, keyToString(keyBinds[4]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the L button setting
    wxBoxSizer *lSizer = new wxBoxSizer(wxHORIZONTAL);
    lSizer->Add(new wxStaticText(buttonTab, wxID_ANY, TR_I18N("KEY_L")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    lSizer->Add(keyL = new wxButton(buttonTab, REMAP_L, keyToString(keyBinds[9]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the R button setting
    wxBoxSizer *rSizer = new wxBoxSizer(wxHORIZONTAL);
    rSizer->Add(new wxStaticText(buttonTab, wxID_ANY, TR_I18N("KEY_R")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    rSizer->Add(keyR = new wxButton(buttonTab, REMAP_R, keyToString(keyBinds[8]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Combine all of the left button tab contents
    wxBoxSizer *leftContents = new wxBoxSizer(wxVERTICAL);
    leftContents->Add(aSizer, 1, wxEXPAND | wxALL, size / 8);
    leftContents->Add(bSizer, 1, wxEXPAND | wxALL, size / 8);
    leftContents->Add(xSizer, 1, wxEXPAND | wxALL, size / 8);
    leftContents->Add(ySizer, 1, wxEXPAND | wxALL, size / 8);
    leftContents->Add(startSizer, 1, wxEXPAND | wxALL, size / 8);
    leftContents->Add(selectSizer, 1, wxEXPAND | wxALL, size / 8);

    // Combine all of the right button tab contents
    wxBoxSizer *rightContents = new wxBoxSizer(wxVERTICAL);
    rightContents->Add(upSizer, 1, wxEXPAND | wxALL, size / 8);
    rightContents->Add(downSizer, 1, wxEXPAND | wxALL, size / 8);
    rightContents->Add(leftSizer, 1, wxEXPAND | wxALL, size / 8);
    rightContents->Add(rightSizer, 1, wxEXPAND | wxALL, size / 8);
    rightContents->Add(lSizer, 1, wxEXPAND | wxALL, size / 8);
    rightContents->Add(rSizer, 1, wxEXPAND | wxALL, size / 8);

    // Combine the button tab contents and add a final border around it
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(leftContents, 1, wxEXPAND | wxALL, size / 8);
    buttonSizer->Add(rightContents, 1, wxEXPAND | wxALL, size / 8);
    buttonTab->SetSizer(buttonSizer);

    // Set up the fast forward hold hotkey setting
    wxBoxSizer *fastHoldSizer = new wxBoxSizer(wxHORIZONTAL);
    fastHoldSizer->Add(new wxStaticText(hotkeyTab, wxID_ANY, TR_I18N("KEY_FAST_HOLD")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    fastHoldSizer->Add(keyFastHold = new wxButton(hotkeyTab, REMAP_FAST_HOLD, keyToString(keyBinds[12]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the fast forward toggle hotkey setting
    wxBoxSizer *fastToggleSizer = new wxBoxSizer(wxHORIZONTAL);
    fastToggleSizer->Add(new wxStaticText(hotkeyTab, wxID_ANY, TR_I18N("KEY_FAST_TOGGLE")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    fastToggleSizer->Add(keyFastToggle = new wxButton(hotkeyTab, REMAP_FAST_TOGGLE, keyToString(keyBinds[13]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the full screen toggle hotkey setting
    wxBoxSizer *fullScreenSizer = new wxBoxSizer(wxHORIZONTAL);
    fullScreenSizer->Add(new wxStaticText(hotkeyTab, wxID_ANY, TR_I18N("KEY_FULL_SCREEN")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    fullScreenSizer->Add(keyFullScreen = new wxButton(hotkeyTab, REMAP_FULL_SCREEN, keyToString(keyBinds[14]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the screen swap toggle hotkey setting
    wxBoxSizer *screenSwapSizer = new wxBoxSizer(wxHORIZONTAL);
    screenSwapSizer->Add(new wxStaticText(hotkeyTab, wxID_ANY, TR_I18N("KEY_SCREEN_SWAP")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    screenSwapSizer->Add(keyScreenSwap = new wxButton(hotkeyTab, REMAP_SCREEN_SWAP, keyToString(keyBinds[15]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Set up the system pause toggle hotkey setting
    wxBoxSizer *systemPauseSizer = new wxBoxSizer(wxHORIZONTAL);
    systemPauseSizer->Add(new wxStaticText(hotkeyTab, wxID_ANY, TR_I18N("KEY_SYSTEM_PAUSE")), 1, wxALIGN_CENTRE | wxRIGHT, size / 16);
    systemPauseSizer->Add(keySystemPause = new wxButton(hotkeyTab, REMAP_SYSTEM_PAUSE, keyToString(keyBinds[16]), wxDefaultPosition, wxSize(size * 4, size)), 0, wxLEFT, size / 16);

    // Combine all of the hotkey tab contents
    wxBoxSizer *hotkeyContents = new wxBoxSizer(wxVERTICAL);
    hotkeyContents->Add(fastHoldSizer, 1, wxEXPAND | wxALL, size / 8);
    hotkeyContents->Add(fastToggleSizer, 1, wxEXPAND | wxALL, size / 8);
    hotkeyContents->Add(fullScreenSizer, 1, wxEXPAND | wxALL, size / 8);
    hotkeyContents->Add(screenSwapSizer, 1, wxEXPAND | wxALL, size / 8);
    hotkeyContents->Add(systemPauseSizer, 1, wxEXPAND | wxALL, size / 8);
    hotkeyContents->Add(new wxStaticText(hotkeyTab, wxID_ANY, ""), 1);

    // Add a final border around the hotkey tab
    wxBoxSizer *hotkeySizer = new wxBoxSizer(wxHORIZONTAL);
    hotkeySizer->Add(hotkeyContents, 1, wxEXPAND | wxALL, size / 8);
    hotkeyTab->SetSizer(hotkeySizer);

    // Set up the common navigation buttons
    wxBoxSizer *naviSizer = new wxBoxSizer(wxHORIZONTAL);
    naviSizer->Add(new wxStaticText(this, wxID_ANY, ""), 1);
    naviSizer->Add(new wxButton(this, CLEAR_MAP, TR_I18N("INPUT_CLEAR")), 0, wxRIGHT, size / 16);
    naviSizer->Add(new wxButton(this, wxID_CANCEL, TR_I18N("INPUT_CANCEL")), 0, wxLEFT | wxRIGHT, size / 16);
    naviSizer->Add(new wxButton(this, wxID_OK, TR_I18N("INPUT_CONFIRM")), 0, wxLEFT, size / 16);

    // Populate the dialog
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(notebook, 1, wxEXPAND);
    sizer->Add(naviSizer, 0, wxEXPAND | wxALL, size / 8);
    SetSizerAndFit(sizer);

    // Size the window to prevent resizing
    SetMinSize(GetSize());
    SetMaxSize(GetSize());

    // Set up joystick input if a joystick is connected
    if (joystick) {
        // Save the initial axis values so inputs can be detected as offsets instead of raw values
        // This avoids issues with axes that have non-zero values in their resting positions
        for (int i = 0; i < joystick->GetNumberAxes(); i++)
            axisBases.push_back(joystick->GetPosition(i));

        // Start a timer to update joystick input, since wxJoystickEvents are unreliable
        timer = new wxTimer(this, UPDATE_JOY);
        timer->Start(10);
    }
}

InputDialog::~InputDialog() {
    // Clean up the joystick timer
    if (joystick)
        delete timer;
}

void InputDialog::resetLabels() {
    // Reset the button labels
    keyA->SetLabel(keyToString(keyBinds[0]));
    keyB->SetLabel(keyToString(keyBinds[1]));
    keyX->SetLabel(keyToString(keyBinds[10]));
    keyY->SetLabel(keyToString(keyBinds[11]));
    keyStart->SetLabel(keyToString(keyBinds[3]));
    keySelect->SetLabel(keyToString(keyBinds[2]));
    keyUp->SetLabel(keyToString(keyBinds[6]));
    keyDown->SetLabel(keyToString(keyBinds[7]));
    keyLeft->SetLabel(keyToString(keyBinds[5]));
    keyRight->SetLabel(keyToString(keyBinds[4]));
    keyL->SetLabel(keyToString(keyBinds[9]));
    keyR->SetLabel(keyToString(keyBinds[8]));
    keyFastHold->SetLabel(keyToString(keyBinds[12]));
    keyFastToggle->SetLabel(keyToString(keyBinds[13]));
    keyFullScreen->SetLabel(keyToString(keyBinds[14]));
    keyScreenSwap->SetLabel(keyToString(keyBinds[15]));
    keySystemPause->SetLabel(keyToString(keyBinds[16]));
    current = nullptr;
}

const char* const PRESS_A_KEY = "INPUT_PRESS_KEY";

void InputDialog::remapA(wxCommandEvent &event) {
    // Prepare the A button for remapping
    resetLabels();
    keyA->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyA;
    keyIndex = 0;
}

void InputDialog::remapB(wxCommandEvent &event) {
    // Prepare the B button for remapping
    resetLabels();
    keyB->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyB;
    keyIndex = 1;
}

void InputDialog::remapX(wxCommandEvent &event) {
    // Prepare the X button for remapping
    resetLabels();
    keyX->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyX;
    keyIndex = 10;
}

void InputDialog::remapY(wxCommandEvent &event) {
    // Prepare the Y button for remapping
    resetLabels();
    keyY->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyY;
    keyIndex = 11;
}

void InputDialog::remapStart(wxCommandEvent &event) {
    // Prepare the Start button for remapping
    resetLabels();
    keyStart->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyStart;
    keyIndex = 3;
}

void InputDialog::remapSelect(wxCommandEvent &event) {
    // Prepare the Select button for remapping
    resetLabels();
    keySelect->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keySelect;
    keyIndex = 2;
}

void InputDialog::remapUp(wxCommandEvent &event) {
    // Prepare the Up button for remapping
    resetLabels();
    keyUp->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyUp;
    keyIndex = 6;
}

void InputDialog::remapDown(wxCommandEvent &event) {
    // Prepare the Down button for remapping
    resetLabels();
    keyDown->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyDown;
    keyIndex = 7;
}

void InputDialog::remapLeft(wxCommandEvent &event) {
    // Prepare the Left button for remapping
    resetLabels();
    keyLeft->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyLeft;
    keyIndex = 5;
}

void InputDialog::remapRight(wxCommandEvent &event) {
    // Prepare the Right button for remapping
    resetLabels();
    keyRight->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyRight;
    keyIndex = 4;
}

void InputDialog::remapL(wxCommandEvent &event) {
    // Prepare the L button for remapping
    resetLabels();
    keyL->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyL;
    keyIndex = 9;
}

void InputDialog::remapR(wxCommandEvent &event) {
    // Prepare the R button for remapping
    resetLabels();
    keyR->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyR;
    keyIndex = 8;
}

void InputDialog::remapFastHold(wxCommandEvent &event) {
    // Prepare the fast forward hold hotkey for remapping
    resetLabels();
    keyFastHold->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyFastHold;
    keyIndex = 12;
}

void InputDialog::remapFastToggle(wxCommandEvent &event) {
    // Prepare the fast forward toggle hotkey for remapping
    resetLabels();
    keyFastToggle->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyFastToggle;
    keyIndex = 13;
}

void InputDialog::remapFullScreen(wxCommandEvent &event) {
    // Prepare the full screen toggle hotkey for remapping
    resetLabels();
    keyFullScreen->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyFullScreen;
    keyIndex = 14;
}

void InputDialog::remapScreenSwap(wxCommandEvent &event) {
    // Prepare the screen swap toggle hotkey for remapping
    resetLabels();
    keyScreenSwap->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keyScreenSwap;
    keyIndex = 15;
}

void InputDialog::remapSystemPause(wxCommandEvent &event) {
    // Prepare the system pause toggle hotkey for remapping
    resetLabels();
    keySystemPause->SetLabel(TR_I18N(PRESS_A_KEY));
    current = keySystemPause;
    keyIndex = 16;
}

void InputDialog::clearMap(wxCommandEvent &event) {
    if (current) {
        // If a button is selected, clear only its mapping
        keyBinds[keyIndex] = 0;
        current->SetLabel(keyToString(keyBinds[keyIndex]));
        current = nullptr;
    }
    else {
        // If no button is selected, clear all mappings
        for (int i = 0; i < MAX_KEYS; i++)
            keyBinds[i] = 0;
        resetLabels();
    }
}

void InputDialog::updateJoystick(wxTimerEvent &event) {
    // Map the current button to a joystick button if one is pressed
    if (!current) return;
    for (int i = 0; i < joystick->GetNumberButtons(); i++) {
        if (joystick->GetButtonState(i)) {
            keyBinds[keyIndex] = 1000 + i;
            current->SetLabel(keyToString(keyBinds[keyIndex]));
            current = nullptr;
            return;
        }
    }

    // Map the current button to a joystick axis if one is held
    for (int i = 0; i < joystick->GetNumberAxes(); i++) {
        if (joystick->GetPosition(i) - axisBases[i] > 16384) { // Positive axis
            keyBinds[keyIndex] = 2000 + i;
            current->SetLabel(keyToString(keyBinds[keyIndex]));
            current = nullptr;
            return;
        }
        else if (joystick->GetPosition(i) - axisBases[i] < -16384) { // Negative axis
            keyBinds[keyIndex] = 3000 + i;
            current->SetLabel(keyToString(keyBinds[keyIndex]));
            current = nullptr;
            return;
        }
    }
}

void InputDialog::confirm(wxCommandEvent &event) {
    // Save the key mappings
    memcpy(NooApp::keyBinds, keyBinds, sizeof(keyBinds));
    Settings::save();
    event.Skip(true);
}

void InputDialog::pressKey(wxKeyEvent &event) {
    // Map the current button to the pressed key
    if (!current) return;
    keyBinds[keyIndex] = event.GetKeyCode();
    current->SetLabel(keyToString(keyBinds[keyIndex]));
    current = nullptr;
}
