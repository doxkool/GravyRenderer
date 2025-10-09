#include "Gravy.h"

#include "Defines.h"

#include "Core/Layer.h"
#include "Core/ImGUI.h"
#include "Core/Audio.h"

#include "Entity/Light.h"

using namespace Renderer;

Window* m_window = nullptr;
Camera MainCam;

Audio m_Audio;
ImGUI m_ImGUI;
int Audio1ID = -1;

void CheckForInput()
{
    auto m_Window = GetWindowInst();

    if (Input::IsKeyJustPressed(KEY_GRAVE_ACCENT))
    {
        LOG_INFO("Escape key pressed, exiting Sandbox...");
        CloseWindow();
    }

    if (Input::IsMouseButtonJustPressed(MOUSE_RIGHT_CLICK))
    {
        if (Input::IsMouseGrabed())
        {
            Input::SetCursorMode(released);
            MainCam.b_MouseInput = false;
        }
        else
        {
            Input::SetCursorMode(grabed);
            MainCam.b_MouseInput = true;
        }
    }

    if (Input::IsKeyPressed(KEY_W))              { MainCam.Move(FORWARD); }
    if (Input::IsKeyPressed(KEY_S))              { MainCam.Move(BACKWARD); }
    if (Input::IsKeyPressed(KEY_A))              { MainCam.Move(LEFT); }
    if (Input::IsKeyPressed(KEY_D))              { MainCam.Move(RIGHT); }
    if (Input::IsKeyPressed(KEY_SPACE))          { MainCam.Move(UP); }
    if (Input::IsKeyPressed(KEY_LEFT_CONTROL))   { MainCam.Move(DOWN); }

    if(Input::IsKeyJustPressed(KEY_1))
    {
        m_Audio.PlayAudioTrack(Audio1ID);
    }

    if(Input::IsKeyJustPressed(KEY_0))
    {
        m_Audio.StopAllAudio();
    }
}

Model sponza;
Model cube0;
Light light0(Directional);

void RenderScene(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);

    // cubes
    model = cube0.GetModelMatrix();
    shader.SetMat4fv(model, "model");
    cube0.Render();

    // sponza
    model = sponza.GetModelMatrix();
    shader.SetMat4fv(model, "model");
    sponza.Render();

    cube0.Rotate({10.0, 10.0, 10.0});
}

void Run()
{
    m_window = GetWindowInst();

    SetClearColor(GRAY);

    MainCam.Position = {0.0, 15.0, -5.0};
    SetMainCamera(&MainCam);

    t_AudioTrackInfo audio1 = {
        .filePath   = "assets/musics/Ice_and_Snow.mp3",
        .loop       = true,
        .audioBus   = 0
    };
    Audio1ID = m_Audio.LoadAudioTrack(&audio1);

    Shader shader;
    shader.LoadShader(DEFAULT_VER_SHADER, DEFAULT_FRAG_SHADER);
    //shader.LoadShader("assets/shaders/vert_shadow_mapping.glsl", "assets/shaders/frag_shadow_mapping.glsl");

    light0.Transform.Position = {10.0f, 160.0f, -10.0f};
    light0.m_DepthShader.LoadShader("assets/shaders/vert_shadow_mapping_depth.glsl", "assets/shaders/frag_shadow_mapping_depth.glsl");

    sponza.LoadModel("assets/models/sponza/sponza.obj");
    sponza.SetTransform({0.0f, -1.0f, 0.0}, {0.0f, 90.0f, 0.0}, {0.1, 0.1, 0.1});

    cube0.LoadPrimitive(Cube);
    cube0.SetTransform({0.0f, 8.0f, 20.0}, {-50.0f, 0.0f, 20.0}, {5.0, 5.0, 5.0});

    // load textures
    // -------------
    Texture texture;
    texture.LoadTexture(DEFAULT_TEX);
 
    // configure light Shadow Map
    // -----------------------
    light0.CreateShadowMap({4096, 4096});

    // shader configuration
    // --------------------
    shader.Bind();
    shader.Set1i(0, "material.diffuse");
    shader.Set1i(1, "material.specular");
    shader.Set1i(2, "material.shadowMap");

    while (IsRunning())
    {
        CheckForInput();

        Time::UpdateDeltaTime();
        OpenGL::ClearBuffer();

        MainCam.Update();

        // render scene from light's point of view

        light0.UpdateMatrices();

        light0.m_DepthShader.Bind();
        light0.m_DepthShader.SetMat4fv(light0.GetLightSpaceMatrix(), "lightSpaceMatrix");

        OpenGL::SetViewportRes(light0.m_ShadowRes);
        light0.m_DepthMapFBO.Bind();
        OpenGL::ClearBuffer({GL_DEPTH_BUFFER_BIT});
        light0.m_DepthMapTexture.SetActiveTexture(GL_TEXTURE0);
        light0.m_DepthMapTexture.Bind();
        RenderScene(light0.m_DepthShader);
        light0.m_DepthMapTexture.UnBind();
        light0.m_DepthMapFBO.UnBind();

        // reset viewport
        OpenGL::SetViewportRes(GetCurrentResolution());
        OpenGL::ClearBuffer({GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT});

        // 2. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        shader.Bind();
        shader.SetMat4fv(MainCam.GetProjectionMatrix(), "projection");
        shader.SetMat4fv(MainCam.GetViewMatrix(), "view");

        // set light uniforms
        shader.SetVec3f(MainCam.Position, "viewPos");
        //shader.SetVec3f(light0.Transform.Position, "lightPos");
        //shader.SetVec3f(glm::vec3(0.8), "lightColor");
        //shader.SetVec3f(glm::vec3(0.5), "lightAmbient");

        shader.SetVec3f(glm::vec3(-0.1, -1.0, -0.2), "dirLight.direction");
        shader.SetVec3f(glm::vec3(0.8), "dirLight.diffuse");
        shader.SetVec3f(glm::vec3(0.8), "dirLight.specular");
        shader.SetVec3f(glm::vec3(0.2), "dirLight.ambient");


        shader.SetMat4fv(light0.GetLightSpaceMatrix(), "lightSpaceMatrix");
        texture.SetActiveTexture(GL_TEXTURE0);
        texture.Bind();
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, light0.m_DepthMapFBO.GetTexture());
        RenderScene(shader);
        texture.UnBind();

        m_ImGUI.RenderGUI();

        m_window->SwapScreenBuffer();
        m_window->Update();
    }    
}

int main()
{
    WindowSpec windowSpec = {
        .windowName         = "Sandbox",
        .windowResX         = 1920,
        .windowResY         = 1080,
        .windowMode         = windowed,
        .vsync              = true,
        .windowResizable    = true,
        .transparentFB      = false
    };

    RendererSpec rendererSpec = {
        .windowSpec = windowSpec,
        .renderingAPI = Opengl,
        .apiVersionMajor = 4,
        .apiVersionMinor = 6,
        .apiEnableMessageCallBack = true,
        .apiLoggingLevel = trace
    };
    
    int ret_Gravy = Init(&rendererSpec);

    m_Audio.Init();
    m_ImGUI.Init();

    if(ret_Gravy == 0)
    {
        Run();
    }

    Shutdown();

    m_ImGUI.Shutdown();
    m_Audio.Shutdown();

    return 0;
}