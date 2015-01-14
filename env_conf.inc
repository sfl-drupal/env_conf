<?php

/**
 * @file
 * Per-environment configuration module,
 * helper functions.
 */

/**
 * Helper function to list all available environments,
 * ordered by "weight" value.
 */
function _get_available_environments() {
  $available_envs = module_invoke_all('env_conf_available_env');
  drupal_alter('env_conf_available_env', $available_envs);
  if (uasort($available_envs, '_ordered_by_weight_value')) {
    return $available_envs;
  }
  return FALSE;
}

/**
 * Helper function to sort two environments,
 * using "weight" value.
 * @see _get_available_environments()
 */
function _ordered_by_weight_value($a, $b) {
  if ($a['weight'] == $b['weight']) {
    return 0;
  }
  return ($a['weight'] < $b['weight']) ? -1 : 1;
}

/**
 * Helper function to list all available environments (names only),
 * ordered by "weight" value.
 * @see _get_available_environments()
 */
function _get_simple_available_environments() {
  if ($full_list = _get_available_environments()) {
    $envs = array();
    foreach ($full_list as $key => $env) {
      $envs[$key] = $key;
    }
    return $envs;
  }
  return FALSE;
}

/**
 * Process functions to deploy an selected environment configuration
 */
function _deploy_environment_configuration($selected_environment) {

  // List available environments configurations
  $available_envs = _get_available_environments();
  dsm($available_envs);
  dsm($available_envs[$selected_environment]);

  // Include file to define callback function(s)
  foreach ($available_envs[$selected_environment]['include_file'] as $file) {
    // $file = $available_envs[$selected_environment]['include_file'];
    if (isset($file['type']) && isset($file['module'])) {
      if (isset($file['name'])) {
        module_load_include($file['type'], $file['module'], $file['name']);
      }
      else {
        module_load_include($file['type'], $file['module']);
      }
    }
    else {
      drupal_set_message(t('Impossible to include file where functions are defined.'), 'warning');
    }
  }

  // Execute each functions or alert
  $errors = 0;
  foreach ($available_envs[$selected_environment]['callbacks'] as $callback) {
    if (function_exists($callback)) {
      $callback();
      drupal_set_message(t('<strong>!function</strong>() function has been processed.', array('!function' => $callback)));
    }
    else {
      drupal_set_message(t('Call to undefined function <strong>!function</strong>().', array('!function' => $callback)), 'error');
      $errors++;
    }
  }

  // Alert if error(s) while processing configurations functions
  if ($errors > 1) {
    drupal_set_message(t('!nb functions of <strong>!env</strong> configuration are undefined. Your site is probably misconfigured.', array('!nb' => $errors, '!env' => $selected_environment)), 'error');
  }
  elseif ($errors === 1) {
    drupal_set_message(t('One function of <strong>!env</strong> configuration is undefined. Your site is probably misconfigured.', array('!nb' => $errors, '!env' => $selected_environment)), 'error');
  }

  // Store deployed configuration
  variable_set('env_conf_last_deployed_conf', $selected_environment);
}
