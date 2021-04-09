#include "common.h"
#include "Image.h"
#include "Player.h"
#include <unistd.h>

#define GLFW_DLL
#include <GLFW/glfw3.h>

constexpr GLsizei WINDOW_WIDTH = tileSize * tileNum, WINDOW_HEIGHT = tileSize * tileNum;

char lab[5][5];

int curr_room_i, curr_room_j;
char curr_room_ch = '\0';
char room_buff[25][25];

bool trap_activated = false;
bool trap_on_screen = false;

bool win = false;
bool lose = false;

struct InputState {
    bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
    GLfloat lastX = 400, lastY = 300; //исходное положение мыши
    bool firstMouse = true;
    bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
    bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode) {
    switch (key) {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
            break;
        case GLFW_KEY_1:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case GLFW_KEY_2:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        default:
            if (action == GLFW_PRESS) {
                Input.keys[key] = true;
            } else if (action == GLFW_RELEASE) {
                Input.keys[key] = false;
            }
    }
}

void processPlayerMovement(Player &player) {
    if (Input.keys[GLFW_KEY_W]) {
        player.ProcessInput(MovementDir::UP, room_buff, &curr_room_ch, &trap_activated, &win, &lose);
    } else if (Input.keys[GLFW_KEY_S]) {
        player.ProcessInput(MovementDir::DOWN, room_buff, &curr_room_ch, &trap_activated, &win, &lose);
    } if (Input.keys[GLFW_KEY_A]) {
        player.ProcessInput(MovementDir::LEFT, room_buff, &curr_room_ch, &trap_activated, &win, &lose);
    } else if (Input.keys[GLFW_KEY_D]) {
        player.ProcessInput(MovementDir::RIGHT, room_buff, &curr_room_ch, &trap_activated, &win, &lose);
    }
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        Input.captureMouse = !Input.captureMouse;
    }
    if (Input.captureMouse) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        Input.capturedMouseJustNow = true;
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos) {
    if (Input.firstMouse) {
        Input.lastX = float(xpos);
        Input.lastY = float(ypos);
        Input.firstMouse = false;
    }

    GLfloat xoffset = float(xpos) - Input.lastX;
    GLfloat yoffset = Input.lastY - float(ypos);

    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
  // ...
}


int initGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    std::cout << "Controls: "<< std::endl;
    std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
    std::cout << "W, A, S, D - movement  "<< std::endl;
    std::cout << "press ESC to exit" << std::endl;

    return 0;
}

void readRoom(char ch) {
    std::ifstream file;
    std::string path = "./resources/_";
    path += ch;
    path += ".txt";
    file.open(path);
    std::string line;
    for (int i = 0; i < tileNum; i++) {
        file >> line;
        for (int j = 0; j < tileNum; j++) {
            room_buff[i][j] = line[j];
        }
    }
    for (int i = 0; i < tileNum; i++) {
        for (int j = 0; j < tileNum; j++) {
            std::cout << room_buff[i][j];
        }
        std::cout << std::endl;
    }
    file.close();
}

void findRoom() {
    if (curr_room_ch == 'U') {
        curr_room_i--;
    } else if (curr_room_ch == 'D') {
        curr_room_i++;
    } else if (curr_room_ch == 'L') {
        curr_room_j--;
    } else if (curr_room_ch == 'R') {
        curr_room_j++;
    }
}

void initLabirint() {
    std::ifstream file;
    file.open("./resources/global_map.txt");
    std::string line;
    for (int i = 0; i < 5; i++) {
        file >> line;
        for (int j = 0; j < 5; j++) {
            lab[i][j] = line[j];
            if (lab[i][j] == 'A') {
                curr_room_i = i;
                curr_room_j = j;
            }
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            std::cout << lab[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "curr_room - [" << curr_room_i << "][" << curr_room_j << "]\n";
    file.close();
}

void initDraw(Image &screenBuffer, Image &background, Image &wall, Image &floor, Image &gate, Image &trap, Image &winpic) {
    Point coords {.x = 0, .y = tileSize * (tileNum - 1)};
    for (int i = 0; i < tileNum; i++) {
        for (int j = 0; j < tileNum; j++) {
            for (int y = coords.y, p = 0; p < tileSize; y++, p++) {
                for (int x = coords.x, q = 0; q < tileSize; x++, q++) {
                    if (room_buff[i][j] == '#') {
                        screenBuffer.PutPixel(x, y, wall.GetPixel(q, p));
                        background.PutPixel(x, y, wall.GetPixel(q, p));
                    } else if (room_buff[i][j] == 'U' || room_buff[i][j] == 'D'
                            || room_buff[i][j] == 'L' || room_buff[i][j] == 'R') {
                        screenBuffer.PutPixel(x, y, gate.GetPixel(q, p));
                        background.PutPixel(x, y, gate.GetPixel(q, p));
                    } else if (room_buff[i][j] == '!') {
                        screenBuffer.PutPixel(x, y, winpic.GetPixel(q, tileSize - p));
                        background.PutPixel(x, y, winpic.GetPixel(q, tileSize - p));
                    } else if (room_buff[i][j] == 't' && trap_activated) {
                        screenBuffer.PutPixel(x, y, trap.GetPixel(q, tileSize - p));
                        background.PutPixel(x, y, trap.GetPixel(q, tileSize - p));
                    } else {
                        screenBuffer.PutPixel(x, y, floor.GetPixel(q, p));
                        background.PutPixel(x, y, floor.GetPixel(q, p));
                    }
                }
            }
            coords.x += tileSize;
        }
        coords.x = 0;
        coords.y -= tileSize;
    }
}

int main(int argc, char** argv) {
    if (!glfwInit()) {
        return -1;
    }
    //	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback        (window, OnKeyboardPressed);
    glfwSetCursorPosCallback  (window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
    glfwSetScrollCallback     (window, OnMouseScroll);

    if (initGL() != 0) {
        return -1;
    }

    //Reset any OpenGL errors which could be present for some reason
    GLenum gl_error = glGetError();
    while (gl_error != GL_NO_ERROR) {
        gl_error = glGetError();
    }


    Point starting_pos{.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2};
    Player player{starting_pos};

    Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
    Image wall("./resources/wall.png");
    Image floor("./resources/floor.png");
    Image gate("./resources/gate.png");
    Image trap("./resources/trap.png");
    Image winpic("./resources/win.png");
    Image background(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
    Image boi("./resources/player.png");

    initLabirint();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

    readRoom('A');
    initDraw(screenBuffer, background, wall, floor, gate, trap, winpic);
    //game loop
    while (!glfwWindowShouldClose(window) && !win && !lose) {
        if (curr_room_ch != '\0') {
            findRoom();
            readRoom(lab[curr_room_i][curr_room_j]);
            initDraw(screenBuffer, background, wall, floor, gate, trap, winpic);
            player.changePos(&curr_room_ch);
            trap_activated = false;
            trap_on_screen = false;
        }
        if (!trap_on_screen && trap_activated) {
            initDraw(screenBuffer, background, wall, floor, gate, trap, winpic);
            player.changePos(&curr_room_ch);
            trap_on_screen = true;
        }
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        player.ctimeInc(deltaTime);
        glfwPollEvents();

        curr_room_ch = '\0';
        processPlayerMovement(player);
        player.Draw(screenBuffer, background, boi);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

        glfwSwapBuffers(window);
    }
    if (win) {
        Image youwin("./resources/youwin.png");
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            for (int x = 0; x < WINDOW_WIDTH; x++) {
                screenBuffer.PutPixel(x, y, youwin.GetPixel(x, WINDOW_HEIGHT - y - 1));
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
        glfwSwapBuffers(window);
    } else if (lose) {
        Image youlose("./resources/youlose.png");
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            for (int x = 0; x < WINDOW_WIDTH; x++) {
                screenBuffer.PutPixel(x, y, youlose.GetPixel(x, WINDOW_HEIGHT - y - 1));
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
        glfwSwapBuffers(window);
    }
    sleep(2);
    glfwTerminate();
    return 0;
}
