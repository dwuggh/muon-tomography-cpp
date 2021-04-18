#pragma once

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>

#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>

#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Shaders/Phong.h>

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>


#include "utils.h"

using namespace Magnum;

typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;
typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;

// inline Vector3 fromArma(arma::vec3 vec) {
//     return Vector3{
//         static_cast<float>(vec[0]),
//         static_cast<float>(vec[1]),
//         static_cast<float>(vec[2]),
//     };
// }

class Voxel : public Object3D, SceneGraph::Drawable3D {
  public:
    explicit Voxel(uint id, vec3 size, Vector3 pos, double scatteringDensity,
                   Shaders::Flat3D& shader, GL::Mesh& mesh, Object3D& parent,
                   SceneGraph::DrawableGroup3D& drawables);

    void setAlpha(float power = 3.0);

    inline void setDensity(double density) {
        this->density = density;
    }
    
    static float getAlphaFromDensity(double density, double level = 0.5);
    static Color4 scatteringDensityToColor(double density, double level = 0.5);
    

  private:
    virtual void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera);

    uint id;
    Vector3 size;
    Vector3 pos;
    double density;
    Color4 color;
    Shaders::Flat3D& shader;
    GL::Mesh& mesh;
};
