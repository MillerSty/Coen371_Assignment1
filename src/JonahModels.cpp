// Authors: Jonah
#pragma once
#include "JonahModels.h"


using namespace glm;


JonahModels::JonahModels() {
    initTranslate = vec3(0.0f, 3.0f, 0.0f);
    VAO = createVertexArrayObject();
}

JonahModels::~JonahModels() {

}

	
void JonahModels::drawRacketJ(mat4 groupMatrix, vec3 modelTranslationVec, GLint colorLocation, GLuint worldMatrixLocation, float rotateAngle = 0.0f, float scaleFactor = 1.0f, float armRotateAngle = 0.0f)
    {
        glBindVertexArray(VAO);
        armTranslate = translate(mat4(1.0f), vec3(modelTranslationVec.x + initTranslate.x, modelTranslationVec.y + initTranslate.y, modelTranslationVec.z + initTranslate.z));
        armRotateMatrix = rotate(mat4(1.0f), radians(rotateAngle), vec3(.0f, 1.0f, .0f));
        armLocal = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(2.0f, 0.3f, 0.3f));
        armParentMatrix = scale(mat4(1.0f), vec3(1.0f * scaleFactor, 1.0f * scaleFactor, 1.0f * scaleFactor)) * armTranslate * armRotateMatrix;
        worldMatrix = groupMatrix * armParentMatrix * armLocal;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glUniform4f(colorLocation, 1.0f, 219.0f / 255.0f, 172.0f / 255.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        upperArmTranslate = translate(mat4(1.0f), vec3(initTranslate.x, initTranslate.y + 0.9f, initTranslate.z));
        upperArmRotate = translate(mat4(1.0f), vec3(initTranslate.x, initTranslate.y, initTranslate.z)) * rotate(mat4(1.0f), radians(0.0f + armRotateAngle), vec3(.0f, .0f, 1.0f)) * translate(mat4(1.0f), vec3(-initTranslate.x, -initTranslate.y, -initTranslate.z));
        upperArmLocal = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(2.0f, 0.3f, 0.3f));
        upperArmParent = armParentMatrix * upperArmRotate * upperArmTranslate;
        worldMatrix = groupMatrix * upperArmParent * upperArmLocal;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        racketBaseTranslate = translate(mat4(1.0f), vec3(0.0f, 1.75f, 0.0f));
        racketBaseLocal = scale(mat4(1.0f), vec3(0.25f, 1.5f, 0.25f));
        racketParentMatrix = upperArmParent * racketBaseTranslate;
        worldMatrix = groupMatrix * racketParentMatrix * racketBaseLocal;
        glUniform4f(colorLocation, 0.0f, 0.0f, 204.0f / 255.0f, 1.0f);
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        racketBaseTranslate = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f));
        racketBaseLocal = scale(mat4(1.0f), vec3(2.0f, 0.25f, 0.25f));
        racketParentMatrix = upperArmParent * racketBaseTranslate;
        worldMatrix = groupMatrix * racketParentMatrix * racketBaseLocal;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        racketBaseTranslate = translate(mat4(1.0f), vec3(-0.875f, 3.625f, 0.0f));
        racketBaseLocal = scale(mat4(1.0f), vec3(0.25f, 2.5f, 0.25f));
        racketParentMatrix = upperArmParent * racketBaseTranslate;
        worldMatrix = groupMatrix * racketParentMatrix * racketBaseLocal;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        racketBaseTranslate = translate(mat4(1.0f), vec3(0.875f, 3.625f, 0.0f));
        racketBaseLocal = scale(mat4(1.0f), vec3(0.25f, 2.5f, 0.25f));
        racketParentMatrix = upperArmParent * racketBaseTranslate;
        worldMatrix = groupMatrix * racketParentMatrix * racketBaseLocal;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        racketBaseTranslate = translate(mat4(1.0f), vec3(0.0f, 4.875f, 0.0f));
        racketBaseLocal = scale(mat4(1.0f), vec3(2.0f, 0.25f, 0.25f));
        racketParentMatrix = upperArmParent * racketBaseTranslate;
        worldMatrix = groupMatrix * racketParentMatrix * racketBaseLocal;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUniform4f(colorLocation, 0.7f, 1.0f, 0.2f, 1.0f);
        for (int i = 1; i < 9; ++i)
        {
            racketBaseTranslate = translate(mat4(1.0f), vec3(0.0f, 4.8125f - (i * 0.25), 0.0f));
            racketBaseLocal = scale(mat4(1.0f), vec3(1.9f, 0.05f, 0.05f));
            racketParentMatrix = upperArmParent * racketBaseTranslate;
            worldMatrix = groupMatrix * racketParentMatrix * racketBaseLocal;
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        for (int i = 1; i < 8; ++i)
        {
            racketBaseTranslate = translate(mat4(1.0f), vec3(0.8f - (i * 0.2), 3.625f, 0.0f));
            racketBaseLocal = scale(mat4(1.0f), vec3(0.05f, 2.4f, 0.05f));
            racketParentMatrix = upperArmParent * racketBaseTranslate;
            worldMatrix = groupMatrix * racketParentMatrix * racketBaseLocal;
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
    }

int JonahModels::createVertexArrayObject()
{
    // Cube model
    vec3 vertexArray[] = {
        // position
        vec3(-0.5f,-0.5f,-0.5f),
        vec3(-0.5f,-0.5f, 0.5f),
        vec3(-0.5f, 0.5f, 0.5f),

        vec3(-0.5f,-0.5f,-0.5f),
        vec3(-0.5f, 0.5f, 0.5f),
        vec3(-0.5f, 0.5f,-0.5f),

        vec3(0.5f, 0.5f,-0.5f),
        vec3(-0.5f,-0.5f,-0.5f),
        vec3(-0.5f, 0.5f,-0.5f),

        vec3(0.5f, 0.5f,-0.5f),
        vec3(0.5f,-0.5f,-0.5f),
        vec3(-0.5f,-0.5f,-0.5f),

        vec3(0.5f,-0.5f, 0.5f),
        vec3(-0.5f,-0.5f,-0.5f),
        vec3(0.5f,-0.5f,-0.5f),

        vec3(0.5f,-0.5f, 0.5f),
        vec3(-0.5f,-0.5f, 0.5f),
        vec3(-0.5f,-0.5f,-0.5f),

        vec3(-0.5f, 0.5f, 0.5f),
        vec3(-0.5f,-0.5f, 0.5f),
        vec3(0.5f,-0.5f, 0.5f),

        vec3(0.5f, 0.5f, 0.5f),
        vec3(-0.5f, 0.5f, 0.5f),
        vec3(0.5f,-0.5f, 0.5f),

        vec3(0.5f, 0.5f, 0.5f),
        vec3(0.5f,-0.5f,-0.5f),
        vec3(0.5f, 0.5f,-0.5f),

        vec3(0.5f,-0.5f,-0.5f),
        vec3(0.5f, 0.5f, 0.5f),
        vec3(0.5f,-0.5f, 0.5f),

        vec3(0.5f, 0.5f, 0.5f),
        vec3(0.5f, 0.5f,-0.5f),
        vec3(-0.5f, 0.5f,-0.5f),

        vec3(0.5f, 0.5f, 0.5f),
        vec3(-0.5f, 0.5f,-0.5f),
        vec3(-0.5f, 0.5f, 0.5f)
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, // attribute 0 matches aPos in Vertex Shader
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride - each vertex contains only 1 vec3 (position)
        (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}


