#include "Gravy.h"

#include "ImGUI.h"
#include "Audio.h"

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
        }
        else
        {
            Input::SetCursorMode(grabed);
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

    MainCam.b_MouseInput = Input::IsMouseGrabed();
}

// meshes
unsigned int planeVAO = 0;
unsigned int planeVBO = 0;
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;

Model floor0;
Model cube0;
Model cube1;
Model cube2;

void renderScene(Shader &shader)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    model = floor0.GetModelMatrix();
    shader.SetMat4fv(model, "model");
    floor0.Render();

    // cubes
    model = glm::mat4(1.0f);
    model = cube0.GetModelMatrix();
    shader.SetMat4fv(model, "model");
    cube0.Render();

    model = glm::mat4(1.0f);
    model = cube1.GetModelMatrix();
    shader.SetMat4fv(model, "model");
    cube1.Render();

    model = glm::mat4(1.0f);
    model = cube2.GetModelMatrix();
    shader.SetMat4fv(model, "model");
    cube2.Render();

    cube0.Rotate({10.0, 10.0, 10.0});
    cube1.Rotate({15.0, 15.0, 15.0});
    cube2.Rotate({20.0, 20.0, 20.0});
}

void Run()
{
    m_window = GetWindowInst();

    SetClearColor(GRAY);

    MainCam.Position = {0.0, 0.5, 0.0};

    t_AudioTrackInfo audio1 = {
        .filePath   = "assets/musics/Ice_and_Snow.mp3",
        .loop       = true,
        .audioBus   = 0
    };
    Audio1ID = m_Audio.LoadAudioTrack(&audio1);

    // build and compile shaders
    // -------------------------
    Shader shader;
    shader.LoadShader("assets/shaders/vert_shadow_mapping.glsl", "assets/shaders/frag_shadow_mapping.glsl");
    Shader simpleDepthShader;
    simpleDepthShader.LoadShader("assets/shaders/vert_shadow_mapping_depth.glsl", "assets/shaders/frag_shadow_mapping_depth.glsl");
    Shader debugDepthQuad;
    debugDepthQuad.LoadShader("assets/shaders/vert_debug_quad.glsl", "assets/shaders/frag_debug_quad_depth.glsl");

    floor0.LoadPrimitive(Quad);
    floor0.SetTransform({0.0f, -1.0f, 0.0}, {90.0f, 0.0f, 0.0}, {20.0, 20.0, 20.0});

    cube0.LoadPrimitive(Cube);
    cube0.SetTransform({-2.5f, 1.0f, 6.0}, {-50.0f, 0.0f, 20.0}, {1.0, 1.0, 1.0});

    cube1.LoadPrimitive(Cube);
    cube1.SetTransform({0.0f, 1.5f, 3.0}, {0.0f, 0.0f, 0.0}, {0.5, 0.5, 0.5});

    cube2.LoadPrimitive(Cube);
    cube2.SetTransform({2.0f, 0.0f, 4.0}, {0.0f, 0.0f, .0}, {0.5, 0.5, 0.5});

    Light light0;

    // load textures
    // -------------
    Texture Texture;
    Texture.LoadTexture(DEFAULT_TEX);

    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

    FrameBuffer depthMapFBO;
    depthMapFBO.Create(SHADOW_WIDTH, SHADOW_HEIGHT, true);


    // shader configuration
    // --------------------
    shader.Bind();
    shader.Set1i(0, "diffuseTexture");
    shader.Set1i(1, "shadowMap");
    debugDepthQuad.Bind();
    debugDepthQuad.Set1i(0, "depthMap");

    // lighting info
    // -------------
    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

    while (IsRunning())
    {
        CheckForInput();

        Time::UpdateDeltaTime();
        OpenGL::ClearBuffer();

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 10.0f;
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.Bind();
        simpleDepthShader.SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");

        OpenGL::SetViewportRes(SHADOW_WIDTH, SHADOW_HEIGHT);
        depthMapFBO.Bind();
        OpenGL::ClearBuffer({GL_DEPTH_BUFFER_BIT});
        Texture.SetActiveTexture(GL_TEXTURE0);
        Texture.Bind();
        renderScene(simpleDepthShader);
        Texture.UnBind();
        depthMapFBO.UnBind();

        // reset viewport
        OpenGL::SetViewportRes(GetCurrentResolution());
        OpenGL::ClearBuffer({GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT});

        // 2. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        shader.Bind();
        glm::mat4 projection = MainCam.GetProjectionMatrix();
        glm::mat4 view = MainCam.GetViewMatrix();
        shader.SetMat4fv(projection, "projection");
        shader.SetMat4fv(view, "view");
        // set light uniforms
        shader.SetVec3f(MainCam.Position, "viewPos");
        shader.SetVec3f(lightPos, "lightPos");
        shader.SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");
        Texture.SetActiveTexture(GL_TEXTURE0);
        Texture.Bind();
        auto depthMap = depthMapFBO.GetTexture();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderScene(shader);

        // render Depth map to quad for visual debugging
        // ---------------------------------------------
        debugDepthQuad.Bind();
        debugDepthQuad.Set1f(near_plane, "near_plane");
        debugDepthQuad.Set1f(far_plane, "far_plane");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);

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