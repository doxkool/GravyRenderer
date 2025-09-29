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

void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void renderScene(Shader &shader)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.SetMat4fv(model, "model");
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.SetMat4fv(model, "model");
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.SetMat4fv(model, "model");
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    shader.SetMat4fv(model, "model");
    renderCube();
}

void RunShadowTest()
{
    m_window = GetWindowInst();

    SetClearColor(GRAY);

    // build and compile shaders
    // -------------------------
    Shader shader;
    shader.LoadShader("assets/shaders/vert_shadow_mapping.glsl", "assets/shaders/frag_shadow_mapping.glsl");
    Shader simpleDepthShader;
    simpleDepthShader.LoadShader("assets/shaders/vert_shadow_mapping_depth.glsl", "assets/shaders/frag_shadow_mapping_depth.glsl");
    Shader debugDepthQuad;
    debugDepthQuad.LoadShader("assets/shaders/vert_debug_quad.glsl", "assets/shaders/frag_debug_quad_depth.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f
    };
    // plane VAO
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
    Texture woodTexture;
    woodTexture.LoadTexture(DEFAULT_TEX);

    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


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
        float near_plane = 1.0f, far_plane = 7.5f;
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.Bind();
        simpleDepthShader.SetMat4fv(lightSpaceMatrix, "lightSpaceMatrix");

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, woodTexture);
            woodTexture.Bind();
            renderScene(simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        OpenGL::SetFrameBufferRes(GetCurrentResolution());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, woodTexture);
        woodTexture.Bind();
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

void Run()
{
    m_window = GetWindowInst();

    SetClearColor(GRAY);

    t_AudioTrackInfo audio1 = {
        .filePath   = "assets/musics/Ice_and_Snow.mp3",
        .loop       = true,
        .audioBus   = 0
    };
    Audio1ID = m_Audio.LoadAudioTrack(&audio1);

    Model floor0(Quad);
    Shader floor0Shader;
    floor0Shader.LoadShader(FLAT_VER_SHADER, FLAT_FRAG_SHADER);
    floor0.SetTransform({0.f, -2.0f, 0.0f}, {90.f, 0.0f, 0.0f}, {10.0f, 10.0f, 10.0f});

    Model cube0(Cube);
    Shader cube0Shader;
    cube0Shader.LoadShader(FLAT_VER_SHADER, FLAT_FRAG_SHADER);

    Light light0;
    light0.CreateShadowMap();

    MainCam.Position = {0.0f, 0.0f, -4.0f};
    MainCam.MovementSpeed = 25.0f;
    SetMainCamera(&MainCam);
    
    while (IsRunning())
    {
        CheckForInput();

        cube0.Rotate({0.0f, 50.0f, 0.0f});

        // --- Render Scene
        NewFrame();

        MainCam.Update();

        cube0.UpdateMatrix();
        floor0.UpdateMatrix();

        cube0.Render(&cube0Shader, &MainCam);
        floor0.Render(&floor0Shader, &MainCam);

        m_ImGUI.RenderGUI();

        EndFrame();
        
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
        //Run();
        RunShadowTest();
    }

    Shutdown();

    m_ImGUI.Shutdown();
    m_Audio.Shutdown();

    return 0;
}