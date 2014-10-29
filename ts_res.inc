<?php
class ResMigration extends BaseTourinsoftMigration {
	protected $ts_object = 'res';

	public function __construct() {
		parent::__construct();

		$this->description = t('Tourinsoft RES objects');
		
		// Source.
		$ts_fields = array(
			'DetailDESCRIPTIONLONGUEOT' => 'Long description',
			'DetailDESCRIPTIONSUCCINTEOT' => 'Short description',
			'DetailSPECIALITESCULINAIRES' => 'Food specialties',
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
		$this->addFieldMapping('field_ts_specialitesculinaires', 'food_specialties');
	}
	
	function prepareRow($row) {
		parent::prepareRow($row);
		
		// DetailSPECIALITESCULINAIRES
		$row->food_specialties = array();
		$food_details = $row->DetailSPECIALITESCULINAIRES;
		$details = explode('#', $food_details);
		foreach ($details as $detail) {
			$row->food_specialties[] = $detail;
		}
		
		// 	public function prepare(stdClass $entity, stdClass $row) {
		// 	}
	}
}