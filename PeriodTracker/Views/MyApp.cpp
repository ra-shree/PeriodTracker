#include <MyApp.h>
#include <ImGui/imgui_internal.h>
void mainWindow()
{

    /* Setting the window and default docking flags for the main window */

    ImGuiWindowFlags window_flags = 0;
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_AutoHideTabBar;

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
 
    /* Actual Start of the Window */
    ImGui::Begin("Window", nullptr, window_flags);

    if (ImGui::Button("Set")) {
        std::cout << "Hello";
    }
    if (ImGui::Button("Reset")) {
        std::cout << "Hello World!";
    }
    ImGui::End();
}
