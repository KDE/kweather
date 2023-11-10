/*
 * SPDX-FileCopyrightText: 2022 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

#include <QOpenGLFunctions>
#include <QtQuick/QQuickFramebufferObject>
#include <qqmlregistration.h>

class BackgroundRenderer;

class WeatherBackgroundRenderer : public QQuickFramebufferObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(WeatherBackground)
    Q_PROPERTY(bool rain READ rain WRITE setRain NOTIFY rainChanged)
    Q_PROPERTY(bool cloud READ cloud WRITE setCloud NOTIFY cloudChanged)
    Q_PROPERTY(bool sun READ sun WRITE setSun NOTIFY sunChanged)
    Q_PROPERTY(bool star READ star WRITE setStar NOTIFY starChanged)
    Q_PROPERTY(bool snow READ snow WRITE setSnow NOTIFY snowChanged)
    Q_PROPERTY(QColor colorTop READ colorTop WRITE setColorTop NOTIFY colorTopChanged)
    Q_PROPERTY(QColor colorBottom READ colorBottom WRITE setColorBottom NOTIFY colorBottomChanged)
    Q_PROPERTY(QColor cloudColor READ cloudColor WRITE setCloudColor NOTIFY cloudColorChanged)
public:
    explicit WeatherBackgroundRenderer(QQuickItem *parent = nullptr);
    Renderer *createRenderer() const override;
    bool rain() const;
    bool cloud() const;
    bool sun() const;
    bool star() const;
    bool snow() const;
    QColor colorTop() const;
    QColor colorBottom() const;
    QColor cloudColor() const;

    void setRain(bool);
    void setCloud(bool);
    void setSun(bool);
    void setStar(bool);
    void setSnow(bool);
    void setColorTop(const QColor &);
    void setColorBottom(const QColor &);
    void setCloudColor(const QColor &);

Q_SIGNALS:
    void rainChanged();
    void cloudChanged();
    void sunChanged();
    void starChanged();
    void snowChanged();
    void colorTopChanged();
    void colorBottomChanged();
    void cloudColorChanged();

private Q_SLOTS:
    void handleTimeout();

protected:
    friend class WeatherBackgroundContentRenderer;
    int minFPS = 60;
    int modifiedMinFPS = 60;

private:
    QTimer *m_timer = nullptr;
    bool m_rain = false;
    bool m_cloud = false;
    bool m_sun = false;
    bool m_star = false;
    bool m_snow = false;

    QColor m_colourTop;
    QColor m_colourBottom;
    QColor m_cloudColor;
};

class CloudsRenderer;
class RainRenderer;
class SunRenderer;
class StarsRendererBase;
class SnowRendererBase;
class WeatherBackgroundContentRenderer : public QQuickFramebufferObject::Renderer, QOpenGLFunctions
{
public:
    void render() override;
    void synchronize(QQuickFramebufferObject *item) override;
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;

protected:
    friend class WeatherBackgroundRenderer;
    WeatherBackgroundContentRenderer();
    WeatherBackgroundContentRenderer(const WeatherBackgroundContentRenderer &other) = delete;
    WeatherBackgroundContentRenderer(WeatherBackgroundContentRenderer &&other) = delete;
    WeatherBackgroundContentRenderer operator=(const WeatherBackgroundContentRenderer &other) = delete;

private:
    CloudsRenderer *m_clouds = nullptr;
    RainRenderer *m_rain = nullptr;
    std::function<float()> dice;
    BackgroundRenderer *background;
    SunRenderer *m_sun = nullptr;
    StarsRendererBase *m_stars = nullptr;
    SnowRendererBase *m_snow = nullptr;

    float aspectRatio = 1;
    int m_minFPS = 60;
    bool synchronized = false;

    bool m_showRain = false;
    bool m_showCloud = false;
    bool m_showSun = false;
    bool m_showStar = false;
    bool m_showSnow = false;
    QColor m_colourTop;
    QColor m_colourBottom;
    QColor m_cloudColor;
    bool m_legacyMode = false;
};
