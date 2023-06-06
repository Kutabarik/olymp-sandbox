<?php

class Config
{
    public static $dataDir = "/var/www/data/user/";

    public static $nrConsumers = 10;
    /**
     * @var $consumerTimeOut
     */
    public static $consumerTimeOut = 3;
    /**
     * @var $producerTimeOut
     */
    public static $producerTimeOut = 1;
    public static $nrThreads = 4;

    public static $dbuser = '';
    public static $dbname = '';
    public static $dbpass = '';
    public static $dbhost = '';

    public static function init()
    {
        self::$dbuser = getenv("MARIADB_USER");
        self::$dbname = getenv("MARIADB_DATABASE");
        self::$dbpass = getenv("MARIADB_PASSWORD");
        self::$dbhost = getenv("MARIADBHOST");
    }
}

Config::init();
