<?php
/**
 * @file
 * ts_res.features.inc
 */

/**
 * Implements hook_node_info().
 */
function ts_res_node_info() {
  $items = array(
    'ts_res' => array(
      'name' => t('Restaurant'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Nom'),
      'help' => '',
    ),
  );
  return $items;
}
