<?php

/**
 * @file
 * Admin page callbacks for the Search API Solr module.
 */

/**
 * Generates form to show Solr config files.
 */
function search_api_solr_solr_config_form($form, &$form_state, $server) {
  define('ACQUIA_DEVELOPMENT_NOSSL', TRUE);

  $form['title'] = array(
    '#prefix'  => '<h3>',
    '#markup'  => t('List of files found on the Solr server:'),
    '#suffix'  => '</h3>',
  );

  $form['files'] = array(
    '#type'  => 'vertical_tabs',
  );

  // Retrieving list of files available.
  $files_data = json_decode(search_api_solr_server_get_file($server->name)->data, TRUE);
  $files_list = $files_data['files'];

  ksort($files_list);
  
  // Generating fieldset for each file.
  foreach ($files_list as $file_name => $file_info) {
    $file_data = search_api_solr_server_get_file($server->name, $file_name);
    
    if (!empty($file_info['directory'])) {
      // It's a directory, skipping.
      continue;
    }
    
    $file_date = format_date(strtotime($file_info['modified']));

    $form['files'][$file_name] = array(
      '#title' => check_plain($file_name),
      '#description' => t('Last modified: <b>!time</b>', array(
        '!time' => $file_date,
      )),
      '#type'  => 'fieldset',
    );

    if ($file_info['size'] > 0) {
      // File is not empty.
      $form['files'][$file_name]['data'] = array(
        '#type'  => 'textarea',
        '#value' => $file_data->data,
        '#rows'  => '30',
      );
    }
    else {
      // File is empty (0 bytes).
      $form['files'][$file_name]['data'] = array(
        '#prefix' => '<h3>',
        '#markup' => t('File is empty'),
        '#suffix' => '</h3>',
      );
    }
  }

  return $form;
}
