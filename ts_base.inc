<?php
/**
 * @file
 * Base Tourinsoft migration classes.
 */

/**
 * The extending class TourinsoftListJSON allows overriding
 * of getIDsFromJSON() and computeCount() functions
 * to face the Tourinsoft specific $data array to be parsed for IDs
 * and whose structure is as follows :
 * 
 * Array (
 *     [d] => Array (
 *         [0] => Array (
 *             ....
 *             [SyndicObjectID] => HOTLAR0340000848
               [Published] => /Date(999734400000)/
               [Updated] => /Date(1412780111877)/
               ....
               )
           [1] => Array (
 *             ....
 *             [SyndicObjectID] => HOTLAR034FS0007B
               [Published] => /Date(1253092645000)/
               [Updated] => /Date(1412943696000)/
               ....
           ....
       )
 * 
 * @author chris
 *
 */
class TourinsoftListJSON extends MigrateListJSON {
	/**
	 * The default implementation assumes the IDs are top-level array elements,
	 * In Tourinsoft case, IDs are found at $data['d']['xx']['SyndicObjectID']
	 * (see structure above)
	 */
	protected function getIDsFromJSON(array $data) {
		$ids = array();
		foreach ($data['d'] as $item) {
		  $ids[] = $item['SyndicObjectID'];
		}
		return $ids;
	}

	/**
	 * Return a count of all available IDs from the source listing. 
	 * The default implementation assumes the count of top-level elements reflects the number of IDs available.
	 * In Tourinsoft case, IDs are found at $data['d']['xx']['SyndicObjectID']
	 * (see structure above)
	 */
  public function computeCount() {
    $count = 0;
    if (empty($this->httpOptions)) {
      $json = file_get_contents($this->listUrl);
    }
    else {
      $response = drupal_http_request($this->listUrl, $this->httpOptions);
      $json = $response->data;
    }
    if ($json) {
      $data = drupal_json_decode($json);
      if ($data) {
        $count = count($data['d']);
      }
    }
    return $count;
  }
}

/**
 * The extending class TourinsoftItemJSON allows overriding of the MigrateItemJSON() function 
 * to face the Tourinsoft specific $data array to be parsed for tourism items.
 * 
 * @author chris
 *
 */
class TourinsoftItemJSON extends MigrateItemJSON {
	/**
	 * Two problems with the default getItem() 
	 * - it reads and parses the JSON on each call, which we don't want to do for multiple items in the file; 
	 * - we need to retrieve a given item from the file according to its 'id' element.
	*/
	protected $data = array();
	
	public function getItem($id) {
		// We cache the parsed JSON at $this->data.
		if (empty($this->data)) {
			$data = $this->loadJSONUrl($this->itemUrl);
			if ($data) {
				// Let's index $this->data array by the ID for easy retrieval.
				foreach ($data->d as $item) {
					$this->data[$item->SyndicObjectID] = $item;
				}
			}
			else {
				// Error-handling here....
			}
		}
		// Return the requested item
		if (isset($this->data[$id])) {
			return $this->data[$id];
		}
		else {
			return NULL;
		}
	}
}

abstract class BaseTourinsoftMigration extends Migration {
	// Tourinsoft JSON files dir (instanciated during construction);
	protected $ts_files_dir = '';
	
	// Source key (for mapping in extending classes)
	protected $source_key = array(
			'SyndicObjectID' => array(
					'type' => 'varchar',
					'length' => 255,
					'not null' => TRUE,
			)
	);
	
	public function __construct() {
		// Always call the parent constructor first for basic setup
		parent::__construct();
		
		// JSON files dir
		$this->ts_files_dir = variable_get('file_public_path', conf_path() . '/files/') . 'tourinsoft/';
		
		// Source fields.
		// Here, we declare the fields that are common to all Tourinsoft objects AND that have the same label.
		// Common fields but diplaying different labels must be
		$this->base_fields = array(
			'SyndicObjectID' => 'Tourinsoft object identifier',
			'Published' => 'Publication date',
			'Updated' => 'Update date',
			'SyndicObjectName' => 'Tourinsoft object name',
			'DetailPHOTOS' => 'Photographies',
		);
				
		// Fields mapping
		$this->addFieldMapping('field_ts_syndicobjectid', 'SyndicObjectID');
		$this->addFieldMapping('title', 'SyndicObjectName');
		$this->addFieldMapping('field_ts_published', 'Published');
		$this->addFieldMapping('field_ts_updated', 'Updated');
		$this->addFieldMapping('field_ts_photos', 'images');
		$this->addFieldMapping('field_ts_photos:file_replace')->defaultValue(FILE_EXISTS_REPLACE);
		$this->addFieldMapping('field_ts_photos:alt', 'images_titles');
		$this->addFieldMapping('field_ts_photos:title', 'image_titles');
		
	}
	
	function prepareRow($row) {
		// Published and Updated dates.
		// Dates are provided by Tourinsoft in json files in the ISO format (ex. 2014-10-08T14:55:11).
		// But the drupal_json_decode() function (which uses the PHP function json_decode()) outputs date fields in the /Date(999734400000)/ format.
		// So, we must alter dates in order to make them human readable.
		// NOTE ! The timestamp date in the Drupal PHP array (after decoding) is systematically 2 hours greater than the iso date visible in the json file.
		$published_date = $row->Published;
		$published_timestamp = substr($published_date, 6, 10) - 7200;
		$row->Published = date('Y-m-d H:i:s', $published_timestamp);
		
		$updated_date = $row->Updated;
		$updated_timestamp = substr($updated_date, 6, 10) - 7200;
		$row->Updated = date('Y-m-d H:i:s', $updated_timestamp);
		
		// Photos.
		$row->images = array();
		$row->image_titles = array();
		$detail_photos = explode('#', $row->DetailPHOTOS);
		foreach ($detail_photos as $detail_photo) {
			$data = explode('|', $detail_photo);
			$row->images[] = substr($data[0], 0, strrpos($data[0], '/')) . '/' . $data[1];
			$row->image_titles[] = substr($data[0], strrpos($data[0], '/') +1);
			// Copyright data are not always present.
			if (isset($data[2])) {
				$row->image_copyrights[] = $data[2];
			}
			dpm($row->images);
		}
	}
	
	// 	public function prepare(stdClass $entity, stdClass $row) {
	// 	}
}