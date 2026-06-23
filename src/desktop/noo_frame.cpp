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
#include <wx/stdpaths.h>

#include "noo_frame.h"
#include "cheat_dialog.h"
#include "input_dialog.h"
#include "layout_dialog.h"
#include "noo_app.h"
#include "noo_canvas.h"
#include "path_dialog.h"
#include "save_dialog.h"
#include "../i18n/i18n.h"
#include "../settings.h"
#include "../../icon/icon.xpm"

enum FrameEvent {
    LOAD_ROM = 1,
    BOOT_FIRMWARE,
    SAVE_STATE,
    LOAD_STATE,
    TRIM_ROM,
    CHANGE_SAVE,
    QUIT,
    PAUSE,
    RESTART,
    STOP,
    ACTION_REPLAY,
    ADD_SYSTEM,
    DIRECT_BOOT,
    ROM_IN_RAM,
    FPS_LIMITER,
    FRAMESKIP_0,
    FRAMESKIP_1,
    FRAMESKIP_2,
    FRAMESKIP_3,
    FRAMESKIP_4,
    FRAMESKIP_5,
    THREADED_2D,
    THREADED_3D_0,
    THREADED_3D_1,
    THREADED_3D_2,
    THREADED_3D_3,
    THREADED_3D_4,
    HIGH_RES_3D,
    SCREEN_GHOST,
    EMULATE_AUDIO,
    AUDIO_16_BIT,
    MIC_ENABLE,
    ARM7_HLE,
    DSI_MODE,
    PATH_SETTINGS,
    SCREEN_LAYOUT,
    INPUT_BINDINGS,
    LANGUAGE_ENGLISH,
    LANGUAGE_CHINESE,
    UPDATE_JOY
};

wxBEGIN_EVENT_TABLE(NooFrame, wxFrame)
EVT_MENU(LOAD_ROM, NooFrame::loadRom)
EVT_MENU(BOOT_FIRMWARE, NooFrame::bootFirmware)
EVT_MENU(SAVE_STATE, NooFrame::saveState)
EVT_MENU(LOAD_STATE, NooFrame::loadState)
EVT_MENU(TRIM_ROM, NooFrame::trimRom)
EVT_MENU(CHANGE_SAVE, NooFrame::changeSave)
EVT_MENU(QUIT, NooFrame::quit)
EVT_MENU(PAUSE, NooFrame::pause)
EVT_MENU(RESTART, NooFrame::restart)
EVT_MENU(STOP, NooFrame::stop)
EVT_MENU(ACTION_REPLAY, NooFrame::actionReplay)
EVT_MENU(ADD_SYSTEM, NooFrame::addSystem)
EVT_MENU(DIRECT_BOOT, NooFrame::directBoot)
EVT_MENU(ROM_IN_RAM, NooFrame::romInRam)
EVT_MENU(FPS_LIMITER, NooFrame::fpsLimiter)
EVT_MENU(FRAMESKIP_0, NooFrame::frameskip<0>)
EVT_MENU(FRAMESKIP_1, NooFrame::frameskip<1>)
EVT_MENU(FRAMESKIP_2, NooFrame::frameskip<2>)
EVT_MENU(FRAMESKIP_3, NooFrame::frameskip<3>)
EVT_MENU(FRAMESKIP_4, NooFrame::frameskip<4>)
EVT_MENU(FRAMESKIP_5, NooFrame::frameskip<5>)
EVT_MENU(THREADED_2D, NooFrame::threaded2D)
EVT_MENU(THREADED_3D_0, NooFrame::threaded3D<0>)
EVT_MENU(THREADED_3D_1, NooFrame::threaded3D<1>)
EVT_MENU(THREADED_3D_2, NooFrame::threaded3D<2>)
EVT_MENU(THREADED_3D_3, NooFrame::threaded3D<3>)
EVT_MENU(THREADED_3D_4, NooFrame::threaded3D<4>)
EVT_MENU(HIGH_RES_3D, NooFrame::highRes3D)
EVT_MENU(SCREEN_GHOST, NooFrame::screenGhost)
EVT_MENU(EMULATE_AUDIO, NooFrame::emulateAudio)
EVT_MENU(AUDIO_16_BIT, NooFrame::audio16Bit)
EVT_MENU(MIC_ENABLE, NooFrame::micEnable)
EVT_MENU(ARM7_HLE, NooFrame::arm7Hle)
EVT_MENU(DSI_MODE, NooFrame::dsiMode)
EVT_MENU(PATH_SETTINGS, NooFrame::pathSettings)
EVT_MENU(SCREEN_LAYOUT, NooFrame::layoutSettings)
EVT_MENU(INPUT_BINDINGS, NooFrame::inputSettings)
EVT_MENU(LANGUAGE_ENGLISH, NooFrame::languageEnglish)
EVT_MENU(LANGUAGE_CHINESE, NooFrame::languageChinese)
EVT_TIMER(UPDATE_JOY, NooFrame::updateJoystick)
EVT_DROP_FILES(NooFrame::dropFiles)
EVT_CLOSE(NooFrame::close)
wxEND_EVENT_TABLE()

NooFrame::NooFrame(NooApp *app, int id, std::string path, NooFrame *partner):
    wxFrame(nullptr, wxID_ANY, "NooDS"), app(app), id(id), partner(partner), mainFrame(!partner) {
    // Set the icon
    wxIcon icon(icon_xpm);
    SetIcon(icon);

    if (mainFrame) {
        // Set up the file menu
        fileMenu = new wxMenu();
        fileMenu->Append(LOAD_ROM, TR_I18N("FILE_LOAD_ROM"));
        fileMenu->Append(BOOT_FIRMWARE, TR_I18N("FILE_BOOT_FIRMWARE"));
        fileMenu->AppendSeparator();
        fileMenu->Append(SAVE_STATE, TR_I18N("FILE_SAVE_STATE"));
        fileMenu->Append(LOAD_STATE, TR_I18N("FILE_LOAD_STATE"));
        fileMenu->AppendSeparator();
        fileMenu->Append(TRIM_ROM, TR_I18N("FILE_TRIM_ROM"));
        fileMenu->Append(CHANGE_SAVE, TR_I18N("FILE_CHANGE_SAVE"));
        fileMenu->AppendSeparator();
        fileMenu->Append(QUIT, TR_I18N("FILE_QUIT"));

        // Set up the system menu
        systemMenu = new wxMenu();
        systemMenu->Append(PAUSE, TR_I18N("SYSTEM_RESUME"));
        systemMenu->Append(RESTART, TR_I18N("SYSTEM_RESTART"));
        systemMenu->Append(STOP, TR_I18N("SYSTEM_STOP"));
        systemMenu->AppendSeparator();
        systemMenu->Append(ACTION_REPLAY, TR_I18N("SYSTEM_ACTION_REPLAY"));
        systemMenu->Append(ADD_SYSTEM, TR_I18N("SYSTEM_ADD_SYSTEM"));

        // Disable some menu items until the core is running
        fileMenu->Enable(TRIM_ROM, false);
        fileMenu->Enable(CHANGE_SAVE, false);
        fileMenu->Enable(SAVE_STATE, false);
        fileMenu->Enable(LOAD_STATE, false);
        systemMenu->Enable(PAUSE, false);
        systemMenu->Enable(RESTART, false);
        systemMenu->Enable(STOP, false);
        systemMenu->Enable(ACTION_REPLAY, false);

        // Set up the skip frames submenu
        wxMenu *frameskip = new wxMenu();
        frameskip->AppendRadioItem(FRAMESKIP_0, TR_I18N("FRAMESKIP_NONE"));
        frameskip->AppendRadioItem(FRAMESKIP_1, TR_I18N("FRAMESKIP_1"));
        frameskip->AppendRadioItem(FRAMESKIP_2, TR_I18N("FRAMESKIP_2"));
        frameskip->AppendRadioItem(FRAMESKIP_3, TR_I18N("FRAMESKIP_3"));
        frameskip->AppendRadioItem(FRAMESKIP_4, TR_I18N("FRAMESKIP_4"));
        frameskip->AppendRadioItem(FRAMESKIP_5, TR_I18N("FRAMESKIP_5"));

        // Set up the threaded 3D submenu
        wxMenu *threaded3D = new wxMenu();
        threaded3D->AppendRadioItem(THREADED_3D_0, TR_I18N("THREADED_3D_DISABLED"));
        threaded3D->AppendRadioItem(THREADED_3D_1, TR_I18N("THREADED_3D_1"));
        threaded3D->AppendRadioItem(THREADED_3D_2, TR_I18N("THREADED_3D_2"));
        threaded3D->AppendRadioItem(THREADED_3D_3, TR_I18N("THREADED_3D_3"));
        threaded3D->AppendRadioItem(THREADED_3D_4, TR_I18N("THREADED_3D_4"));

        // Set up the general settings submenu
        wxMenu *generalMenu = new wxMenu();
        generalMenu->AppendCheckItem(DIRECT_BOOT, TR_I18N("GEN_DIRECT_BOOT"));
        generalMenu->AppendCheckItem(ROM_IN_RAM, TR_I18N("GEN_ROM_IN_RAM"));
        generalMenu->AppendCheckItem(FPS_LIMITER, TR_I18N("GEN_FPS_LIMITER"));

        // Set up the graphics settings submenu
        wxMenu *graphicsMenu = new wxMenu();
        graphicsMenu->AppendSubMenu(frameskip, TR_I18N("GFX_SKIP_FRAMES"));
        graphicsMenu->AppendCheckItem(THREADED_2D, TR_I18N("GFX_THREADED_2D"));
        graphicsMenu->AppendSubMenu(threaded3D, TR_I18N("GFX_THREADED_3D"));
        graphicsMenu->AppendCheckItem(HIGH_RES_3D, TR_I18N("GFX_HIGH_RES_3D"));
        graphicsMenu->AppendCheckItem(SCREEN_GHOST, TR_I18N("GFX_SCREEN_GHOST"));

        // Set up the audio settings submenu
        wxMenu *audioMenu = new wxMenu();
        audioMenu->AppendCheckItem(EMULATE_AUDIO, TR_I18N("AUDIO_EMULATE"));
        audioMenu->AppendCheckItem(AUDIO_16_BIT, TR_I18N("AUDIO_16BIT"));
        audioMenu->AppendCheckItem(MIC_ENABLE, TR_I18N("AUDIO_MIC"));

        // Set up the experimental settings submenu
        wxMenu *experiMenu = new wxMenu();
        experiMenu->AppendCheckItem(ARM7_HLE, TR_I18N("EXP_ARM7_HLE"));
        experiMenu->AppendCheckItem(DSI_MODE, TR_I18N("EXP_DSI_MODE"));

        // Set up the language submenu
        wxMenu *languageMenu = new wxMenu();
        languageMenu->AppendRadioItem(LANGUAGE_ENGLISH, TR_I18N("LANG_ENGLISH"));
        languageMenu->AppendRadioItem(LANGUAGE_CHINESE, TR_I18N("LANG_CHINESE"));
        // Mark the current language as selected
        if (Settings::language == I18n::LANG_ZH)
            languageMenu->Check(LANGUAGE_CHINESE, true);
        else
            languageMenu->Check(LANGUAGE_ENGLISH, true);

        // Set up the settings menu
        wxMenu *settingsMenu = new wxMenu();
        settingsMenu->AppendSubMenu(generalMenu, TR_I18N("SETTINGS_GENERAL"));
        settingsMenu->AppendSubMenu(graphicsMenu, TR_I18N("SETTINGS_GRAPHICS"));
        settingsMenu->AppendSubMenu(audioMenu, TR_I18N("SETTINGS_AUDIO"));
        settingsMenu->AppendSubMenu(experiMenu, TR_I18N("SETTINGS_EXPERIMENTAL"));
        settingsMenu->AppendSeparator();
        settingsMenu->Append(PATH_SETTINGS, TR_I18N("SETTINGS_PATH"));
        settingsMenu->Append(SCREEN_LAYOUT, TR_I18N("SETTINGS_LAYOUT"));
        settingsMenu->Append(INPUT_BINDINGS, TR_I18N("SETTINGS_INPUT"));
        settingsMenu->AppendSubMenu(languageMenu, TR_I18N("SETTINGS_LANGUAGE"));

        // Set the initial settings checkbox states
        settingsMenu->Check(DIRECT_BOOT, Settings::directBoot);
        settingsMenu->Check(ROM_IN_RAM, Settings::romInRam);
        settingsMenu->Check(FPS_LIMITER, Settings::fpsLimiter);
        settingsMenu->Check(THREADED_2D, Settings::threaded2D);
        settingsMenu->Check(HIGH_RES_3D, Settings::highRes3D);
        settingsMenu->Check(SCREEN_GHOST, Settings::screenGhost);
        settingsMenu->Check(EMULATE_AUDIO, Settings::emulateAudio);
        settingsMenu->Check(AUDIO_16_BIT, Settings::audio16Bit);
        settingsMenu->Check(MIC_ENABLE, NooApp::micEnable);
        settingsMenu->Check(ARM7_HLE, Settings::arm7Hle);
        settingsMenu->Check(DSI_MODE, Settings::dsiMode);

        // Set the initial radio setting selections
        frameskip->Check(FRAMESKIP_0 + std::min<uint8_t>(Settings::frameskip, 5), true);
        threaded3D->Check(THREADED_3D_0 + std::min<uint8_t>(Settings::threaded3D, 4), true);

        // Set up the menu bar
        wxMenuBar *menuBar = new wxMenuBar();
        menuBar->Append(fileMenu, TR_I18N("MENU_FILE"));
        menuBar->Append(systemMenu, TR_I18N("MENU_SYSTEM"));
        if (id == 0) // Only show settings in the main instance
            menuBar->Append(settingsMenu, TR_I18N("MENU_SETTINGS"));
        SetMenuBar(menuBar);
    }

    // Set the initial window size based on the current screen layout
    ScreenLayout layout;
    layout.update(0, 0, false, NooApp::splitScreens && ScreenLayout::screenArrangement != 3);
    SetClientSize(wxSize(layout.minWidth, layout.minHeight));

    // Prepare and show the window
    DragAcceptFiles(true);
    SetBackgroundColour(*wxBLACK);
    Centre();
    Show(true);

    // Create and add a canvas for drawing the framebuffer
    canvas = new NooCanvas(this);
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(canvas, 1, wxEXPAND);
    SetSizer(sizer);

    // Prepare a joystick if one is connected
    joystick = new wxJoystick();
    if (joystick->IsOk()) {
        // Save the initial axis values so inputs can be detected as offsets instead of raw values
        // This avoids issues with axes that have non-zero values in their resting positions
        for (int i = 0; i < joystick->GetNumberAxes(); i++)
            axisBases.push_back(joystick->GetPosition(i));

        // Start a timer to update joystick input, since wxJoystickEvents are unreliable
        timer = new wxTimer(this, UPDATE_JOY);
        timer->Start(10);
    }
    else {
        // Don't use a joystick if one isn't connected
        delete joystick;
        joystick = nullptr;
        timer = nullptr;
    }

    // Load a filename passed through the command line
    if (path != "")
        loadRomPath(path);
}

void NooFrame::Refresh() {
    // Override the refresh function to also update the FPS counter
    wxFrame::Refresh();
    wxString label = "NooDS";
    if (id > 0) label += wxString::Format(" (%d)", id + 1);
    if (running) label += wxString::Format(" - %d FPS", core->fps);
    SetLabel(label);

    // Manage the main frame's partner frame
    if (mainFrame) {
        bool split = NooApp::splitScreens && ScreenLayout::screenArrangement != 3 && !canvas->gbaMode;
        if (split && !partner) {
            // Create the partner frame if needed
            partner = new NooFrame(app, id, "", this);
            partner->core = core;
            partner->running = running;
        }
        else if (!split && partner) {
            // Remove the partner frame if not needed
            delete partner;
            partner = nullptr;
        }
        else if (partner) {
            // Update the partner frame
            partner->Refresh();
        }
    }
}

void NooFrame::runCore() {
    // Run the emulator
    while (running)
        core->runCore();
}

void NooFrame::checkSave() {
    while (running) {
        // Check save files every few seconds and update them if changed
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait_for(lock, std::chrono::seconds(3), [&]{ return !running; });
        core->cartridgeNds.writeSave();
        core->cartridgeGba.writeSave();
    }
}

void NooFrame::startCore(bool full) {
    if (full) {
        // Ensure the core is shut down
        stopCore(true);

        try {
            // Attempt to boot the core
            core = new Core(ndsPath, gbaPath, id);
            if (partner) partner->core = core;
            app->connectCore(id);
        }
        catch (CoreError e) {
            // Inform the user of the error if loading wasn't successful
            switch (e) {
            case ERROR_BIOS: // Missing BIOS files
                wxMessageDialog(this, TR_I18N("ERR_BIOS_MSG"),
                    TR_I18N("ERR_BIOS_TITLE"), wxICON_NONE).ShowModal();
                return;

            case ERROR_FIRM: // Non-bootable firmware file
                wxMessageDialog(this, TR_I18N("ERR_FIRM_MSG"),
                    TR_I18N("ERR_FIRM_TITLE"), wxICON_NONE).ShowModal();
                return;

            case ERROR_ROM: // Unreadable ROM file
                wxMessageDialog(this, TR_I18N("ERR_ROM_MSG"),
                    TR_I18N("ERR_ROM_TITLE"), wxICON_NONE).ShowModal();
                return;
            }
        }
    }

    if (core) {
        // Enable some file menu items if a ROM is loaded
        if (ndsPath != "" || gbaPath != "") {
            fileMenu->Enable(TRIM_ROM, true);
            fileMenu->Enable(CHANGE_SAVE, true);
            fileMenu->Enable(SAVE_STATE, true);
            fileMenu->Enable(LOAD_STATE, true);
        }

        // Update the system menu for running
        systemMenu->SetLabel(PAUSE, TR_I18N("SYSTEM_PAUSE"));
        systemMenu->Enable(PAUSE, true);
        systemMenu->Enable(RESTART, true);
        systemMenu->Enable(STOP, true);
        systemMenu->Enable(ACTION_REPLAY, ndsPath != "");

        // Start the threads
        running = true;
        if (partner) partner->running = running;
        coreThread = new std::thread(&NooFrame::runCore, this);
        saveThread = new std::thread(&NooFrame::checkSave, this);
    }
}

void NooFrame::stopCore(bool full) {
    { // Signal for the threads to stop if the core is running
        std::lock_guard<std::mutex> guard(mutex);
        running = false;
        if (partner) partner->running = running;
        cond.notify_one();
    }

    // Wait for the core thread to stop
    if (coreThread) {
        coreThread->join();
        delete coreThread;
        coreThread = nullptr;
    }

    // Wait for the save thread to stop
    if (saveThread) {
        saveThread->join();
        delete saveThread;
        saveThread = nullptr;
    }

    // Update the system menu for being paused
    systemMenu->SetLabel(PAUSE, TR_I18N("SYSTEM_RESUME"));

    if (full) {
        // Disable some menu items
        fileMenu->Enable(TRIM_ROM, false);
        fileMenu->Enable(CHANGE_SAVE, false);
        fileMenu->Enable(SAVE_STATE, false);
        fileMenu->Enable(LOAD_STATE, false);
        systemMenu->Enable(PAUSE, false);
        systemMenu->Enable(RESTART, false);
        systemMenu->Enable(STOP, false);

        // Shut down the core
        if (core) {
            app->disconnCore(id);
            delete core;
            core = nullptr;
            if (partner) partner->core = core;
        }
    }
}

void NooFrame::pressKey(int key) {
    // Handle a key press separate from the key's actual mapping
    switch (key) {
    case 12: // Fast Forward Hold
        // Disable the FPS limiter
        if (Settings::fpsLimiter != 0) {
            fpsLimiterBackup = Settings::fpsLimiter;
            Settings::fpsLimiter = 0;
        }
        break;

    case 13: // Fast Forward Toggle
        // Toggle the FPS limiter on or off
        if (!(hotkeyToggles & BIT(0))) {
            if (Settings::fpsLimiter != 0) {
                // Disable the FPS limiter
                fpsLimiterBackup = Settings::fpsLimiter;
                Settings::fpsLimiter = 0;
            }
            else if (fpsLimiterBackup != 0) {
                // Restore the previous FPS limiter setting
                Settings::fpsLimiter = fpsLimiterBackup;
                fpsLimiterBackup = 0;
            }

            hotkeyToggles |= BIT(0);
        }
        break;

    case 14: // Full Screen Toggle
        // Toggle full screen mode
        ShowFullScreen(fullScreen = !fullScreen);
        if (!fullScreen) canvas->resetFrame();
        break;

    case 15: // Screen Swap Toggle
        // Toggle between favoring the top or bottom screen
        if (!(hotkeyToggles & BIT(2))) {
            ScreenLayout::screenSizing = (ScreenLayout::screenSizing == 1) ? 2 : 1;
            app->updateLayouts();
            hotkeyToggles |= BIT(2);
        }
        break;

    case 16: // System Pause Toggle
        // Toggle between pausing or resuming the core
        if (!(hotkeyToggles & BIT(3))) {
            running ? stopCore(false) : startCore(false);
            hotkeyToggles |= BIT(3);
        }
        break;

    default: // Core input
        // Send a key press to the core
        if (running)
            core->input.pressKey(key);
        break;
    }
}

void NooFrame::releaseKey(int key) {
    // Handle a key release separate from the key's actual mapping
    switch (key) {
    case 12: // Fast Forward Hold
        // Restore the previous FPS limiter setting
        if (fpsLimiterBackup != 0) {
            Settings::fpsLimiter = fpsLimiterBackup;
            fpsLimiterBackup = 0;
        }
        break;

    case 13: // Fast Forward Toggle
    case 15: // Screen Swap Toggle
    case 16: // System Pause Toggle
        // Clear a toggle bit so a hotkey can be used again
        hotkeyToggles &= ~BIT(key - 13);
        break;

    default: // Core input
        // Send a key release to the core
        if (running)
            core->input.releaseKey(key);
        break;
    }
}

void NooFrame::loadRomPath(std::string path) {
    // Set the NDS or GBA ROM path depending on the extension of the given file
    // If a ROM of the other type is already loaded, ask if it should be loaded alongside the new ROM
    if (path.find(".nds", path.length() - 4) != std::string::npos) { // NDS ROM
        if (gbaPath != "") {
            wxMessageDialog dialog(this, TR_I18N("ROM_LOADING_NDS_MSG"), TR_I18N("ROM_LOADING_NDS_TITLE"), wxYES_NO | wxICON_NONE);
            if (dialog.ShowModal() != wxID_YES) gbaPath = "";
        }
        ndsPath = path;
    }
    else if (path.find(".gba", path.length() - 4) != std::string::npos) { // GBA ROM
        if (ndsPath != "") {
            wxMessageDialog dialog(this, TR_I18N("ROM_LOADING_GBA_MSG"), TR_I18N("ROM_LOADING_GBA_TITLE"), wxYES_NO | wxICON_NONE);
            if (dialog.ShowModal() != wxID_YES) ndsPath = "";
        }
        gbaPath = path;
    }
    else {
        return;
    }

    // Restart the core
    startCore(true);
}

void NooFrame::loadRom(wxCommandEvent &event) {
    // Show the file browser
    wxFileDialog romSelect(this, TR_I18N("ROM_SELECT_FILE"), "", "", TR_I18N("ROM_FILTER"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (romSelect.ShowModal() != wxID_CANCEL)
        loadRomPath((const char*)romSelect.GetPath().mb_str(wxConvUTF8));
}

void NooFrame::bootFirmware(wxCommandEvent &event) {
    // Start the core with no ROM
    ndsPath = "";
    gbaPath = "";
    startCore(true);
}

void NooFrame::trimRom(wxCommandEvent &event) {
    bool gba = core->gbaMode;

    // Confirm that the current ROM should be trimmed
    wxMessageDialog dialog(this, TR_I18N("ROM_TRIM_MSG"), TR_I18N("ROM_TRIM_TITLE"), wxYES_NO | wxICON_NONE);
    if (dialog.ShowModal() == wxID_YES) {
        int oldSize, newSize;

        // Pause the core for safety and trim the ROM
        stopCore(false);
        Cartridge *cartridge = gba ? (Cartridge*)&core->cartridgeGba : (Cartridge*)&core->cartridgeNds;
        oldSize = cartridge->getRomSize();
        cartridge->trimRom();
        newSize = cartridge->getRomSize();
        startCore(false);

        // Show the results
        wxString str;
        if (oldSize != newSize)
            str.Printf(TR_I18N("ROM_TRIM_RESULT"), (float)oldSize / 1024 / 1024, (float)newSize / 1024 / 1024);
        else
            str = TR_I18N("ROM_TRIM_NOCHANGE");
        wxMessageDialog(this, str, TR_I18N("ROM_TRIMMED"), wxICON_NONE).ShowModal();
    }
}

void NooFrame::changeSave(wxCommandEvent &event) {
    // Show the save dialog
    SaveDialog saveDialog(this);
    saveDialog.ShowModal();
}

void NooFrame::saveState(wxCommandEvent &event) {
    // Create a confirmation dialog, with extra information if a state file doesn't exist yet
    wxMessageDialog *dialog;
    switch (core->saveStates.checkState()) {
    case STATE_FILE_FAIL:
        dialog = new wxMessageDialog(this, TR_I18N("STATE_SAVE_NEW"),
            TR_I18N("STATE_SAVE_TITLE"), wxYES_NO | wxICON_NONE);
        break;

    default:
        dialog = new wxMessageDialog(this, TR_I18N("STATE_SAVE_OVERWRITE"),
            TR_I18N("STATE_SAVE_TITLE"), wxYES_NO | wxICON_NONE);
        break;
    }

    // Show the dialog and save the state if confirmed
    if (dialog->ShowModal() == wxID_YES) {
        stopCore(false);
        core->saveStates.saveState();
        startCore(false);
    }
    delete dialog;
}

void NooFrame::loadState(wxCommandEvent &event) {
    // Create a confirmation dialog, or an error if something went wrong
    wxMessageDialog *dialog;
    switch (core->saveStates.checkState()) {
    case STATE_SUCCESS:
        dialog = new wxMessageDialog(this, TR_I18N("STATE_LOAD_CONFIRM"),
            TR_I18N("STATE_LOAD_TITLE"), wxYES_NO | wxICON_NONE);
        break;

    case STATE_FILE_FAIL:
        dialog = new wxMessageDialog(this, TR_I18N("STATE_LOAD_FILE_FAIL"),
            TR_I18N("STATE_LOAD_FILE_FAIL_TITLE"), wxICON_NONE);
        break;

    case STATE_FORMAT_FAIL:
        dialog = new wxMessageDialog(this, TR_I18N("STATE_LOAD_FORMAT_FAIL"),
            TR_I18N("STATE_LOAD_FILE_FAIL_TITLE"), wxICON_NONE);
        break;

    case STATE_VERSION_FAIL:
        dialog = new wxMessageDialog(this, TR_I18N("STATE_LOAD_VERSION_FAIL"),
            TR_I18N("STATE_LOAD_FILE_FAIL_TITLE"), wxICON_NONE);
        break;
    }

    // Show the dialog and load the state if confirmed
    if (dialog->ShowModal() == wxID_YES) {
        stopCore(false);
        core->saveStates.loadState();
        startCore(false);
    }
    delete dialog;
}

void NooFrame::quit(wxCommandEvent &event) {
    // Close the program
    Close(true);
}

void NooFrame::pause(wxCommandEvent &event) {
    // Pause or resume the core
    if (running)
        stopCore(false);
    else
        startCore(false);
}

void NooFrame::restart(wxCommandEvent &event) {
    // Restart the core
    startCore(true);
}

void NooFrame::stop(wxCommandEvent &event) {
    // Stop the core
    stopCore(true);
}

void NooFrame::actionReplay(wxCommandEvent &event) {
    // Show the AR cheats dialog
    CheatDialog cheatDialog(core);
    cheatDialog.ShowModal();
}

void NooFrame::addSystem(wxCommandEvent &event) {
    // Create a new emulator instance
    app->createFrame();
}

void NooFrame::directBoot(wxCommandEvent &event) {
    // Toggle the direct boot setting
    Settings::directBoot = !Settings::directBoot;
    Settings::save();
}

void NooFrame::romInRam(wxCommandEvent &event) {
    // Toggle the ROM in RAM setting
    Settings::romInRam = !Settings::romInRam;
    Settings::save();
}

void NooFrame::fpsLimiter(wxCommandEvent &event) {
    // Toggle the FPS limiter setting
    Settings::fpsLimiter = !Settings::fpsLimiter;
    Settings::save();
}

template <int value> void NooFrame::frameskip(wxCommandEvent &event) {
    // Set the skip frames setting
    Settings::frameskip = value;
    Settings::save();
}

void NooFrame::threaded2D(wxCommandEvent &event) {
    // Toggle the threaded 2D setting
    Settings::threaded2D = !Settings::threaded2D;
    Settings::save();
}

template <int value> void NooFrame::threaded3D(wxCommandEvent &event) {
    // Set the threaded 3D setting
    Settings::threaded3D = value;
    Settings::save();
}

void NooFrame::highRes3D(wxCommandEvent &event) {
    // Toggle the high-resolution 3D setting
    Settings::highRes3D = !Settings::highRes3D;
    Settings::save();
}

void NooFrame::screenGhost(wxCommandEvent &event) {
    // Toggle the simulate ghosting setting
    Settings::screenGhost = !Settings::screenGhost;
    Settings::save();
}

void NooFrame::emulateAudio(wxCommandEvent &event) {
    // Toggle the audio emulation setting
    Settings::emulateAudio = !Settings::emulateAudio;
    Settings::save();
}

void NooFrame::audio16Bit(wxCommandEvent &event) {
    // Toggle the 16-bit audio output setting
    Settings::audio16Bit = !Settings::audio16Bit;
    Settings::save();
}

void NooFrame::micEnable(wxCommandEvent &event) {
    // Toggle the use microphone setting
    NooApp::micEnable = !NooApp::micEnable;
    NooApp::micEnable ? app->startStream(1) : app->stopStream(1);
    Settings::save();
}

void NooFrame::arm7Hle(wxCommandEvent &event) {
    // Toggle the high-level ARM7 setting
    Settings::arm7Hle = !Settings::arm7Hle;
    Settings::save();
}

void NooFrame::dsiMode(wxCommandEvent &event) {
    // Toggle the DSi homebrew mode setting
    Settings::dsiMode = !Settings::dsiMode;
    Settings::save();
}

void NooFrame::pathSettings(wxCommandEvent &event) {
    // Show the path settings dialog
    PathDialog pathDialog;
    pathDialog.ShowModal();
}

void NooFrame::layoutSettings(wxCommandEvent &event) {
    // Show the layout settings dialog
    LayoutDialog layoutDialog(app);
    layoutDialog.ShowModal();
}

void NooFrame::inputSettings(wxCommandEvent &event) {
    // Pause joystick updates and show the input settings dialog
    if (timer) timer->Stop();
    InputDialog inputDialog(joystick);
    inputDialog.ShowModal();
    if (timer) timer->Start(10);
}

void NooFrame::languageEnglish(wxCommandEvent &event) {
    // Switch the interface language to English
    applyLanguageChange(I18n::LANG_EN, true);
}

void NooFrame::languageChinese(wxCommandEvent &event) {
    // Switch the interface language to Chinese
    applyLanguageChange(I18n::LANG_ZH, true);
}

void NooFrame::updateJoystick(wxTimerEvent &event) {
    // Check the status of mapped joystick inputs and trigger key presses and releases accordingly
    for (int i = 0; i < MAX_KEYS; i++) {
        if (NooApp::keyBinds[i] >= 3000 && joystick->GetNumberAxes() > NooApp::keyBinds[i] - 3000) { // Axis -
            if (joystick->GetPosition(NooApp::keyBinds[i] - 3000) - axisBases[NooApp::keyBinds[i] - 3000] < -16384)
                pressKey(i);
            else
                releaseKey(i);
        }
        else if (NooApp::keyBinds[i] >= 2000 && joystick->GetNumberAxes() > NooApp::keyBinds[i] - 2000) { // Axis +
            if (joystick->GetPosition(NooApp::keyBinds[i] - 2000) - axisBases[NooApp::keyBinds[i] - 2000] > 16384)
                pressKey(i);
            else
                releaseKey(i);
        }
        else if (NooApp::keyBinds[i] >= 1000 && joystick->GetNumberButtons() > NooApp::keyBinds[i] - 1000) { // Button
            if (joystick->GetButtonState(NooApp::keyBinds[i] - 1000))
                pressKey(i);
            else
                releaseKey(i);
        }
    }
}

void NooFrame::dropFiles(wxDropFilesEvent &event) {
    // Load a single dropped file
    if (event.GetNumberOfFiles() != 1) return;
    wxString path = event.GetFiles()[0];
    if (!wxFileExists(path)) return;
    (mainFrame ? this : partner)->loadRomPath((const char*)path.mb_str(wxConvUTF8));
}

void NooFrame::close(wxCloseEvent &event) {
    // Properly shut down the emulator
    (mainFrame ? this : partner)->stopCore(true);
    app->removeFrame(id);
    canvas->finish();
    if (partner) delete partner;
    event.Skip(true);
}

void NooFrame::applyLanguageChange(int newLang, bool interactive) {
    // Apply a new language selection. Because all visible menu/dialog strings
    // are translated once at startup, the change is persisted to the settings
    // file and the user is prompted to restart so the new strings are loaded.
    if (newLang == Settings::language)
        return;

    Settings::language = newLang;
    Settings::save();

    if (!interactive)
        return;

    wxMessageDialog dialog(this, TR_I18N("LANG_RESTART_MSG"),
        TR_I18N("LANG_RESTART_TITLE"), wxYES_NO | wxICON_NONE);
    if (dialog.ShowModal() == wxID_YES) {
        // Relaunch the current executable with the original command-line
        // arguments (e.g. ROM path) and then close the running process so the
        // new language takes effect.
        wxString exe = wxStandardPaths::Get().GetExecutablePath();
        if (!exe.IsEmpty()) {
            wxString cmd = wxString::Format("\"%s\"", exe);
            for (int i = 1; i < wxGetApp().argc; i++)
                cmd += wxString::Format(" \"%s\"", wxGetApp().argv[i]);
            wxExecute(cmd);
        }
        Close(true);
    }
}
