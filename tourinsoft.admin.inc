<?php
/**
 * Tourinsoft syndication settings and data handling.
 */

function tourinsoft_get_files() {
	
	$syndicated_sheets = tourinsoft_get_syndicated_sheets();
//dpm($syndicated_sheets);
	$header = array('Code', 'Designation', 'Action');
	$rows = array();
	foreach ($syndicated_sheets as $syndic_id=>$sheet) {
		$rows[] = array(
			$sheet[0], 
			$sheet[1], 
			l(t('Download'), 'admin/config/services/tourinsoft/download/' . $syndic_id . '/' . 	$sheet[0]));
	}
	
	$build['sheets_table'] = array(
			'#theme' => 'table',
			'#header' => $header,
			'#rows' => $rows,
			'#empty' => t('No synchronization set up yet. <a href="@link">Add synchronization</a>.', array('@link' => url('admin/config/services/tourinsoft/configure'))),
	);
	
	return $build;
}

function tourinsoft_download($syndic_id, $object_code) {
	$xml_files_path = 'public://tourinsoft';
	if (!file_prepare_directory($xml_files_path, FILE_CREATE_DIRECTORY)) {
		$msg = t('The Tourinsoft directory %path could not be created. Please ensure that the webserver has permission to create directory.',
				array('%path' => file_uri_target($xml_files_path)));
		watchdog('tourinsoft', $msg);
		drupal_set_message($msg);
	}
	else {
		$ws_url = variable_get('tourinsoft_ws_url', '');
		$client_id = variable_get('tourinsoft_client_id', '');
		$xml_data_url = $ws_url . $client_id . '/' . $syndic_id . '/Objects?$format=json';
		$xml_file_name = strtolower($object_code) . '.json';
		
// 		$vars = array(
// 			'%url' => $ws_url,
// 			'%id' => $client_id,
// 			'%xml_data_url' => $xml_data_url,
// 			'%xml_file_name' => $xml_file_name,
// 		);
// 		watchdog('tourinsoft', 'ws_url=%url, cliend_id=%id, xml_data_url=%xml_data_url, xml_file_name=%xml_file_name', $vars);
		
		if($file = system_retrieve_file($xml_data_url, $xml_files_path . '/' . $xml_file_name, FALSE, FILE_EXISTS_REPLACE)) {
			$msg = t('XML file %file saved successfully in \'%path\'', array('%file' => $xml_file_name, '%path' => $xml_files_path));
			drupal_set_message($msg);
			watchdog('tourinsoft', $msg);
		}
		else {
			$msg = t('XML file %file couldn\'t be savec in \'%path\'. Check directory permissions and try again.', array('%file' => $xml_file_name, '%path' => $xml_files_path));
			'couldn\t be saved';
			drupal_set_message($msg);
			watchdog('tourinsoft', $msg);
		}
		drupal_goto('admin/config/services/tourinsoft');
	}
}

function tourinsoft_manual_update() {
	return "Drupal manual update (à construire)";
}

function tourinsoft_admin_settings() {
	// Tourinsoft webservice URL.
	$form['webservice_url'] = array(
			'#type' => 'fieldset',
			'#title' => t('Web service url'),
	);

	$form['webservice_url']['tourinsoft_ws_url'] = array(
			'#type' => 'textfield',
			'#title' => t('Tourinsoft webservice URL'),
			'#description' => t('Type in the URL which the Tourinsoft XML files are available from.'),
			'#size' => 100,
			'#default_value' => variable_get('tourinsoft_ws_url', ''),
	);
	
	// Client identifier.
	$form['tourinsoft_client'] = array(
			'#type' => 'fieldset',
			'#title' => t('Tourinsoft client'),
	);
	
	$form['tourinsoft_client']['tourinsoft_client_id'] = array(
			'#type' => 'textfield',
			'#title' => t('Client identifier'),
			'#description' => t('Type in the tourinsoft client identifier (i.e "cdt34".'),
			'#size' => 15,
			'#default_value' => variable_get('tourinsoft_client_id', ''),
	);
	
	// Tourism office syndication keys
	$form['syndication_keys'] = array(
			'#type' => 'fieldset',
			'#title' => t('Syndication identifiers (ID)'),
			'#description' => 'Type in the syndication keys of the Tourinsoft objects you want to synchronize.',
	);
	
	$form['syndication_keys']['ts_asc'] = array(
			'#type' => 'textfield',
			'#title' => t('ASC (Activités sportives, culurelles et formules itinérantes)'),
			'#default_value' => variable_get('ts_asc', 0),
	);
	
	$form['syndication_keys']['ts_com'] = array(
			'#type' => 'textfield',
			'#title' => t('COM (Commerces)'),
			'#default_value' => variable_get('ts_com', 0),
	);
	
	$form['syndication_keys']['ts_deg'] = array(
			'#type' => 'textfield',
			'#title' => t('DEG (Dégustations)'),
			'#default_value' => variable_get('ts_deg', 0),
	);
	
	$form['syndication_keys']['ts_fma'] = array(
			'#type' => 'textfield',
			'#title' => t('FMA (Fêtes et manifestations)'),
			'#default_value' => variable_get('ts_fma', 0),
	);
	
	$form['syndication_keys']['ts_hlo'] = array(
			'#type' => 'textfield',
			'#title' => t('HLO (Hébergements locatifs)'),
			'#default_value' => variable_get('ts_hlo', 0),
	);
	
	$form['syndication_keys']['ts_hot'] = array(
			'#type' => 'textfield',
			'#title' => t('HOT (Hôtellerie)'),
			'#default_value' => variable_get('ts_hot', 0),
	);
	
	$form['syndication_keys']['ts_hpa'] = array(
			'#type' => 'textfield',
			'#title' => t('HPA (Hôtellerie de plein air)'),
			'#default_value' => variable_get('ts_hpa', 0),
	);
	
	$form['syndication_keys']['ts_iti'] = array(
			'#type' => 'textfield',
			'#title' => t('ITI (Itinéraires touristiques)'),
			'#default_value' => variable_get('ts_iti', 0),
	);
	
	$form['syndication_keys']['ts_loi'] = array(
			'#type' => 'textfield',
			'#title' => t('LOI (Loisirs)'),
			'#default_value' => variable_get('ts_loi', 0),
	);
	
	$form['syndication_keys']['ts_org'] = array(
			'#type' => 'textfield',
			'#title' => t('ORG (Organismes et entreprises)'),
			'#default_value' => variable_get('ts_org', 0),
	);
	
	$form['syndication_keys']['ts_pcu'] = array(
			'#type' => 'textfield',
			'#title' => t('PCU (Patrimoine culturel)'),
			'#default_value' => variable_get('ts_pcu', 0),
	);
	
	$form['syndication_keys']['ts_pna'] = array(
			'#type' => 'textfield',
			'#title' => t('PNA (Patrimoine naturel)'),
			'#default_value' => variable_get('ts_pna', 0),
	);
	
	$form['syndication_keys']['ts_res'] = array(
			'#type' => 'textfield',
			'#title' => t('RES (Restauration)'),
			'#default_value' => variable_get('ts_res', 0),
	);
	
	$form['syndication_keys']['ts_vil'] = array(
			'#type' => 'textfield',
			'#title' => t('VIL (Villages de vacances)'),
			'#default_value' => variable_get('ts_vil', 0),
	);
	
	return system_settings_form($form);
}
