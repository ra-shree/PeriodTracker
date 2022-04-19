#include "../include/PeriodTracker/pch.h"
/* All multi-line comments "starting with /*" were added by me. Single-line comments (starting with //) are from the ImGui*/

/* change the properties / linker / system subsystem to console and the preprocessor directive below if you want to see the background terminal otherwise change it to windows
 so the stupid console doesn't show up when you execute the app */
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
/*DO NOT TOUCH THIS GODDAMN FILE.*/

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(300, 500, "Period Tracker", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    /* Enable Docker Mode by Default */
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

     // Load Fonts
    io.Fonts->AddFontFromFileTTF("../Roboto-Medium.ttf", 16.0f);

    ImFont* title_font = io.Fonts->AddFontFromFileTTF("../Roboto-Medium.ttf", 32.0f);
    ImFont* text_font = io.Fonts->AddFontFromFileTTF("../Roboto-Medium.ttf", 18.0f);
    ImFont* large_font = io.Fonts->AddFontFromFileTTF("../Roboto-Medium.ttf", 64.0f);

    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f)
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
    // Our state
    bool show_demo_window = false;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    tracker AppInstance;
    if (Check_File_Exists()) {
        Load_Data_From_File(AppInstance);
        AppInstance.Days_Between_App_Open();
    } else {
        Runner(AppInstance);
    }
    int days_before_period = AppInstance.countdown_predicted_date;
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        
        /* My Code starts here */

        /* Setting the window and default docking flags for the main window */
        ImGuiWindowFlags window_flags = 0;
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_AutoHideTabBar;
        static bool light_theme = false;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        /* Setting up default style as dockspace */
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        /* Actual Start of the Window Drawing */
        ImGui::Begin("Window", nullptr, window_flags);

        ImGui::SetCursorPosX(60);
        ImGui::PushFont(title_font);
        ImGui::Text("Period Tracker");
        ImGui::PopFont();

        /* Next Period Button */
        ImVec2 button_sz(120, 40);
        ImVec2 ButtonPos(90, 200);
        ImGui::SetCursorPos(ButtonPos);
        if (ImGui::Button("Start Next Cycle", button_sz)) {
            Runner(AppInstance);
            days_before_period = AppInstance.countdown_predicted_date;
            Unload_Data_To_File(AppInstance);
        }

        /* Diplaying the actual period date */
        ImVec2 TextPos(80, 100);
        ImGui::SetCursorPos(TextPos);
        ImGui::PushFont(text_font);
        ImGui::Text("Your period starts in: ");
        ImGui::PopFont();

        ImGui::SetCursorPosX(110);
        ImGui::PushFont(large_font);
        ImGui::Text("%d", days_before_period);
        ImGui::PopFont();

        ImVec2 DaysPos(176, 160);
        ImGui::SetCursorPos(DaysPos);
        ImGui::Text("days");
        ImGui::NewLine();

        /* Delete all data button */
        ImVec2 DeleteButtonPos(90, 400);
        ImGui::SetCursorPos(DeleteButtonPos);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.6f, 0.6f));
        if (ImGui::Button("Delete All Data", button_sz)) {
            Delete_File(AppInstance);
            Runner(AppInstance);
            days_before_period = AppInstance.countdown_predicted_date;
            Unload_Data_To_File(AppInstance);
        }
        ImGui::PopStyleColor(1);

        /* Switch between light theme and dark theme */
        ImVec2 checkBoxPos(190, 470);
        ImGui::SetCursorPos(checkBoxPos);
        ImGui::Checkbox("Light Theme", &light_theme);
        if (light_theme) {
            ImGui::StyleColorsLight();
        }
        else {
            ImGui::StyleColorsDark();
        }
        ImGui::End();

        /* My code ends here */

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}