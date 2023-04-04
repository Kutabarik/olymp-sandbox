<?php
class FileManager
{

	private $config_path;
	private $solution_path;
	private $compile_command;

	public function __construct($config_path, $solution_path)
	{
		$this->config_path = $config_path;
		$this->solution_path = $solution_path;
		$this->compile_command = '';
	}

	public function copyAndRenameFile($old_file_path, $new_file_name)
	{
		$new_file_path = dirname($old_file_path) . '/' . $new_file_name;
		if (copy($old_file_path, $new_file_path)) {
			return $new_file_path;
		} else {
			return false;
		}
	}

	private function parseConfig($solution_extension)
	{
		$config_json = file_get_contents($this->config_path);
		$config_decoded = json_decode($config_json, true);
		foreach ($config_decoded as $item) {
			if ($item['extension'] === $solution_extension) {
				if (isset($item["compiler"])) {
					$compiler = $item["compiler"];
					$binary = $item["binary"];
					$source = $item["source"];
					$compile = str_replace("\${compiler}", $compiler, $item["compile"]);
					$compile = str_replace("\${source}", $source, $compile);
					$compile = str_replace("\${binary}", $binary, $compile);

					$this->compile_command = $compile;
				}
			}
		}
	}

	public function compileFile($file_path)
	{
		$solution_extension = pathinfo($file_path, PATHINFO_EXTENSION);
		$new_file_name = 'temp.' . $solution_extension;
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
