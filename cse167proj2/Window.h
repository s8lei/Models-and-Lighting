#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "shader.h"

class Window
{
public:
    static int width;
    static int height;
    static const char* windowTitle;
    static PointCloud * bunnyPoints;
    static PointCloud * bearPoints;
    static PointCloud * dragonPoints;
    static PointCloud * lightPoints;
    static PointCloud * spotPoints;
    static Object * currentObj;
    static glm::mat4 projection;
    static glm::mat4 view;
    static glm::vec3 eye, center, up;
    static GLuint program, projectionLoc, viewLoc, modelLoc, colorLoc, colorModeLoc, isLightLoc, lightPosLoc, isDirLightLoc, isPtLightLoc, isSptLightLoc, dirLightDirLoc, sptLightDirLoc, materialALoc, materialDLoc, materialSLoc, materialShLoc, cutoffLoc;
    static bool colorMode, isPtLight, isDirLight, isSptLight;
    static int keyMode;
    static bool rotate;
    static glm::vec3 lastPos;
    static glm::vec3 curPos;
    static glm::vec3 lightPos;
    static glm::vec3 dirLightDir;
    static glm::vec3 sptLightDir;
    static bool initializeProgram();
    static bool initializeObjects();
    static void cleanUp();
    static GLFWwindow* createWindow(int width, int height);
    static void resizeCallback(GLFWwindow* window, int width, int height);
    static void idleCallback();
    static void displayCallback(GLFWwindow*);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static glm::vec3 trackBallMapping(glm::vec2 point);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
};

#endif

