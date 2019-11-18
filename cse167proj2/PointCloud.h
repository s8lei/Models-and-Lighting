#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <map>

#include "Object.h"

class PointCloud : public Object
{
private:
    std::vector<glm::vec3> points;
    GLuint vao, lightVao;
    GLuint vbos[2];
    GLuint ebo;
    GLuint lightVbo;
    GLfloat pointSize;
    glm::vec3 objCenter;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> indices;
    glm::mat4 lightModel;
    std::vector<glm::vec3> lightPoints;
    
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
    PointCloud(std::string objFilename, GLfloat pointSize);
    ~PointCloud();

    void draw();
    void update();

    void updatePointSize(GLfloat size);
    void spin(float deg);
    void parse(std::string objFilename);
    void center();
    void scale();
    void scale(float scale);
    void rotate(float angle, glm::vec3 axis);
    void translate(glm::vec3 displacement);
    GLfloat getPointSize();
    static std::map<std::string, int> indexForObj;
};

#endif


