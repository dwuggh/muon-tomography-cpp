#include "frame.h"


CubeFrame::CubeFrame(uint id, Vector3 size, Vector3 pos, Shaders::Flat3D& shader, GL::Mesh& mesh, Object3D& parent, SceneGraph::DrawableGroup3D& drawables) : Object3D{&parent}, SceneGraph::Drawable3D{*this, &drawables}, id(id), pos(pos),
      shader(shader), mesh(mesh) {
    using namespace Math::Literals;
    this->color = Color4::fromHsv({30.0_degf, 1.0, 1.0}, 1.0);
    Debug{} << size;
    this->scale(size / 2);
}

void CubeFrame::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
    using namespace Math::Literals;
    shader.setTransformationProjectionMatrix(camera.projectionMatrix() * transformationMatrix)
        .setColor(this->color)
        .setObjectId(id)
        .draw(mesh);
}
