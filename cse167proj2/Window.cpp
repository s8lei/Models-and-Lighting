#include "Window.h"

int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

// Objects to display.
PointCloud * Window::bunnyPoints;
PointCloud * Window::bearPoints;
PointCloud * Window::dragonPoints;
PointCloud * Window::lightPoints;
PointCloud * Window::spotPoints;

// The object currently displaying.
Object * Window::currentObj;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 0, 20); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint Window::program; // The shader program id.

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.
GLuint Window::colorModeLoc;
GLuint Window::isLightLoc;
GLuint Window::lightPosLoc;
GLuint Window::isDirLightLoc;
GLuint Window::isPtLightLoc;
GLuint Window::isSptLightLoc;
GLuint Window::dirLightDirLoc;
GLuint Window::sptLightDirLoc;
GLuint Window::materialALoc;
GLuint Window::materialDLoc;
GLuint Window::materialSLoc;
GLuint Window::materialShLoc;
GLuint Window::cutoffLoc;

bool Window::colorMode;
bool Window::isPtLight = true;
bool Window::isDirLight = false;
bool Window::isSptLight = false;
int Window::keyMode = 0;

bool Window::rotate = false;
glm::vec3 Window::lastPos;
glm::vec3 Window::curPos;

glm::vec3 Window::lightPos = glm::vec3(1.2f, 1.0f, 15.0f);
glm::vec3 Window::dirLightDir = glm::vec3(-0.2f, -1.0f, -0.3f);
glm::vec3 Window::sptLightDir = glm::vec3(1.2f, 1.0f, 15.0f);
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    program = LoadShaders("shader.vert", "shader.frag");

    // Check the shader program.
    if (!program)
    {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    // Activate the shader program.
    glUseProgram(program);
    // Get the locations of uniform variables.
    projectionLoc = glGetUniformLocation(program, "projection");
    viewLoc = glGetUniformLocation(program, "view");
    modelLoc = glGetUniformLocation(program, "model");
    colorLoc = glGetUniformLocation(program, "color");
    colorModeLoc = glGetUniformLocation(program, "colorMode");
    isLightLoc = glGetUniformLocation(program, "isLight");
    isDirLightLoc = glGetUniformLocation(program, "isDirLight");
    isPtLightLoc = glGetUniformLocation(program, "isPtLight");
    isSptLightLoc = glGetUniformLocation(program, "isSptLight");
    lightPosLoc = glGetUniformLocation(program, "lightPos");
    dirLightDirLoc = glGetUniformLocation(program, "dirLightDir");
    sptLightDirLoc = glGetUniformLocation(program, "sptLightDir");
    materialALoc = glGetUniformLocation(program, "material.ambient");
    materialDLoc = glGetUniformLocation(program, "material.diffuse");
    materialSLoc = glGetUniformLocation(program, "material.specular");
    materialShLoc = glGetUniformLocation(program, "material.shininess");
    cutoffLoc = glGetUniformLocation(program, "cutoff");
    return true;
}

bool Window::initializeObjects()
{
    // Create a point cloud consisting of cube vertices.
    bunnyPoints = new PointCloud("bunny.obj", 1);
    bearPoints = new PointCloud("bear.obj", 1);
    dragonPoints = new PointCloud("dragon.obj", 1);
    // Set cube to be the first to display
    currentObj = bunnyPoints;
    lightPoints = new PointCloud("sphere.obj", 1);
    spotPoints = new PointCloud("sphere.obj", 1);
    spotPoints->ambient = glm::vec3(0.0, 0.0, 0.0);
    spotPoints->translate(glm::vec3(-3.6f, -3.0f, 0.0f));
    
    return true;
}

void Window::cleanUp()
{
    // Deallcoate the objects.
    delete bunnyPoints;
    delete bearPoints;
    delete dragonPoints;
    delete lightPoints;
    

    // Delete the shader program.
    glDeleteProgram(program);
}

GLFWwindow* Window::createWindow(int width, int height)
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.

    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

    // Initialize GLEW.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    // Set the projection matrix.
    Window::projection = glm::perspective(glm::radians(60.0),
        double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
    // Perform any updates as necessary.
    currentObj->update();
        
}

void Window::displayCallback(GLFWwindow* window)
{
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Specify the values of the uniform variables we are going to use.
    glm::mat4 model = currentObj->getModel();
    glm::vec3 color = currentObj->getColor();
    glm::vec3 ambient = ((PointCloud*)currentObj)->ambient;
    glm::vec3 diffuse = ((PointCloud*)currentObj)->diffuse;
    glm::vec3 specular = ((PointCloud*)currentObj)->specular;
    GLfloat shininess = ((PointCloud*)currentObj)->shininess;
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    glUniform1i(colorModeLoc, colorMode);
    glUniform1i(isLightLoc, false);
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    glUniform3fv(materialALoc, 1, glm::value_ptr(ambient));
    glUniform3fv(materialDLoc, 1, glm::value_ptr(diffuse));
    glUniform3fv(materialSLoc, 1, glm::value_ptr(specular));
    glUniform1f(materialShLoc, shininess);
    // Render the object.
    currentObj->draw();
    glm::mat4 lightModel = lightPoints->getModel();
//    lightModel = glm::scale(glm::vec3(0.05f)) * lightModel;
//    lightModel = glm::translate(lightPos) * lightModel;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform1i(isLightLoc, true);
    glUniform3fv(materialALoc, 1, glm::value_ptr(lightPoints->ambient));
    glUniform3fv(materialDLoc, 1, glm::value_ptr(lightPoints->diffuse));
    glUniform3fv(materialSLoc, 1, glm::value_ptr(lightPoints->specular));
    glUniform1f(materialShLoc, 0.0f);
    lightPoints->draw();
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(spotPoints->getModel()));
    glUniform1i(isLightLoc, true);
    glUniform3fv(materialALoc, 1, glm::value_ptr(spotPoints->ambient));
    glUniform3fv(materialDLoc, 1, glm::value_ptr(spotPoints->diffuse));
    glUniform3fv(materialSLoc, 1, glm::value_ptr(spotPoints->specular));
    glUniform1f(materialShLoc, 0.0f);
    glUniform1f(cutoffLoc, glm::cos(glm::radians(12.5f)));
    spotPoints->draw();
    
    
    glUniform1i(isDirLightLoc, isDirLight);
    glUniform1i(isPtLightLoc, isPtLight);
    glUniform1i(isSptLightLoc, isSptLight);
    glUniform3fv(dirLightDirLoc, 1, glm::value_ptr(dirLightDir));
    glUniform3fv(sptLightDirLoc, 1, glm::value_ptr(sptLightDir));
    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    /*
     * TODO: Modify below to add your key callbacks.
     */
    
    // Check for a key press.
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            // Close the window. This causes the program to also terminate.
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_F1:
            // Set currentObj to bunny
            currentObj = bunnyPoints;
            break;
        case GLFW_KEY_F2:
            // Set currentObj to bear
            currentObj = bearPoints;
            break;
        case GLFW_KEY_F3:
            // Set currentObj to dragon
            currentObj = dragonPoints;
            break;
        case GLFW_KEY_P:
            if (action == GLFW_PRESS && mods == GLFW_MOD_SHIFT)
            {
                currentObj->updatePointSize(currentObj->getPointSize()+0.1);
            }
            else
            {
                currentObj->updatePointSize(currentObj->getPointSize()-0.1);
            }
            break;
        case GLFW_KEY_N:
            colorMode = !colorMode;
            break;
        case GLFW_KEY_1:
            keyMode = 1;
            break;
        case GLFW_KEY_2:
            keyMode = 2;
            break;
        case GLFW_KEY_3:
            keyMode = 3;
            break;
        case GLFW_KEY_4:
            isPtLight = !isPtLight;
            lightPoints->ambient = isPtLight? glm::vec3(1.0f, 0.5f, 0.31f): glm::vec3(0.0, 0.0, 0.0);
            std::cout<< "point light is "<<isPtLight;
            break;
        case GLFW_KEY_5:
            isDirLight = !isDirLight;
            std::cout<< "dir light is "<<isDirLight;
            break;
        case GLFW_KEY_6:
            isSptLight = !isSptLight;
            spotPoints->ambient = isSptLight? glm::vec3(1.0f, 0.5f, 0.31f): glm::vec3(0.0, 0.0, 0.0);
            std::cout<< "spot light is "<<isSptLight;
            break;
        case GLFW_KEY_7:
            keyMode = 7;
            break;
        case GLFW_KEY_8:
            keyMode = 8;
            break;
        default:
            break;
        }
    }
}

glm::vec3 Window::trackBallMapping(glm::vec2 point)
{
    glm::vec3 v;
    float  d;
    
    v.x = (2.0f * point.x - 640) / 640;
    v.y = (480 - 2.0f * point.y) / 480;
    v.z = 0.0f;
    
    d = glm::length(v);
    
    d = (d < 1.0f) ? d : 1.0f;
    v.z = sqrt(1.001f - d*d);
    v = glm::normalize(v);
    return v;
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    float offset = (yoffset > 0) ? 1+yoffset: 1/(1-yoffset);
    switch (keyMode) {
        case 0:
            ((PointCloud*)currentObj)->scale(offset);
            break;
        case 1:
            ((PointCloud*)currentObj)->scale(offset);
            break;
        case 2:
        {
            glm::vec3 center = glm::vec3(currentObj->getModel()*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            glm::vec3 prePos = lightPos;
            lightPos = glm::vec3(center + offset* (lightPos - center));
            lightPoints -> translate(lightPos - prePos);
            break;
        }
        case 3:
        {
            ((PointCloud*)currentObj)->scale(offset);
            glm::vec3 center = glm::vec3(currentObj->getModel()*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            glm::vec3 prePos = lightPos;
            lightPos = glm::vec3(center + offset* (lightPos - center));
            lightPoints -> translate(lightPos - prePos);
            break;
        }
        default:
            break;
    }
}
void Window::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    glm::vec2 cursor = glm::vec2(xpos, ypos);
    curPos = trackBallMapping(cursor);
    glm::vec3 dir;
    float pixel_diff;
    float rot_angle, zoom_factor;
    
    if(rotate)
    {
        dir = curPos - lastPos;
        float velocity = dir.length();
        if (velocity > 0.0001)
        {
            glm::vec3 rotAxis;
            rotAxis = glm::cross(lastPos, curPos);
            rot_angle = asin(sqrt(glm::dot(rotAxis,rotAxis))/(sqrt(glm::dot(curPos,curPos))*sqrt(glm::dot(lastPos,lastPos))));
            
            switch (keyMode) {
                case 0:
                    ((PointCloud*)currentObj)->rotate(rot_angle, rotAxis);
                    break;
                    
                case 1:
                {
                    ((PointCloud*)currentObj)->rotate(rot_angle, rotAxis);
                    break;
                }
                case 2:
                {
                    lightPoints->rotate(rot_angle, rotAxis);
                    lightPos = lightPoints->getModel()*glm::vec4(lightPos, 1.0f);
                    //std::cout<<"light pos: "<<lightPos.x<<" "<<lightPos.y<<" "<<lightPos.z;
                    break;
                }
                case 3:
                {
                    ((PointCloud*)currentObj)->rotate(rot_angle, rotAxis);
                    lightPoints->rotate(rot_angle, rotAxis);
                    lightPos = lightPoints->getModel()*glm::vec4(lightPos, 1.0f);
                }
                case 7:
                {
                    dirLightDir = glm::vec3(glm::rotate(rot_angle, rotAxis)* glm::vec4(dirLightDir, 1.0f));
                }
                case 8:
                {
                    sptLightDir = glm::vec3(glm::rotate(rot_angle, rotAxis)* glm::vec4(sptLightDir, 1.0f));
                }
                default:
                    break;
            }
        }
    }
    lastPos = curPos;
}

void Window::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            rotate = true;
            lastPos = curPos;
        }
        else
            rotate = false;
    }
}
