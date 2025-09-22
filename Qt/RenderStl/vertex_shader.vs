#version 330 core
layout (location = 0) in vec3 mPosition;
layout (location = 1) in vec3 mNormal;

out vec4 vColor;

uniform mat4 MVP;
uniform mat4 MVP_grid;
uniform mat4 model_to_world;
uniform mat4 world_to_view;
uniform int select_obj;

const vec3 directionToLight0 = normalize(vec3(0.0, 10.0, -5.0));
const vec3 directionToLight1 = normalize(vec3(0.0, 10.0, 5.0));
const vec3 directionToLight2 = normalize(vec3(5.0, 0.0, 0.0));
const vec3 directionToLight3 = normalize(vec3(-5.0, 0.0, 0.0));
const int GL_OBJ_BED = 0;
const int GL_OBJ_FRAME = 1;
const int GL_OBJ_SLICE = 2;
const int GL_OBJ_INFILL = 3;
const int GL_OBJ_SUPPORT = 4;
const int GL_OBJ_BBOX = 5;
const int GL_OBJ_DEFAULT = 6;
const int GL_OBJ_DEBUG = 7;

void main() {
        if (select_obj == GL_OBJ_BED) {
            gl_Position =  MVP_grid * vec4(mPosition, 1);
        } else if (select_obj == GL_OBJ_SLICE) {
            gl_Position =  MVP * vec4(mPosition, 1);
            vColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
            return;
        } else if (select_obj == GL_OBJ_FRAME) {
            gl_Position =  MVP_grid * vec4(mPosition, 1);
            vColor = vec4(1.0f, 0.2f, 0.4f, 1.0f);
            return;
        } else if (select_obj == GL_OBJ_DEBUG) {
            gl_Position =  MVP * vec4(mPosition, 1);
            vColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
            return;
        } else {
            gl_Position =  MVP * vec4(mPosition, 1);
        }

        vec3 tNormal;
        if (select_obj == GL_OBJ_DEBUG) {
            tNormal =  (model_to_world * vec4(mNormal, 1)).xyz;
        } else {
            tNormal = mNormal;
        }

        float clampedCosine0 = clamp(dot(tNormal, directionToLight0), 0.0, 1.0);
        float clampedCosine1 = clamp(dot(tNormal, directionToLight1), 0.0, 1.0);
        float clampedCosine2 = clamp(dot(tNormal, directionToLight2), 0.0, 1.0);
        float clampedCosine3 = clamp(dot(tNormal, directionToLight3), 0.0, 1.0);

        vec3 litColor = vec3(clampedCosine0 + clampedCosine1 + clampedCosine2 +
                        clampedCosine3);

        if (select_obj == GL_OBJ_DEFAULT) {
            litColor *= vec3(1.0f, 0.3f, 0.3f);
        }

        vColor = vec4(litColor, 1);
}
