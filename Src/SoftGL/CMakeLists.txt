

add_library(SoftGL INTERFACE)

target_include_directories(SoftGL
INTERFACE
    ../
)

target_link_libraries(SoftGL
INTERFACE
    LMath
)

target_sources(SoftGL
INTERFACE
    Bitmap.h
    BlockRasterizer.h
    Buffer.h
    CMakeLists.txt
    config.h
    Debug.h
    DisplayMode.h
    dynamic_buffer.h
    FpsCounter.h
    Graphics2D.h
    IndexBuffer.h
    InputLayout.h
    InputSystem.h
    IRenderWindow.h
    IShader.h
    LString.h
    mip_utils.h
    PixelShader.h
    PSDefault.h
    PSDefaultColor.h
    PSFont.h
    PSTextured.h
    RasterizerSettings.h
    Ray.h
    RegisterBlock.h
    ResourceManager.h
    StaticBuffer.h
    StaticTexture.h
    static_mip_chain.h
    static_vector.h
    Texture.h
    TextureSampler.h
    texture_utils.h
    Tools.h
    Vertex.h
    VertexShader.h
    Viewport.h
    VSDefault.h
    VSScreenSpace.h
INTERFACE
    BlockRasterizer.cpp
 
)