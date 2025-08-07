#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio.hpp>

class Audio
{
private:
    sf::SoundBuffer buffer_;
    std::unique_ptr<sf::Sound> sound_;

public:
    Audio(std::filesystem::path path);

    void play();
};

class ResourceManager
{
private:
    static inline std::shared_ptr<ResourceManager> instance_;

    std::map<const std::string, std::shared_ptr<sf::Image>> images_;
    std::map<const std::string, std::shared_ptr<sf::Font>> fonts_;
    std::map<const std::string, std::shared_ptr<Audio>> sounds_;
    std::map<const std::string, std::shared_ptr<sf::Music>> musics_;

    ResourceManager(std::filesystem::path path);

public:
    ResourceManager(const ResourceManager &) = delete;
    void operator=(const ResourceManager &) = delete;

    static void init(std::filesystem::path path);
    static std::shared_ptr<ResourceManager> &get_instance();

    std::shared_ptr<sf::Image> get_image(const std::string &name);
    std::shared_ptr<sf::Font> get_font(const std::string &name);
    std::shared_ptr<Audio> get_sound(const std::string &name);
};
