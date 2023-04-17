<?php

include_once __DIR__."/config.php";
include_once __DIR__."/db.php";
include_once __DIR__."/fileManager.php";
include_once __DIR__."/threadManager.php";

use parallel\Channel;

$channel = Channel::make("data_channel");

ThreadManager::init($nrThreads, $produceTask, $consumeTask);

ThreadManager::start();

ThreadManager::finalize();
