/*
  surfaceviewer.h

  Copyright (c) 2012, Jeremiah LaRocco jeremiah.larocco@gmail.com

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>

#ifdef __APPLE_CC__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

// Some constants...
static const size_t NUM_MATERIALS=2;
static const size_t NUM_LIGHTS=2;
static const size_t NUM_LISTS=2;
static const size_t LINE_MAT=0;
static const size_t SURF_MAT=1;

/*!
  STLViewer is the QT widget that displays an STL file
*/
class SurfaceViewer : public QGLWidget {
    Q_OBJECT;

public:
    SurfaceViewer(QWidget *parent = 0);
    ~SurfaceViewer();
    
    void resetView();

    void setShowPolygons(bool show);
    void setShowFacets(bool show);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
  
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    // Returns the "name" of the object at pos
    int nameAtPos(const QPoint &pos);

    // Initialization functions
    void initMaterials();
    void initLights();
    void initLists();
    void regenList();
    
    // Error handler for OpenGL errors
    void handleGLError(size_t ln);

    float calculateMinimumZoom();
    // Arrays to hold material properties
    GLfloat mat_specular[NUM_MATERIALS][4];
    GLfloat mat_shininess[NUM_MATERIALS][1];
    GLfloat mat_diffuse[NUM_MATERIALS][4];
    GLfloat mat_ambient[NUM_MATERIALS][4];

    // Arrays to hold light properties
    GLfloat light_position[NUM_LIGHTS][4];
    GLfloat light_color[NUM_LIGHTS][4];
    GLfloat lmodel_ambient[NUM_LIGHTS][4];

    // Array of display lists
    GLuint dispLists[NUM_LISTS];

    // Stores last mouse position for rotation
    QPoint lastPos;

    // Rotation angles
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;

    // Zoom translation
    GLfloat translate;

    bool clicked;
    
    size_t num_tris;
    float *verts;
    float *norms;
    unsigned int *indices;

    bool showPolygons;
    bool showFacets;
};
