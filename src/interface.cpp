
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
    m_window = glfwCreateWindow(1280, 720, "Space Invaders", nullptr, nullptr);
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
    return !glfwWindowShouldClose(m_window);
}

void Interface::startFrame(){
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Interface::renderFrame(){
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
}

void drawSquare() {
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);   // White
    glVertex2f(-1.f, -1.f);
    // glColor3f(0.0f, 1.0f, 1.0f);   // Cyan
    glVertex2f(-0.5f, 0.5f);
    // glColor3f(1.0f, 0.0f, 1.0f);   // Magenta
    glVertex2f(0.5f, 0.5f);
    // glColor3f(1.0f, 0.5f, 0.0f);   // Orange
    glVertex2f(0.5f, -0.5f);
    glEnd();
}

void Interface::drawRectangle(Eigen::Vector2f topLeft, Eigen::Vector2f widthHeight, Eigen::Vector3f color = {1., 1., 1.}) {
    glBegin(GL_QUADS);
    glColor3f(color.x(), color.y(), color.z());
    glVertex2f(topLeft.x(), topLeft.y());
    glVertex2f(topLeft.x(), topLeft.y() - widthHeight.y());
    glVertex2f(topLeft.x() + widthHeight.x(), topLeft.y() - widthHeight.y());
    glVertex2f(topLeft.x() + widthHeight.x(), topLeft.y());
    glEnd();
}

void Interface::renderScene(float pos) {
    glClear(GL_COLOR_BUFFER_BIT);

    // drawSquare();
    // drawTriangle();
    drawRectangle({pos, 0}, {.1, .1}, {1, 1, 1});

    glfwSwapBuffers(m_window);
}
