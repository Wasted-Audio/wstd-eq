/**
 * Copyright (c) Wasted Audio 2023 - GPL-3.0-or-later
 */

#include "DistrhoUI.hpp"
#ifdef DISTRHO_OS_WASM
#include "DistrhoStandaloneUtils.hpp"
#endif
#include "ResizeHandle.hpp"
#include "veramobd.hpp"
#include "wstdcolors.hpp"


START_NAMESPACE_DISTRHO

// --------------------------------------------------------------------------------------------------------------------

class ImGuiPluginUI : public UI
{
    float ffreq = 1337.0f;
    float fhigh = 0.0f;
    float flow = 0.0f;
    float fmid = 0.0f;

    ResizeHandle fResizeHandle;

    // ----------------------------------------------------------------------------------------------------------------

public:
   /**
      UI class constructor.
      The UI should be initialized to a default state that matches the plugin side.
    */
    ImGuiPluginUI()
        : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true),
          fResizeHandle(this)
    {
        setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true);

        ImGuiIO& io(ImGui::GetIO());

        ImFontConfig fc;
        fc.FontDataOwnedByAtlas = true;
        fc.OversampleH = 1;
        fc.OversampleV = 1;
        fc.PixelSnapH = true;

        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 16.0f * getScaleFactor(), &fc);
        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 21.0f * getScaleFactor(), &fc);
        io.Fonts->Build();
        io.FontDefault = io.Fonts->Fonts[1];

        fResizeHandle.hide();
    }

protected:
    // ----------------------------------------------------------------------------------------------------------------
    // DSP/Plugin Callbacks

   /**
      A parameter has changed on the plugin side.@n
      This is called by the host to inform the UI about parameter changes.
    */
    void parameterChanged(uint32_t index, float value) override
    {
        switch (index) {
            case 0: {
                fhigh = value;
                break;
            }
            case 1: {
                flow = value;
                break;
            }
            case 2: {
                fmid = value;
                break;
            }
            case 3: {
                ffreq = value;
                break;
            }

            default: return;
        }

        repaint();
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Widget Callbacks

   /**
      ImGui specific onDisplay function.
    */
    void onImGuiDisplay() override
    {

        const float width = getWidth();
        const float height = getHeight();
        const float margin = 0.0f;
        #ifdef DISTRHO_OS_WASM
        static bool inputActive = false;
        #endif

        ImGui::SetNextWindowPos(ImVec2(margin, margin));
        ImGui::SetNextWindowSize(ImVec2(width - 2 * margin, height - 2 * margin));

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

        style.Colors[ImGuiCol_TitleBgActive] = (ImVec4)WstdTitleBgActive;
        style.Colors[ImGuiCol_WindowBg] = (ImVec4)WstdWindowBg;

        ImGuiIO& io(ImGui::GetIO());
        ImFont* defaultFont = ImGui::GetFont();
        ImFont* titleBarFont = io.Fonts->Fonts[2];

        auto HighColorActive     = ColorBright(Blue, fhigh);
        auto HighColorHovered    = ColorBright(BlueBr, fhigh);
        auto MidColorActive      = ColorMid(Blue, Green, Red, fmid, ffreq);
        auto MidColorHovered     = ColorMid(BlueBr, GreenBr, RedBr, fmid, ffreq);
        auto MidFreqColorActive  = ColorMid(BlueBr, GreenDr, RedBr, fmid, ffreq);
        auto MidFreqColorHovered = ColorMid(Blue, Green, Red, fmid, ffreq);
        auto LowColorActive      = ColorBright(Red, flow);
        auto LowColorHovered     = ColorBright(RedBr, flow);

        const float hundred = 100 * getScaleFactor();
        const float seventy = 70 * getScaleFactor();

        auto dbstep = 0.1f;
        auto hzstep = 20.0f;

        if (io.KeyShift)
        {
            dbstep = 0.01f;
            hzstep = 1.0f;
        }

        ImGui::PushFont(titleBarFont);
        if (ImGui::Begin("WSTD EQ", nullptr, ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoCollapse))
        {
            ImGui::Dummy(ImVec2(0.0f, 8.0f * getScaleFactor()));
            ImGui::PushFont(defaultFont);

            #ifdef DISTRHO_OS_WASM
            if (!inputActive)
            {
                ImGui::OpenPopup("Activate");
            }

            if (ImGui::BeginPopupModal("Activate", nullptr, ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoMove))
            {
                if (ImGui::Button("OK", ImVec2(80, 0)))
                {
                    requestAudioInput();
                    inputActive = true;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            #endif

            auto ImGuiKnob_Flags = ImGuiKnobFlags_DoubleClickReset + ImGuiKnobFlags_ValueTooltip + ImGuiKnobFlags_NoInput + ImGuiKnobFlags_ValueTooltipHideOnClick;
            auto ImGuiKnob_FlagsDB = ImGuiKnob_Flags + ImGuiKnobFlags_dB;
            auto ImGuiKnob_FlagsLog = ImGuiKnob_Flags + ImGuiKnobFlags_Logarithmic;

            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)HighColorActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)HighColorHovered);
            if (ImGuiKnobs::Knob("High", &fhigh, -15.0f, 15.0, dbstep, "%.2fdB", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_FlagsDB, 7))
            {
                if (ImGui::IsItemActivated())
                {
                    editParameter(0, true);
                    if (ImGui::IsMouseDoubleClicked(0))
                        fhigh = 0.0f;
                }
                setParameterValue(0, fhigh);
            }
            ImGui::PopStyleColor(2);

            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidColorActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidColorHovered);
            if (ImGuiKnobs::Knob("Mid", &fmid, -15.0f, 15.0, dbstep, "%.2fdB", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_FlagsDB, 7))
            {
                if (ImGui::IsItemActivated())
                {
                    editParameter(2, true);
                    if (ImGui::IsMouseDoubleClicked(0))
                        fmid = 0.0f;
                }
                setParameterValue(2, fmid);
            }
            ImGui::PopStyleColor(2);

            ImGui::Dummy(ImVec2(7.5f * getScaleFactor(), 0.0f)); ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidFreqColorActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidFreqColorHovered);
            if (ImGuiKnobs::Knob("Mid Freq", &ffreq, 313.3f, 5705.6f, hzstep, "%.1fHz", ImGuiKnobVariant_SteppedTick, seventy, ImGuiKnob_FlagsLog, 11))
            {
                if (ImGui::IsItemActivated())
                {
                    editParameter(3, true);
                    if (ImGui::IsMouseDoubleClicked(0))
                        ffreq = 1337.0f;
                }
                setParameterValue(3, ffreq);
            }
            ImGui::PopStyleColor(2);

            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)LowColorActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)LowColorHovered);
            if (ImGuiKnobs::Knob("Low", &flow, -15.0f, 15.0, dbstep, "%.2fdB", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_FlagsDB, 7))
            {
                if (ImGui::IsItemActivated())
                {
                    editParameter(1, true);
                    if (ImGui::IsMouseDoubleClicked(0))
                        flow = 0.0f;
                }
                setParameterValue(1, flow);
            }
            ImGui::PopStyleColor(2);

            if (ImGui::IsItemDeactivated())
            {
                editParameter(0, false);
                editParameter(1, false);
                editParameter(2, false);
                editParameter(3, false);
            }

            ImGui::PopFont();
        }
        ImGui::PopFont();
        ImGui::End();
    }

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImGuiPluginUI)
};

// --------------------------------------------------------------------------------------------------------------------

UI* createUI()
{
    return new ImGuiPluginUI();
}

// --------------------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
