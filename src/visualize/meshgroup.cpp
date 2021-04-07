#include "meshgroup.h"

Voxel::Voxel(uint id, arma::vec3 size, Vector3 pos, double scatteringDensity,
             Shaders::Flat3D& shader, GL::Mesh& mesh, Object3D& parent,
             SceneGraph::DrawableGroup3D& drawables)
    : Object3D{&parent}, SceneGraph::Drawable3D{*this, &drawables}, id(id), pos(pos),
      shader(shader), mesh(mesh) {

    this->color = this->scatteringDensityToColor(scatteringDensity);
    this->size  = fromArma(size);
    // this->scale(this->size);
}
float Voxel::getAlphaFromDensity(double density) {
    if (density < 0.7) {
        Debug{} << density;
        return 0.0f;
    }
    else {
        return static_cast<float>(Math::pow(density, 1.5));
    }
}

Color4 Voxel::scatteringDensityToColor(double density) const {
    using namespace Math::Literals;
    // add 0.5f, enable color from blue to red
    float saturation = static_cast<float>(density) * 2 - 1.0f;
    float alpha      = getAlphaFromDensity(density); 
    return Color4::fromHsv({0.0_degf, saturation, 0.8f}, alpha);
}

void Voxel::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
    using namespace Math::Literals;
    // Debug{} << "drawing" << this->id;
    shader.setTransformationProjectionMatrix(camera.projectionMatrix() * transformationMatrix)
        .setColor(this->color)
        .setObjectId(id)
        .draw(mesh);
}
