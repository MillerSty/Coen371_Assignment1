#include "UnitCube.h"

GLuint createUnitCubeVAO(vec3 leftColor,
                     vec3 backColor,
                     vec3 bottomColor,
                     vec3 frontColor,
                     vec3 rightColor,
                     vec3 topColor
) {
    vec3 vertexData[] = {
            vec3(-0.5f, -0.5f, -0.5f), leftColor,
            vec3(-0.5f, -0.5f, 0.5f), leftColor,
            vec3(-0.5f, 0.5f, 0.5f), leftColor,

            vec3(-0.5f, -0.5f, -0.5f), leftColor,
            vec3(-0.5f, 0.5f, 0.5f), leftColor,
            vec3(-0.5f, 0.5f, -0.5f), leftColor,

            vec3(0.5f, 0.5f, -0.5f), backColor,
            vec3(-0.5f, -0.5f, -0.5f), backColor,
            vec3(-0.5f, 0.5f, -0.5f), backColor,

            vec3(0.5f, 0.5f, -0.5f), backColor,
            vec3(0.5f, -0.5f, -0.5f), backColor,
            vec3(-0.5f, -0.5f, -0.5f), backColor,

            vec3(0.5f, -0.5f, 0.5f), bottomColor,
            vec3(-0.5f, -0.5f, -0.5f), bottomColor,
            vec3(0.5f, -0.5f, -0.5f), bottomColor,

            vec3(0.5f, -0.5f, 0.5f), bottomColor,
            vec3(-0.5f, -0.5f, 0.5f), bottomColor,
            vec3(-0.5f, -0.5f, -0.5f), bottomColor,

            vec3(-0.5f, 0.5f, 0.5f), frontColor,
            vec3(-0.5f, -0.5f, 0.5f), frontColor,
            vec3(0.5f, -0.5f, 0.5f), frontColor,

            vec3(0.5f, 0.5f, 0.5f), frontColor,
            vec3(-0.5f, 0.5f, 0.5f), frontColor,
            vec3(0.5f, -0.5f, 0.5f), frontColor,

            vec3(0.5f, 0.5f, 0.5f), rightColor,
            vec3(0.5f, -0.5f, -0.5f), rightColor,
            vec3(0.5f, 0.5f, -0.5f), rightColor,

            vec3(0.5f, -0.5f, -0.5f), rightColor,
            vec3(0.5f, 0.5f, 0.5f), rightColor,
            vec3(0.5f, -0.5f, 0.5f), rightColor,

            vec3(0.5f, 0.5f, 0.5f), topColor,
            vec3(0.5f, 0.5f, -0.5f), topColor,
            vec3(-0.5f, 0.5f, -0.5f), topColor,

            vec3(0.5f, 0.5f, 0.5f), topColor,
            vec3(-0.5f, 0.5f, -0.5f), topColor,
            vec3(-0.5f, 0.5f, 0.5f), topColor,
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(vertexData)), vertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6*sizeof(float),
                          (void *)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6*sizeof(float),
                          (void *)(3*sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}