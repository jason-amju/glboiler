all:glboiler_exe

clean:
	rm glboiler *.o ../*.o ../geom/*.o ../macosx/*.o ../../assets/glboiler ../md3/*.o ../utils/*.o

run:
	cp glboiler ../../assets/; cd ../../assets/; ./glboiler; cd -

CC=clang++

WARNINGS=-Wall -Wno-deprecated-declarations 

INCLUDES=\
  -I.. \
  -I../thirdparty/googletest/include  \
  -I../thirdparty/lodepng \
  -I../geom \
  -I../macosx \
  -I../utils \
  -I../md3 \
  -I../obj

CFLAGS=-c $(WARNINGS) $(INCLUDES) -std=c++11 -O0 -g -DMACOSX

LDFLAGS=-framework OpenGL -framework GLUT 

SOURCES=glut_main.cpp \
../gl_1_1_renderer.cpp \
../gl_2_renderer.cpp \
../gl_1_1_solid_scene_node.cpp \
../gl_2_solid_scene_node.cpp \
../gl_system.cpp \
../gl_shader.cpp \
../int_graph.cpp \
../load_png.cpp \
../material.cpp \
../md3_node.cpp \
../md3_state.cpp \
../obj_scene_node.cpp \
../obj_state.cpp \
../opaque_pass.cpp \
../renderer.cpp \
../render_pass.cpp \
../render_to_texture.cpp \
../resource_manager.cpp \
../scene_description.cpp \
../scene_graph.cpp \
../scene_node.cpp \
../shadow_depth_pass.cpp \
../simple_forward_pass.cpp \
../state.cpp \
../texture.cpp \
../vertex.cpp \
../view.cpp \
../macosx/timer.cpp \
../md3/md3.cpp \
../geom/aabb.cpp \
../geom/camera.cpp \
../geom/frustum.cpp \
../geom/look_at.cpp \
../geom/projection.cpp \
../geom/quat.cpp \
../light.cpp \
../log.cpp \
../obj/obj_material.cpp \
../obj/obj_mesh.cpp \
../obj/obj_util.cpp \
../utils/file.cpp \
../utils/string_utils.cpp \
../thirdparty/lodepng/lodepng.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=glboiler

glboiler_exe: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

