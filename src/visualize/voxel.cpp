#include "voxel.h"

Voxel::Voxel(uint id, vec3 size, Vector3 pos, double scatteringDensity,
             Shaders::Flat3D& shader, GL::Mesh& mesh, Object3D& parent,
             SceneGraph::DrawableGroup3D& drawables)
    : Object3D{&parent}, SceneGraph::Drawable3D{*this, &drawables}, id(id), pos(pos),
      shader(shader), mesh(mesh) {

    this->setDensity(scatteringDensity);
    this->color = this->scatteringDensityToColor(scatteringDensity);
    // TODO
    this->size  = {static_cast<float>(size[0]), static_cast<float>(size[1]), static_cast<float>(size[2])};
    // magnum's cube mesh originally has size of 2x2x2 divide size by 2 eliminates this.
    this->scale(this->size / 2);
}

void Voxel::setAlpha(float power) {
    this->color = scatteringDensityToColor(this->density, static_cast<double>(power));
}

float Voxel::getAlphaFromDensity(double density, double level) {
    const double threshold_max = 0.95;
    const double threshold_min = 0.05;
    if (density < threshold_min || density < level - 0.1) {
        return 0.0f;
    } else if (density > threshold_max) {
        return 1.0f;
    } else {
        // double power = level * 5 + 1;
        double power = 0.5;
        return static_cast<float>(Math::pow(density, power));
    }
}

Color4 Voxel::scatteringDensityToColor(double density, double level) {
    using namespace Math::Literals;
    // add 0.5f, enable color from blue to red
    float saturation = static_cast<float>(density) * 2 - 1.0f;
    float alpha      = getAlphaFromDensity(density, level);
    return Color4::fromHsv({0.0_degf, saturation, 0.5f}, alpha);
}

void Voxel::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
    using namespace Math::Literals;
    shader.setTransformationProjectionMatrix(camera.projectionMatrix() * transformationMatrix)
        .setColor(this->color)
        .setObjectId(id)
        .draw(mesh);
}
