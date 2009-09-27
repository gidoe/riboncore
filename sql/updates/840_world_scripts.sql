ALTER TABLE db_version CHANGE COLUMN required_832_world_scripts_dk required_840_world_scripts bit;
update gameobject_template set scriptName = 'go_shallow_grave' where entry in (128308,128403)