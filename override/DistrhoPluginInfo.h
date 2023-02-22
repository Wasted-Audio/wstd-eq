/** Copyright (c) Wasted Audio 2023 - GPL-3.0-or-later */

#pragma once

#define DISTRHO_PLUGIN_NAME                 "WSTD EQ"
#define DISTRHO_PLUGIN_URI                  "https://wasted.audio/software/wstd_eq"
#define DISTRHO_PLUGIN_CLAP_ID              "audio.wasted.wstd_eq"
#define DISTRHO_PLUGIN_NUM_INPUTS           2
#define DISTRHO_PLUGIN_NUM_OUTPUTS          2
#define DISTRHO_PLUGIN_IS_SYNTH             0
#define DISTRHO_PLUGIN_HAS_UI               1
#define DISTRHO_PLUGIN_IS_RT_SAFE           1
#define DISTRHO_PLUGIN_WANT_PROGRAMS        0
#define DISTRHO_PLUGIN_WANT_STATE           0
#define DISTRHO_PLUGIN_WANT_TIMEPOS         1
#define DISTRHO_PLUGIN_WANT_FULL_STATE      0
#define DISTRHO_PLUGIN_WANT_MIDI_INPUT      0
#define DISTRHO_PLUGIN_WANT_MIDI_OUTPUT     0
#define DISTRHO_PLUGIN_LV2_CATEGORY         "lv2:EQPlugin"
#define DISTRHO_PLUGIN_VST3_CATEGORIES      "Fx|EQ|Stereo"
#define DISTRHO_PLUGIN_CLAP_FEATURES        "audio-effect", "equalizer", "stereo"

// for level monitoring
#define DISTRHO_PLUGIN_WANT_DIRECT_ACCESS   0

#define DISTRHO_UI_USE_CUSTOM               1
#define DISTRHO_UI_CUSTOM_INCLUDE_PATH      "DearImGui.hpp"
#define DISTRHO_UI_CUSTOM_WIDGET_TYPE       DGL_NAMESPACE::ImGuiTopLevelWidget
#define DISTRHO_UI_DEFAULT_WIDTH            120
#define DISTRHO_UI_DEFAULT_HEIGHT           520

#ifdef __MOD_DEVICES__
#undef DISTRHO_PLUGIN_USES_MODGUI
#define DISTRHO_PLUGIN_USES_MODGUI 1
#endif
