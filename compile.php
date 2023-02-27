<?php

$options = getopt("s:c:t:u:", ["solution:", "config:", "taskId:", "userId:"]);

$solution = $options['s'] ?? $options['solution'] ?? null;
$compiler_config = $options['c'] ?? $options['config'] ?? null;
$taskId = $options['t'] ?? $options['taskId'] ?? null;
$userId = $options['u'] ?? $options['userId'] ?? null;

if (!$solution || !$compiler_config || !$taskId || !$userId) {
  echo "Usage: php script.php -s <solution> -c <config> -t <taskId> -u <userId>\n";
  exit(1);
}

$solution_extension = pathinfo($solution, PATHINFO_EXTENSION);

//defining a new name
// $new_name = 'solution.' . $solution_extension;
// $new_path = './solutions/' . $new_name;

//create directory if not exist
// if (!file_exists('./solutions')) {
//   mkdir('./solutions');
// }

//copy the file to the working directory
// copy($solution_file, $new_path);

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
