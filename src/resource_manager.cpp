#include <spdlog/spdlog.h>
#include <SFML/System/Exception.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include "resource_manager.hpp"

void ResourceManager::Init(std::filesystem::path path)
{
    spdlog::info("Initializing resource manager...");
    if (ResourceManager::instance_ == nullptr)
    {
        ResourceManager::instance_ = std::shared_ptr<ResourceManager>(new ResourceManager(path));
    }
}

std::shared_ptr<ResourceManager> &ResourceManager::get_instance()
{
    if (ResourceManager::instance_ == nullptr)
    {
        spdlog::error("Resource manager is not initialized");
    }

    return ResourceManager::instance_;
}

ResourceManager::ResourceManager(std::filesystem::path path) : images_(std::map<const std::string, std::shared_ptr<sf::Image>>()), fonts_(std::map<const std::string, std::shared_ptr<sf::Font>>())
{
    std::ifstream file = std::ifstream("../res/resources.json");

    nlohmann::json j = nlohmann::json::parse(file);
    if (j.contains("images"))
    {
        for (const auto [k, v] : j.at("images").items())
        {
            try
            {
                auto image = std::make_shared<sf::Image>(std::filesystem::path(v.get<std::string>()));
                images_.insert({k, image});
            }
            catch (const sf::Exception &ex)
            {
                spdlog::warn("Could not load image: {}", k);
            }
        }
    }

    if (j.contains("fonts"))
    {
        for (const auto [k, v] : j.at("fonts").items())
        {
            try
            {
                auto font = std::make_shared<sf::Font>(std::filesystem::path(v.get<std::string>()));
                fonts_.insert({k, font});
            }
            catch (const sf::Exception &ex)
            {
                spdlog::warn("Could not load font: {}", k);
            }
        }
    }
}

std::shared_ptr<sf::Image> ResourceManager::get_image(const std::string &name)
{
    if (images_.contains(name))
    {
        return images_.at(name);
    }

    return nullptr;
}

std::shared_ptr<sf::Font> ResourceManager::get_font(const std::string &name)
{
    if (fonts_.contains(name))
    {
        return fonts_.at(name);
    }

    return nullptr;
}
