<?php

/**
 * "language": "C++x17",
 *    "extension": "cpp",
 *    "source": "solution.cpp",
 *    "compile": "${compiler} ${source} -O2 -std=c++17 -o ${binary}",
 *    "run": "${binary}",
 *    "compiler": "/usr/bin/g++",
 *    "binary": "solution",
 *    "version": "${compiler} -v"
 */
class LanguageConfig
{
    public $language;
    public $extension;
    public $source;
    public $compile;
    public $run;
    public $compiler;
    public $interpreter;
    public $binary;
    public $version;

    public function __construct($obj)
    {
        $this->language = $obj->language;
        $this->extension = $obj->extension;
        $this->source = $obj->source;
        $this->compile = $obj->compile ?? "";
        $this->run = $obj->run;
        $this->compiler = $obj->compiler ?? "";
        $this->interpreter = $obj->interpreter ?? "";
        $this->binary = $obj->binary ?? "";
        $this->version = $obj->version ?? "";

        $this->prepare();
    }

    private function prepare()
    {
        $this->compile = strtr($this->compile, [
            '${compiler}' => $this->compiler,
            '${source}' => $this->source,
            '${binary}' => $this->binary,
        ]);
        $this->run = strtr($this->run, [
            '${compiler}' => $this->compiler,
            '${interpreter}' => $this->interpreter,
            '${source}' => $this->source,
            '${binary}' => $this->binary,
        ]);
        $this->version = strtr($this->version, [
            '${compiler}' => $this->compiler,
            '${interpreter}' => $this->interpreter,
        ]);
    }
}