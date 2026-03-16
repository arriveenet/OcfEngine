option(OCF_USE_GL "Use OpenGL" ON)

# ==============================================================================
# Renderer Sources and headers
# ==============================================================================
set(OCF_RENDERER_HEADER
    "${OCF_ROOT_PATH}/include/ocf/renderer/CustomCommand.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/Image.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/MeshCommand.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/OpenGLUtility.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/Program.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/ProgramState.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/QuadCommand.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/RenderCommand.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/Renderer.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/RenderQueue.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/Shader.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/ShaderManager.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/Texture2D.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/TextureAtlas.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/TextureCube.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/TextureManager.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/TrianglesCommand.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/Types.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/VertexArray.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/VertexBuffer.h"
    )
    
set(OCF_RENDERER_SRC
    "renderer/CustomCommand.cpp"
    "renderer/Image.cpp"
    "renderer/MeshCommand.cpp"
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
    )

# ==============================================================================
# Backend Sources and headers
# ==============================================================================
list(APPEND OCF_RENDERER_HEADER
    "${OCF_ROOT_PATH}/include/ocf/renderer/backend/Driver.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/backend/DriverBase.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/backend/DriverEnums.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/backend/Handle.h"
    "${OCF_ROOT_PATH}/include/ocf/renderer/backend/HandleAllocator.h"
)

list(APPEND OCF_RENDERER_SRC
    "renderer/backend/DriverBase.cpp"
    "renderer/backend/HandleAllocator.cpp"
)

# ==============================================================================
# OpenGL Sources
# ==============================================================================
if(OCF_USE_GL)
    list(APPEND OCF_RENDERER_HEADER
        "${OCF_ROOT_PATH}/include/ocf/renderer/backend/opengl/GLTexture.h"
        "${OCF_ROOT_PATH}/include/ocf/renderer/backend/opengl/OpenGLContext.h"
        "${OCF_ROOT_PATH}/include/ocf/renderer/backend/opengl/OpenGLDriver.h"
        "${OCF_ROOT_PATH}/include/ocf/renderer/backend/opengl/OpenGLInclude.h"
        "${OCF_ROOT_PATH}/include/ocf/renderer/backend/opengl/OpenGLUtility.h"
        )

    list(APPEND OCF_RENDERER_SRC
        "renderer/backend/opengl/OpenGLContext.cpp"
        "renderer/backend/opengl/OpenGLDriver.cpp"
        "renderer/backend/opengl/OpenGLUtility.cpp"
        )
endif()
