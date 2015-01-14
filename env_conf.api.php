<?php

/**
 * @file
 * Per-environment configuration module,
 * provided hooks.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Declare new environment(s), with callback(s).
 *
 * This hook is used to create new environments for your app/site.
 * To correctly create an environment, you must declare:
 * - one or more callback function(s) (executed when deploying configuration)
 * - file to include to correctly define callbacks function(s)
 * - a weight, to order environment(s) on administration page
 *
 * @return array
 *   An array representing declared environments
 *   - environment_name: The unique name of the custom environment.
 *   - callbacks: array, callback functions list.
 *   - include_file: array, list of files to include, where callback functions
 *                   are defined
 *                   @see module_load_include()
 *   - weight: integer, used to order environment in administration page
 *
 * @see README.md file
 */
function hook_env_conf_available_env() {
  return array(
    'environment_name' => array(
      'callbacks' => array(
        'first_function',
        'second_function',
      ),
      'include_file' => array(
        array(
          'type' => 'inc',
          'module' => 'mymodulecustom',
          'name' => 'mymodulecustom.env_conf',
        ),
      ),
      'weight' => 1,
    ),
  );
}

/**
 * Alter existing environments.
 *
 * This hook is used to alter existing environments for your app/site.
 * You only have to alter $environments variable,
 * mostly, adding one or more callbacks functions and files to include
 *
 * @param array $environments
 *   An array representing existing environments
 *   @see hook_env_conf_available_env()
 */
function hook_env_conf_available_env_alter(&$environments) {
  $environments['environment_name']['callbacks'][] = 'third_function'
  $environments['environment_name']['include_file'][] = array(
    'type' => 'inc',
    'module' => 'mymodulecustom',
    'name' => 'mymodulecustom.other_file',
  );
  $environments['environment_name']['weight'] = 3;
}

/**
 * @} End of "addtogroup hooks".
 */
