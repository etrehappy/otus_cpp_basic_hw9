#include "settings.h"

void gset::CheckPath(const std::filesystem::path& p)
{
    namespace fs = std::filesystem;

    try
    {
        if (fs::status(p).type() == fs::file_type::not_found)
        {
            throw fs::filesystem_error("not found", p,
                std::make_error_code(std::errc::no_such_file_or_directory));
        }
    }
    catch (const fs::filesystem_error& ex) {

        std::ofstream log(gset::kLogError, std::ios::out);
        log << "***\nFile system error:  " << ex.what();
        log.close();
    }
}

void gset::LoadFonts(sf::Font& font, const std::filesystem::path& path)
{
    namespace fs = std::filesystem;

    gset::CheckPath(path);

    try
    {  
        if (!font.loadFromFile(path.string()))
        {
            throw std::runtime_error("Failed to load font file: " + path.string());
        }
    }
    catch (const std::runtime_error& ex) {

        std::ofstream log(gset::kLogError, std::ios::app);
        log << "\n***\n" << ex.what();
        log.close();
    }
}

void gset::LoadMusic(sf::Music& music, const std::filesystem::path& path)
{
    namespace fs = std::filesystem;

    gset::CheckPath(path);

    try
    {
        if (!music.openFromFile(path.string()))
        {
            throw std::runtime_error("Failed to load music file: " + path.string());
        }
    }
    catch (const std::runtime_error& ex) {

        std::ofstream log(gset::kLogError, std::ios::app);
        log << "\n***\n" << ex.what();
        log.close();
    }
}

void gset::LoadTexture(sf::Texture& texture, const std::filesystem::path& path)
{
    namespace fs = std::filesystem;

    gset::CheckPath(path);

    try
    {
        if (!texture.loadFromFile(path.string()))
        {
            throw std::runtime_error("Failed to load texture file: " + path.string());
        }
    }
    catch (const std::runtime_error& ex) {

        std::ofstream log(gset::kLogError, std::ios::app);
        log << "\n***\n" << ex.what();
        log.close();
        std::terminate();
    }
}
