#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "space_invaders/interface.h"
#include "space_invaders/entity.h"


static const ImGuiWindowFlags dummyWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

OverlayConfig getOverlayConfig(OverlayType type) {
    switch (type) {
        case OverlayType::Starting:
            return {"starting", "SPACE INVADERS",
                    "A/D: MOVE LEFT/RIGHT\nW: FIRE\n[S]TART\n[P]AUSE\n[R]ESTART\n[Q]UIT"};
        case OverlayType::Pause:
            return {"pause", "PAUSE", nullptr};
        case OverlayType::GameOver:
            return {"gameover", "GAME OVER!", "[R]ESTART?"};
        default:
            return {"", "", nullptr};
    }
}

// Helper function to render horizontally centered text at specified vertical position
void renderText(const char* text, float posY, ImVec2 windowWidthHeight, ImFont* font = nullptr) {
    if (font) ImGui::PushFont(font);
    ImVec2 textSize = ImGui::CalcTextSize(text);
    ImGui::SetCursorPosX((windowWidthHeight.x - textSize.x) * 0.5f);
    ImGui::SetCursorPosY((windowWidthHeight.y - textSize.y) * posY);
    ImGui::Text("%s", text);
    ImGui::PopFont();
}

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

    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    glEnable(GL_BLEND);
    glClearColor(INTERFACE_CLEAR_COLOR.x(), INTERFACE_CLEAR_COLOR.y(), INTERFACE_CLEAR_COLOR.z(), INTERFACE_CLEAR_COLOR.w());
    glClear(GL_COLOR_BUFFER_BIT);

    updateWindowSize();
}

void Interface::renderFrame() const {
    drawDashboard();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
}

void Interface::drawRectangle(
    Array2f bottomLeft, Array2f widthHeight, const Array4f& color, bool onGameCanvas
    ) const {
    if (onGameCanvas) m_gameCanvas.mapToGlobal(widthHeight, bottomLeft);
    glBegin(GL_QUADS);
    glColor4f(color.x(), color.y(), color.z(), color.w());
    glVertex2f(bottomLeft.x(), bottomLeft.y());
    glVertex2f(bottomLeft.x(), bottomLeft.y() + widthHeight.y());
    glVertex2f(bottomLeft.x() + widthHeight.x(), bottomLeft.y() + widthHeight.y());
    glVertex2f(bottomLeft.x() + widthHeight.x(), bottomLeft.y());
    glColor4f(1.f, 1.f, 1.f, 1.f);  // reset color
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
    Array2f bottomLeft, Array2f widthHeight, bool onGameCanvas
    ) const {
    if (onGameCanvas) m_gameCanvas.mapToGlobal(widthHeight, bottomLeft);
    drawTexture(textureID, bottomLeft.x(), bottomLeft.y(), widthHeight.x(), widthHeight.y());
}

void Interface::pollKeyboardEvents(keyboardEvents& eventsOut) const {
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;
    if (ImGui::IsKeyPressed(ImGuiKey_A))
        eventsOut.playerStepLeft = true;
    if (ImGui::IsKeyPressed(ImGuiKey_D))
        eventsOut.playerStepRight = true;
    if (ImGui::IsKeyPressed(ImGuiKey_W))
        eventsOut.playerFire = true;
    if (ImGui::IsKeyPressed(ImGuiKey_S))
        eventsOut.gameStart = true;
    if (ImGui::IsKeyPressed(ImGuiKey_P))
        eventsOut.gamePause = true;
    if (ImGui::IsKeyPressed(ImGuiKey_R))
        eventsOut.gameRestart = true;
}

void Interface::drawBorder(const Array2f& bottomLeft, const Array2f& widthHeight, float thickness, const Array4f& color) const {
    // Border shape:
    // |--
    // | |
    // --|
    const Array2f borderWidthHeight = widthHeight + thickness;
    drawRectangle(bottomLeft - Array2f{thickness, 0.f},
                  {thickness, borderWidthHeight.y()}, color);
    drawRectangle(bottomLeft + Array2f{widthHeight.x(), -thickness},
                  {thickness, borderWidthHeight.y()}, color);

    drawRectangle(bottomLeft + Array2f{0.f, widthHeight.y()},
                  {borderWidthHeight.x(), thickness}, color);
    drawRectangle(bottomLeft - Array2f{thickness, thickness},
                  {borderWidthHeight.x(), thickness}, color);
}

void Interface::drawDashboard() const {
    // draw border around game canvas and dashboard
    const float borderThickness = INTERFACE_BORDER_THICKNESS;
    const Array4f borderColor{1.f, 1.f, 1.f, 1.f};
    drawBorder(m_gameCanvas.m_bottomLeft, m_gameCanvas.m_widthHeight, borderThickness);
    const Array2f dashboardBL = m_gameCanvas.m_bottomLeft + Array2f{0.f, m_gameCanvas.m_widthHeight.y() + 2 * borderThickness};
    const Array2f dashboardWH = Array2f{m_gameCanvas.m_widthHeight.x(), 2 - m_gameCanvas.m_widthHeight.y() - 4 * borderThickness};
    drawBorder(dashboardBL, dashboardWH, borderThickness);

    // draw score and lives
    ImGui::PushFont(m_fontBody);
    std::string scoreText = "SCORE:\t" + std::to_string(*m_player->getScore());
    ImVec2 textSize = ImGui::CalcTextSize(scoreText.c_str());
    Array2f dashboardWH_imgui = dashboardWH * m_windowWidthHeight.cast<float>() / 2;
    // Array2f dashboardTL_imgui = Array2f{borderThickness, borderThickness} * m_windowWidthHeight.cast<float>() / 2;
    ImVec2 scorePos{
        (dashboardWH_imgui.x() - textSize.x) * 0.1f,
        (dashboardWH_imgui.y() - textSize.y) * 0.5f
    };
    ImGui::GetBackgroundDrawList()->AddText(scorePos, ImColor(1.0f, 1.0f, 1.0f, 1.0f), scoreText.c_str());
    ImVec2 livesPos{
        (dashboardWH_imgui.x() - textSize.x) * 0.6f,
        (dashboardWH_imgui.y() - textSize.y) * 0.5f
    };
    ImGui::GetBackgroundDrawList()->AddText(livesPos, ImColor(1.0f, 1.0f, 1.0f, 1.0f), "LIVES:\t");
    ImGui::PopFont();

    // draw a player texture for each life left
    const float dashboardMidY = (dashboardBL.y() + 1) / 2;
    const float livesOffsetX = .25;
    for (int life = 0; life < PLAYER_NUM_LIVES; life++) {
        if (m_player->getNumLives() > life) {
            drawTexture(m_player->getDefaultAnimation().getCurrentTexture(),
                        Array2f{livesOffsetX + life * .2, dashboardMidY - m_player->getWidthHeight().y() / 2},
                        m_player->getWidthHeight());
        }
    }
}

void Interface::displayOverlay(OverlayType type) const {
    ImGui::SetNextWindowBgAlpha(INTERFACE_OVERLAY_ALPHA);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(m_windowWidthHeight.x(), m_windowWidthHeight.y()));

    OverlayConfig config = getOverlayConfig(type);
    ImGui::Begin(config.windowTitle, NULL, dummyWindowFlags);
    ImVec2 windowWidthHeight = ImGui::GetWindowSize();

    // Display the heading text
    renderText(config.headingText, 0.3, windowWidthHeight, m_fontHeading);
    // Display the body text if available
    if (config.bodyText) renderText(config.bodyText, 0.6, windowWidthHeight, m_fontBody);

    ImGui::End();
}

