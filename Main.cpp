

#include "Main.h"
#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>

class DuplicateItemException : public std::runtime_error {
public:
    DuplicateItemException(const std::string& msg) : std::runtime_error(msg) {}
};

class ItemNotFoundException : public std::runtime_error {
public:
    ItemNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

class StoredItem {
private:
    std::string id;
    std::string description;
    std::string location;

public:
    StoredItem(std::string id, std::string desc, std::string loc)
        : id(id), description(desc), location(loc) {}

    std::string getId() const { return id; }
    std::string getDescription() const { return description; }
    std::string getLocation() const { return location; }
};

class StorageManager {
private:
    std::unordered_map<std::string, std::shared_ptr<StoredItem>> itemById;
    std::map<std::string, std::shared_ptr<StoredItem>> itemByDescription;

public:
    void addItem(const std::shared_ptr<StoredItem>& item) {
        const std::string&id = item->getId();
        std::cout << "Adding item " << id << std::endl;
        if (itemById.find(id) != itemById.end()) {
            throw DuplicateItemException("Item already exists");
        }
        itemById[id] = item;
        itemByDescription[item->getDescription()] = item;
        std::cout << "Item " << id << " added to system." << std::endl << std::endl;
    }

    std::shared_ptr<StoredItem> findById(const std::string& id) const {
        auto it = itemById.find(id);
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item not found");
        }
        return it->second;
    }

    void removeItem(const std::string& id) {
        auto it = itemById.find(id);
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item not found");
        }
        auto description = it->second->getDescription();
        itemByDescription.erase(description);
        std::cout << "Item " << id << " removed from system." << std::endl;
        itemById.erase(it);
    }

    void listItemsByDescription() const {
        for (const auto& pair : itemByDescription) {
            const auto& item = pair.second;
            std::cout << "ID: "<< item->getId()
                      << ", Description: "<< item->getDescription()
                      << ", Location: "<< item->getLocation() << std::endl;
        }
    }
};

void testDuplicateAddition(StorageManager manager) {
    try {
        auto item4 = std::make_shared<StoredItem>("AS0051FP321", "1/16 Hex Bolt", "Aisle 6 Shelf A");
        manager.addItem(item4);
    } catch (const DuplicateItemException& e) {
        std::cerr << "DuplicateItemException: " << e.what() << std::endl;
    }
}
void testItemNotFound(const StorageManager& manager) {
    auto found = manager.findById("AS0051FP325");
}

int main() {
    StorageManager manager;

    auto item1 = std::make_shared<StoredItem>("AS0051FP321", "1/16 Hex Bolt", "Aisle 6 Shelf A");
    auto item2 = std::make_shared<StoredItem>("AS0052FP322", "1/16 Flat Washer", "Aisle 6 Shelf A");
    auto item3 = std::make_shared<StoredItem>("AS0051FP322", "1/16 Hex Nut", "Aisle 6 Shelf A");

    try { // add items
        manager.addItem(item1);
        manager.addItem(item2);
        manager.addItem(item3);

        // duplicate test
        testDuplicateAddition(manager);

        // look and remove
        auto found = manager.findById("AS0051FP321");
        std::cout << "Item Nomen Found: " << found->getDescription() << std::endl;

        manager.removeItem("AS0051FP321");


        testItemNotFound(manager);
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }

    std::cout << "\nItems Held:" << std::endl;
    manager.listItemsByDescription();

    return 0;
}