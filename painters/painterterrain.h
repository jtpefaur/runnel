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
        static const int render_drainage = 1;
        static const int render_network = 2;
        static const int render_patterns = 3;
        static const int render_water = 4;
        int render_tab;
        PainterTerrain();
        virtual ~PainterTerrain();
        Terrain* ter;
        float exag_z;
        std::unordered_map<std::string, glm::vec3> color_conf;

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
        ShaderTerrain* shader_terrain;
        ShaderGradient* shader_normal;
        ShaderDrainage* shader_gradient;
        ShaderGradient* shader_axis;
        bool glew_initialized;
        bool render_normal;
        bool render_gradient;
        bool render_axis;
    public slots:
        void changeModelTerrain(int index);
        void changeExag(int number);
        void showRenderGradientVector(bool value);
        void showRenderNormalVector(bool value);
        void showRenderCoordinateAxis(bool value);
    signals:
        void glewIsReady();
        void drawGoogleEarth(std::vector<glm::vec3> values);

};

#endif // PAINTERTERRAIN_H
