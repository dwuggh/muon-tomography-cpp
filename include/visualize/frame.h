#pragma once

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>

#include <Magnum/Math/Color.h>

#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Shaders/Flat.h>

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>

using namespace Magnum;

typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;
typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;

class CubeFrame : public Object3D, SceneGraph::Drawable3D {
  public:
    explicit CubeFrame(uint id, Vector3 size, Vector3 pos,
                   Shaders::Flat3D& shader, GL::Mesh& mesh, Object3D& parent,
                   SceneGraph::DrawableGroup3D& drawables);

    ~CubeFrame() {
        Debug{} << "destructing frame";
    }

  private:
    virtual void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera);

    uint id;
    Vector3 size;
    Vector3 pos;
    Color4 color;
    Shaders::Flat3D& shader;
    GL::Mesh& mesh;
};
