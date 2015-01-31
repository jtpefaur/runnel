#ifndef PAINTERTERRAIN_H
#define PAINTERTERRAIN_H

#include "terrain.h"
#include "drainageAlgorithms/drainagealgorithms.h"
#include "shaders/shaderutils.h"
#include "shaders/shaderterrain.h"
#include "patternsAlgorithms/arbol.h"
#include "painters/glwidget.h"

class PainterTerrain : public GLWidget {
        Q_OBJECT

    public:
        static const int MALLA = 0;
        static const int POLIGONO = 1;
        static const int POINTS = 2;
        PainterTerrain();
        virtual ~PainterTerrain();
        Terrain* ter;
        float exag_z;
        std::unordered_map<std::string, glm::vec3> color_conf;
        ShaderTerrain* shader_terrain;
        glm::mat4 ortho_matrix;
        glm::mat4 matrix_final;
        std::unordered_map<std::string, GLuint> attr_buffer;
        void changeAttrConf(std::string name, glm::vec3 value);
        std::unordered_map<std::string, glm::vec3> confColor();
        void changeConf();
        bool checkExists(std::string file);
        void InitializeVertexBuffer();
        void setTerrain(Terrain* t);
        void paintGL();
        void initGL();

        void initColorBuffer();
        void mouseDoubleClickEvent(QMouseEvent * event);

        void ObtainPositionFromView(int x, int y, glm::mat4 view, glm::mat4 projection);

        void setDrainageAlgorithm(DrainageAlgorithms* alg);
    private:
        DrainageAlgorithms* drainage_algorithm;
        bool glew_initialized;
    public slots:
        void changeModelTerrain(int index);
        void changeExag(int number);
    signals:
        void glewIsReady();

};

#endif // PAINTERTERRAIN_H
