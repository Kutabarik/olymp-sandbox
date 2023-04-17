<?php

class FileManager
{

    private $configPath;
    private $compileCommand;
    private $runCommand;

    public function __construct($config_path)
    {
        $this->configPath = $config_path;
        $this->compileCommand = '';
        $this->runCommand = '';
    }

    public function copyAndRenameFile($filePath): string
    {
        $fileInfo = pathinfo($filePath);
        $newFilePath = $fileInfo['dirname'].DIRECTORY_SEPARATOR.'solution'.'.'
            .$fileInfo['extension'];
        if (!copy($filePath, $newFilePath)) {
            throw new Exception('Failed to copy file.');
        } else {
            return $newFilePath;
        }
    }

    public function parseConfig($solutionExtension): void
    {
        $configJSON = file_get_contents($this->configPath);
        $configDecoded = json_decode($configJSON, true);
        foreach ($configDecoded as $language) {
            $extension = $language['extension'];
            if ($extension === $solutionExtension) {
                switch ($extension) {
                    case 'c':
                    case 'cpp':
                        $compiler = $language["compiler"];
                        $binary = $language["binary"];
                        $source = $language["source"];
                        $compile = str_replace(
                            '${compiler}',
                            $compiler,
                            $language['compile']
                        );
                        $compile = str_replace('${source}', $source, $compile);
                        $compile = str_replace('${binary}', $binary, $compile);
                        $this->compileCommand = $compile;

                        break;
                    case 'java':
                        $compiler = $language['compiler'];
                        $interpreter = $language['interpreter'];
                        $binary = $language['binary'];
                        $source = $language['source'];
                        $compile = str_replace(
                            '${compiler}',
                            $compiler,
                            $language['compile']
                        );
                        $compile = str_replace('${source}', $source, $compile);
                        $run = str_replace(
                            '${interpreter}',
                            $interpreter,
                            $language['run']
                        );
                        $run = str_replace('${binary}', $binary, $run);

                        $this->compileCommand = $compile;
                        $this->runCommand = $run;

                        break;
                    case 'php':
                    case 'py':
                    case 'js':
                        $interpreter = $language["interpreter"];
                        $source = $language["source"];
                        $run = str_replace(
                            '${source}',
                            $source,
                            $language['run']
                        );
                        $run = str_replace(
                            '${interpreter}',
                            $interpreter,
                            $run
                        );

                        $this->runCommand = $run;

                        break;
                }
            }
        }
    }

    public function compileFile($filePath): bool|string
    {
        $solutionExtension = pathinfo($filePath, PATHINFO_EXTENSION);
        try {
            $solutionPath = $this->copyAndRenameFile($filePath);
        } catch (Exception $e) {
            echo 'Error: '.$e->getMessage();
        }

        $this->parseConfig($solutionExtension);

        if ($this->compileCommand !== '') {
            chdir(dirname($solutionPath));
            exec($this->compileCommand);

            return pathinfo($solutionPath, PATHINFO_FILENAME);
        } else {
            return false;
        }
    }

    public function getRunCommand(): string
    {
        return $this->runCommand;
    }
}
