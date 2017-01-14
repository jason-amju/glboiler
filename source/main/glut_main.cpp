// -----------------------------------------------------------------------------
// glboiler - Jason Colman 2016-2017 - OpenGL experiments
// -----------------------------------------------------------------------------

// Build libpng: in libpng, 
//   cp scripts/makefile.darwin .. ; make -f makefile.darwin
// That builds libpng.a in the libpng dir. Then in source/main/
//   make clean ; make ; make run

#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include "camera.h"
#include "forward_renderer.h"
#include "gl_includes.h"
#include "gl_shader.h"
#include "gl_system.h"
#include "look_at.h"
#include "projection.h"
#include "teapot_scene_node.h" // TODO TEMP TEST
#include "timer.h"

static int WIN_X = 1000;
static int WIN_Y = 500;

void display()
{
    //time_this_block("Rendering");

    GL_CHECK(glClearColor(1, 0, 0, 1));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GL_CHECK(glEnable(GL_DEPTH_TEST));

    forward_renderer rend;
    rend.init_on_gl_thread();

    camera left_cam, right_cam;
    float eye_sep = 0.5f;
    float z_dist = 12.0f;
    vec3 left(-eye_sep, 6, z_dist);
    vec3 right(eye_sep, 6, z_dist);
    vec3 up(0, 1, 0);
    look_at(left, -left, up).set_matrix(left_cam.look_at_matrix);
    look_at(right, -right, up).set_matrix(right_cam.look_at_matrix);

    perspective p(45, 1, 0.1, 100);
    p.set_matrix(left_cam.proj_matrix);
    p.set_matrix(right_cam.proj_matrix);

    rend.set_view(0, view(viewport(0, 0, WIN_X / 2, WIN_Y), left_cam));
    rend.set_view(1, view(viewport(WIN_X / 2, 0, WIN_X / 2, WIN_Y), right_cam));

    scene_graph sg;
    auto root = std::make_shared<teapot_scene_node>();
    auto child = std::make_shared<teapot_scene_node>();
    child->get_xform().translate(vec3(2.5f, 0, 0));
    auto gchild = std::make_shared<teapot_scene_node>();
    gchild->get_xform().translate(vec3(0, 0, 2.5f));
    auto ggchild = std::make_shared<teapot_scene_node>();
    ggchild->get_xform().translate(vec3(2.5f, 0, 0));
    sg.add_node(root);
    sg.add_node(child);
    sg.add_node(gchild);
    sg.add_node(ggchild);
    sg.add_connection(root->get_id(), child->get_id());
    sg.add_connection(child->get_id(), gchild->get_id());
    sg.add_connection(gchild->get_id(), ggchild->get_id());

    rend.render_on_gl_thread(sg);
    
    rend.destroy_on_gl_thread();
    glutSwapBuffers();
    usleep(10000); // to stop my mac melting :(
    glutPostRedisplay();
}

void reshape(int x, int y)
{
    WIN_X = x;
    WIN_Y = y;
    
    glViewport(0, 0, x, y);
}

void mouse(int button, int state, int x, int y)
{
}

void key(unsigned char c, int x, int y)
{
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    // https://pleiades.ucsc.edu/hyades/OpenGL_on_OS_X
    glutInitDisplayMode(
        //GLUT_3_2_CORE_PROFILE | 
        GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(WIN_X, WIN_Y);
    glutCreateWindow("Hello"); 

	glewInit();

    // log_gl_info();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutMainLoop();
    return 0;
} 
