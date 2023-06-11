<?php

include_once __DIR__.'/LanguageConfig.php';
include_once __DIR__.'/Config.php';

class FileManager
{

    private $configPath;
    private $languageConfigs;

    public function __construct(string $configPath)
    {
        $this->configPath = $configPath;
        $this->languageConfigs = [];

        $this->parseConfig();
    }

    public function copyAndRenameFile(string $filePath, int $userId): string
    {
        $fileInfo = pathinfo($filePath);
//        $newFilePath = $fileInfo['dirname'] . DIRECTORY_SEPARATOR . 'solution' . '.' . $fileInfo['extension'];
        $newFilePath = Config::$solutionsDir.DIRECTORY_SEPARATOR.$userId
            .DIRECTORY_SEPARATOR.'solution'.'.'.$fileInfo['extension'];

        if (!copy($filePath, $newFilePath)) {
            throw new Exception('Failed to copy file.');
        } else {
            return $newFilePath;
        }
    }

    public function parseConfig(): void
    {
        $configJSON = file_get_contents($this->configPath);
        $configDecoded = json_decode($configJSON);


        foreach ($configDecoded as $language) {
            $extension = $language->extension;
            $this->languageConfigs[$extension] = new LanguageConfig($language);
        }
    }

    public function getLanguageConfig($extension): LanguageConfig
    {
        return $this->languageConfigs[$extension];
    }

    public function compileFile(string $filePath, int $userId): string
    {
        $solutionExtension = pathinfo($filePath, PATHINFO_EXTENSION);

        try {
            $solutionPath = $this->copyAndRenameFile($filePath, $userId);
        } catch (Exception $e) {
            echo 'Error: '.$e->getMessage();
        }

        $languageConfig = $this->getLanguageConfig($solutionExtension);

        if ($languageConfig->compile !== '') {
            chdir(dirname($solutionPath));
            exec($languageConfig->compile);
        }
        return $languageConfig->run;
    }
}
