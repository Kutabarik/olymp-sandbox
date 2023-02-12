<?php

$options = getopt("s:c:t:", array("solution:", "config:", "taskId:"));

$solution_file = $options["s"];
$compiler_config = $options["c"];
$task_id = $options["t"];

if (empty($solution_file) || empty($compiler_config) || empty($task_id)) {
  echo "Usage: script.php -s <solution_file> -c <compiler_config> -t <task_id>\n";
  exit;
}

//defining a new name
$solution_extension = pathinfo($solution_file, PATHINFO_EXTENSION);
$new_name = 'solution.' . $solution_extension;
$new_path = './solutions/' . $new_name;

//create directory if not exist
if (!file_exists('./solutions')) {
  mkdir('./solutions');
}

//copy the file to the working directory
copy($solution_file, $new_path);

$config = file_get_contents($compiler_config);
$config_decoded = json_decode($config, true);

//create string to compile file
foreach ($config_decoded as $item) {
  if ($item['extension'] === $solution_extension) {
    if (isset($item["compiler"])) {
      $compiler = $item["compiler"];
      $binary = $item["binary"];
      $source = $item["source"];
      $compile = str_replace("\${compiler}", $compiler, $item["compile"]);
      $compile = str_replace("\${source}", $source, $compile);
      $compile = str_replace("\${binary}", $binary, $compile);

      print_r($compile);
    }
  }
}
