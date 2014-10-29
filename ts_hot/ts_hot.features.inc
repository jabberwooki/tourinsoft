<?php
/**
 * @file
 * ts_hot.features.inc
 */

/**
 * Implements hook_node_info().
 */
function ts_hot_node_info() {
  $items = array(
    'ts_hot' => array(
      'name' => t('Hôtel'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Nom'),
      'help' => '',
    ),
  );
  return $items;
}
