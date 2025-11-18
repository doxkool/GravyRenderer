#include "RendererGL.h"
#include "Application.h"

#include "Input.h"

#include "Defines.h"

#include "Core/Layer.h"
#include "Core/ImGUI.h"
#include "Core/Audio.h"

#include "Entity/Light.h"

//using namespace Renderer;

Application AppInst;

double DeltaTime;

Window* m_window = nullptr;
Renderer::Camera MainCam;

Audio m_Audio;
ImGUI m_ImGUI;
int Audio1ID = -1;

std::vector<Renderer::Light> Lights;

Renderer::Model sponza;
Renderer::Model cube0;
Renderer::Light dirLight(Renderer::DirectionalLight);
Renderer::Light spotLight0(Renderer::SpotLight);

void CheckForInput()
{
    auto m_Window = AppInst.GetWindowInst();

    if (Input::IsKeyJustPressed(KEY_GRAVE_ACCENT))
    {
        LOG_INFO("Escape key pressed, exiting Sandbox...");
        AppInst.CloseWindow();
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

    if (Input::IsKeyPressed(KEY_W))              { MainCam.Move(FORWARD, DeltaTime); }
    if (Input::IsKeyPressed(KEY_S))              { MainCam.Move(BACKWARD, DeltaTime); }
    if (Input::IsKeyPressed(KEY_A))              { MainCam.Move(LEFT, DeltaTime); }
    if (Input::IsKeyPressed(KEY_D))              { MainCam.Move(RIGHT, DeltaTime); }
    if (Input::IsKeyPressed(KEY_SPACE))          { MainCam.Move(UP, DeltaTime); }
    if (Input::IsKeyPressed(KEY_LEFT_CONTROL))   { MainCam.Move(DOWN, DeltaTime); }

    if(Input::IsKeyJustPressed(KEY_1))
    {
        m_Audio.PlayAudioTrack(Audio1ID);
    }

    if(Input::IsKeyJustPressed(KEY_0))
    {
        m_Audio.StopAllAudio();
    }
}

void RenderScene(Renderer::Shader &shader)
{
    shader.Set1i(true, "blinn");
    shader.Set1i(false, "fogEnabled");
    shader.SetMat4fv(MainCam.GetProjectionMatrix(), "projection");
    shader.SetMat4fv(MainCam.GetViewMatrix(), "view");

    // set lights uniforms
    shader.SetVec3f(MainCam.Position, "viewPos");
    //shader.SetVec3f(light0.Transform.Position, "lightPos");
    //shader.SetVec3f(glm::vec3(0.8), "lightColor");
    //shader.SetVec3f(glm::vec3(0.5), "lightAmbient");

    shader.Set1i(1, "nbOfDirLight");
    shader.SetVec3f(glm::vec3(-0.1, -1.0, -0.2), "dirLight.direction");
    shader.SetVec3f(glm::vec3(0.6), "dirLight.diffuse");
    shader.SetVec3f(glm::vec3(0.6), "dirLight.specular");
    shader.SetVec3f(glm::vec3(0.2), "dirLight.ambient");

    //shader.Set1i(1, "nbOfSpotLight");
    //shader.SetVec3f(light0.Transform.Position,  "spotLights[0].position");
    //shader.SetVec3f({0.1, 1.0, 0.2},            "spotLights[0].direction");
    //shader.SetVec3f({1.0f, 1.0f, 1.0f},         "spotLights[0].ambient");
    //shader.SetVec3f({1.0f, 1.0f, 1.0f},         "spotLights[0].diffuse");
    //shader.SetVec3f({1.0f, 1.0f, 1.0f},         "spotLights[0].specular");
    //shader.Set1f(0.0f,                          "spotLights[0].constant");
    //shader.Set1f(0.05f,                         "spotLights[0].linear");
    //shader.Set1f(0.005f,                        "spotLights[0].quadratic");
    //shader.Set1f(35.0f,                         "spotLights[0].cutOff");
    //shader.Set1f(55.0f,                         "spotLights[0].outerCutOff");


    glm::mat4 model = glm::mat4(1.0f);

    // cubes
    model = cube0.GetModelMatrix();
    shader.SetMat4fv(model, "model");
    cube0.Render();

    // sponza
    model = sponza.GetModelMatrix();
    shader.SetMat4fv(model, "model");
    sponza.Render();

    cube0.Rotate({10.0, 10.0, 10.0}, DeltaTime);
}

void RenderShadowMap(Renderer::Light light)
{
    light.UpdateMatrices();

    light.m_DepthShader.Bind();
    light.m_DepthShader.SetMat4fv(light.GetLightSpaceMatrix(), "lightSpaceMatrix");

    OpenGL::SetViewportRes(light.m_ShadowRes);
    light.m_DepthMapFBO.Bind();
    OpenGL::ClearBuffer({GL_DEPTH_BUFFER_BIT});
    light.m_DepthMapTexture.SetActiveTexture(GL_TEXTURE0);
    light.m_DepthMapTexture.Bind();
    RenderScene(light.m_DepthShader);
    light.m_DepthMapTexture.UnBind();
    light.m_DepthMapFBO.UnBind();

    // reset viewport
    OpenGL::SetViewportRes(AppInst.GetCurrentResolution());
    OpenGL::ClearBuffer({GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT});
}

void Run()
{
    m_window = AppInst.GetWindowInst();

    //Renderer::SetClearColor(GRAY);

    MainCam.Position = {0.0, 15.0, -5.0};
    //SetMainCamera(&MainCam);

    t_AudioTrackInfo audio1 = {
        .filePath   = "assets/musics/Ice_and_Snow.mp3",
        .loop       = true,
        .audioBus   = 0
    };
    Audio1ID = m_Audio.LoadAudioTrack(&audio1);

    Renderer::Shader shader;
    shader.LoadShader(DEFAULT_VER_SHADER, DEFAULT_FRAG_SHADER);

    sponza.LoadModel("assets/models/sponza/sponza.obj");
    sponza.SetTransform({0.0f, -1.0f, 0.0}, {0.0f, 90.0f, 0.0}, {0.1, 0.1, 0.1});

    cube0.LoadPrimitive(Cube);
    cube0.SetTransform({0.0f, 8.0f, 20.0}, {-50.0f, 0.0f, 20.0}, {5.0, 5.0, 5.0});

    // load textures
    // -------------
    Renderer::Texture texture;
    texture.LoadTexture(DEFAULT_TEX);
 
    // configure lights
    // -----------------------
    dirLight.Transform.Position = {10.0f, 160.0f, -10.0f};
    dirLight.m_DepthShader.LoadShader("assets/shaders/shadow_mapping_depth.vert", "assets/shaders/shadow_mapping_depth.frag");
    dirLight.CreateShadowMap({4096, 4096});
    Lights.push_back(dirLight);

    spotLight0.Transform.Position = {-1.0f, 30.0f, 15.0f};
    spotLight0.m_DepthShader.LoadShader("assets/shaders/shadow_mapping_depth.vert", "assets/shaders/shadow_mapping_depth.frag");
    spotLight0.CreateShadowMap({4096, 4096});
    Lights.push_back(spotLight0);

    // shader configuration
    // --------------------
    shader.Bind();
    shader.Set1i(0, "material.diffuse");
    //shader.Set1i(1, "material.specular");
    shader.Set1i(2, "material.shadowMap");

    while (AppInst.IsRunning())
    {
        CheckForInput();

        DeltaTime = Time::UpdateDeltaTime();
        OpenGL::ClearBuffer();

        //MainCam.Update();

        // render scene from light's point of view
        for (auto light : Lights)
        {
            RenderShadowMap(light);
        }

        // 2. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        //ZoneScopedN("Bind Shader");
        shader.Bind();
        shader.SetMat4fv(dirLight.GetLightSpaceMatrix(), "lightSpaceMatrix");

        {
            texture.SetActiveTexture(GL_TEXTURE0);
            texture.Bind();
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, dirLight.m_DepthMapFBO.GetTexture());
        }
        
        {
            RenderScene(shader);
        }

        texture.UnBind();

        //ZoneScopedN("Render ImGUI");
        m_ImGUI.RenderGUI();

        //ZoneScopedN("Window stuff");
        m_window->SwapScreenBuffer();
        m_window->Update();
    }    
}

int main()
{
    AppSpec appSpec = {

    };

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
        //.apiVersionMajor = 4,
        //.apiVersionMinor = 6,
        //.apiEnableMessageCallBack = true,
        //.apiLoggingLevel = trace
    };
    
    int ret_Gravy = AppInst.Init(&appSpec);

    Logger::Init(windowSpec.windowName);

    m_Audio.Init();
    m_ImGUI.Init(AppInst.GetWindowInst()->GetGLFW()->GetNativeWindow());

    if(ret_Gravy == 0)
    {
        Run();
    }

    AppInst.Shutdown();
    m_ImGUI.Shutdown();
    m_Audio.Shutdown();

    return 0;
}