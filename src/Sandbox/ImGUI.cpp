#include "ImGUI.h"

using namespace Gravy;

void ImGUI::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
    auto nativeWindow = Gravy::GetWindowInst()->GetGLFW()->GetNativeWindow();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(nativeWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    LOG_DEBUG("ImGUI initiated successfully.");
}

void ImGUI::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGUI::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGUI::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGUI::RenderGUI()
{
    NewFrame();

    {
        ImGui::SetNextWindowSize(ImVec2(300.f, 55.f), ImGuiCond_FirstUseEver);
        ImGui::Begin("Perf Monitor");
        ImGui::Text("%.1f FPS | %.3f Miliseconds", ImGui::GetIO().Framerate, 1 / ImGui::GetIO().Framerate * 1000.0f);
        ImGui::SameLine();
        if(ImGui::Checkbox("vSync", &GetWindowConfig()->vsync)) { SetVsync(GetWindowConfig()->vsync); };
        ImGui::End();
    }
    {
        auto camera = GetMainCamera();
        ImGui::SetNextWindowSize(ImVec2(300.f, 155.f), ImGuiCond_FirstUseEver);
        ImGui::Begin("Camera Options");
        ImGui::Text("Position Vector :\n    %.1f X | %.1f Y | %.1f Z", camera->Position.x, camera->Position.y, camera->Position.z);
        ImGui::Text("Front Vector :\n    %.1f X | %.1f Y | %.1f Z", camera->Front.x, camera->Front.y, camera->Front.z);
        ImGui::Separator();
        ImGui::Text("FOV : %.1f", camera->FOV);
        ImGui::Text("Speed : %.1f", camera->MovementSpeed);
        ImGui::Text("Sensitivity : %.1f", camera->MouseSensitivity);
        ImGui::End();
    }
    EndFrame();
}