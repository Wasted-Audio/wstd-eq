/*
/** Copyright (c) Wasted Audio 2023 - GPL-3.0-or-later */

#include "DistrhoUI.hpp"
#include "ResizeHandle.hpp"

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
        : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT),
          fResizeHandle(this)
    {
        setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true);

        // hide handle if UI is resizable
        // if (isResizable())
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
        // const float margin = 20.0f * getScaleFactor();
        const float margin = 0.0f;

        ImGui::SetNextWindowPos(ImVec2(margin, margin));
        ImGui::SetNextWindowSize(ImVec2(width - 2 * margin, height - 2 * margin));

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

        style.Colors[ImGuiCol_TitleBgActive] = (ImVec4)ImColor::HSV(3.31f / 3.6f, 0.64f, 0.40f);
        style.Colors[ImGuiCol_WindowBg] = (ImVec4)ImColor::HSV(3.31f / 3.6f, 0.64f, 0.10f);

        ImFont* defaultFont = ImGui::GetFont();
        ImFont* titleBarFont = new ImFont(*defaultFont);
        titleBarFont->Scale = 1.3f;

        ImGui::PushFont(titleBarFont);
        if (ImGui::Begin("WSTD EQ", nullptr, ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoCollapse))
        {

            ImGui::Dummy(ImVec2(0.0f, 8.0f));
            ImGui::PushFont(defaultFont);
            auto ImGuiKnob_Flags = ImGuiKnobFlags_DoubleClickReset + ImGuiKnobFlags_ValueTooltip + ImGuiKnobFlags_NoInput + ImGuiKnobFlags_ValueTooltipHideOnClick;
            auto ImGuiKnob_FlagsDB = ImGuiKnob_Flags + ImGuiKnobFlags_dB;
            auto ImGuiKnob_FlagsLog = ImGuiKnob_Flags + ImGuiKnobFlags_Logarithmic;

            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(2.04f / 3.6f, 0.83f, 0.64f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(2.04f / 3.6f, 0.83f, 0.84f));
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
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(1.6f / 3.6f, 0.77f, 0.64f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(1.6f / 3.6f, 0.77f, 0.74f));
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
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::SetCursorPosX(23.0f);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(1.6f / 3.6f, 0.77f, 0.44f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(1.6f / 3.6f, 0.77f, 0.64f));
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
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(0.03f / 3.6f, 0.76f, 0.74f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(0.03f / 3.6f, 0.76f, 0.84f));
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
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

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
