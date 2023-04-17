<?php

class FileManager
{

    private $config_path;
    private $compile_command;
    private $run_command;

    public function __construct($config_path)
    {
        $this->config_path = $config_path;
        $this->compile_command = '';
        $this->run_command = '';
    }

    public function copyAndRenameFile($old_file_path, $task_id): bool|string
    {
        $file_extension = pathinfo($old_file_path, PATHINFO_EXTENSION);
        $new_file_path = '/tmp/solutions/'.$task_id.'/solution.'
            .$file_extension;
        if (copy($old_file_path, $new_file_path)) {
            return $new_file_path;
        } else {
            return false;
        }
    }

    public function parseConfig($solution_extension): void
    {
        $config_json = file_get_contents($this->config_path);
        $config_decoded = json_decode($config_json, true);
        foreach ($config_decoded as $language) {
            $extension = $language['extension'];
            if ($extension === $solution_extension) {
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
                        $this->compile_command = $compile;
                        echo $compile;
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

                        $this->compile_command = $compile;
                        $this->run_command = $run;
                        echo $run;
                        echo $compile;
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

                        $this->run_command = $run;

                        echo $run;
                        break;
                }
            }
        }
    }

    public function compileFile($file_path): bool|string
    {
        $solution_extension = pathinfo($file_path, PATHINFO_EXTENSION);
        $new_file_name = 'temp.'.$solution_extension;
        $new_file_path = $this->copyAndRenameFile($file_path, $new_file_name);

        if ($new_file_path !== false) {
            $this->parseConfig($solution_extension);

            if ($this->compile_command !== '') {
                chdir(dirname($new_file_path));
                exec($this->compile_command);
                return pathinfo($new_file_path, PATHINFO_FILENAME);
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}
