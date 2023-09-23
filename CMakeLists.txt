

add_library(SoftGL INTERFACE)

target_include_directories(SoftGL
INTERFACE
    Src
)

target_link_libraries(SoftGL
INTERFACE
    LMath
)

target_sources(SoftGL
INTERFACE
    Src/SoftGL/Bitmap.h
    Src/SoftGL/BlockRasterizer.h
    Src/SoftGL/Buffer.h
    Src/SoftGL/config.h
    Src/SoftGL/Debug.h
    Src/SoftGL/DisplayMode.h
    Src/SoftGL/dynamic_buffer.h
    Src/SoftGL/Engine/FpsCounter.h
    Src/SoftGL/Graphics2D.h
    Src/SoftGL/IndexBuffer.h
    Src/SoftGL/InputLayout.h
    Src/SoftGL/InputSystem.h
    Src/SoftGL/IRenderWindow.h
    Src/SoftGL/IShader.h
    Src/SoftGL/LString.h
    Src/SoftGL/mip_utils.h
    Src/SoftGL/PixelShader.h
    Src/SoftGL/PSDefault.h
    Src/SoftGL/PSDefaultColor.h
    Src/SoftGL/PSFont.h
    Src/SoftGL/PSTextured.h
    Src/SoftGL/RasterizerSettings.h
    Src/SoftGL/Ray.h
    Src/SoftGL/RegisterBlock.h
    Src/SoftGL/ResourceManager.h
    Src/SoftGL/StaticBuffer.h
    Src/SoftGL/StaticTexture.h
    Src/SoftGL/static_mip_chain.h
    Src/SoftGL/static_vector.h
    Src/SoftGL/Texture.h
    Src/SoftGL/TextureSampler.h
    Src/SoftGL/texture_utils.h
    Src/SoftGL/Tools.h
    Src/SoftGL/Vertex.h
    Src/SoftGL/VertexShader.h
    Src/SoftGL/Viewport.h
    Src/SoftGL/VSDefault.h
    Src/SoftGL/VSScreenSpace.h


    Src/SoftGL/Engine/camera.h
    Src/SoftGL/Engine/camera_controller.h
    Src/SoftGL/Engine/cube_generator.h
    Src/SoftGL/Engine/FpsCounter.h
    Src/SoftGL/Engine/fresnel.h
    Src/SoftGL/Engine/gameobject.h
    Src/SoftGL/Engine/imesh.h
    Src/SoftGL/Engine/mesh.h
    Src/SoftGL/Engine/plane_generator.h
    Src/SoftGL/Engine/PsNormalMap.h
    Src/SoftGL/Engine/transform.h
    Src/SoftGL/Engine/shaders/textured_no_lit.h

    Src/SoftGL/Platform/Windows/RenderWindow.cpp
INTERFACE
    Src/SoftGL/BlockRasterizer.cpp
 
)