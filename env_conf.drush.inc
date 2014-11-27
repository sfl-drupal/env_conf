<?php

/**
 * @file
 * Per-environment configuration module,
 * drush commands.
 */

/**
 * Implementation of hook_drush_help().
 */
function env_conf_drush_help($section) {
  switch ($section) {
    case 'drush:deploy-configuration':
      return dt('Deploy a selected environment configuration.');
  }
}

/**
 * Implements hook_drush_command().
 */
function env_conf_drush_command() {
  return array(
    'deploy-configuration' => array(
      'description' => 'Deploy a selected environment configuration.',
      'aliases' => array('d-conf'),
      'callback' => 'drush_deploy_environment_configuration',
      'arguments' => array(
        'selected-environment' => 'Environment you want to deploy the configuration.',
      ),
    ),
  );
}

function drush_deploy_environment_configuration($selected_environment = NULL) {
  module_load_include('inc', 'env_conf');

  // Alert if selected environment isn't available
  $available_envs = _get_simple_available_environments();

  // Determine default environment if not selected
  if ($selected_environment === NULL) {
    $selected_environment = variable_get('environment', NULL);
    if ($selected_environment === NULL) {
      drupal_set_message(t('No environment configured in <i>settings.php</i>. You should edit it and add "$conf[\'environment\'] = \'env_name\';".'), 'error');
      drupal_set_message(t('Unable to deploy any environment configuration.'), 'error');
      return FALSE;
    }
    else {
      drupal_set_message(t('Environment detected in <i>settings.php</i>: "!env".', array('!env' => $selected_environment)));
    }
  }
  elseif (!in_array($selected_environment, $available_envs)) {
    drupal_set_message(t('Selected environment is "!env", but available environments are "!available_envs". Please select one of them or define "!env" as an available environment by implementing "hook_env_conf_available_env()".', array('!env' => $selected_environment, '!available_envs' => implode(', ', $available_envs))), 'error');
    return FALSE;
  }

  // Deploy selected configuration
  drupal_set_message(t('Starting deploy "!env" configuration.', array('!env' => $selected_environment)));
  _deploy_environment_configuration($selected_environment);
  drupal_set_message(t('"!env" configuration deployed.', array('!env' => $selected_environment)));
}
