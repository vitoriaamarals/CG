#include <vart/scene.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/point4d.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace VART;

MeshObject criarTriangulo(Material mat) {
    MeshObject obj;

    Point4D ponto1(0.0, 1.0, 0.0);   // Vértice de cima do triângulo
    Point4D ponto2(-1.0, -1.0, 0.0); // Vértice inferior esquerdo
    Point4D ponto3(1.0, -1.0, 0.0);  // Vértice inferior direito

    vector<Point4D> vertices;

    vertices.push_back(ponto1);
    vertices.push_back(ponto2);
    vertices.push_back(ponto3);
    
    obj.SetVertices(vertices);

    //Cria o mesh
    Mesh mesh;
    //Define o tipo de mesh
    mesh.type = Mesh::MeshType::POLYGON;
    mesh.indexVec.push_back(0);
    mesh.indexVec.push_back(1);
    mesh.indexVec.push_back(2);

    //Define a cor da face
    mesh.material = Color::CYAN();
    //Adiciona no meshObject        
    obj.AddMesh(mesh);
    obj.ComputeVertexNormals();

    obj.SetMaterial(mat);
    return obj;
}

int main (int argc, char* argv[]) {

    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT

    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)

    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D::ORIGIN(),VART::Point4D::Y());

    /*MeshObject obj;

    Point4D ponto1(0.0, 1.0, 0.0);   // Vértice de cima do triângulo
    Point4D ponto2(-1.0, -1.0, 0.0); // Vértice inferior esquerdo
    Point4D ponto3(1.0, -1.0, 0.0);  // Vértice inferior direito

    vector<Point4D> vertices;

    vertices.push_back(ponto1);
    vertices.push_back(ponto2);
    vertices.push_back(ponto3);
    
    obj.SetVertices(vertices);

    //Cria o mesh
    Mesh mesh;
    //Define o tipo de mesh
    mesh.type = Mesh::MeshType::POLYGON;
    mesh.indexVec.push_back(0);
    mesh.indexVec.push_back(1);
    mesh.indexVec.push_back(2);

    //Define a cor da face
    mesh.material = Color::CYAN();
    //Adiciona no meshObject        
    obj.AddMesh(mesh);
    obj.ComputeVertexNormals();

    obj.SetMaterial(Material::PLASTIC_GREEN());*/

    MeshObject obj = criarTriangulo(Material::PLASTIC_GREEN());
    MeshObject obj2 = criarTriangulo(Material::PLASTIC_BLUE());

   /* scene.AddObject(&obj2);
    scene.AddObject(&obj);*/

    Transform trans1;
    trans1.MakeTranslation(-1.5, -1.5, 0);
    scene.AddObject(&trans1);
    trans1.AddChild(obj);

    Transform trans2;
    trans2.MakeTranslation(1.5, 0, 0);
    scene.AddObject(&trans2);
    trans2.AddChild(obj2);
    
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.MakeCameraViewAll();

    // Set up the viewer
    viewer.SetTitle("DOIS TRIANGULOS V-ART");
    viewer.SetScene(scene); // attach the scene

    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;

}