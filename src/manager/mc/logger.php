<?php

namespace mc;

/**
 * Description of log
 *
 * @author Croitor Mihail <mcroitor@gmail.com>
 */
class logger {

    public const INFO = 1;  // standard color
    public const PASS = 2;  // green color
    public const WARN = 4;  // yellow color
    public const ERROR = 8; // red color
    public const FAIL = 16; // red color
    public const DEBUG = 32; // red color
    
    private const LOG_TYPE = [
        self::INFO => "INFO",
        self::PASS => "PASS",
        self::WARN => "WARN",
        self::ERROR => "ERROR",
        self::FAIL => "FAIL",
        self::DEBUG => "DEBUG"
    ];

    var $logfile;
    var $pretifier = null;

    /**
     * 
     * @param string $logfile
     */
    public function __construct(string $logfile = "php://stdout") {
        $this->logfile = $logfile;
    }

    public function setPretifier(callable $pretifier) {
        $this->pretifier = $pretifier;
    }

    /**
     * write a message with specific log type marker
     * @param string $data
     * @param string $log_type
     */
    private function write(string $data,string  $log_type) {
        if (isset($_SESSION["timezone"])) {
            date_default_timezone_set($_SESSION["timezone"]);
        }
        $type = self::LOG_TYPE[$log_type];
        $text = date("Y-m-d H:i:s") . "\t{$type}: {$data}" . PHP_EOL;
        if ($this->pretifier) {
            $text = call_user_func($this->pretifier, $text);
        }
        file_put_contents($this->logfile, $text, FILE_APPEND);
    }

    /**
     * info message
     * @param string $data
     */
    public function info(string $data) {
        $this->write($data, self::INFO);
    }

    /**
     * warn message
     * @param string $data
     */
    public function warn(string $data) {
        $this->write($data, self::WARN);
    }

    /**
     * pass message
     * @param string $data
     */
    public function pass(string $data) {
        $this->write($data, self::PASS);
    }

    /**
     * error message
     * @param string $data
     */
    public function error(string $data) {
        $this->write($data, self::ERROR);
    }

    /**
     * fail message
     * @param string $data
     */
    public function fail(string $data) {
        $this->write($data, self::FAIL);
    }
    
    public function debug(string $data) {
        $this->write($data, self::DEBUG);
    }

    /**
     * stdout logger builder
     * @return \mc\logger
     */
    public static function stdout(){
        return new logger();
    }
}
