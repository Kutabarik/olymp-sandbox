<?php

use parallel\{Channel, Runtime};

/**
 * simple thread manager with one producer and $nrThreads consumers.
 */
class ThreadManager
{

    private static array $consumers = [];
    private static $producer;
    private static $nrConsumers;
    private static $consumerTask;
    private static $producerTask;

    public static function init(
        int $nrThreads,
        Closure $produce,
        Closure $consume
    ): void {
        echo "init".PHP_EOL;
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
        echo "start".PHP_EOL;
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
        echo "finalize".PHP_EOL;
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
    include_once __DIR__."/config.php";
    include_once __DIR__."/fileManager.php";
    include_once __DIR__."/DB.php";

    $db = new DB($dbhost, $dbuser, $dbpass, $dbname);
    $db->connect();

    $fileManager = new FileManager(__DIR__."/../compile-config.json");

    $channel = Channel::open("data_channel");
    //echo "run task ${$taskId}".PHP_EOL;

    while (($solution = $channel->recv()) != null) {
        //        echo "[${$taskId}] consumer read data ".json_encode($data).PHP_EOL;
        //        sleep($consumerTimeOut);
        $execCommand = $fileManager->compileFile(
            '/data/user/'.$solution['user_id'].'/'.$solution['filename']
        );
        $db->updateSolutionStatus($solution['id'], 2);
        //наверное берем из БД данные о задаче
        //$tests = $db->getTests($solution['task_id']);
        $tests = [['input' => 10, 'output' => 20]];
        list($time, $memory) = [10, 20];
        //list($time, $memory) = $db->getTaskLimits($solution['task_id']);
        foreach ($tests as $test) {
            $command
                = "olymp-sandbox -a /data/user/{$solution['user_id']}/{$execCommand}"
                .
                " -t {$time} -m {$memory}".
                " -i {$test["input"]} -o {$test["output"]}";
            echo '\n';
            print_r($command);
            exec($command, $output, $return_value);
        }
    }

    echo "consumer ${$taskId} stops".PHP_EOL;
};

/**
 * producer task, reads data from database and place it into
 * channel `data_channel`.
 */
$produceTask = function (int $nrConsumers) {
    include_once __DIR__."/DB.php";
    include_once __DIR__."/config.php";

    $db = new DB($dbhost, $dbuser, $dbpass, $dbname);
    $db->connect();

    $reads = 0;
    $channel = Channel::open("data_channel");
    echo "run producer".PHP_EOL;

    while (true) {
        //while (true) {
        //$data = [random_int(1, 5), random_int(1, 5), random_int(1, 5), random_int(1, 5),];
        $solutions = $db->getSolutionsByStatus(0);

        foreach ($solutions as $solution) {
            $channel->send($solution);
            $db->updateSolutionStatus($solution['id'], 1);
        }
        sleep($producerTimeOut);
    }

    for ($i = 0; $i < $nrConsumers; ++$i) {
        $channel->send(null);
    }
    echo "producer stops".PHP_EOL;
};
