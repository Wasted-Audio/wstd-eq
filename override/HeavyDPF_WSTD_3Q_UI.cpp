/**
 * Copyright (c) Wasted Audio 2023 - GPL-3.0-or-later
 */

#include "DistrhoUI.hpp"
#include "ResizeHandle.hpp"
#include "veramobd.hpp"


START_NAMESPACE_DISTRHO

// --------------------------------------------------------------------------------------------------------------------

ImVec4 ColorBright(ImVec4 color, float bright)
{
    if (bright > -15.0f)
    {
        auto outcol = ImVec4(
            color.x + (bright / 100.0f),
            color.y + (bright / 100.0f),
            color.z + (bright / 100.0f),
            color.w
        );
        return outcol;
    }
    else
    {
        auto outcol = ImVec4(
            0.3f,
            0.3f,
            0.3f,
            color.w
        );
        return outcol;
    }
}


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
        : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT),
          fResizeHandle(this)
    {
        setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true);

        ImGuiIO& io(ImGui::GetIO());

        ImFontConfig fc;
        fc.FontDataOwnedByAtlas = true;
        fc.OversampleH = 1;
        fc.OversampleV = 1;
        fc.PixelSnapH = true;

        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 16.0f, &fc);
        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 21.0f, &fc);
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

        ImGui::SetNextWindowPos(ImVec2(margin, margin));
        ImGui::SetNextWindowSize(ImVec2(width - 2 * margin, height - 2 * margin));

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);


        style.Colors[ImGuiCol_TitleBgActive] = (ImVec4)ImColor::HSV(3.31f / 3.6f, 0.64f, 0.40f);
        style.Colors[ImGuiCol_WindowBg] = (ImVec4)ImColor::HSV(3.31f / 3.6f, 0.64f, 0.10f);

        ImGuiIO& io(ImGui::GetIO());
        ImFont* defaultFont = ImGui::GetFont();
        ImFont* titleBarFont = io.Fonts->Fonts[2];

        auto HighColorActive     = ColorBright(ImColor::HSV(2.04f / 3.6f, 0.83f, 0.64f), fhigh);
        auto HighColorHovered    = ColorBright(ImColor::HSV(2.04f / 3.6f, 0.83f, 0.84f), fhigh);
        auto MidColorActive      = ColorBright(ImColor::HSV(1.6f / 3.6f, 0.77f, 0.64f), fmid);
        auto MidColorHovered     = ColorBright(ImColor::HSV(1.6f / 3.6f, 0.77f, 0.74f), fmid);
        auto MidFreqColorActive  = ColorBright(ImColor::HSV(1.6f / 3.6f, 0.77f, 0.44f), fmid);
        auto MidFreqColorHovered = ColorBright(ImColor::HSV(1.6f / 3.6f, 0.77f, 0.64f), fmid);
        auto LowColorActive      = ColorBright(ImColor::HSV(0.03f / 3.6f, 0.76f, 0.74f), flow);
        auto LowColorHovered     = ColorBright(ImColor::HSV(0.03f / 3.6f, 0.76f, 0.84f), flow);

        ImGui::PushFont(titleBarFont);
        if (ImGui::Begin("WSTD 3Q", nullptr, ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoCollapse))
        {

            ImGui::Dummy(ImVec2(0.0f, 8.0f));
            ImGui::PushFont(defaultFont);
            auto ImGuiKnob_Flags = ImGuiKnobFlags_DoubleClickReset + ImGuiKnobFlags_ValueTooltip + ImGuiKnobFlags_NoInput + ImGuiKnobFlags_ValueTooltipHideOnClick;
            auto ImGuiKnob_FlagsDB = ImGuiKnob_Flags + ImGuiKnobFlags_dB;
            auto ImGuiKnob_FlagsLog = ImGuiKnob_Flags + ImGuiKnobFlags_Logarithmic;

            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)HighColorActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)HighColorHovered);
            if (ImGuiKnobs::Knob("High", &fhigh, -15.0f, 15.0, 0.2f, "%.1fdB", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_FlagsDB, 7))
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
            if (ImGuiKnobs::Knob("Mid", &fmid, -15.0f, 15.0, 0.2f, "%.1fdB", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_FlagsDB, 7))
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

            ImGui::SetCursorPosX(23.0f);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidFreqColorActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidFreqColorHovered);
            if (ImGuiKnobs::Knob("Mid Freq", &ffreq, 313.3f, 5705.6f, 50.0f, "%.1fHz", ImGuiKnobVariant_SteppedTick, 70, ImGuiKnob_FlagsLog, 11))
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
            if (ImGuiKnobs::Knob("Low", &flow, -15.0f, 15.0, 0.2f, "%.1fdB", ImGuiKnobVariant_SteppedTick, 100, ImGuiKnob_FlagsDB, 7))
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
