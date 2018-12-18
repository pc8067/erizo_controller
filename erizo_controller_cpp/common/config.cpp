#include "config.h"

#include <fstream>

DEFINE_LOGGER(Config, "Config");

Config *Config::instance_ = nullptr;

Config::Config()
{
    port_ = 8080;
    ssl_key_ = "cert/key.pem";
    ssl_cert_ = "cert/cert.pem";
    ssl_port_ = 443;

    mysql_url_ = "tcp://119.28.70.47:3306";
    mysql_username_ = "root";
    mysql_password_ = "cathy978";

    redis_ip_ = "127.0.0.1";
    redis_port_ = 6379;
    redis_password_ = "cathy978";

    rabbitmq_username_ = "linmin";
    rabbitmq_passwd_ = "linmin";
    rabbitmq_hostname_ = "localhost";
    rabbitmq_port_ = 5672;
}

Config *Config::getInstance()
{
    if (instance_ == nullptr)
        instance_ = new Config;
    return instance_;
}

Config::~Config()
{
    if (instance_ != nullptr)
    {
        delete instance_;
        instance_ = nullptr;
    }
}

int Config::init(const std::string &config_file)
{
    std::ifstream ifs(config_file, std::ios::binary);
    if (!ifs.is_open())
    {
        ELOG_ERROR("Open %s failed", config_file);
        return 1;
    }

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root))
    {
        ELOG_ERROR("Parse %s failed", config_file);
        return 1;
    }

    Json::Value websocket = root["websocket"];
    if (websocket.isNull() ||
        websocket.type() != Json::objectValue ||
        websocket["port"].isNull() ||
        websocket["port"].type() != Json::intValue ||
        websocket["ssl_key"].isNull() ||
        websocket["ssl_key"].type() != Json::stringValue ||
        websocket["ssl_cert"].isNull() ||
        websocket["ssl_cert"].type() != Json::stringValue ||
        websocket["ssl_port"].isNull() ||
        websocket["ssl_port"].type() != Json::intValue)
    {
        ELOG_ERROR("Websocket config check error");
        return 1;
    }

    Json::Value mysql = root["mysql"];
    if (mysql.isNull() ||
        mysql.type() != Json::objectValue ||
        mysql["url"].isNull() ||
        mysql["url"].type() != Json::stringValue ||
        mysql["username"].isNull() ||
        mysql["username"].type() != Json::stringValue ||
        mysql["password"].isNull() ||
        mysql["password"].type() != Json::stringValue)
    {
        ELOG_ERROR("Mysql config check error");
        return 1;
    }

    Json::Value redis = root["redis"];
    if (redis.isNull() ||
        redis.type() != Json::objectValue ||
        redis["ip"].isNull() ||
        redis["ip"].type() != Json::stringValue ||
        redis["port"].isNull() ||
        redis["port"].type() != Json::intValue ||
        redis["password"].isNull() ||
        redis["password"].type() != Json::stringValue)
    {
        ELOG_ERROR("Redis config check error");
        return 1;
    }

    Json::Value rabbitmq = root["rabbitmq"];
    if (rabbitmq.isNull() ||
        rabbitmq.type() != Json::objectValue ||
        rabbitmq["host"].isNull() ||
        rabbitmq["host"].type() != Json::stringValue ||
        rabbitmq["port"].isNull() ||
        rabbitmq["port"].type() != Json::intValue ||
        rabbitmq["username"].isNull() ||
        rabbitmq["username"].type() != Json::stringValue ||
        rabbitmq["password"].isNull() ||
        rabbitmq["password"].type() != Json::stringValue)
    {
        ELOG_ERROR("Rabbitmq config check error");
        return 1;
    }

    port_ = websocket["port"].asInt();
    ssl_key_ = websocket["ssl_key"].asString();
    ssl_cert_ = websocket["ssl_cert"].asString();
    ssl_port_ = websocket["ssl_port"].asInt();

    mysql_url_ = mysql["url"].asString();
    mysql_username_ = mysql["username"].asString();
    mysql_password_ = mysql["password"].asString();

    redis_ip_ = redis["ip"].asString();
    redis_port_ = redis["port"].asInt();
    redis_password_ = redis["password"].asString();

    rabbitmq_hostname_ = rabbitmq["host"].asString();
    rabbitmq_port_ = rabbitmq["port"].asInt();
    rabbitmq_username_ = rabbitmq["username"].asString();
    rabbitmq_passwd_ = rabbitmq["password"].asString();

    return 0;
}
