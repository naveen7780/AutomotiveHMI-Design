#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

class VehicleData {
public:
    int speed;
    int fuelLevel;
    int engineTemp;

    VehicleData() : speed(0), fuelLevel(100), engineTemp(90) {}

    void updateData() {
        speed = rand() % 200;
        fuelLevel = rand() % 101;
        engineTemp = rand() % 150;
    }

    void checkWarnings() const {
        if (engineTemp > 100) {
            std::cout << "Warning: Engine temperature is too high (" << engineTemp << "°C)!" << std::endl;
        }
        if (fuelLevel < 10) {
            std::cout << "Warning: Fuel level is critically low (" << fuelLevel << "%)!" << std::endl;
        }
    }
};

class Display {
public:
    void showData(const VehicleData& data) {
        std::cout << "Speed: " << data.speed << " km/h\t";
        std::cout << "Fuel: " << data.fuelLevel << "%\t";
        std::cout << "Temperature: " << data.engineTemp << "°C" << std::endl;
    }

    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }
};

void updateAndDisplay(VehicleData& data, Display& display) {
    while (true) {
        data.updateData();
        display.clearScreen();
        display.showData(data);
        data.checkWarnings();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    VehicleData vehicleData;
    Display display;

    std::thread dataThread(updateAndDisplay, std::ref(vehicleData), std::ref(display));

    dataThread.join();

    return 0;
}

