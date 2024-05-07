#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
    m_window = glfwCreateWindow(1000, 1000, "Space Invaders", nullptr, nullptr);
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

void Interface::startFrame() const {
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

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImVec4 clear_color = ImVec4(0.15f, 0.05f, 0.20f, 1.00f);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    keyboardEvent();
}

void Interface::renderFrame() const {
    GLuint out_texture;
    int out_width, out_height;
    const std::string filename("../assets/zeros10x10x3.png");
    bool loaded = LoadTextureFromFile(filename.c_str(), &out_texture, &out_width, &out_height);
    IM_ASSERT(loaded);
    drawTexture(0, 0, .1, .1, 0.0, 0.0, 1, 1, out_texture);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
}

void Interface::drawRectangle(
    Eigen::Array2f bottomLeft, Eigen::Array2f widthHeight, Eigen::Array3f color
    ) const {
    glBegin(GL_QUADS);
    glColor3f(color.x(), color.y(), color.z());
    glVertex2f(bottomLeft.x(), bottomLeft.y());
    glVertex2f(bottomLeft.x(), bottomLeft.y() + widthHeight.y());
    glVertex2f(bottomLeft.x() + widthHeight.x(), bottomLeft.y() + widthHeight.y());
    glVertex2f(bottomLeft.x() + widthHeight.x(), bottomLeft.y());
    glEnd();
}

void Interface::drawTexture(float x, float y, float width, float height, float texCoordX, float texCoordY, float texWidth, float texHeight, GLuint textureID) const {
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
    glTexCoord2f(texCoordX, texCoordY);
    glVertex2f(x, y);

    glTexCoord2f(texCoordX + texWidth, texCoordY);
    glVertex2f(x + width, y);

    glTexCoord2f(texCoordX + texWidth, texCoordY + texHeight);
    glVertex2f(x + width, y + height);

    glTexCoord2f(texCoordX, texCoordY + texHeight);
    glVertex2f(x, y + height);
    glEnd();
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

void Interface::setPlayer(std::shared_ptr<Player> player) {
    m_player = player;
}
