// SPDX-FileCopyrightText: 2024 The KWeather Authors
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <KRunner/AbstractRunner>
#include <KRunner/QueryMatch>
#include <KRunner/RunnerContext>

class KWeatherRunner : public KRunner::AbstractRunner
{
    Q_OBJECT

public:
    explicit KWeatherRunner(QObject *parent, const KPluginMetaData &metaData);
    ~KWeatherRunner() override;

    void match(KRunner::RunnerContext &context) override;
    void run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) override;

private:
    void fetchAndAddWeather(KRunner::RunnerContext &context, double lat, double lon, const QString &locationName);
};
