Per-environment configuration module
====================================

Ce projet est un module Drupal permettant le déploiement automatique d'une certaine configuration, selon l'environnement défini.

Pour défininr l'environnement courant, il est recommandé de rajouter cette ligne dans le fichier settings.php de votre Drupal. Ou mieux, dans un fichier settings.local.php, qui sera inclus dans le fichier settings.php.

    $conf['environment'] = 'environment_name';

Une fois cette variable définie, il suffit d'exécuter la commande Drush suivante. Elle se basera automatiquement sur la variable définie par le fichier settings.php.

    drush d-conf

Il est possible de forcer la configuration à déployer, en la donnant comme argument à la commande.

    drush d-conf forced_environment

Pour customiser la configuration, il suffit d'implémenter le hook_env_conf_available_env() en suivant l'exemple ci-dessous.

    /**
     * Implements hook_env_conf_available_env().
     */
    function mymodulecustom_env_conf_available_env() {
      return array(
        'first_environment_name' => array(
          'callbacks' => array(
            'first_function',
            'second_function',
            ...
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
        'second_environment_name' => array(
          'callbacks' => array(
            'first_function',
            'third_function',
            ...
          ),
          'include_file' => array(
            array(
              'type' => 'inc',
              'module' => 'mymodulecustom',
              'name' => 'mymodulecustom.env_conf',
            ),
          ),
          'weight' => 2,
        ),
        ...
      );
    }

Il est possible d'utiliser les mêmes fonctions de callbacks sur plusieurs environnements, ou d'en définir des différentes pour chaque environnement.

Il est également possible d'implémenter le hook_env_conf_available_env_alter() pour modifier les environnements par défaut.
