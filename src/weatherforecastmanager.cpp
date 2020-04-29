#include "weatherforecastmanager.h"
#include "openweathermap.h"
#include "norwegianmeteorologicalinstitute.h"
#include "openweathermap.h"
#include "weatherlocationmodel.h"
#include <QDebug>
#include <QTimer>
WeatherforecastManager::WeatherforecastManager(WeatherLocationListModel& model,int defaultAPI) :
    model_(model),
    api_(defaultAPI)
{
  if (defaultAPI != NORWEGIAN || defaultAPI != OPENWEATHER){
    qDebug() << "wrong api";
    exit(1);
  }
  distribution = new std::uniform_int_distribution<int>(0,30*3600); // uniform random update interval, 60 min to 90 min
  auto rand = std::bind ( *distribution, generator );

  timer = new QTimer(this);
  timer->setSingleShot(true);
  connect(timer, &QTimer::timeout, this, &WeatherforecastManager::update);
  if (api_ == NORWEGIAN)
    timer->start(1000*3600 + rand());
  else
    timer->start(1000*3*3600 + rand());
}

WeatherforecastManager& WeatherforecastManager::instance(WeatherLocationListModel& model) {
  static WeatherforecastManager singleton(model);

  return singleton;
}

void WeatherforecastManager::update(){
    if (api_ == NORWEGIAN){
        auto locations = model_.getList();
        while(static_cast<int>(norwegian_->size()) > locations.size()){ // I don't think we will track locations above
            delete norwegian_->back();                        // size limit of int
            norwegian_->pop_back(); //remove excess api instance
        }
        while (static_cast<int>(norwegian_->size()) < locations.size()){
            norwegian_->push_back(new NorwegianMeteorologicalInstitute()); // add instance
        }
        for (int i=0; i< static_cast<int>(norwegian_->size());i++){
            // why do I write spaghetti code?
            // cause race condition is killing me!!!
            norwegian_->at(i)->setLocation(locations.at(i)->latitude(),locations.at(i)->longitude());
            norwegian_->at(i)->update();
            connect(norwegian_->at(i), &AbstractAPI::updated, locations.at(i), &WeatherLocation::update);
        }
    }
}
