#include "glwidget.h"
#include <GL/gl.h>
#include <iostream>
#include <QMessageBox>
#include "lib/glm/gtc/matrix_transform.hpp"



void printMat(glm::mat4  mat){
  int i,j;
  for (j=0; j<4; j++){
    for (i=0; i<4; i++){
        std::cout << mat[i][j] << " ";
  }
   std::cout << "" << std::endl;
 }
}

GLWidget::GLWidget(QWidget *parent)
     : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    rot = glm::vec3(0.0f,0.0f,0.0f);
    matrix_glwidget = glm::mat4();
    wheel_move = 0;
    normal_view = false;
    angle_point = false;
    angle_edge = false;
    gradient_view = false;
    drainage_view = false;
    drainage_callaghan = false;
    arbolito_mode = false;
    arbolito_mode_type = false;

    model_view_matrix = glm::mat4();
    model_matrix = glm::mat4();
    drawing_mode = GLWidget::WIREFRAME;
    this->setFocusPolicy(Qt::StrongFocus); /*Es para que funcione el apretar las teclas*/


}

void GLWidget::glVersionPopup(){
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    QString glvendor = QString("GL Vendor : ")+QString((const char*)vendor);
    QString glrenderer = QString("\nGL Renderer : ")+QString((const char*)renderer);
    QString glversions = QString("\nGL Version (string) : ")+QString((const char*)version);
    QString glversioni = QString("\nGL Version (integer) : ")+
                         QString::number(major)+QString(".")+QString::number(minor);
    QString glslversion = QString("\nGLSL Version : ")+QString((const char*)glslVersion);
    QMessageBox s;
    s.setText(glvendor+glrenderer+glversions+glversioni+glslversion);
    s.exec();
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != rot.x) {
        rot.x = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
   // std::cout << "setYRotation " << std::endl;
    qNormalizeAngle(angle);
    if (angle != rot.y) {
        rot.y = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
 //   std::cout << "setZRotation " << std::endl;
    qNormalizeAngle(angle);
    if (angle != rot.z) {
        rot.z = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::initializeGL()
{
    qglClearColor(qtPurple.dark());
    glEnable(GL_DEPTH_TEST); //para que no dibuje la parte de atras y adelante a la vez
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0.0f,0.0f, width, height);
    std::cout << "Windows size resizeGL: "<<height<<", "<<width<<std::endl;
    glLoadIdentity();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{

    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    if (event->buttons() & Qt::LeftButton) {
        setXRotation(rot.x + 8 * dy);
        setYRotation(rot.y + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(rot.x + 8 * dy);
        setZRotation(rot.z + 8 * dx);
    }
    lastPos = event->pos();

}

void GLWidget::paintGL(){
    glClearColor ( 170.0/255.0 , 243.0/255.0  , 226.0/255.0 , 0.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //para borrar el buffer de profundidad
    if(drawing_mode == GLWidget::NUBE_PUNTOS){
        glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
    }
    if(drawing_mode == GLWidget::WIREFRAME){
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    if(drawing_mode == GLWidget::FILL_POLYGONS){
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    glLoadIdentity();
    matrix_glwidget = glm::mat4();
    matrix_glwidget = glm::rotate(matrix_glwidget, rot.x / 16.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    matrix_glwidget = glm::rotate(matrix_glwidget, rot.y / 16.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    matrix_glwidget = glm::rotate(matrix_glwidget, rot.z / 16.0f, glm::vec3(0.0f, 0.0f, 1.0f));
//    std::cout << "Matrix deps de la rotacion " << std::endl;
//    printMat(matrix_glwidget);
    float paso = 20;
    if(wheel_move == 0){
        matrix_glwidget = matrix_glwidget;
    }else{
        matrix_glwidget = glm::scale(matrix_glwidget, glm::vec3(wheel_move/paso,wheel_move/paso,wheel_move/paso));
    }

    matrix_glwidget = glm::translate(matrix_glwidget, vec_move_final);
   // std::cout << "Matrix deps del scale " << std::endl;
  //  printMat(matrix_glwidget);
}

void GLWidget::matrixPropertiesMovement(){
}

void GLWidget::moveTranslate(glm::vec3 vec_move){
    vec_move_final = vec_move_final + vec_move;
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    if (event->orientation() == Qt::Vertical) {
            wheel_move+= numSteps;
    }
    event->accept();
    updateGL();
}


void GLWidget::keyPressEvent( QKeyEvent *e ){
   // std::cout << "presiono la tecla " << e->key() << std::endl;
    glm::vec3 vect_move = glm::vec3();
    float value_move = 0.001;
    switch( e->key() )
    {
        case Qt::Key_Up:
            vect_move = glm::vec3(value_move, 0, 0);
            moveTranslate(vect_move);
            break;
        case Qt::Key_Down:
            vect_move = glm::vec3(-value_move, 0, 0);
            moveTranslate(vect_move);
            break;
        case Qt::Key_Left:
            vect_move = glm::vec3(0, value_move, 0);
            moveTranslate(vect_move);
            break;
        case Qt::Key_Right:
            vect_move = glm::vec3(0, -value_move, 0);
            moveTranslate(vect_move);
            break;
        case Qt::Key_P:
            drawing_mode = GLWidget::NUBE_PUNTOS;
            break;
        case Qt::Key_L:
            drawing_mode = GLWidget::WIREFRAME;
            break;
        case Qt::Key_F:
            drawing_mode = GLWidget::FILL_POLYGONS;
            break;

        case Qt::Key_N:
            normal_view = !normal_view;
            break;
        case Qt::Key_H:
             GLWidget::glVersionPopup();
             break;
        case Qt::Key_S:
            angle_point = !angle_point;
            break;
        case Qt::Key_A:
            angle_edge = !angle_edge;
            break;

        case Qt::Key_D:
            drainage_view= !drainage_view;
             break;
        case Qt::Key_G:
            gradient_view = !gradient_view;
            break;
        case Qt::Key_C:
            drainage_callaghan = !drainage_callaghan;
            break;
        case Qt::Key_B:
            arbolito_mode = !arbolito_mode;
            break;
        case Qt::Key_T:
            arbolito_mode_type = !arbolito_mode_type;
            break;

    default:
      QGLWidget::keyPressEvent( e );
    }
    updateGL();
}


