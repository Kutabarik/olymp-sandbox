<?php

include_once __DIR__."/Config.php";
include_once __DIR__."/DB.php";
include_once __DIR__."/FileManager.php";
include_once __DIR__."/ThreadManager.php";

use parallel\Channel;

$channel = Channel::make("data_channel");

ThreadManager::init(Config::$nrThreads, $produceTask, $consumeTask);

ThreadManager::start();

ThreadManager::finalize();
