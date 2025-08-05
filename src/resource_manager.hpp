/*

JSON-file:
{
    "font": {"name_of_font":filepath,
              "name_of_font2": filepath},
    "sound": {"name":filepath},
    "music": {...}
    "image": {...}
}

*/

#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>

class ResourceManager
{
private:
    static inline std::shared_ptr<ResourceManager> instance_;

    std::map<const std::string, std::shared_ptr<sf::Image>> images_;
    std::map<const std::string, std::shared_ptr<sf::Font>> fonts_;

    ResourceManager(std::filesystem::path path);

public:
    ResourceManager(const ResourceManager &) = delete;
    void operator=(const ResourceManager &) = delete;
    ~ResourceManager() {};

    static void Init(std::filesystem::path path);
    static std::shared_ptr<ResourceManager> &get_instance();

    std::shared_ptr<sf::Image> get_image(const std::string &name);
    std::shared_ptr<sf::Font> get_font(const std::string &font);
};
