#pragma once

#include <Corrade/Containers/StringView.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData.h>

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>

#include <Magnum/ImGuiIntegration/Context.hpp>

#include <armadillo>

#include "frame.h"
#include "grid.h"
#include "voxel.h"

using namespace Magnum;

typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;
typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;

class MuonTomography : public Platform::Application {
  public:
    explicit MuonTomography(const Arguments& arguments, const Grid& grid,
                            arma::cube scattering_density);

    void scaleAlpha(float level);

  private:
    void drawEvent() override;
    void mousePressEvent(MouseEvent& event) override;
    void mouseMoveEvent(MouseMoveEvent& event) override;
    void mouseReleaseEvent(MouseEvent& event) override;

    void viewportEvent(ViewportEvent& event) override;

    inline void sorted_voxels();

    Vector2i _previousMousePosition, _mousePressPosition;

    ImGuiIntegration::Context imgui{NoCreate};
    float opacityLevel;

    // a cube
    GL::Mesh voxelMesh;
    arma::cube scattering_density;
    Shaders::Flat3D shader;
    Matrix4 transformation;
    Matrix4 projection;
    Scene3D scene;
    std::vector<Voxel*> voxels;
    Object3D* cameraObject;
    SceneGraph::Camera3D* camera;
    SceneGraph::DrawableGroup3D drawables;
    SceneGraph::DrawableGroup3D frameDrawables;
    // GL::Framebuffer framebuffer;
    // GL::Renderbuffer color, objectId, depth;
};
