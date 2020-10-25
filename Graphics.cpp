#include "pch.h"
#include "Graphics.h"
#include "ShaderController.hpp"
#include <iostream>

int glInit(int argc, char *argv[]);
void display();
void idle();
void errorCallback(int errorCode, const char* errorDescription);
void genBuffers();

GLuint renderingProgram;
GLuint vertexArrayObject;

void Graphics::run()
{
    while (!glfwWindowShouldClose(window)) {
     static const GLfloat red[] = { 0.0f, 0.2f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR,0,red);
        glVertexAttrib4fv(1, red);
        //Cause of Custom Tess Shader, glDrawArrays sholud use GL_PACHES
        glDrawArrays(GL_PATCHES, 0, 3);
        //It must be used without Custom Tess Shader
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
}
	//glutMainLoop();
}

int Graphics::glInit(int argc, char *argv[])
{
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        std::cout << "Error: GLFW 초기화 실패" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(
                                          800, // width
                                          600, // height
                                          "OpenGL Example", // window title
                                          NULL, NULL);
    if (!window)
    {
        fprintf( stderr, "GLFW 윈도우를 여는데 실패했습니다. Intel GPU 를 사용한다면, 3.3 지원을 하지 않습니다. 2.1 버전용 튜토리얼을 시도하세요.\n" );
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

	//glutIdleFunc(idle);
	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    
    //GLuint ret = glGetError();
    //if(ret==GL_INVALID_OPERATION) std::cout <<  "glShaderSource return invalid value" << std::endl;
    
    //glMatrixMode(GL_PROJECTION);
    //glMatrixMode(GL_MODELVIEW);
    renderingProgram = ShaderController::GetInstance()->makeProgram();
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
    
    glUseProgram(renderingProgram);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glfwSwapInterval(1);
    glfwSwapBuffers(window);
    genBuffers();
    
	return 1;
}

//void

void genBuffers()
{
    GLuint buffer;
    __glewGenBuffers(1,&buffer);
    
}

// 소스코드 -> 버퍼 -> 유니폼블록 의 구조로 데이터 전달됨.
void genUniformBlock()
{
    
    GLuint bufferA;
    __glewGenBuffers(1, &bufferA);
    __glewBindBuffer(GL_UNIFORM_BUFFER, bufferA);
    
    //버퍼에 데이터를 넣는 방법 세가지.
    
    //첫번째, 초기값을 지정. 데이터가 준비되어있고 생성과 동시에 값을 대입하는 경우
    //__glewBufferData(GL_ARRAY_BUFFER, 1024*1024, data, GL_STATIC_DRAW);
    
    //두번째, 함수를 통해 값 전달. 데이터가 준비되어있지만 생성시점과 대입시점이 다른 경우
    __glewBufferData(GL_UNIFORM_BUFFER, 4096, NULL, GL_DYNAMIC_DRAW);
    /*
    //세번째, 값이 들어갈 메모리 포인터를 얻어 직접 대입. 데이터를 준비하면서 넣는 경우(파일을 읽어서 넣는 경우 등)
    //OpenGL 4.2부터 사용하는 기능이라, 4.1인 현재는 테스트불가
    void * ptr = __glewMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, data, sizeof(data));
    __glewUnmapBuffer(GL_ARRAY_BUFFER);
    
    //정해진 값으로 데이터를 모두 채우려는 경우.
    __glewClearBufferSubData(GL_ARRAY_BUFFER, GL_RGBA, 0, sizeof(data), GL_RGBA, GL_FLOAT,data);
    
    //값을 카피하려는 경우. 단,한 타깃에 하나의 버퍼만 바인딩되있을 수 있으므로
     */
     
    
    //유니폼 내 인자들의 이름을 미리 저장
    static const GLchar * uniformNames[3] =
    {
        "TransformBlock.scale",
        "TransformBlock.position",
        "TransformBlock.rotation"
    };
    //유니폼 내 인자들의 인덱스가 저장될 변수
    GLuint uniformIndices[3];
    //유니폼 내 인자들의 오프셋이 저장될 변수
    GLint uniformOffsets[3];
    //유니폼 내 인자들의 배열 스트라이드(배열 내 각 요소의 크기)가 저장될 변수. 오프셋에 이 값을 더함으로써 배열의 다음 원소 오프셋을 알아냄
    GLint arrayStrides[3];
    //유니폼 내 인자들의 행렬스트라이드가 저장될 변수
    GLint matrixStrides[3];
    //유니폼 내 인자들의 인덱스를 찾는 함수
    GLint uniformLength[3];
    __glewGetUniformIndices(renderingProgram,3,uniformNames,uniformIndices);
    //유니폼 내 인덱스를 통해, 각 인자들의 오프셋, 스트라이드를 찾는 함수들
    __glewGetActiveUniformsiv(renderingProgram,3,uniformIndices,GL_UNIFORM_OFFSET,uniformOffsets);
    __glewGetActiveUniformsiv(renderingProgram,3,uniformIndices,GL_UNIFORM_ARRAY_STRIDE,arrayStrides);
    __glewGetActiveUniformsiv(renderingProgram,3,uniformIndices,GL_UNIFORM_MATRIX_STRIDE,matrixStrides);
    __glewGetActiveUniformsiv(renderingProgram,3,uniformIndices,GL_UNIFORM_BLOCK,uniformLength);
    //GLfloat offset1 = __glewGetUniformLocation(renderingProgram,"offset1");
    //GLfloat offset2 = __glewGetUniformLocation(renderingProgram,"offset2");
    //GLfloat offset3 = __glewGetUniformLocation(renderingProgram,"offset3");
    
    float position[5][4] = {{0,0,0,0},
        {0.25f, -0.25f, 0.5f, 0.51f},
        {-0.25f,-0.25f,0.5f,0.5f},
        {0.25f,0.25f,0.5f,1.0f},
        {0,0,0,0}
    };
    for(int i=0;i<3;i++)
    std::cout << "Length : " << uniformLength[i] << std::endl;
    GLchar *buffer = (GLchar *)malloc(4096);
    
    for(int i=0;i<3; i++)
    {
        for(int j=0;j<4;j++) {
            *(float *)(buffer + uniformOffsets[1] + arrayStrides[1]*i + j*sizeof(GLfloat)) = position[i][j];
            std::cout << (uniformOffsets[1] + arrayStrides[1]*i + j*sizeof(float)) << " : " << ((unsigned long)&(position[i+1][j]) - (unsigned long)position) << std::endl;
        }
    }
    std::cout << sizeof(buffer) << " , " << sizeof(position) << std::endl;
    std::cout << "index " << uniformIndices[1] <<  ",offset " << uniformOffsets[1] << ", stride " << arrayStrides[1] << std::endl;
    
    for(GLuint uniformIndex : uniformIndices)
        __glewUniformBlockBinding(renderingProgram,uniformIndex,0);
       
    __glewBindBufferBase(GL_UNIFORM_BUFFER,0,bufferA);
    __glewBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(position),buffer);
    free(buffer);
    /*
    glUniform4f(uniformOffsets[1],0.25f, -0.25f, 0.5f, 0.51f);
    glUniform4f(uniformOffsets[1]+arrayStrides[1],-0.25f, -0.25f, 0.5f, 0.5f);
    glUniform4f(uniformOffsets[1]+ arrayStrides[1] + arrayStrides[1],0.25f, 0.25f, 0.5f, 1.0f);*/
}

void errorCallback(int errorCode, const char* errorDescription)
{
    fprintf(stderr, "Error: %s\n", errorDescription);
}

void showGLError()
{
    GLenum error = glGetError();
    if(error == GL_NO_ERROR) std::cout << "No Error" << std::endl;
    if(error == GL_INVALID_ENUM) std::cout << "Error ENUM" << std::endl;
    if(error == GL_INVALID_VALUE) std::cout << "Error VALUE" << std::endl;
    if(error == GL_INVALID_OPERATION) std::cout << "Error OPERATION" << std::endl;
}
