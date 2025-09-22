#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint
LoadShaders (const char * vertex_file_path, const char * fragment_file_path)
{
        /* Create the shaders */
        GLuint VertexShaderID = glCreateShader (GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader (GL_FRAGMENT_SHADER);

        /* Read the Vertex Shader code from the file */
        std::string VertexShaderCode;
        std::ifstream VertexShaderStream (vertex_file_path, std::ios::in);
        if (VertexShaderStream.is_open()) {
                std::string Line = "";
                while (getline(VertexShaderStream, Line))
                        VertexShaderCode += "\n" + Line;
                VertexShaderStream.close();
        } else {
                std::cout << "Impossible to open " << vertex_file_path <<" Are you in the right directory ? "
                "Don't forget to read the FAQ !\n";
                getchar();
                return 0;
        }

        /* Read the Fragment Shader code from the file */
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream (fragment_file_path, std::ios::in);
        if (FragmentShaderStream.is_open()){
                std::string Line = "";
                while (getline(FragmentShaderStream, Line))
                        FragmentShaderCode += "\n" + Line;
                FragmentShaderStream.close ();
        }
        GLint Result = GL_FALSE;
        int InfoLogLength;

        /* Compile Vertex Shader */
        printf ("Compiling shader : %s\n", vertex_file_path);
        char const * VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource (VertexShaderID, 1, &VertexSourcePointer , NULL);
        glCompileShader (VertexShaderID);

        /* Check Vertex Shader */
        glGetShaderiv (VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv (VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0 ){
                std::vector<char> VertexShaderErrorMessage (InfoLogLength+1);
                glGetShaderInfoLog (VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
                printf ("%s\n", &VertexShaderErrorMessage[0]);
        }

        /* Compile Fragment Shader */
        printf ("Compiling shader : %s\n", fragment_file_path);
        char const * FragmentSourcePointer = FragmentShaderCode.c_str ();
        glShaderSource (FragmentShaderID, 1, &FragmentSourcePointer , NULL);
        glCompileShader (FragmentShaderID);

        /* Check Fragment Shader */
        glGetShaderiv (FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv (FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
                std::vector<char> FragmentShaderErrorMessage (InfoLogLength+1);
                glGetShaderInfoLog (FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
                printf ("%s\n", &FragmentShaderErrorMessage[0]);
        }

        /* Link the program */
        printf ("Linking program\n");
        GLuint ProgramID = glCreateProgram ();
        glAttachShader (ProgramID, VertexShaderID);
        glAttachShader (ProgramID, FragmentShaderID);
        glLinkProgram (ProgramID);

        /* Check the program */
        glGetProgramiv (ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv (ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
                std::vector<char> ProgramErrorMessage (InfoLogLength+1);
                glGetProgramInfoLog (ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
                printf ("%s\n", &ProgramErrorMessage[0]);
        }

        glDetachShader (ProgramID, VertexShaderID);
        glDetachShader (ProgramID, FragmentShaderID);

        glDeleteShader (VertexShaderID);
        glDeleteShader (FragmentShaderID);

        return ProgramID;
}

int main (int argc, char **argv)
{
        std::ifstream fp;

        if (argc != 2) {
                std::cout <<"Syntax: ./render <filename.stl>" <<std::endl;
                return -1;
        }

        fp.open (argv[1], std::ios::in | std::ios::binary);

        char header[80] = "";
        int number_of_facets = 0;

        fp.read (header, 80);
        fp.read ((char *)&number_of_facets, 4);

        std::vector<glm::vec3> normal;
        std::vector<glm::vec3> vertices;

        normal.reserve (number_of_facets);
        vertices.reserve (number_of_facets * 3);

        float min_x = 0.0, min_y = 0.0, min_z = 0.0;
        float max_x = 0.0, max_y = 0.0, max_z = 0.0;

        for (int i = 0; i < number_of_facets; ++i) {
                glm::vec3 normal_v (0.0, 0.0, 0.0);
                glm::vec3 vertex (0.0, 0.0, 0.0);
                uint16_t attr_byte_count;

                fp.read ((char *)&normal_v.x, sizeof (float));
                fp.read ((char *)&normal_v.y, sizeof (float));
                fp.read ((char *)&normal_v.z, sizeof (float));
                normal.push_back (normal_v);
                fp.read ((char *)&vertex.x, sizeof (float));
                fp.read ((char *)&vertex.y, sizeof (float));
                fp.read ((char *)&vertex.z, sizeof (float));

                vertices.push_back (vertex);
                if (i == 0) {
                        min_x = vertex.x; min_y = vertex.y; min_z = vertex.z;
                        max_x = vertex.x; max_y = vertex.y; max_z = vertex.z;
                } else {
                        if (min_x > vertex.x) min_x = vertex.x;
                        if (min_y > vertex.y) min_y = vertex.y;
                        if (min_z > vertex.z) min_z = vertex.z;

                        if (max_x < vertex.x) max_x = vertex.x;
                        if (max_y < vertex.y) max_y = vertex.y;
                        if (max_z < vertex.z) max_z = vertex.z;
                }

                fp.read ((char *)&vertex.x, sizeof (float));
                fp.read ((char *)&vertex.y, sizeof (float));
                fp.read ((char *)&vertex.z, sizeof (float));

                vertices.push_back (vertex);
                if (min_x > vertex.x) min_x = vertex.x;
                if (min_y > vertex.y) min_y = vertex.y;
                if (min_z > vertex.z) min_z = vertex.z;

                if (max_x < vertex.x) max_x = vertex.x;
                if (max_y < vertex.y) max_y = vertex.y;
                if (max_z < vertex.z) max_z = vertex.z;

                fp.read ((char *)&vertex.x, sizeof (float));
                fp.read ((char *)&vertex.y, sizeof (float));
                fp.read ((char *)&vertex.z, sizeof (float));

                vertices.push_back (vertex);
                if (min_x > vertex.x) min_x = vertex.x;
                if (min_y > vertex.y) min_y = vertex.y;
                if (min_z > vertex.z) min_z = vertex.z;

                if (max_x < vertex.x) max_x = vertex.x;
                if (max_y < vertex.y) max_y = vertex.y;
                if (max_z < vertex.z) max_z = vertex.z;

                fp.read ((char *)&attr_byte_count, sizeof (uint16_t));
        }

#if 0
        std::cout << "Min " << min_x << " " << min_y << " ";
        std::cout << min_z << std::endl;

        std::cout << "Max " << max_x << " " << max_y << " ";
        std::cout << max_z << std::endl;
#endif

        float scale_x = max_x - min_x;
        float scale_y = max_y - min_y;
        float scale_z = max_z - min_z;

        for (int i = 0; i < vertices.size (); ++i) {
                vertices[i].x = (vertices[i].x / scale_x);
                vertices[i].y = (vertices[i].y / scale_y);
                vertices[i].z = (vertices[i].z / scale_z);
        }

#if 0
        for (int i = 0; i < vertices.size (); ++i) {
                std::cout << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << "\n";
        }
        std::cout << std::endl;
#endif

        if (!glfwInit()) {
                std::cout << "Failed to initialize GLFW\n";
                return -1;
        }


        /* Create window */
        /* 4x antialiasing */
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Open a window and create its OpenGL context */
        GLFWwindow* window;
        window = glfwCreateWindow (1024, 768, "Slicing", NULL, NULL);
        if (window == NULL) {
                std::cout << "Failed to open GLFW window. If you have an Intel GPU,"
                "they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
                glfwTerminate();
                return -1;
        }

        glfwMakeContextCurrent (window);
        glewExperimental = true;

        if (glewInit() != GLEW_OK) {
                std::cout << "Failed to initialize GLEW\n";
                return -1;
        }

        /* Capture the escape key */
        glfwSetInputMode (window, GLFW_STICKY_KEYS, GL_TRUE);

	    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

        glEnable (GL_DEPTH_TEST);
        glDepthFunc (GL_LESS);

        /* Compile the GLSL shaders */
        GLuint programID = LoadShaders ("SimpleVertexShader.vertexshader",
                                        "SimpleFragmentShader.fragmentshader");

        /* Get a handle for our "MVP" uniform */
        GLuint MatrixID = glGetUniformLocation (programID, "MVP");

        /* Projection matrix : 45° Field of View, 4:3 ratio,
            display range : 0.1 unit <- > 100 units */
        glm::mat4 Projection = glm::perspective (45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        /* Camera matrix */
        glm::mat4 View       = glm::lookAt (
                               glm::vec3 (4,3,-3), // Camera location in World Space
                               glm::vec3 (0,0,0),  // and looks at the origin
                               glm::vec3 (0,1,0)   // Head is up
                                                   // (set to 0,-1,0 to look upside-down)
						       );

        /* Model matrix : an identity matrix (model will be at the origin) */
        glm::mat4 Model      = glm::mat4 (1.0f);
        /* Our ModelViewProjection : multiplication of our 3 matrices
         * Remember, matrix multiplication is the other way around */
        glm::mat4 MVP        = Projection * View * Model;


        /* Create Vertex Array Object and set it as current */
        GLuint VertexArrayID;
        glGenVertexArrays (1, &VertexArrayID);
        glBindVertexArray (VertexArrayID);

        /* Draw the triangle */
        GLuint vertexbuffer;
        glGenBuffers (1, &vertexbuffer);
        glBindBuffer (GL_ARRAY_BUFFER, vertexbuffer);
        //glBufferData (GL_ARRAY_BUFFER, sizeof (g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        glBufferData (GL_ARRAY_BUFFER, vertices.size() * sizeof (glm::vec3), (void *)&vertices[0], GL_STATIC_DRAW);


        do {
                /* Clear screen and use shader */
                glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glUseProgram (programID);

                // Send our transformation to the currently bound shader,
                // in the "MVP" uniform
                glUniformMatrix4fv (MatrixID, 1, GL_FALSE, &MVP[0][0]);

                /* 1rst attribute buffer : vertices */
                glEnableVertexAttribArray (0);
                glBindBuffer (GL_ARRAY_BUFFER, vertexbuffer);
                glVertexAttribPointer (
                   0,         // attribute 0. No particular reason for 0, but must match the layout in the shader.
                   3,         // size
                   GL_FLOAT,  // type
                   GL_FALSE,  // normalized?
                   0,         // stride
                   (void*)0   // array buffer offset
                   );

                /* Draw the triangle ! */
                glDrawArrays (GL_TRIANGLES, 0, vertices.size ());
                //glDrawArrays (GL_TRIANGLES, 0, 3);
                glDisableVertexAttribArray (0);

                /* Swap buffers */
                glfwSwapBuffers (window);
                glfwPollEvents ();

        } while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
                 glfwWindowShouldClose(window) == 0 );

        return 1;
}
