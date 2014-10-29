<?php
/**
 * @file
 * Declares our migrations.
 */

/**
 * Implements hook_migrate_api().
 */
function tourinsoft_migrate_api() {
  $api = array(
    'api' => 2,
    'groups' => array(
      'tourinsoft' => array(
        'title' => t('Tourinsoft migrations'),
      ),
    ),
    'migrations' => array(
      'hot' => array(
        'class_name' => 'HotMigration',
        'group_name' => 'tourinsoft',
      ),
      'res' => array(
        'class_name' => 'ResMigration',
        'group_name' => 'tourinsoft',
      ),
    ),
  );
  return $api;
}
?>