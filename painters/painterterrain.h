#ifndef PAINTERTERRAIN_H
#define PAINTERTERRAIN_H

#include "terrain.h"
#include "drainageAlgorithms/drainagealgorithms.h"
#include "waterPathAlgorithms/pathwateralgorithm.h"
#include "patternsAlgorithms/algorithmpatron.h"
#include "buildNetwork/buildnetwork.h"
#include "shaders/shaderutils.h"
#include "shaders/shaderterrain.h"
#include "shaders/shadergradient.h"
#include "shaders/shaderdrainage.h"
#include "patternsAlgorithms/arbol.h"
#include "painters/glwidget.h"

class PainterTerrain : public GLWidget {
        Q_OBJECT

    public:
        static const int MALLA = 0;
        static const int POLIGONO = 2;
        static const int POINTS = 1;
        PainterTerrain();
        virtual ~PainterTerrain();
        Terrain* ter;
        float exag_z;
        std::unordered_map<std::string, glm::vec3> color_conf;
        ShaderTerrain* shader_terrain;
        ShaderGradient* shader_normal;
        ShaderDrainage* shader_gradient;
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
        void setPatternAlgorithm(AlgorithmPatron* alg);
        void setWaterAlgorithm(PathWaterAlgorithm* alg);
        void setNetworkAlgorithm(BuildNetwork* alg);
    private:
        DrainageAlgorithms* drainage_algorithm;
        AlgorithmPatron* pattern_algorithm;
        PathWaterAlgorithm* water_algorithm;
        BuildNetwork* build_network;
        bool glew_initialized;
        bool render_normal;
        bool render_gradient;
    public slots:
        void changeModelTerrain(int index);
        void changeExag(int number);
        void showRenderGradientVector(bool value);
        void showRenderNormalVector(bool value);
    signals:
        void glewIsReady();

};

#endif // PAINTERTERRAIN_H
