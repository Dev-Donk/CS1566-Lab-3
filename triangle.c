/*
 * triangle.c
 *
 *  Created on: Aug 28, 2017
 *      Author: Thumrongsak Kosiyatrakul
 */


#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "initShader.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int display_mode = 0;

typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vec4;

vec4 vertices[63] =
{{-0.50, 0.75, 0.0, 1.0},	// Triangle Top
 {-0.75, 0.25, 0.0, 1.0},	// Triangle Bottom Left
 {-0.25, 0.25, 0.0, 1.0},	// Triangle Bottom Right
 { 0.25,  0.75, 0.0, 1.0},	// Square 1/2 Top Left
 { 0.25,  0.25, 0.0, 1.0},	// Square 1/2 Bottom Left
 { 0.75, 0.25, 0.0, 1.0},	// Square 1/2 Bottom Right
 { 0.75, 0.75, 0.0, 1.0},   // Square 2/2 Top Right
 { 0.25, 0.75, 0.0, 1.0},   // Square 2/2 
 { 0.75, 0.25, 0.0, 1.0}};

vec4 colors[63] =
{{1.0, 0.0, 0.0, 1.0},	// red   (for top)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom left)
 {0.0, 0.0, 1.0, 1.0},	// blue  (for bottom right)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom right)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom right)
 {0.0, 1.0, 0.0, 1.0},
 {0.0, 1.0, 0.0, 1.0},
 {0.0, 1.0, 0.0, 1.0},
 {0.0, 1.0, 0.0, 1.0}};	// green (for bottom right)

int num_vertices = 63;

void get_row_of_tris_ready() {
    float pos_x_1 = -0.825; // top of triangle
    float pos_y_1 = -0.25;

    float pos_x_2 = -1.0; // bottom verts
    float pos_y_2 = -0.50;

    int curr_pos = 9;

    for (int i = 0; i < 3; i++) {
        for(int k = 0; k < 18; k++) {
            if(k % 3 == 0) {
                // TOP OF TRIANGLES
                vertices[curr_pos].x = pos_x_1;
                vertices[curr_pos].y = pos_y_1;
                vertices[curr_pos].z = 0.0;
                vertices[curr_pos].w = 1.0;
                pos_x_1 += 0.333;
            } else {
                // BOTTOMS OF TRIANGLES
                vertices[curr_pos].x = pos_x_2;
                vertices[curr_pos].y = pos_y_2;
                vertices[curr_pos].z = 0.0;
                vertices[curr_pos].w = 1.0;
                if ((k + 1) % 3 != 0)
                    pos_x_2 += 0.333;
            }
            if (i == 0) { // RED
                colors[curr_pos].x = 1.0;
                colors[curr_pos].y = 0.0;
                colors[curr_pos].z = 0.0;
                colors[curr_pos].w = 1.0;
            } else if (i == 1) { // GREEN
                colors[curr_pos].x = 0.0;
                colors[curr_pos].y = 1.0;
                colors[curr_pos].z = 0.0;
                colors[curr_pos].w = 1.0;
            } else if (i == 2) { // BLUE
                colors[curr_pos].x = 0.0;
                colors[curr_pos].y = 0.0;
                colors[curr_pos].z = 1.0;
                colors[curr_pos].w = 1.0;
            }

            curr_pos++;
        }

        pos_x_1 = -0.825;
        pos_x_2 = -1.0;
        pos_y_1 -= 0.25;
        pos_y_2 -= 0.25;
    }
}

void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);


    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));
    

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    if(display_mode == 0)
        glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    else if(display_mode == 1)
        glDrawArrays(GL_TRIANGLES, 0, 3);
    else if(display_mode == 2) {
        glDrawArrays(GL_TRIANGLES, 3, 6);
    }
    else if (display_mode == 3) {
        glDrawArrays(GL_TRIANGLES, 9, num_vertices);
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	glutLeaveMainLoop();
    if(key == '1')
        display_mode = 0;
    if(key == '2')
        display_mode = 1;
    if(key == '3') 
        display_mode = 2;
    if(key == '4') 
        display_mode = 3;

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Triangle");
    glewInit();
    get_row_of_tris_ready();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
