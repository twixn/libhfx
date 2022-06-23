#include <hfx.h>
#include <hfx_types.h>
#include <string.h>
#include <math.h>
#include <hfx_int.h>

#define TO_RAD(x) ((x)*(M_PI)/180.0f)

void hfx_matrix_multiply(hfx_state *state, float *a, float *b, float *result)
{
    float temp[16] = {0};

    for(int i=0; i < 4; i++)
    {
        for(int j=0; j < 4; j++)
        {
            for(int k=0; k < 4; k++)
            {
                temp[i*4 + j] += a[k*4 + j]*b[i*4 + k];
            }
        }
    }

    memcpy(result, temp, sizeof(temp));
}

void hfx_matrix_vector_multiply(hfx_state *state, float *mat, float *vec, float *result)
{
    float temp[4] = {0};

    for(int i=0; i < 4; i++)
    {
        for(int j=0; j < 4; j++)
        {
            temp[i] += mat[j*4 + i]*vec[j];
        }
    }

    memcpy(result, temp, sizeof(temp));
}

void hfx_normalize(hfx_state *state, float *vector, float *result)
{
    float temp[4];
    float mag = sqrtf(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2] + vector[3]*vector[3]);

    if(mag == 0)
    {
        hfx_fatal_error(state, "normalize zero vector");
    }
    else
    {
        temp[0] = vector[0]/mag;
        temp[1] = vector[1]/mag;
        temp[2] = vector[2]/mag;
        temp[3] = vector[3]/mag;
    }
    
    memcpy(result, temp, sizeof(temp));
}


void hfx_normalize3(hfx_state *state, float *vector, float *result)
{
    float temp[4];
    float mag = sqrtf(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);

    if(mag == 0)
    {
        hfx_fatal_error(state, "normalize zero vector");
    }
    else
    {
        temp[0] = vector[0]/mag;
        temp[1] = vector[1]/mag;
        temp[2] = vector[2]/mag;
        temp[3] = 1.0f;
    }
    
    memcpy(result, temp, sizeof(temp));
}

float hfx_dot3(float *a, float *b)
{
    return (a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
}

void hfx_cross3(float *a, float *b, float *result)
{
    float temp[4];
    temp[0] = a[1]*b[2] - a[2]*b[1];
    temp[1] = a[2]*b[0] - a[0]*b[2];
    temp[2] = a[0]*b[1] - a[1]*b[0];
    temp[3] = 1.0f;
    memcpy(result, temp, sizeof(temp));
}

void hfx_load_identity(hfx_state *state)
{
    state->model_matrix[0] = 1.0f;
    state->model_matrix[1] = 0.0f;
    state->model_matrix[2] = 0.0f;
    state->model_matrix[3] = 0.0f;

    state->model_matrix[4] = 0.0f;
    state->model_matrix[5] = 1.0f;
    state->model_matrix[6] = 0.0f;
    state->model_matrix[7] = 0.0f;

    state->model_matrix[8] = 0.0f;
    state->model_matrix[9] = 0.0f;
    state->model_matrix[10] = 1.0f;
    state->model_matrix[11] = 0.0f;

    state->model_matrix[12] = 0.0f;
    state->model_matrix[13] = 0.0f;
    state->model_matrix[14] = 0.0f;
    state->model_matrix[15] = 1.0f;
}

void hfx_translate_f(hfx_state *state, float x, float y, float z)
{
    float result[16] = {0};
    result[0] = 1.0f;
    result[5] = 1.0f;
    result[10] = 1.0f;
    result[12] = x;
    result[13] = y;
    result[14] = z;
    result[15] = 1.0f;

    hfx_matrix_multiply(state, state->model_matrix, result, state->model_matrix);
}

void hfx_rotate_f(hfx_state *state, float angle, float x, float y, float z)
{
    float axis[4] = {0};
    float normalized_axis[4];
    float result[16];
    float angle_rad = TO_RAD(angle);
    float c = cos(angle_rad);
    float s = sin(angle_rad);

    axis[0] = x;
    axis[1] = y;
    axis[2] = z;
    
    hfx_normalize(state, axis, normalized_axis);

    result[0] = c + (1-c)*normalized_axis[0]*normalized_axis[0];
    result[1] = (1-c)*normalized_axis[0]*normalized_axis[1] + s*normalized_axis[2];
    result[2] = (1-c)*normalized_axis[0]*normalized_axis[2] - s*normalized_axis[1];
    result[3] = 0.0f;

    result[4] = (1-c)*normalized_axis[1]*normalized_axis[0] - s*normalized_axis[2];
    result[5] = c + (1-c)*normalized_axis[1]*normalized_axis[1];
    result[6] = (1-c)*normalized_axis[1]*normalized_axis[2] + s*normalized_axis[0];
    result[7] = 0.0f;

    result[8] = (1-c)*normalized_axis[2]*normalized_axis[0] + s*normalized_axis[1];
    result[9] = (1-c)*normalized_axis[2]*normalized_axis[1] - s*normalized_axis[0];
    result[10] = c + (1-c)*normalized_axis[2]*normalized_axis[2];
    result[11] = 0.0f;

    result[12] = 0.0f;
    result[13] = 0.0f;
    result[14] = 0.0f;
    result[15] = 1.0f;

    hfx_matrix_multiply(state, state->model_matrix, result, state->model_matrix);
}

void hfx_scale_f(hfx_state *state, float sx, float sy, float sz)
{
    float result[16] = {0};

    result[0] = sx;
    result[5] = sy;
    result[10] = sz;
    result[15] = 1.0f;

    hfx_matrix_multiply(state, state->model_matrix, result, state->model_matrix);
}

void hfx_ortho_f(hfx_state *state, float left, float right, float top, float bottom, float near, float far)
{
    float result[16] = {0};

    result[0] = 2.0f / (right - left);
    result[5] = 2.0f / (top - bottom);
    result[10] = -2.0f / (far-near);
    result[12] = -(right + left) / (right - left);
    result[13] = -(top + bottom) / (top - bottom);
    result[14] = -(far + near) / (far - near);
    result[15] = 1.0f;

    hfx_matrix_multiply(state, state->model_matrix, result, state->model_matrix);
}

void hfx_persp_f(hfx_state *state, float fovy, float aspect, float znear, float zfar)
{
    float result[16] = {0};
    float tan_half_fov = tan(TO_RAD(fovy) / 2.0f);

    result[0] = 1.0f / (aspect * tan_half_fov);
    result[5] = 1.0f / tan_half_fov;
    result[10] = (zfar + znear) / (znear - zfar);
    result[11] = -1.0f;
    result[14] = (2.0f * zfar * znear) / (znear - zfar);

    hfx_matrix_multiply(state, state->model_matrix, result, state->model_matrix);
}


void hfx_lookat_f(hfx_state *state, float eye_x, float eye_y, float eye_z, float at_x, float at_y, float at_z, float up_x, float up_y, float up_z)
{
    float result[16] = {0};

    float dir[4];
    float up[4];
    float eye[4];
    float xaxis[4];
    float yaxis[4];
    float zaxis[4];
    float side[4];

    //dir[0] = at_x - eye_x;
    //dir[1] = at_y - eye_y;
    //dir[2] = at_z - eye_z;
    dir[0] = eye_x - at_x;
    dir[1] = eye_y - at_y;
    dir[2] = eye_z - at_z;

    up[0] = up_x;
    up[1] = up_y;
    up[2] = up_z;
    eye[0] = eye_x;
    eye[1] = eye_y;
    eye[2] = eye_z;

    hfx_normalize3(state, dir, zaxis);
    hfx_cross3(up, zaxis, side);
    hfx_normalize3(state, side, xaxis);
    hfx_cross3(zaxis, xaxis, yaxis);
    
    result[0] = xaxis[0];
    result[1] = yaxis[0];
    result[2] = zaxis[0];
    result[3] = 0.0f;

    result[4] = xaxis[1];
    result[5] = yaxis[1];
    result[6] = zaxis[1];
    result[7] = 0.0f;

    result[8] = xaxis[2];
    result[9] = yaxis[2];
    result[10] = zaxis[2];
    result[11] = 0.0f;

    result[12] = hfx_dot3(xaxis,eye) * -1;//-eye[0];//
    result[13] = hfx_dot3(yaxis,eye) * -1;//-eye[1];//
    result[14] = hfx_dot3(zaxis,eye) * -1;//-eye[2];//
    //result[12] = -eye[0];//
    //result[13] = -eye[1];//
    //result[14] = -eye[2];//
    result[15] = 1.0f;

    hfx_matrix_multiply(state, state->model_matrix, result, state->model_matrix);
}

void hfx_mult_matrix_f(hfx_state *state, float *mat)
{
    hfx_matrix_multiply(state, state->model_matrix, mat, state->model_matrix);
}