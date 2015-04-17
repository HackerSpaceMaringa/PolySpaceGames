#ifndef ENTITY_H
#define ENTITY_H

#include "Actor.h"
#include "LinkedList.h"

enum IAType {
  easy, normal, hard;
};

class Entity : Actor {
  private:
    LinkedList<Events&> events;
    IAType iaType;

  public:
    void setIAType(IAType I) {
      iaType = I;
    }

    IAType getIAType() {
      return iaType;
    }

    void addEvent(Event &E) {
      events.add(E);
    }

    Event& getEvent(int i) {
      return events.get(i);
    }

    int getNumEvents() {
      return events.size();
    }
};

#endif
