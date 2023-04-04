<?php

include_once __DIR__ . "config.php";

use parallel\{Channel, Runtime};

/**
 * simple thread manager with one producer and $nrThreads consumers.
 */
class ThreadManager
{

    private static $consumers = [];
    private static $producer;
    private static $nrConsumers;
    private static $consumerTask;
    private static $producerTask;

    public static function init(int $nrThreads, Closure $produce, Closure $consume)
    {
        echo "init" . PHP_EOL;
        self::$nrConsumers = $nrThreads;
        self::$consumerTask = $consume;
        self::$producerTask = $produce;

        /**
         * Creates $nrConsumers or Runtime objects. Each object is associated with
         * uniqid ID. 
         */
        for ($i = 0; $i < self::$nrConsumers; ++$i) {
            $threadId = uniqid();
            self::$consumers[$threadId] = new Runtime();
        }

        /**
         * create a producer runtime
         */
        self::$producer = new Runtime();
    }

    public static function start(): void
    {
        echo "start" . PHP_EOL;
        /**
         * in the Producer we run self::$producerTask with
         * parameter self::$nrConsumers
         */
        self::$producer->run(function (Closure $closure, int $nrThreads) {
            $closure($nrThreads);
        }, [self::$producerTask, self::$nrConsumers]);

        /**
         * in the each consumer we run self::$consumerTask
         * with parameter $key (equal with uniqid of consumer)
         */
        foreach (self::$consumers as $key => $consumer) {
            $consumer->run(self::$consumerTask, [$key]);
        }
    }

    /**
     * close all consumers and producer
     */
    public static function finalize(): void
    {
        echo "finalize" . PHP_EOL;
        self::$producer->close();
        foreach (self::$consumers as $consumer) {
            $consumer->close();
        }
    }
}

/**
 * consumer task. it reads data from channel `data_channel`
 * while not null and process it.
 */
$consumeTask = function (string $taskId) {
    $channel = Channel::open("data_channel");
    echo "run task #{$taskId}" . PHP_EOL;
    while (($data = $channel->recv()) != null) {
        echo "[{$taskId}] consumer read data " . json_encode($data) . PHP_EOL;
        sleep($consumerTimeOut);
    }
    echo "consumer {$taskId} stops" . PHP_EOL;
};

/**
 * produser task, reads data from database and place it into
 * channel `data_channel`.
 */
$produceTask = function (int $nrConsumers) {
    include_once __DIR__ . "db.php";

    $db = new MySQLDB('localhost', 'username', 'password', 'database');
    $db->connect();

    $reads = 0;
    $channel = Channel::open("data_channel");
    echo "run producer" . PHP_EOL;
    while ($reads++ < 50) {
        //$data = [random_int(1, 5), random_int(1, 5), random_int(1, 5), random_int(1, 5),];
        $data = $db->getIdsByStatus(1);
        foreach ($data as $value) {
            echo "[producer] send data" . json_encode($value) . " to channel" . PHP_EOL;
            $channel->send($value);
            $db->updateSolutionStatus($value['id'], 2);
            //копируем файл и переименовываем
        }
        sleep($producerTimeOut);
    }
    for ($i = 0; $i < $nrConsumers; ++$i) {
        $channel->send(null);
    }
    echo "producer stops" . PHP_EOL;
};


// $config = file_get_contents($compiler_config);
// $config_decoded = json_decode($config, true);

// //create string to compile file
// foreach ($config_decoded as $item) {
//   if ($item['extension'] === $solution_extension) {
//     if (isset($item["compiler"])) {
//       $compiler = $item["compiler"];
//       $binary = $item["binary"];
//       $source = $item["source"];
//       $compile = str_replace("\${compiler}", $compiler, $item["compile"]);
//       $compile = str_replace("\${source}", $source, $compile);
//       $compile = str_replace("\${binary}", $binary, $compile);

//       print_r($compile);
//     }
//   }
// }
