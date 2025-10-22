#include <vart/scene.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/transform.h>
#include <vart/point4d.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>

#include <iostream>
#include <vector>
#include <cstdlib> // Para rand() e srand()
#include <ctime>   // Para time()
#include <algorithm> // Para std::max e std::min

using namespace std;
using namespace VART;

// Função para criar o objeto do terreno.
// Organizar o código em funções deixa o main() mais limpo.
MeshObject* criarTerreno()
{
    // Dimensões do terreno
    const int numLinhas = 20;
    const int numColunas = 20;
    const float tamanhoCelula = 1.0; // Distância entre os pontos no chão

    // --- 1. Geração dos Vértices ---
    vector<Point4D> vertices;
    // Reserva memória para todos os vértices de uma vez para maior eficiência
    vertices.reserve(numLinhas * numColunas);

    // Inicializa o gerador de números aleatórios com o tempo atual
    srand(time(0));

    for (int i = 0; i < numLinhas; ++i)
    {
        for (int j = 0; j < numColunas; ++j)
        {
            float altura = 0;
            // A inclinação de 45° significa que a diferença de altura (delta Y)
            // não pode ser maior que a distância no chão (tamanhoCelula).
            float maxVariacaoAltura = tamanhoCelula;

            // Para suavizar o terreno, a altura de um ponto é baseada nos seus vizinhos já criados.
            if (i > 0 && j > 0) // Se não for a primeira linha ou coluna
            {
                // Pega a altura dos vizinhos de trás e da esquerda
                float alturaAtras = vertices[(i - 1) * numColunas + j].GetY();
                float alturaEsquerda = vertices[i * numColunas + j - 1].GetY();
                float alturaBase = (alturaAtras + alturaEsquerda) / 2.0f;

                // Gera uma nova altura próxima à média dos vizinhos, respeitando a inclinação.
                float minH = alturaBase - maxVariacaoAltura * 0.5f;
                float maxH = alturaBase + maxVariacaoAltura * 0.5f;
                altura = minH + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxH - minH)));
            }
            else // Para a primeira linha e coluna, a altura é totalmente aleatória
            {
                altura = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / maxVariacaoAltura));
            }

            // Adiciona o novo vértice. O terreno será visível por um observador em Y positivo.
            vertices.push_back(Point4D(j * tamanhoCelula, altura, i * tamanhoCelula));
        }
    }

    // --- 2. Criação do MeshObject ---
    MeshObject* terreno = new MeshObject();
    terreno->SetVertices(vertices);

    // --- 3. Criação das Fitas de Triângulos (Triangle Strips) ---
    // Uma "fita de triângulos" é uma forma eficiente de desenhar uma malha retangular.
    int ultimaLinha = numLinhas - 1;
    for (int linha = 0; linha < ultimaLinha; ++linha)
    {
        // Para cada linha da nossa grade, criamos uma nova Mesh (uma fita).
        Mesh fitaDeTriangulos;
        fitaDeTriangulos.type = Mesh::TRIANGLE_STRIP;

        for (int coluna = 0; coluna < numColunas; ++coluna)
        {
            // Adiciona um vértice da linha atual
            fitaDeTriangulos.indexVec.push_back(linha * numColunas + coluna);
            // Adiciona um vértice da linha seguinte
            fitaDeTriangulos.indexVec.push_back((linha + 1) * numColunas + coluna);
        }
        // Adiciona a fita completa ao objeto do terreno
        terreno->AddMesh(fitaDeTriangulos);
    }

    // --- 4. Finalização do Objeto ---
    cout << "Calculando normais dos vertices para iluminacao..." << endl;
    terreno->ComputeVertexNormals();
    terreno->SetMaterial(Material::PLASTIC_GREEN());

    return terreno;
}

int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv);
    static VART::Scene scene;
    static VART::ViewerGlutOGL viewer;
    //static VART::MouseControl mouseControl(&scene, &viewer);

    // Cria o objeto do terreno chamando nossa função
    MeshObject* meuTerreno = criarTerreno();
    scene.AddObject(meuTerreno);

    // Adiciona uma luz e uma câmera à cena
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    VART::Camera camera(VART::Point4D(10, 25, 35),   // Posição da câmera
                        VART::Point4D(10, 0, 10),   // Ponto para onde a câmera olha (centro do terreno)
                        VART::Point4D::Y());        // Vetor "up" (Y é para cima)
    scene.AddCamera(&camera);

    // Ajusta a câmera automaticamente para enquadrar tudo (opcional, mas bom para garantir)
    // scene.MakeCameraViewAll();

    // Configura a janela e o controle do mouse
    viewer.SetTitle("V-ART Gerador de Terreno");
    viewer.SetScene(scene);
    //viewer.SetIdleHandler(&mouseControl);

    // Inicia a aplicação
    scene.DrawLightsOGL();
    VART::ViewerGlutOGL::MainLoop();

    return 0;
}