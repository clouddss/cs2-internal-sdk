#include "main_window.h"

namespace main_window
{
    bool is_open = false;

    void Draw()
    {
        ImGui::Begin("Sensum", 0, ImGuiWindowFlags_NoCollapse);

        ImGui::Checkbox("Fov changer", &settings::visuals::m_bFovChanger);
        ImGui::Checkbox("Bone ESP", &settings::visuals::m_bBoneEsp);

        if (settings::visuals::m_bFovChanger)
            ImGui::SliderInt("Fov slider", &settings::visuals::m_iFov, 0, 200);

        ImGui::End();
    }
}
