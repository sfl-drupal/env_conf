<?php

/**
 * @file
 * Per-environment configuration module,
 * forms.
 */

/**
 * Allow user to lauch configuration deployment,
 * with default, or manually defined, parameter.
 */
function env_conf_form() {
  module_load_include('inc', 'env_conf');
  $form = array();

  // List available environments
  $available_envs = _get_simple_available_environments();

  // Get current environment name (normally in settings.php,
  // but possibly in a Drupal variable,
  // so let's get it with variable_get function)
  // and alert if there is a problem
  $environment = variable_get('environment', NULL);
  if ($environment === NULL) {
    drupal_set_message(t('No environment configured in <i>settings.php</i>. You should edit it and add <strong>$conf[\'environment\'] = \'env_name\';</strong>.'), 'error');
  }
  elseif (!in_array($environment, $available_envs)) {
    drupal_set_message(t('Environment is defined as <strong>!env</strong> in <i>settings.php</i>, but available environments are <strong>!available_envs</strong>. You should edit <i>settings.php</i> or define <strong>!env</strong> as an available environment by implementing <strong>hook_env_conf_available_env()</strong>.', array('!env' => $environment, '!last_deployed_conf' => $last_deployed_conf, '!available_envs' => implode(', ', $available_envs))), 'warning');
  }
  else {

    // Alert if last deployed configuration doesn't match
    // the one defined in settings.php or if configuration
    // has to be deployed.
    $last_deployed_conf = variable_get('env_conf_last_deployed_conf', NULL);
    if ($last_deployed_conf === NULL || empty($last_deployed_conf)) {
      drupal_set_message(t('Default environment is defined as <strong>!env</strong> in <i>settings.php</i>.', array('!env' => $environment)));
      drupal_set_message(t('But no environment configuration has been deployed. You should do it now!'), 'error');
    }
    elseif ($last_deployed_conf !== $environment) {
      drupal_set_message(t('Environment is defined as <strong>!env</strong> in <i>settings.php</i>.', array('!env' => $environment)));
      drupal_set_message(t('But last deployed configuration is <strong>!last_deployed_conf</strong>.', array('!last_deployed_conf' => $last_deployed_conf)), 'warning');
    }
    elseif ($last_deployed_conf === $environment) {
      drupal_set_message(t('Last deployed configuration is <strong>!last_deployed_conf</strong>, as it\'s defined in <i>settings.php</i>.', array('!last_deployed_conf' => $last_deployed_conf)));
    }

    // Manually select environment to execute
    $form['select_env'] = array(
      '#type' => 'select',
      '#title' => t('Configuration to deploy'),
      '#description' => t('Select which configuration you want to deploy. Default value is defined in <i>settings.php</i>.'),
      '#default_value' => 'default',
      '#options' => array('default' => 'default') + $available_envs,
    );
    if ($environment === NULL) {
      $form['select_env']['#disabled'] = TRUE;
    }

    // Deploy button
    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Deploy configuration'),
    );
    if ($environment === NULL) {
      $form['buttons']['submit']['#disabled'] = TRUE;
    }
  }

  return $form;
}

/**
 * Previous form submission
 * @see env_conf_form()
 */
function env_conf_form_submit($form, &$form_state) {
  module_load_include('inc', 'env_conf');

  // Clean flash messages queue
  drupal_get_messages();

  // Define configuration to deploy
  $selected_environment = $form_state['values']['select_env'];
  if ($selected_environment === 'default') {
    $selected_environment = variable_get('environment', NULL);
  }

  // Deploy configuration
  _deploy_environment_configuration($selected_environment);
}
