#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

class MenuItem {
public:
    virtual ~MenuItem() = default;
    virtual void display(int indent = 0) const = 0;
    virtual shared_ptr<MenuItem> getChild(int index) { return nullptr; }
    virtual int getChildCount() const { return 0; }
    virtual string getName() const = 0;
};

class Menu : public MenuItem {
public:
    Menu(const string& name) : name(name) {}

    void addItem(shared_ptr<MenuItem> item) {
        items.push_back(item);
    }

    void display(int indent = 0) const override {
        string indentString(indent, ' ');
        cout << indentString << name << endl;

        for (size_t i = 0; i < items.size(); ++i) {
            items[i]->display(indent + 2);
        }
    }

    shared_ptr<MenuItem> getChild(int index) override {
        if (index >= 0 && static_cast<size_t>(index) < items.size()) {
            return items[index];
        }
        return nullptr;
    }

    int getChildCount() const override {
        return static_cast<int>(items.size());
    }

    string getName() const override {
        return name;
    }

private:
    string name;
    vector<shared_ptr<MenuItem>> items;
};

class SimpleMenuItem : public MenuItem {
public:
    SimpleMenuItem(const string& name) : name(name) {}

    void display(int indent = 0) const override {
        string indentString(indent, ' ');
        cout << indentString << name << endl;
    }

    string getName() const override {
        return name;
    }

private:
    string name;
};

void displayMenu(const shared_ptr<MenuItem>& currentMenu) {
    currentMenu->display();
}

void navigateMenu(shared_ptr<MenuItem> currentMenu) {
    shared_ptr<MenuItem> selectedMenu = currentMenu;
    bool running = true;
    int selection = -1;
    while (running) {
        displayMenu(selectedMenu);

        cout << "\nNavigate Menu: \n";
        cout << "1. Down (Select Submenu)\n";
        cout << "2. Up (Go Back)\n";
        cout << "3. Enter (Choose Option)\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> selection;

        if (selection == 1) {
            if (selectedMenu->getChildCount() > 0) {
                cout << "Select a submenu to go down to:\n";
                for (int i = 0; i < selectedMenu->getChildCount(); ++i) {
                    cout << i + 1 << ". " << selectedMenu->getChild(i)->getName() << endl;
                }
                cout << "Enter your choice (1-" << selectedMenu->getChildCount() << "): ";
                int choice;
                cin >> choice;
                if (choice >= 1 && choice <= selectedMenu->getChildCount()) {
                    selectedMenu = selectedMenu->getChild(choice - 1);
                }
            }
            else {
                cout << "No submenus to go down to.\n";
            }
        }
        else if (selection == 2) {
            if (currentMenu == selectedMenu) {
                cout << "You are already at the root menu.\n";
            }
            else {
                selectedMenu = currentMenu;
            }
        }
        else if (selection == 3) {
            if (selectedMenu->getChildCount() == 0) {
                cout << "You selected the option: " << selectedMenu->getName() << endl;
                cout << "Returning to main menu...\n";
                selectedMenu = currentMenu;
            }
            else {
                cout << "Please select a submenu first.\n";
            }
        }
        else if (selection == 4) {
            running = false;
            cout << "Exiting the menu navigation system.\n";
        }
        else {
            cout << "Invalid option. Please try again.\n";
        }
    }
}

int main() {
    auto mainMenu = make_shared<Menu>("Main Menu");
    auto settingsMenu = make_shared<Menu>("Settings");
    auto displaySettings = make_shared<Menu>("Display Settings");
    auto audioSettings = make_shared<Menu>("Audio Settings");
    auto mediaMenu = make_shared<Menu>("Media");
    auto radioOption = make_shared<SimpleMenuItem>("Radio");
    auto bluetoothAudio = make_shared<SimpleMenuItem>("Bluetooth Audio");

    displaySettings->addItem(make_shared<SimpleMenuItem>("Brightness"));
    displaySettings->addItem(make_shared<SimpleMenuItem>("Contrast"));

    audioSettings->addItem(make_shared<SimpleMenuItem>("Volume"));
    audioSettings->addItem(make_shared<SimpleMenuItem>("Balance"));

    mediaMenu->addItem(radioOption);
    mediaMenu->addItem(bluetoothAudio);

    settingsMenu->addItem(displaySettings);
    settingsMenu->addItem(audioSettings);

    mainMenu->addItem(settingsMenu);
    mainMenu->addItem(mediaMenu);

    navigateMenu(mainMenu);

    return 0;
}
