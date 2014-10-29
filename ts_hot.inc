<?php
class HotMigration extends BaseTourinsoftMigration {
	protected $ts_object = 'hot';

	public function __construct() {
		parent::__construct();

		$this->description = t('Tourinsoft HOT objects');
		
		// Source.
		$ts_fields = array(
			'DetailDESCRIPTIONLONGUEOT' => 'Long description',
			'DetailDESCRIPTIONSUCCINTEOT' => 'Short description',
			'DetailCONFORTCHAMBRE' => 'Rooms facilities',
		);
		$fields = array_merge($this->base_fields, $ts_fields);
		
		$json_file = $this->ts_files_dir . $this->ts_object . '.json';
		$this->source = new MigrateSourceList(new TourinsoftListJSON($json_file), new TourinsoftItemJSON($json_file), $fields);
		
		// Destination.
		$this->destination = new MigrateDestinationNode('ts_' . $this->ts_object);
		
		// Map.
		$this->map = new MigrateSQLMap($this->machineName, $this->source_key,	MigrateDestinationNode::getKeySchema());
		
		// Fields mapping.
		$this->addFieldMapping('field_ts_descriptioncourte', 'DetailDESCRIPTIONSUCCINTEOT');
		$this->addFieldMapping('field_ts_descriptionlongue', 'DetailDESCRIPTIONLONGUEOT');
		$this->addFieldMapping('field_ts_confortchambre', 'room_details');
	}
	
	function prepareRow($row) {
		parent::prepareRow($row);
		
		// DetailCONFORTCHAMBRE
		$row->room_details = array();
		$room_details = $row->DetailCONFORTCHAMBRE;
		$details = explode('#', $room_details);
		foreach ($details as $detail) {
			$row->room_details[] = $detail;
		}
	}
	
	// 	public function prepare(stdClass $entity, stdClass $row) {
	// 	}
}