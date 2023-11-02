/*
 * SPDX-FileCopyrightText: 2022 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "weatherbackground.h"
#include <QTimer>
#include <QtMath>

#include <QOpenGLFramebufferObject>

#include <QOpenGLExtraFunctions>
#include <QtQuick/QQuickWindow>
#include <qopenglshaderprogram.h>

#include <array>
#include <random>
class StarsRendererBase
{
public:
    float speedModifier = 1, aspectRatio = 1, scaleFactor = 1;
    int minFPS = 24;
    virtual ~StarsRendererBase() = default;
    virtual void render() = 0;
};
class SnowRendererBase
{
public:
    float aspectRatio = 1;
    float speedModifier = 1;
    int minFPS = 60;
    virtual ~SnowRendererBase() = default;
    virtual void render() = 0;
};
class StarsRendererLegacy : public StarsRendererBase, protected QOpenGLFunctions
{
public:
    StarsRendererLegacy(std::function<float()> &dice)
    {
        initializeOpenGLFunctions();
        int starCount = 30;
        for (int i = 0; i < starCount; i++) {
            starPositions.push_back({dice(), dice()});
            float originalOpacity = dice() / 10;
            float animationSpeed = dice() / 10 + 1;
            starMetaData.push_back({animationSpeed, originalOpacity, dice() > 0});
            starColors.push_back({1, 1, 1, dice() / 10});
        }

        const char *vertexShaderSrc2 =
            "attribute highp vec4 posAttr;\n"
            "attribute highp vec4 colorAttr;\n"
            "varying lowp vec4 col;\n"
            "void main() {\n"
            "   col = colorAttr;\n"
            "   gl_PointSize = 3.0;\n"
            "   gl_Position = posAttr;\n"
            "}\n";
        const char *fragmentShaderSrc =
            "varying lowp vec4 col;\n"
            "void main() {\n"
            "   gl_FragColor = col;\n"
            "}\n";
        program.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc2);
        program.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc);
        program.link();
        vertexAttr = program.attributeLocation("posAttr");
        colorAttr = program.attributeLocation("colorAttr");
    }

    void render() override
    {
        program.bind();
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        program.setAttributeArray(vertexAttr, starPositions.data());
        program.setAttributeArray(colorAttr, starColors.data());
        glDrawArrays(GL_POINTS, 0, starPositions.size());
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        program.release();

        float theta = 0.01;
        for (int i = 0; i < int(starPositions.size()); i++) {
            auto &[speed, opacity, direction] = starMetaData[i];
            float _theta = theta * speed * speedModifier;
            if (!direction) {
                _theta = -_theta;
            }
            starColors[i].setW(starColors[i].w() + _theta);
            if (starColors[i].w() > 1) {
                direction = false;
            } else if (starColors[i].w() < opacity) {
                direction = true;
            }
        }
    }

private:
    std::vector<QVector2D> starPositions;
    std::vector<QVector4D> starColors;
    std::vector<std::tuple<float, float, bool>> starMetaData; // animation speed, original opacity, direction
    QOpenGLShaderProgram program;
    int vertexAttr, colorAttr;
};

class StarsRenderer : public StarsRendererBase, protected QOpenGLExtraFunctions
{
public:
    StarsRenderer(std::function<float()> &dice)
    {
        initializeOpenGLFunctions();
        int starCount = 80;
        for (int i = 0; i < starCount; i++) {
            starPositions.push_back({dice(), dice(), 0, 1});
            float originalOpacity = dice() / 10;
            float animationSpeed = dice() / 10 + 1;
            starMetaData.push_back({animationSpeed, originalOpacity, dice() > 0});
            starColors.push_back({1, 1, 1, dice() / 10});
        }

        int num_segments = 10;

        float x = 0.005; // we start at angle = 0
        float y = 0;
        float theta = 2 * 3.1415926 / float(num_segments);
        float c = cosf(theta); // precalculate the sine and cosine
        float s = sinf(theta);
        float t;
        for (int ii = 0; ii < num_segments; ii++) {
            starVertexs.push_back(QVector4D{x, y, 0, 1}); // output vertex

            // apply the rotation matrix
            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
        }

        const char *vertexShaderSrc2 =
            "#version 330 core\n"
            "layout (location = 0) in vec4 colorAttr;\n"
            "layout (location = 1) in vec4 posAttr;\n"
            "layout (location = 2) in vec4 vertAttr;\n"
            "uniform mat4 matrix;\n"
            "out vec4 col;\n"
            "void main() {\n"
            "   col = colorAttr;\n"
            "   gl_Position = vec4((matrix * vertAttr).xy + posAttr.xy, 0, 1);\n"
            "}\n";
        const char *fragmentShaderSrc =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "in vec4 col;\n"
            "void main() {\n"
            "   FragColor = col;\n"
            "}\n";
        program.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc2);
        program.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc);
        program.link();

        vertexAttr = program.attributeLocation("vertAttr");
        posAttr = program.attributeLocation("posAttr");
        colorAttr = program.attributeLocation("colorAttr");
        matrixL = program.uniformLocation("matrix");
    }

    void render() override
    {
        glVertexAttribDivisor(colorAttr, 1);
        glVertexAttribDivisor(posAttr, 1);
        program.bind();
        program.setAttributeArray(posAttr, starPositions.data());
        program.setAttributeArray(vertexAttr, starVertexs.data());
        program.setAttributeArray(colorAttr, starColors.data());
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        QMatrix4x4 matrix;
        matrix.scale(scaleFactor, scaleFactor * aspectRatio);
        program.setUniformValue(matrixL, matrix);
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, starVertexs.size(), starPositions.size());
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        program.release();
        glVertexAttribDivisor(colorAttr, 0);
        glVertexAttribDivisor(posAttr, 0);
        float theta = 0.01;
        for (int i = 0; i < int(starPositions.size()); i++) {
            auto &[speed, opacity, direction] = starMetaData[i];
            float _theta = theta * speed * speedModifier;
            if (!direction) {
                _theta = -_theta;
            }
            starColors[i].setW(starColors[i].w() + _theta);
            if (starColors[i].w() > 1) {
                direction = false;
            } else if (starColors[i].w() < opacity) {
                direction = true;
            }
        }
    }

private:
    std::vector<QVector4D> starPositions;
    std::vector<QVector4D> starColors;
    std::vector<QVector4D> starVertexs;
    std::vector<std::tuple<float, float, bool>> starMetaData; // animation speed, original opacity, direction
    QOpenGLShaderProgram program;
    int posAttr, vertexAttr, colorAttr, matrixL;
};

class SunRenderer : protected QOpenGLFunctions
{
public:
    float aspectRatio = 1, speedModifier = 1;
    int minFPS = 60;
    SunRenderer()
        : vertices({QVector3D{-1, -1, 0}, {1, -1, 0}, {1, 1, 0}, {-1, 1, 0}})
        , innerColor({1, 0.4313, 0, 0.5})
        , outerColor({1, 0.5607, 0, 0.5})
        , outerOuterColor({1, 0.6274, 0, 0.4})
        , pos(QVector2D{0.9, -0.9})
        , innerWidth(0.8 / 2.5)
        , outerWidth(1.6 / 2.5)
        , outerOuterWidth(3.2 / 2.5)
    {
        initializeOpenGLFunctions();

        for (int i = 0; i < int(innerSun.size()); i++) {
            innerSun[i] = i * (90 / innerSun.size());
        }

        for (int i = 0; i < int(outerSun.size()); i++) {
            outerSun[i] = i * (90 / outerSun.size());
        }

        for (int i = 0; i < int(outerOuterSun.size()); i++) {
            outerOuterSun[i] = i * (90 / int(outerOuterSun.size()));
        }

        const char *vertexShaderSrc2 =
            "attribute highp vec4 posAttr;\n"
            "uniform highp mat4 matrix;\n"
            "uniform lowp vec4 sunCol;\n"
            "varying lowp vec4 col;\n"
            "void main() {\n"
            "   col = sunCol;\n"
            "   gl_Position = matrix * posAttr;\n"
            "}\n";
        const char *fragmentShaderSrc =
            "varying lowp vec4 col;\n"
            "void main() {\n"
            "   gl_FragColor = col;\n"
            "}\n";
        program.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc2);
        program.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc);
        program.link();
        vertexAttr = program.attributeLocation("posAttr");
        sunColorL = program.uniformLocation("sunCol");
        matrixL = program.uniformLocation("matrix");
    }

    void render()
    {
        program.bind();
        glEnableVertexAttribArray(0);
        program.setAttributeArray(vertexAttr, vertices.data());
        program.setUniformValue(sunColorL, outerOuterColor);
        for (auto &rotation : outerOuterSun) {
            QMatrix4x4 matrix;
            matrix.translate(pos.x(), pos.y());
            matrix.scale(outerOuterWidth, outerOuterWidth * aspectRatio);
            matrix.rotate(rotation, 0, 0, 1);
            program.setUniformValue(matrixL, matrix);
            glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
            rotation += 0.6 * speedModifier;
        }
        program.setUniformValue(sunColorL, outerColor);
        for (auto &rotation : outerSun) {
            QMatrix4x4 matrix;
            matrix.translate(pos.x(), pos.y());
            matrix.scale(outerWidth, outerWidth * aspectRatio);
            matrix.rotate(rotation, 0, 0, 1);
            program.setUniformValue(matrixL, matrix);
            glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
            rotation += 0.375 * speedModifier;
        }
        program.setUniformValue(sunColorL, innerColor);
        for (auto &rotation : innerSun) {
            QMatrix4x4 matrix;
            matrix.translate(pos.x(), pos.y());
            matrix.scale(innerWidth, innerWidth * aspectRatio);
            matrix.rotate(rotation, 0, 0, 1);
            program.setUniformValue(matrixL, matrix);
            glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
            rotation += 0.3 * speedModifier;
        }
        glDisableVertexAttribArray(0);
        program.release();
    }

private:
    std::array<QVector3D, 4> vertices;
    std::array<float, 3> innerSun;
    std::array<float, 5> outerSun;
    std::array<float, 6> outerOuterSun;
    QVector4D innerColor, outerColor, outerOuterColor;
    QVector2D pos;
    float innerWidth, outerWidth, outerOuterWidth;
    QOpenGLShaderProgram program;
    int vertexAttr, sunColorL, matrixL;
};

class BackgroundRenderer : protected QOpenGLFunctions
{
public:
    bool inAnimation = false;
    BackgroundRenderer()
    {
        initializeOpenGLFunctions();
        const char *vertexShaderSrc2 =
            "attribute highp vec4 posAttr;\n"
            "attribute lowp vec4 colAttr;\n"
            "varying lowp vec4 col;\n"
            "void main() {\n"
            "   col = colAttr;\n"
            "   gl_Position = posAttr;\n"
            "}\n";
        const char *fragmentShaderSrc =
            "varying lowp vec4 col;\n"
            "void main() {\n"
            "   gl_FragColor = col;\n"
            "}\n";
        program.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc2);
        program.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc);
        program.link();
        vertexAttr = program.attributeLocation("posAttr");
        colAttr = program.attributeLocation("colAttr");
    }

    void setColors(QVector4D topColor, QVector4D bottomColor)
    {
        if (topColor != m_topColor || bottomColor != m_bottomColor) {
            m_oldBottomColor = m_bottomColor;
            m_oldTopColor = m_topColor;
            m_topColor = topColor;
            m_bottomColor = bottomColor;
            if (!colorInitialized) {
                colorInitialized = true;
            } else {
                animationStartTime = std::chrono::system_clock::now();
                inAnimation = true;
            }
        }
    }

    void render()
    {
        QList<QVector3D> backgroundV = {{-1, 1, 0}, {1, 1, 0}, {1, -1, 0}, {-1, -1, 0}};
        auto topColor = m_topColor, bottomColor = m_bottomColor;

        if (inAnimation) {
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - animationStartTime).count();
            float percentage = float(diff) / float(animationTime);
            if (percentage >= 1) {
                inAnimation = false;
                percentage = 0.999; // prevent negative RGB
            }
            topColor = topColor * percentage + m_oldTopColor * (1 - percentage);
            bottomColor = bottomColor * percentage + m_oldBottomColor * (1 - percentage);
        }

        QList<QVector4D> backgroundC = {bottomColor, bottomColor, topColor, topColor};

        program.bind();
        program.setAttributeArray(vertexAttr, backgroundV.constData());
        program.setAttributeArray(colAttr, backgroundC.constData());
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_TRIANGLE_FAN, 0, backgroundV.size());
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        program.release();
    }

private:
    QOpenGLShaderProgram program;
    int vertexAttr, colAttr;
    QVector4D m_topColor;
    QVector4D m_bottomColor;
    QVector4D m_oldTopColor;
    QVector4D m_oldBottomColor;
    int animationTime = 800; // milliseconds
    bool colorInitialized = false;
    std::chrono::time_point<std::chrono::system_clock> animationStartTime;
};

class RainRenderer : protected QOpenGLFunctions
{
public:
    int minFPS = 60;
    RainRenderer(std::function<float()> &dice)
    {
        initializeOpenGLFunctions();
        int max = 100;

        for (int i = 0; i < max; i++) {
            droplets.push_back({{dice() * 2 - 1, -1 - (dice() + 1) / 4}, 0.1f + (dice() + 1) / 10, 3 + dice() / 2});
        }
        for (auto &[pos, length, speed] : droplets) {
            vertices.push_back(pos.toVector3D());
            vertices.push_back({pos.x(), pos.y() + length, 0});
        }
        const char *vertexShaderSrc =
            "attribute highp vec4 posAttr;\n"
            "void main() {\n"
            "   gl_Position = posAttr;\n"
            "}\n";

        const char *fragmentShaderSrc =
            "void main() {\n"
            "   gl_FragColor = vec4(1,1,1,0.6);\n"
            "}\n";
        shader.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc);
        shader.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc);
        shader.link();
        vertexAttr = shader.attributeLocation("posAttr");
    }
    void render()
    {
        shader.bind();
        shader.setAttributeArray(vertexAttr, vertices.constData());
        glEnableVertexAttribArray(0);
        glLineWidth(3);
        glDrawArrays(GL_LINES, 0, vertices.size());
        glDisableVertexAttribArray(0);
        for (int i = 0; i < vertices.size() - 1; i += 2) {
            auto &s = vertices[i];
            auto &e = vertices[i + 1];
            auto &[pos, length, speed] = droplets[i / 2];
            e[1] += 0.02 * speed;
            s[1] += 0.02 * speed;
            if (s[1] > 1) {
                vertices[i] = {pos.x(), pos.y() - length, 0};
                vertices[i + 1] = {pos.x(), pos.y(), 0};
            }
        }
        shader.release();
    }

private:
    QList<QVector3D> vertices;
    std::vector<std::tuple<QVector2D, float, float>> droplets; // pos, length, speed
    QOpenGLShaderProgram shader;
    int vertexAttr;
    std::function<float()> dice;
};

class SnowRenderer : public SnowRendererBase, protected QOpenGLExtraFunctions
{
public:
    SnowRenderer(std::function<float()> &dice)
    {
        initializeOpenGLFunctions();
        int max = 80, num_segments = 200;

        float x = 1; // we start at angle = 0
        float y = 0;
        float theta = 2 * 3.1415926 / float(num_segments);
        float c = cosf(theta); // precalculate the sine and cosine
        float s = sinf(theta);
        float t;
        for (int ii = 0; ii < num_segments; ii++) {
            vertices.push_back(QVector3D{x, y, 0}); // output vertex

            // apply the rotation matrix
            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
        }

        for (int i = 0; i < max; i++) {
            float r = 0.02 + (dice() + 1) / 50; // 0.02 - 0.06
            QVector2D pos = {dice(), dice()};
            flakes.push_back((dice() + 2) / 2);
            positions.push_back(pos);
            flakeData.push_back({float(0.75 + dice() / 4), r});
        }

        const char *vertexShaderSrc =
            "#version 330 core\n"
            "layout (location = 0) in vec2 posAttr;\n"
            "layout (location = 1) in vec2 flakeDataAttr;\n" // opacity, radius
            "layout (location = 2) in vec2 vertAttr;\n"
            "uniform highp mat4 matrix;\n"
            "out lowp vec4 col;\n"
            "void main() {\n"
            "   col = vec4(1,1,1,flakeDataAttr.x);\n"
            "   gl_Position = vec4((matrix * vec4(vertAttr.x * flakeDataAttr.y, vertAttr.y * flakeDataAttr.y, 0, 1)).xy + posAttr, 0 , 1);\n"
            "}\n";

        const char *fragmentShaderSrc =
            "#version 330 core\n"
            "in lowp vec4 col;\n"
            "out lowp vec4 FragColor;\n"
            "void main() {\n"
            "   FragColor = col;\n"
            "}\n";
        shader.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc);
        shader.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc);
        shader.link();
        vertexAttr = shader.attributeLocation("vertAttr");
        matrixL = shader.uniformLocation("matrix");
        flakeDataAttr = shader.attributeLocation("flakeDataAttr");
        translateAttr = shader.attributeLocation("posAttr");
    }
    void render() override
    {
        shader.bind();
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        shader.setAttributeArray(vertexAttr, vertices.data());
        shader.setAttributeArray(translateAttr, positions.data());
        shader.setAttributeArray(flakeDataAttr, flakeData.data());
        glVertexAttribDivisor(translateAttr, 1);
        glVertexAttribDivisor(flakeDataAttr, 1);
        QMatrix4x4 matrix;
        matrix.scale(1, aspectRatio);
        shader.setUniformValue(matrixL, matrix);

        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, vertices.size(), positions.size());
        int i = 0;
        for (auto &speed : flakes) {
            auto &pos = positions[i];
            auto r = flakeData[i].y();

            float translate = 0.003 * speed * speedModifier;
            if (translate + pos.y() - 2 * r > 1.0) {
                translate = -pos.y() - 1 - r;
            }
            pos.setY(pos.y() + translate);
            i++;
        }
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glVertexAttribDivisor(translateAttr, 0);
        glVertexAttribDivisor(flakeDataAttr, 0);
        shader.release();
    }

private:
    std::vector<QVector3D> vertices;
    std::vector<QVector2D> flakeData;
    std::vector<QVector2D> positions;
    std::vector<float> flakes; // speed
    QOpenGLShaderProgram shader;
    int vertexAttr, flakeDataAttr, matrixL, translateAttr;
    std::function<float()> dice;
};

class SnowRendererLegacy : public SnowRendererBase, protected QOpenGLFunctions
{
public:
    SnowRendererLegacy(std::function<float()> &dice)
    {
        initializeOpenGLFunctions();
        int max = 80, num_segments = 200;

        float x = 1; // we start at angle = 0
        float y = 0;
        float theta = 2 * 3.1415926 / float(num_segments);
        float c = cosf(theta); // precalculate the sine and cosine
        float s = sinf(theta);
        float t;
        for (int ii = 0; ii < num_segments; ii++) {
            vertices.push_back(QVector3D{x, y, 0}); // output vertex

            // apply the rotation matrix
            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
        }

        for (int i = 0; i < max; i++) {
            float r = 0.02 + (dice() + 1) / 50; // 0.02 - 0.06
            QVector2D originalPos = {dice(), dice()};
            flakes.push_back({originalPos, 0, r, (dice() + 2) / 2, 0.75 + dice() / 4});
        }

        const char *vertexShaderSrc =
            "attribute highp vec4 posAttr;\n"
            "uniform lowp vec4 flakeColor;\n"
            "uniform highp mat4 matrix;\n"
            "varying lowp vec4 col;\n"
            "void main() {\n"
            "   col = flakeColor;\n"
            "   gl_Position = matrix * posAttr;\n"
            "}\n";

        const char *fragmentShaderSrc =
            "varying lowp vec4 col;\n"
            "void main() {\n"
            "   gl_FragColor = col;\n"
            "}\n";
        shader.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc);
        shader.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc);
        shader.link();
        vertexAttr = shader.attributeLocation("posAttr");
        translateL = shader.uniformLocation("matrix");
        flakeColorL = shader.uniformLocation("flakeColor");
    }
    void render() override
    {
        shader.bind();
        glEnableVertexAttribArray(0);
        shader.setAttributeArray(vertexAttr, vertices.data());
        for (auto &[pos, translate, r, speed, opacity] : flakes) {
            QMatrix4x4 matrix;
            matrix.translate(pos.x(), pos.y() + translate);
            matrix.scale(r, r * aspectRatio);
            shader.setUniformValue(translateL, matrix);
            shader.setUniformValue(flakeColorL, QVector4D{1, 1, 1, opacity});
            glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

            translate += 0.003 * speed * speedModifier;
            if (translate + pos.y() - 2 * r > 1.0) {
                translate = -pos.y() - 1 - r;
            }
        }
        glDisableVertexAttribArray(0);
        shader.release();
    }

private:
    std::vector<QVector3D> vertices;
    std::vector<std::tuple<QVector2D, float, float, float, float>> flakes; // pos, currentTranslate, r, speed, opacity
    QOpenGLShaderProgram shader;
    int vertexAttr, translateL, flakeColorL;
    std::function<float()> dice;
};

class CloudsRenderer : protected QOpenGLFunctions
{
public:
    float speedModifier = 1;
    int minFPS = 24;
    float aspectRatio = 1;
    QColor cloudColor;
    CloudsRenderer(std::function<float()> &dice)
    {
        initializeOpenGLFunctions();
        const char *vertexShaderSrc =
            "attribute highp vec4 posAttr;\n"
            "uniform lowp vec4 cloudCol;\n"
            "varying lowp vec4 col;\n"
            "uniform highp mat4 matrix;\n"
            "void main() {\n"
            "   col = cloudCol;\n"
            "   gl_Position = matrix * posAttr;\n"
            "}\n";

        const char *fragmentShaderSrc =
            "varying lowp vec4 col;\n"
            "void main() {\n"
            "   gl_FragColor = col;\n"
            "}\n";
        program.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc);
        program.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc);
        program.link();

        vertexAttr = program.attributeLocation("posAttr");
        cloudColorL = program.uniformLocation("cloudCol");
        matrixL = program.uniformLocation("matrix");

        std::array<float, 7> opacity = {0.2f, 0.6, 0.2, 0.2, 0.5, 0.3, 0.5};
        std::array<float, 7> radii = {0.8f, 0.8f, 0.43f, 0.46f, 0.8f, 0.6f, 0.8f};
        float step = 2.0 / (opacity.size() - 1);
        float x = -1.2 + step;
        int i = 0;
        for (auto op : opacity) {
            Cloud cloud;
            QVector2D pos = {x, -1.0f - (dice() + 1) / 20};
            QVector2D coorChange = {dice() / 10, dice() / 10};
            cloud.pos = pos;
            cloud.endVector = coorChange;
            cloud.translate = cloud.endVector;
            cloud.translate.normalize();
            cloud.translate.setX(cloud.translate.x() / 100);
            cloud.translate.setY(cloud.translate.y() / 100);
            cloud.theta = cloud.translate;
            cloud.theta.setX(cloud.theta.x() / 30);
            cloud.theta.setY(cloud.theta.y() / 30);
            x += step;
            cloud.opacity = op;
            cloud.r = radii[i++];
            m_clouds.push_back(cloud);
        }

        int num_segments = 200;
        float theta = 2 * 3.1415926 / float(num_segments);
        float c = cosf(theta); // precalculate the sine and cosine
        float s = sinf(theta);
        float t;

        x = 1; // we start at angle = 0
        float y = 0;
        for (int ii = 0; ii < num_segments; ii++) {
            vertices.push_back(QVector3D{x, y, 0}); // output vertex

            // apply the rotation matrix
            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
        }
    };

    void render()
    {
        program.bind();
        glEnableVertexAttribArray(0);
        program.setAttributeArray(vertexAttr, vertices.data());
        for (auto &[endVector, translate, theta, pos, r, opacity] : m_clouds) {
            program.setUniformValue(cloudColorL,
                                    QVector4D{static_cast<float>(cloudColor.redF()),
                                              static_cast<float>(cloudColor.greenF()),
                                              static_cast<float>(cloudColor.blueF()),
                                              0.65f * opacity});
            if (translate.length() > endVector.length()) {
                endVector.setX(-endVector.x());
                endVector.setY(-endVector.y());
                theta.setX(-theta.x());
                theta.setY(-theta.y());
            }

            translate += theta * speedModifier;

            QMatrix4x4 matrix;
            matrix.translate(translate.x() + pos.x(), translate.y() + pos.y());
            matrix.scale(r, r * aspectRatio);
            program.setUniformValue(matrixL, matrix);
            glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
        }
        glDisableVertexAttribArray(0);

        program.release();
    }

private:
    struct Cloud {
        QVector2D endVector;
        QVector2D translate;
        QVector2D theta;
        QVector2D pos;
        float r;
        float opacity;
    };

    QOpenGLShaderProgram program;
    std::vector<QVector3D> vertices;
    int vertexAttr;
    int cloudColorL;
    int matrixL;
    std::vector<Cloud> m_clouds;
};

WeatherBackgroundContentRenderer::WeatherBackgroundContentRenderer()
{
    initializeOpenGLFunctions();
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-1, 1);
    auto *ctx = QOpenGLContext::currentContext();
    if (ctx->format().majorVersion() < 3) {
        m_legacyMode = true;
    }
    m_legacyMode = true;

    dice = std::bind(distribution, generator);
    background = new BackgroundRenderer();
}

void WeatherBackgroundContentRenderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE);
    glDisable(GL_DEPTH_TEST);
    background->render();
    if (m_showCloud) {
        m_clouds->render();
    }
    if (m_showRain) {
        m_rain->render();
    }
    if (m_showSun) {
        m_sun->render();
    }
    if (m_showStar) {
        m_stars->render();
    }
    if (m_showSnow) {
        m_snow->render();
    }
}

QOpenGLFramebufferObject *WeatherBackgroundContentRenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}
void WeatherBackgroundContentRenderer::synchronize(QQuickFramebufferObject *item)
{
    if (WeatherBackgroundRenderer *bgItem = dynamic_cast<WeatherBackgroundRenderer *>(item)) {
        if (!bgItem->isVisible()) {
            return;
        }
        aspectRatio = bgItem->width() / bgItem->height();
        m_showCloud = bgItem->cloud();
        m_showRain = bgItem->rain();
        m_showSun = bgItem->sun();
        m_showStar = bgItem->star();
        m_showSnow = bgItem->snow();
        m_colourTop = bgItem->colorTop();
        m_colourBottom = bgItem->colorBottom();

        m_cloudColor = bgItem->cloudColor();
        background->setColors({static_cast<float>(m_colourTop.redF()),
                               static_cast<float>(m_colourTop.greenF()),
                               static_cast<float>(m_colourTop.blueF()),
                               static_cast<float>(m_colourTop.alphaF())},
                              {static_cast<float>(m_colourBottom.redF()),
                               static_cast<float>(m_colourBottom.greenF()),
                               static_cast<float>(m_colourBottom.blueF()),
                               static_cast<float>(m_colourBottom.alphaF())});

        // isn't very elegant, but easiest to write without virtual base class
        int minFPS = std::numeric_limits<int>::lowest();
        if (background->inAnimation) {
            minFPS = 60;
        }
        if (m_showCloud) {
            if (!m_clouds) {
                m_clouds = new CloudsRenderer(dice);
            }
            m_clouds->aspectRatio = aspectRatio;
            m_clouds->cloudColor = m_cloudColor;
            minFPS = std::max(minFPS, m_clouds->minFPS);
        }
        if (m_showRain) {
            if (!m_rain) {
                m_rain = new RainRenderer(dice);
            }
            minFPS = std::max(minFPS, m_rain->minFPS);
        }
        if (m_showSun) {
            if (!m_sun) {
                m_sun = new SunRenderer();
            }
            m_sun->aspectRatio = aspectRatio;
            minFPS = std::max(minFPS, m_sun->minFPS);
        }
        if (m_showStar) {
            if (!m_stars) {
                if (!m_legacyMode) {
                    m_stars = new StarsRenderer(dice);
                } else {
                    m_stars = new StarsRendererLegacy(dice);
                }
            }
            m_stars->scaleFactor = 500 / bgItem->width();
            m_stars->aspectRatio = aspectRatio;
            minFPS = std::max(minFPS, m_stars->minFPS);
        }
        if (m_showSnow) {
            if (!m_snow) {
                if (!m_legacyMode) {
                    m_snow = new SnowRenderer(dice);
                } else {
                    m_snow = new SnowRendererLegacy(dice);
                }
            }
            m_snow->aspectRatio = aspectRatio;
            minFPS = std::max(minFPS, m_snow->minFPS);
        }

        // avoid bug, eg. non of the components is visible
        if (minFPS < 10) {
            minFPS = 60;
        }

        if (minFPS != m_minFPS) {
            m_minFPS = minFPS;
            float speedModifier = 60 / minFPS;
            if (m_showCloud) {
                m_clouds->speedModifier = speedModifier;
            }
            // skip rain, rain running on 60 fps which is the standard
            if (m_showSun) {
                m_sun->speedModifier = speedModifier;
            }
            if (m_showStar) {
                m_stars->speedModifier = speedModifier;
            }
            bgItem->modifiedMinFPS = minFPS;
        }
    }
}
WeatherBackgroundRenderer::WeatherBackgroundRenderer(QQuickItem *parent)
    : QQuickFramebufferObject(parent)
    , m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout, this, &WeatherBackgroundRenderer::handleTimeout);
    connect(this, &QQuickItem::visibleChanged, this, [this]() {
        if (isVisible()) {
            m_timer->start(1000 / minFPS);
        } else {
            m_timer->stop();
        }
    });
    m_timer->start(1000 / minFPS);
}

void WeatherBackgroundRenderer::handleTimeout()
{
    if (isVisible()) {
        update();
        if (modifiedMinFPS != minFPS) {
            minFPS = modifiedMinFPS;
            m_timer->start(1000 / minFPS);
        } else if (!m_timer->isActive()) {
            m_timer->start(1000 / minFPS);
        }
    } else {
        m_timer->stop();
    }
}

QQuickFramebufferObject::Renderer *WeatherBackgroundRenderer::createRenderer() const
{
    return new WeatherBackgroundContentRenderer();
}

bool WeatherBackgroundRenderer::rain() const
{
    return m_rain;
}
bool WeatherBackgroundRenderer::cloud() const
{
    return m_cloud;
}
bool WeatherBackgroundRenderer::sun() const
{
    return m_sun;
}
bool WeatherBackgroundRenderer::star() const
{
    return m_star;
}
bool WeatherBackgroundRenderer::snow() const
{
    return m_snow;
}
QColor WeatherBackgroundRenderer::colorTop() const
{
    return m_colourTop;
}
QColor WeatherBackgroundRenderer::colorBottom() const
{
    return m_colourBottom;
}
QColor WeatherBackgroundRenderer::cloudColor() const
{
    return m_cloudColor;
}

void WeatherBackgroundRenderer::setRain(bool r)
{
    m_rain = r;
}
void WeatherBackgroundRenderer::setCloud(bool c)
{
    m_cloud = c;
}
void WeatherBackgroundRenderer::setSun(bool s)
{
    m_sun = s;
}
void WeatherBackgroundRenderer::setStar(bool s)
{
    m_star = s;
}
void WeatherBackgroundRenderer::setSnow(bool s)
{
    m_snow = s;
}
void WeatherBackgroundRenderer::setColorTop(const QColor &c)
{
    m_colourTop = c;
}
void WeatherBackgroundRenderer::setColorBottom(const QColor &c)
{
    m_colourBottom = c;
}
void WeatherBackgroundRenderer::setCloudColor(const QColor &c)
{
    m_cloudColor = c;
}

#include "moc_weatherbackground.cpp"
