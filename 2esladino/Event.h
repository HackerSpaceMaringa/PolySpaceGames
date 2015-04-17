#ifndef EVENT_H
#define EVENT_H

static unsigned int eventId = 0;

class Event {
  private:
    unsigned int id;
 
  public:
    Event() {
      id = eventId++;  
    }

};

#endif
