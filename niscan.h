#ifndef NISCAN_H
#define NISCAN_H

#include "carduino.h"

typedef CarSystemPopulator<ClimateControl> NissanClimateControlPopulator;
/*
template<typename T> class NissanClimateControlPopulator : public CarSystemPopulator<ClimateControl> {
  public:
    void populate(CanPacket* packet);
};
*/

#endif
