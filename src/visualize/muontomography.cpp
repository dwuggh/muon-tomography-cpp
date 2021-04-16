#include "muontomography.h"

MuonTomography::MuonTomography(const Arguments& arguments, const Grid& grid,
                               arma::cube scattering_density)
    : Platform::Application{arguments}, scattering_density(scattering_density) {

    this->setWindowTitle("muon tomography");
    this->opacityLevel = 0.5;

    using namespace Math::Literals;

    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha,
                                   GL::Renderer::BlendFunction::OneMinusSourceAlpha);
    Vector2 scaling = Vector2{framebufferSize()} * dpiScaling() / Vector2{windowSize()};
    Vector2 defaultWindowSize{1920, 1080};
    Vector2 scaledWindowSize = defaultWindowSize / scaling;
    this->setWindowSize(
        {static_cast<int>(scaledWindowSize[0]), static_cast<int>(scaledWindowSize[1])});

    this->imgui = ImGuiIntegration::Context(Vector2{this->windowSize()} / dpiScaling(),
                                            this->windowSize(), framebufferSize());

    Debug{} << "running on" << GL::Context::current().version() << "using"
            << GL::Context::current().rendererString();

    // initialize voxels

    this->voxelMesh = MeshTools::compile(Primitives::cubeSolid());
    this->shader    = Shaders::Flat3D(Shaders::Flat3D::Flag::ObjectId);

    transformation = Matrix4::rotationX(20.0_degf) * Matrix4::rotationY(40.0_degf);
    projection     = Matrix4::perspectiveProjection(75.0_degf, Vector2{windowSize()}.aspectRatio(),
                                                0.01f, 100.0f);

    this->cameraObject = new Object3D(&this->scene);
    this->camera       = new SceneGraph::Camera3D(*this->cameraObject);
    this->cameraObject->translate(Vector3::zAxis(15.0f));
    this->camera->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
        .setProjectionMatrix(this->projection)
        .setViewport(GL::defaultFramebuffer.viewport().size());

    int grain = grid.grain;
    // sort drawables by their distance to camera
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
                translation = translation * 2.02 - 20;
                voxel->translate(fromArma(translation));
                this->voxels.emplace_back(voxel);
            }
        }
    }
}

void MuonTomography::sorted_voxels() {
    auto drawableTransformations = this->camera->drawableTransformations(this->drawables);
    std::sort(drawableTransformations.begin(), drawableTransformations.end(),
              [](const std::pair<std::reference_wrapper<SceneGraph::Drawable3D>, Matrix4>& a,
                 const std::pair<std::reference_wrapper<SceneGraph::Drawable3D>, Matrix4>& b) {
                  return a.second.translation().z() < b.second.translation().z();
              });
    this->camera->draw(drawableTransformations);
}

void MuonTomography::scaleAlpha(float level) {
    // Debug{} << level;
    for (auto voxel : this->voxels) {
        voxel->setAlpha(level);
    }
}

void MuonTomography::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    // ImGui::Begin("window");

    this->imgui.newFrame();
    {
        ImGui::SliderFloat("opacity", &this->opacityLevel, 0.0, 1.0);
        // ImGui::SetNextWindowPos(ImVec2(650,20), ImGuiCond_FirstUseEver);
        ImGui::Text("hello");
        this->imgui.updateApplicationCursor(*this);
    }

    GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
    GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);

    this->imgui.drawFrame();

    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
    GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);

    this->scaleAlpha(this->opacityLevel);
    this->draw_sorted_voxels();

    swapBuffers();
    redraw();
}

void MuonTomography::mousePressEvent(MouseEvent& event) {
    if (this->imgui.handleMousePressEvent(event))
        return;
    if (event.button() != MouseEvent::Button::Left)
        return;

    _previousMousePosition = _mousePressPosition = event.position();
    event.setAccepted();
}

void MuonTomography::mouseMoveEvent(MouseMoveEvent& event) {
    if (this->imgui.handleMouseMoveEvent(event))
        return;

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
    if (this->imgui.handleMouseReleaseEvent(event))
        return;

    if (event.button() != MouseEvent::Button::Left || _mousePressPosition != event.position())
        return;

    /* First scale the position from being relative to window size to being
       relative to framebuffer size as those two can be different on HiDPI
       systems */
    const Vector2i position = event.position() * Vector2{framebufferSize()} / Vector2{windowSize()};
    const Vector2i fbPosition{position.x(),
                              GL::defaultFramebuffer.viewport().sizeY() - position.y() - 1};

    event.setAccepted();
    redraw();
}

void MuonTomography::viewportEvent(ViewportEvent& event) {
    GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
    Debug{} << GL::defaultFramebuffer.viewport();
}
