#include "muontomography.h"

MuonTomography::MuonTomography(const Arguments& arguments, const Grid& grid,
                               arma::cube scattering_density)
    : Platform::Application{arguments}, scattering_density(scattering_density) {
    using namespace Math::Literals;
    // GL::Renderer::setClearColor(0xa5c9ea_rgbf);
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::One,
                                   GL::Renderer::BlendFunction::OneMinusSourceAlpha);

    Debug{} << "running on" << GL::Context::current().version() << "using"
            << GL::Context::current().rendererString();

    this->voxelMesh = MeshTools::compile(Primitives::cubeSolid());
    this->shader    = Shaders::Flat3D(Shaders::Flat3D::Flag::ObjectId);

    // transformation = Matrix4::rotationX(20.0_degf) * Matrix4::rotationY(40.0_degf);
    projection = Matrix4::perspectiveProjection(75.0_degf, Vector2{windowSize()}.aspectRatio(),
                                                0.01f, 100.0f);
    // *
    //              Matrix4::translation(Vector3::zAxis(8.0f));

    this->cameraObject = new Object3D(&this->scene);
    this->camera       = new SceneGraph::Camera3D(*this->cameraObject);
    this->cameraObject->translate(Vector3::zAxis(15.0f));
    this->camera->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
        .setProjectionMatrix(this->projection)
        .setViewport(GL::defaultFramebuffer.viewport().size());

    const auto grain = grid.grain;
    for (auto i = 0; i < grain; i++) {
        for (auto j = 0; j < grain; j++) {
            for (auto k = 0; k < grain; k++) {
                double density = scattering_density(i, j, k);
                uint id        = i * grain * grain + j * grain + k;
                Vector3 pos{static_cast<float>(i), static_cast<float>(j), static_cast<float>(k)};
                auto voxel =
                    new Voxel(id, grid.d, pos, density, shader, voxelMesh, scene, drawables);
                arma::vec3 translation{static_cast<double>(i), static_cast<double>(j),
                                       static_cast<double>(k)};
                translation = translation * 2 - 20;
                voxel->translate(fromArma(translation));
                // voxel->scale({1.1f, 1.1f, 1.1f});
                // Debug{} << translation;
                this->voxels.emplace_back(voxel);
            }
        }
    }

    // color.setStorage(GL::RenderbufferFormat::RGBA8, GL::defaultFramebuffer.viewport().size());
}

void MuonTomography::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    // Debug{} << this->scene.children();
    this->camera->draw(this->drawables);
    // GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
    // GL::AbstractFramebuffer::blit(framebuffer, GL::defaultFramebuffer,
    //     framebuffer.viewport(), GL::FramebufferBlit::Color);

    swapBuffers();
}

void MuonTomography::mousePressEvent(MouseEvent& event) {
    if (event.button() != MouseEvent::Button::Left)
        return;

    _previousMousePosition = _mousePressPosition = event.position();
    event.setAccepted();
}

void MuonTomography::mouseMoveEvent(MouseMoveEvent& event) {
    if (!(event.buttons() & MouseMoveEvent::Button::Left))
        return;

    /* We have to take window size, not framebuffer size, since the position is
       in window coordinates and the two can be different on HiDPI systems */
    const Vector2 delta =
        3.0f * Vector2{event.position() - _previousMousePosition} / Vector2{windowSize()};

    (*cameraObject)
        .rotate(Rad{-delta.y()}, cameraObject->transformation().right().normalized())
        .rotateY(Rad{-delta.x()});

    _previousMousePosition = event.position();
    event.setAccepted();
    redraw();
}

void MuonTomography::mouseReleaseEvent(MouseEvent& event) {
    if (event.button() != MouseEvent::Button::Left || _mousePressPosition != event.position())
        return;

    /* First scale the position from being relative to window size to being
       relative to framebuffer size as those two can be different on HiDPI
       systems */
    const Vector2i position = event.position() * Vector2{framebufferSize()} / Vector2{windowSize()};
    const Vector2i fbPosition{position.x(),
                              GL::defaultFramebuffer.viewport().sizeY() - position.y() - 1};

    /* Read object ID at given click position, and then switch to the color
       attachment again so drawEvent() blits correct buffer */
    // _framebuffer.mapForRead(GL::Framebuffer::ColorAttachment{1});
    // Image2D data = _framebuffer.read(
    //     Range2Di::fromSize(fbPosition, {1, 1}),
    //     {PixelFormat::R32UI});
    // _framebuffer.mapForRead(GL::Framebuffer::ColorAttachment{0});

    /* Highlight object under mouse and deselect all other */
    // for(auto* o: this->voxels) o->setSelected(false);
    // UnsignedInt id = data.pixels<UnsignedInt>()[0][0];
    // if(id > 0 && id < ObjectCount + 1)
    //     _objects[id - 1]->setSelected(true);

    event.setAccepted();
    redraw();
}
