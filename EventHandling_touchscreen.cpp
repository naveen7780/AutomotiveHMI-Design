#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <string>
#include <chrono>
#include <thread>

class Event {
public:
    enum class EventType { Tap, Swipe };

    Event(EventType type, int x, int y)
        : eventType(type), xCoord(x), yCoord(y) {
        timestamp = std::chrono::system_clock::now();
    }

    EventType getEventType() const { return eventType; }
    int getX() const { return xCoord; }
    int getY() const { return yCoord; }
    std::chrono::system_clock::time_point getTimestamp() const { return timestamp; }

private:
    EventType eventType;
    int xCoord;
    int yCoord;
    std::chrono::system_clock::time_point timestamp;
};

class EventQueue {
public:
    void addEvent(const Event& event) {
        eventQueue.push(event);
    }

    bool hasEvents() const {
        return !eventQueue.empty();
    }

    Event getNextEvent() {
        if (hasEvents()) {
            Event event = eventQueue.front();
            eventQueue.pop();
            return event;
        }
        throw std::runtime_error("No events to process");
    }

private:
    std::queue<Event> eventQueue;
};

void processEvent(const Event& event) {
    if (event.getEventType() == Event::EventType::Tap) {
        std::cout << "Tap detected at position: (" << event.getX() << ", " << event.getY() << ")\n";
    }
    else if (event.getEventType() == Event::EventType::Swipe) {
        std::string swipeDirection;

        if (event.getX() > 500) {
            swipeDirection = "Right";
        }
        else if (event.getX() < 200) {
            swipeDirection = "Left";
        }
        else if (event.getY() > 500) {
            swipeDirection = "Down";
        }
        else {
            swipeDirection = "Up";
        }

        std::cout << "Swipe detected, direction: " << swipeDirection << "\n";
    }
}

void generateEvents(EventQueue& queue) {
    for (int i = 0; i < 10; ++i) {
        Event::EventType type = (rand() % 2 == 0) ? Event::EventType::Tap : Event::EventType::Swipe;
        int x = rand() % 800;
        int y = rand() % 600;

        Event event(type, x, y);
        queue.addEvent(event);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    EventQueue eventQueue;

    std::thread generatorThread(generateEvents, std::ref(eventQueue));

    while (true) {
        if (eventQueue.hasEvents()) {
            Event event = eventQueue.getNextEvent();
            processEvent(event);
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    generatorThread.join();

    return 0;
}
