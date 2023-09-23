TEMPLATE = lib
CONFIG += debug \
    staticlib \
    warn_off


DESTDIR = ../../bin
CONFIG -= release \
    warn_on

HEADERS += \
    VSScreenSpace.h \
    VSDefault.h \
    Viewport.h \
    VertexShader.h \
    VertexBuffer.h \
    Vertex.h \
    float4.h \
    float3.h \
    Vector2D.h \
    Types.h \
    Tools.h \
    Timer.h \
    Texture2D.h \
    Scene.h \
    ResourceManager.h \
    RenderWindowXORG.h \
    RenderWindow.h \
    Renderer3D.h \
    Renderer.h \
    Ray.h \
    Quaternion.h \
    PSTextured.h \
    PSFont.h \
    PSDefault.h \
    Primitive.h \
    Plane.h \
    PixelShader.h \
    Mesh.h \
    Matrix4x4.h \
    Matrix3x3.h \
    LString.h \
    LMath.h \
    IShader.h \
    IRenderWindow.h \
    InputSystem.h \
    InputLayout.h \
    Inline.h \
    IndexBuffer.h \
    FpsCounter.h \
    DisplayMode.h \
    Delegate_base.h \
    Delegate.h \
    Camera.h \
    Buffer.h \
    BlockRasterizer.h \
    Bitmap.h \
    TextureSampler.h

SOURCES += \
    RenderWindowXORG.cpp \
    RenderWindow.cpp \
    Mesh.cpp \
    BlockRasterizerTest.cpp \
    BlockRasterizer.cpp

CONFIG += link_pkgconfig
PKGCONFIG += x11
