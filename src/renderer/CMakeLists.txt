set(OCF_RENDERER_HEADER
    "renderer/CustomCommand.h"
    "renderer/Image.h"
    "renderer/MeshCommand.h"
    "renderer/OpenGLInclude.h"
    "renderer/OpenGLUtility.h"
    "renderer/Program.h"
    "renderer/ProgramState.h"
    "renderer/QuadCommand.h"
    "renderer/RenderCommand.h"
    "renderer/Renderer.h"
    "renderer/RenderQueue.h"
    "renderer/Shader.h"
    "renderer/ShaderManager.h"
    "renderer/Texture2D.h"
    "renderer/TextureAtlas.h"
    "renderer/TextureCube.h"
    "renderer/TextureManager.h"
    "renderer/TrianglesCommand.h"
    "renderer/Types.h"

    "renderer/backend/Driver.h"
    "renderer/backend/DriverBase.h"
    )
    
set(OCF_RENDERER_SRC
    "renderer/CustomCommand.cpp"
    "renderer/Image.cpp"
    "renderer/MeshCommand.cpp"
    "renderer/OpenGLUtility.cpp"
    "renderer/Program.cpp"
    "renderer/ProgramState.cpp"
    "renderer/QuadCommand.cpp"
    "renderer/RenderCommand.cpp"
    "renderer/Renderer.cpp"
    "renderer/RenderQueue.cpp"
    "renderer/Shader.cpp"
    "renderer/ShaderManager.cpp"
    "renderer/Texture2D.cpp"
    "renderer/TextureAtlas.cpp"
    "renderer/TextureCube.cpp"
    "renderer/TextureManager.cpp"
    "renderer/TrianglesCommand.cpp"
    "renderer/VertexArray.cpp"
    "renderer/VertexBuffer.cpp"

    "renderer/backend/DriverBase.cpp"
    )

option(OCF_USE_GL "Use OpenGL" ON)

if(OCF_USE_GL)
    list(APPEND OCF_RENDERER_HEADER
        "renderer/backend/opengl/OpenGLContext.h"
        "renderer/backend/opengl/OpenGLDriver.h"
        )

    list(APPEND OCF_RENDERER_SRC
        "renderer/backend/opengl/OpenGLContext.cpp"
        "renderer/backend/opengl/OpenGLDriver.cpp"
        )
endif()
