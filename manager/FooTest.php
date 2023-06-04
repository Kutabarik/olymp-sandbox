+<?php

require_once 'fileManager.php';

class FooTest
{
    function parseTest(): void
    {
        $manager = new FileManager('../compile-config.json');

        $manager->parseConfig('c');
        echo "\n";
        $manager->parseConfig('cpp');
        echo "\n";
        $manager->parseConfig('java');
        echo "\n";
        $manager->parseConfig('py');
        echo "\n";
        $manager->parseConfig('php');
    }

    function copyAndRenameFileTest(): void
    {
        $manager = new FileManager('../compile-config.json');

        $manager->copyAndRenameFile(
            "C:\\Users\\malay\\Documents\\GitHub\\olymp-sandbox\\examples\\example01\\example01.cpp"
        );
    }

    function compileFileTest(): void
    {
        $manager = new FileManager('../compile-config.json');

        $manager->compileFile(
            "C:\\Users\\malay\\Documents\\GitHub\\olymp-sandbox\\examples\\example01\\1.java"
        );
    }

}

$test = new FooTest();
//$test->parseTest();
//$test->copyAndRenameFileTest();
$test->compileFileTest();