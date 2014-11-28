<?php

/**
 * @file
 * Per-environment configuration module,
 * callback functions for Per-environment configuration.
 */

/**
 * Demo function: do nothing,
 * should be remove by implementing hook_env_conf_available_env_alter()
 */
function _demo_env_conf() {
  // here some code to execute
  drupal_set_message(t('You should remove <strong>_demo_env_conf()</strong> function by implementing <strong>hook_env_conf_available_env_alter()</strong>.'), 'warning');
}
