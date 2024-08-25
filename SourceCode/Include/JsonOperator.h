#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include <filesystem>
#include <fstream>
#include <sstream>

bool JsonToString(const Json::Value& json, std::string& str)
{
    Json::StreamWriterBuilder writer;
    str = Json::writeString(writer, json);
    return true;
}

bool StringToJson(const std::string& str, Json::Value& json)
{
    Json::CharReaderBuilder reader;
    std::string             errs;
    std::istringstream      iss(str);

    return Json::parseFromStream(reader, iss, &json, &errs);
}

bool JsonToFile(const Json::Value& json, const std::filesystem::path path)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        return false;
    }

    Json::StreamWriterBuilder writer;
    file << Json::writeString(writer, json);
    return true;
}

bool FileToJson(const std::filesystem::path path, Json::Value& json)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        return false;
    }

    Json::CharReaderBuilder reader;
    std::string             errs;
    return Json::parseFromStream(reader, file, &json, &errs);
}
