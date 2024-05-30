#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "space_invaders/constants.h"
#include <string>

#include "space_invaders/interface.h"

Interface::Interface() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    m_window = glfwCreateWindow(1200, 1000, "Space Invaders", nullptr, nullptr);
    if (m_window == nullptr)
        return;
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    const std::string fontFile = "Pixeled.ttf";
    m_fontBody = io.Fonts->AddFontFromFileTTF((ASSETS_RELATIVE_PATH + fontFile).c_str(), FONT_SIZE_BODY);
    m_fontHeading = io.Fonts->AddFontFromFileTTF((ASSETS_RELATIVE_PATH + fontFile).c_str(), FONT_SIZE_HEADING);
    IM_ASSERT(m_fontBody != NULL);
    IM_ASSERT(m_fontHeading != NULL);

}

Interface::~Interface() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Interface::isAlive() const {
    return !(glfwWindowShouldClose(m_window) || ImGui::IsKeyPressed(ImGuiKey_Q));
}

void Interface::startFrame() {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // {
    //     ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
    //     ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //     ImGui::End();
    // }


    // {
    //     ImGui::SetNextWindowBgAlpha(0.0f);
    //     ImGui::SetNextWindowPos( ImVec2(0,0) );
    //     // begin() with NoTitleBar|NoResize|NoMove|NoScrollbar|NoSavedSettings|NoInputs flags. 
    //     ImGui::Begin("BCKGND", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus );
    //     // ImGui::Begin("empty");
    //     ImGui::GetWindowDrawList()->AddText( ImVec2(000,000), ImColor(1.0f,1.0f,1.0f,1.0f), "Text in Background Layer" );
    //     ImGui::End();
    // }

    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    glEnable(GL_BLEND);
    glClearColor(INTERFACE_CLEAR_COLOR.x(), INTERFACE_CLEAR_COLOR.y(), INTERFACE_CLEAR_COLOR.z(), INTERFACE_CLEAR_COLOR.w());
    glClear(GL_COLOR_BUFFER_BIT);

    updateWindowSize();
    keyboardEvent();
    drawOverlay();
}

void Interface::renderFrame() const {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
}

void Interface::drawRectangle(
    Array2f bottomLeft, Array2f widthHeight, Array3f color, bool globalCanvas
    ) const {
    if (!globalCanvas) m_gameCanvas.mapFromGlobal(widthHeight, bottomLeft);
    glBegin(GL_QUADS);
    glColor3f(color.x(), color.y(), color.z());
    glVertex2f(bottomLeft.x(), bottomLeft.y());
    glVertex2f(bottomLeft.x(), bottomLeft.y() + widthHeight.y());
    glVertex2f(bottomLeft.x() + widthHeight.x(), bottomLeft.y() + widthHeight.y());
    glVertex2f(bottomLeft.x() + widthHeight.x(), bottomLeft.y());
    glColor3f(1, 1, 1);  // reset color
    glEnd();
}

void Interface::drawTexture(
    const GLuint textureID,
    const float& x, const float& y, const float& width, const float& height,
    const float& texCoordX, const float& texCoordY, const float& texWidth, const float& texHeight
    ) const {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);

    glTexCoord2f(texCoordX, texCoordY + texHeight);
    glVertex2f(x, y);
    glTexCoord2f(texCoordX + texWidth, texCoordY + texHeight);
    glVertex2f(x + width, y);
    glTexCoord2f(texCoordX + texWidth, texCoordY);
    glVertex2f(x + width, y + height);
    glTexCoord2f(texCoordX, texCoordY);
    glVertex2f(x, y + height);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Interface::drawTexture(
    const GLuint textureID,
    Array2f bottomLeft, Array2f widthHeight, bool globalCanvas,
    const Array2f& texBottomLeft, const Array2f& texWidthHeight
    ) const {
    if (!globalCanvas) m_gameCanvas.mapFromGlobal(widthHeight, bottomLeft);
    drawTexture(
        textureID,
        bottomLeft.x(), bottomLeft.y(), widthHeight.x(), widthHeight.y(),
        texBottomLeft.x(), texBottomLeft.y(), texWidthHeight.x(), texWidthHeight.y()
    );
}

bool Interface::keyboardEvent() const {
    if (ImGui::GetIO().WantCaptureKeyboard) {
            return false;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_A)) {
        m_player->takeStep(false);
    }
    if (ImGui::IsKeyPressed(ImGuiKey_D)) {
        m_player->takeStep(true);
    }
    if (ImGui::IsKeyPressed(ImGuiKey_W)) {
        m_player->fire();
    }
    return false;
}

void Interface::drawOverlay() const {
    const float frameThickness = .01;
    const Array2f frameWidthHeight = m_gameCanvas.m_widthHeight + frameThickness;
    const Array3f frameColor{1.f, 1.f, 1.f};

    // Frame shape:
    // |--
    // | |
    // --|
    drawRectangle(m_gameCanvas.m_bottomLeft - Array2f{frameThickness, 0.f},
                  {frameThickness, frameWidthHeight.y()}, frameColor, true);
    drawRectangle(m_gameCanvas.m_bottomLeft + Array2f{m_gameCanvas.m_widthHeight.x(), -frameThickness},
                  {frameThickness, frameWidthHeight.y()}, frameColor, true);

    drawRectangle(m_gameCanvas.m_bottomLeft + Array2f{0.f, m_gameCanvas.m_widthHeight.y()},
                  {frameWidthHeight.x(), frameThickness}, frameColor, true);
    drawRectangle(m_gameCanvas.m_bottomLeft - Array2f{frameThickness, frameThickness},
                  {frameWidthHeight.x(), frameThickness}, frameColor, true);

    ImGui::PushFont(m_fontBody);
    const float textLineY = 1.98;
    const ImVec2 livesPosXY(.5 * m_windowWidthHeight.x(), (2 - textLineY) * m_windowWidthHeight.y());
    ImGui::GetForegroundDrawList()->AddText(livesPosXY, ImColor(1.0f, 1.0f, 1.0f, 1.0f), "LIVES:\t");

    const float dashboardBottomY = GAME_CANVAS_BOTTOM_LEFT.y() + GAME_CANVAS_WIDTH_HEIGHT.y();
    const float dashboardMidY = (dashboardBottomY + 1) / 2;
    const float livesOffsetX = .25;
    for (int life = 0; life < PLAYER_NUM_LIVES; life++) {
        if (m_player->getNumLives() > life) {
            drawTexture(m_player->getDefaultAnimation().getCurrentTexture(),
                        Array2f{livesOffsetX + life * .2, dashboardMidY - m_player->getWidthHeight().y()/2},
                        m_player->getWidthHeight(), true);
        }
    }

    // top left corner of text box (x right, y down)
    const ImVec2 scorePosXY(.1 * m_windowWidthHeight.x(), (2 - textLineY) * m_windowWidthHeight.y());
    std::string scoreLine = "SCORE:\t" + std::to_string(*m_player->getScore());
    ImGui::GetForegroundDrawList()->AddText(scorePosXY, ImColor(1.0f, 1.0f, 1.0f, 1.0f), scoreLine.c_str());
    ImGui::PopFont();

}
