#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main() {\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"uniform bool drawBlack;\n" // Перемикач кольору
"void main() {\n"
"   if(drawBlack) FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n" // Чорний для лінії
"   else FragColor = vec4(ourColor, 1.0f);\n"             // Градієнт для фону
"}\n\0";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Interpolated Rectangle with Diagonal", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    float vertices[] = {
        // Позиції          // Кольори
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // В1: Червона
         0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f, // В2: Жовта
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // В3: Зелена
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // В4: Синя
    };


    unsigned int indices[] = {
        0, 1, 3, // Т1
        1, 2, 3  // Т2
    };

    // Індекси для чорної діагоналі (лінія між Жовтою та Синьою вершинами)
    unsigned int diagonalIndices[] = { 1, 3 };

    unsigned int VBO, VAO, EBO, diagonalEBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &diagonalEBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, diagonalEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(diagonalIndices), diagonalIndices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    int drawBlackLoc = glGetUniformLocation(shaderProgram, "drawBlack");

    //Цикл рендерингу
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        //Малюємо заповнені трикутники (градієнт)
        glUniform1i(drawBlackLoc, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //Малюємо чорну діагональ поверх
        glUniform1i(drawBlackLoc, 1); // Вмикаємо чорний режим
        glLineWidth(2.0f);            // Можна трохи потовщити лінію
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, diagonalEBO);
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &diagonalEBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}