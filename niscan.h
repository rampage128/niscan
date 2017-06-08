#ifndef NISCAN_H
#define NISCAN_H

#include "carduino.h"

class NissanClimateControlPopulator : public CarSystemPopulator {
  public:
    NissanClimateControlPopulator(CarSystem* systemInstance) : CarSystemPopulator(systemInstance) {}
    virtual void populate(CanPacket* packet);
};

#endif
