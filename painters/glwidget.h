#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QSize>
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include "lib/glm/glm.hpp"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    glm::mat4 matrix_glwidget;
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    ~GLWidget();
    glm::mat4 model_matrix;
    glm::mat4 model_view_matrix;
    bool normal_view;
    bool angle_point;
    bool angle_edge;
    bool arbolito_mode;
    bool drainage_callaghan;
    bool gradient_view;
    bool drainage_view;
    bool arbolito_mode_type;
    void glVersionPopup();
    void moveTranslate(glm::vec3 vec_move);
    int drawing_mode;
    const int NUBE_PUNTOS = 0;
    const int WIREFRAME = 1;
    const int FILL_POLYGONS = 2;

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL();
    void paintGL();
    void wheelEvent(QWheelEvent *event);
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void matrixPropertiesMovement();
    void keyPressEvent( QKeyEvent *e );


private:
    glm::vec3 rot;
    glm::vec3 vec_move_final;
    int wheel_move;

    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;
};

#endif // GLWIDGET_H


