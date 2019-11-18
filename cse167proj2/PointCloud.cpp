#include "PointCloud.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

std::map<std::string, int> PointCloud::indexForObj = {{"bunny.obj", 1}, {"bear.obj", 2}, {"dragon.obj", 3}, {"sphere.obj", 4}};;
PointCloud::PointCloud(std::string objFilename, GLfloat pointSize)
    : pointSize(pointSize)
{
    
    parse(objFilename);

    
    center();
    scale();
    // Set the model matrix to an identity matrix.
    model = glm::mat4(1);
    // Set the color.
    color = glm::vec3(1, 0, 0);
    switch (indexForObj[objFilename]) {
        case 1:
            ambient = glm::vec3(0.0215, 0.1745, 0.0215);
            diffuse = glm::vec3(0.0, 0.0, 0.0);
            specular = glm::vec3(0.633, 0.727811, 0.633);
            shininess = 0.6;
            break;
        case 2:
            ambient = glm::vec3(0.1, 0.0, 0.0);
            diffuse = glm::vec3(0.61424, 0.04136, 0.04136);
            specular = glm::vec3(0.0, 0.0, 0.0);
            shininess = 0.0;
            break;
        case 3:
            ambient = glm::vec3(0.0, 0.0, 0.0);
            diffuse = glm::vec3(0.54, 0.89, 0.63);
            specular = glm::vec3(0.316228, 0.316228, 0.316228);
            shininess = 0.2;
            break;
        case 4:
            model = glm::scale(glm::vec3(0.05f)) * model;
            model = glm::translate(glm::vec3(1.2f, 1.0f, 15.0f)) * model;
            ambient = glm::vec3(1.0f, 0.5f, 0.31f);
            diffuse = glm::vec3(0.0, 0.0, 0.0);
            specular = glm::vec3(0.0, 0.0, 0.0);
        default:
            break;
    }
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbos);
    glGenBuffers(1, &ebo);
    
    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
        points.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
    normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Bind to the EBO. We will use it to store the indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // Pass in the data.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
        indices.data(), GL_STATIC_DRAW);
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

PointCloud::~PointCloud()
{
    // Delete the VBO and the VAO.
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw()
{
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Set point size.
    //glPointSize(pointSize);
    // Draw points
    //glDrawArrays(GL_POINTS, 0, points.size());
    glDrawElements(GL_TRIANGLES, indices.size()*3, GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    
}

void PointCloud::update()
{
    // Spin the cube by 1 degree.
    spin(0.1f);
}

void PointCloud::updatePointSize(GLfloat size)
{
    /*
     * TODO: Section 3: Implement this function to adjust the point size.
     */
    pointSize = size;
}

void PointCloud::spin(float deg)
{
    // Update the model matrix by multiplying a rotation matrix
    //model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void PointCloud::parse(std::string objFilename)
{
    std::ifstream objFile(objFilename); // The obj file we are reading.

    // Check whether the file can be opened.
    if (objFile.is_open())
    {
        std::string line; // A line in the file.

        // Read lines from the file.
        while (std::getline(objFile, line))
        {
            // Turn the line into a string stream for processing.
            std::stringstream ss;
            ss << line;
            
            // Read the first word of the line.
            std::string label;
            ss >> label;

            // If the line is about vertex (starting with a "v").
            if (label == "v")
            {
                // Read the later three float numbers and use them as the
                // coordinates.
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;

                // Process the point. For example, you can save it to a.
                points.push_back(point);
            }
            if (label == "vn")
            {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
            if (label == "f")
            {
                glm::ivec3 index;
                std::string ind;
                ss >> ind;
                ind = ind.substr(0, ind.size()/2-1);
                index.x = stoi(ind)-1;
                ss >> ind;
                ind = ind.substr(0, ind.size()/2-1);
                index.y = stoi(ind)-1;
                ss >> ind;
                ind = ind.substr(0, ind.size()/2-1);
                index.z = stoi(ind)-1;
                indices.push_back(index);
                //std::cout << "index  is " << index.x << " "<<index.y <<" "<< index.z;
            }
        }
    }
    else
    {
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }

    objFile.close();
}

void PointCloud::center()
{
    float x_min = std::numeric_limits<float>::max();
    float x_max = std::numeric_limits<float>::lowest();
    float y_min = std::numeric_limits<float>::max();
    float y_max = std::numeric_limits<float>::lowest();
    float z_min = std::numeric_limits<float>::max();
    float z_max = std::numeric_limits<float>::lowest();
    for ( int i = 0; i<points.size(); i++ )
    {
        x_min = fmin(x_min, points[i].x);
        x_max = fmax(x_max, points[i].x);
        y_min = fmin(y_min, points[i].y);
        y_max = fmax(y_max, points[i].y);
        z_min = fmin(z_min, points[i].z);
        z_max = fmax(z_max, points[i].z);
        
    }
    
    objCenter.x = (x_max+x_min)/2;
    objCenter.y = (y_max+y_min)/2;
    objCenter.z = (z_max+z_min)/2;
    for ( int i = 0; i<points.size(); i++ )
    {
        points[i].x -= objCenter.x;
        points[i].y -= objCenter.y;
        points[i].z -= objCenter.z;
        
    }
    objCenter.x = 0;
    objCenter.y = 0;
    objCenter.z = 0;
}

void PointCloud::scale()
{
    float maxD = 0;
    for ( int i = 0; i<points.size(); i++ )
    {
        float distance = glm::distance(points[i], objCenter);
        maxD = std::max(maxD, distance);
    }
    
    float scale = maxD/9.9;
    for ( int i = 0; i<points.size(); i++ )
    {
        points[i] *= 1/scale;
    }
}

void PointCloud::scale(float scale)
{
    model = glm::scale(glm::vec3(scale, scale, scale))*model;
}
void PointCloud::rotate(float angle, glm::vec3 axis)
{
    model = glm::rotate(angle, axis)*model;
}
GLfloat PointCloud::getPointSize()
{
    return pointSize;
}

void PointCloud::translate(glm::vec3 displacement)
{
    model = glm::translate(displacement) * model;
}
